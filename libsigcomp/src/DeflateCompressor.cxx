/**
	Copyright (C) 2009 Mamadou Diop <diopmamadou@yahoo.fr>

	This file is part of libSigComp project.

    libSigComp is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    libSigComp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU Lesser General Public License
    along with libSigComp.
*/

#include "global_config.h"
#include "DeflateCompressor.h"
#include "DeflateData.h"
#include "SigCompState.h"
#include "log.h"

//#include <math.h>
#define LIBSIGCOMP_MIN(a, b) (a<b?a:b)

#if USE_ONLY_ACKED_STATES
#define GET_STATE_TO_COMPRESS_WITH(x) ((x)->getGhostAckedState())
#else
#define GET_STATE_TO_COMPRESS_WITH(x) ((x)->getGhostState())
#endif

__NS_DECLARATION_BEGIN__

/**
*/
DeflateCompressor::DeflateCompressor()
{
	
}

/**
*/
DeflateCompressor::~DeflateCompressor()
{
	
}

/**
*/
bool DeflateCompressor::compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream, state_sha_t *used_state_sha)
{
	
	this->lock();

	bool result = true, stateChanged, stateful, zret;
	uint8_t* header;
	size_t compressedDataLen;
	DeflateData* data = NULL;

	SigCompBuffer output_buffer;
	output_buffer.referenceBuff((uint8_t*)output_ptr, output_size);

	size_t pointer =0, state_len_index =0;

	log_log("DeflateCompressor::compress - \t######### SENDING ##########\n");
	log_log("DeflateCompressor::compress - \tcompressing message using compartment %llu\n", lpCompartment->getIdentifier());
	// Compression Data
	if(!lpCompartment->getCompressionData())
		lpCompartment->setCompressionData(new DeflateData(stream));

	data = dynamic_cast<DeflateData*>(lpCompartment->getCompressionData());
	assert(data);

	// State memory size code
	uint8_t smsCode = LIBSIGCOMP_MIN(lpCompartment->getRemoteParameters()->getSmsCode(), lpCompartment->getRemoteParameters()->getDmsCode());
#if USE_ONLY_ACKED_STATES
	stateful = (GET_STATE_TO_COMPRESS_WITH(data) && data->isStateful());
#else
	stateful = (GET_STATE_TO_COMPRESS_WITH(data) != NULL);
#endif

	log_log("DeflateCompressor::compress - \tstateful=%i\n", stateful);
	//
	//	Init zLIB
	//
	int windowBits = ( smsCode - (stream?2:1) ) + 10;
	windowBits = (windowBits < 8) ? 8 : ( (windowBits > 15 ? 15 : windowBits) ); // Because of zlib limitation (windowsize MUST be between 8 and 15)
	if(windowBits != data->zGetWindowBits())
	{
		// Window size changed
		data->freeGhostState();
		data->zSetWindowBits(windowBits);
		if( !(result = data->zReset()) ) goto bail;
	}
	else if(!data->getGhostState())
	{
		// No ghost --> reset zlib
		data->getGhostCopyOffset() = 0;
		if( !(result = data->zReset()) ) goto bail;
	}

	//***********************************************
	//	SigComp headers
	//
	header = output_buffer.getBuffer(pointer++);

	
	/* SigComp Header */
	if(lpCompartment->getReqFeedback() && lpCompartment->getReqFeedback()->getSize()){
		// Return the requested feedback
		*header = 0xfc; // T=1
		::memmove(output_buffer.getBuffer(pointer), lpCompartment->getReqFeedback()->getBuffer(), lpCompartment->getReqFeedback()->getSize());
		pointer+=lpCompartment->getReqFeedback()->getSize();
	}
	else{
		*header = 0xf8;
	}

	//
	// Stateless or stateful?
	//
	if(GET_STATE_TO_COMPRESS_WITH(data))
	{
		::memmove(output_buffer.getBuffer(pointer), const_cast<SigCompBuffer*>(GET_STATE_TO_COMPRESS_WITH(data)->getStateIdentifier())->getBuffer(), 
			PARTIAL_ID_LEN_VALUE);
		pointer+=PARTIAL_ID_LEN_VALUE; *header |= PARTIAL_ID_LEN_CODE;
	}
	else
	{
		uint16_t codeLen = DEFLATE_BYTECODE_LEN;
		// first byte for codelen
		*output_buffer.getBuffer(pointer++) = ((codeLen>>4)& 0x00ff);
		// last 4 bits for codelen
		*output_buffer.getBuffer(pointer) = ((codeLen & 0x000f)<<4); 
		// first and last 4 bits for destination
		*output_buffer.getBuffer(pointer++) |= DEFLATE_BYTECODE_DESTINATION_CODE;

		//////////////////////////////////////////////////
		//	Upload UDVM bytecode
		//
		::memmove(output_buffer.getBuffer(pointer), (uint8_t*)DeflateData::deflate_bytecode, codeLen);
		pointer+= codeLen;

		//////////////////////////////////////////////////
		// FIXME: check for overflow and >=320
		//
		// [DMS]+[Req. Fed. Loc.]+[state_len]+[cpb||dms||sms]+[Sigcomp_version]+[states]
		//*output_buffer.getBuffer(pointer++) = 0x04; //reserved=0, Q=1, S=0, I=0
		//*output_buffer.getBuffer(pointer++) = (this->req_feedback_item++); //requested feedback item

		state_len_index  = pointer;
		*output_buffer.getBuffer(pointer+=4) = 0x00; // [hash_len]+[state_len]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()>>8); // [cpb||dms||sms]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()&0x00ff); // [Sigcomp_version]
#if USE_DICTS_FOR_COMPRESSION
		*output_buffer.getBuffer(pointer++) = 0x00; // First dict byte	// FIXME
		*output_buffer.getBuffer(pointer++) = DEFLATE_FIXME_DICT; // FIXME: also change ghost
#endif
	}

	//***********************************************
	//	Compress data using ZLIB
	//
	compressedDataLen = (output_size-pointer);
	zret = data->zCompress(input_ptr, input_size, output_buffer.getBuffer(pointer), &compressedDataLen, &stateChanged);
	if(!zret){
		goto bail;
	}
	pointer+=compressedDataLen;
	output_size = (pointer);

	//
	// Update state length
	//
	if(!(GET_STATE_TO_COMPRESS_WITH(data)) || !data->getGhostState())
	{		
		uint16_t state_len = ( (1<<(data->zGetWindowBits())) + DEFLATE_UDVM_CIRCULAR_START_INDEX - 64 );
		uint32_t hash_len = (state_len+8);
		
		// FIXME: 131072  could not go in 2-bytes
		*output_buffer.getBuffer(state_len_index) = (hash_len >> 8);
		*output_buffer.getBuffer(state_len_index+1) = (hash_len & 0x00ff);
		*output_buffer.getBuffer(state_len_index+2) = (state_len >> 8);
		*output_buffer.getBuffer(state_len_index+3) = (state_len & 0x00ff);

		//	First time or synchronize failure (NACK reason=STATE_NOT_FOUND)
		if(data->getGhostState()){
			log_log("DeflateCompressor::compress - \tdelete GhostState\n");
			data->freeGhostState();
		}
		log_log("DeflateCompressor::compress - \tcreate GhostState\n");
		data->createGhost(state_len, lpCompartment->getLocalParameters());
	}

#ifdef USE_ONLY_ACKED_STATES
	if (GET_STATE_TO_COMPRESS_WITH(data) && !stateful) {
		uint16_t state_len = ( (1<<(data->zGetWindowBits())) + DEFLATE_UDVM_CIRCULAR_START_INDEX - 64 );
		log_log("DeflateCompressor::compress - \tdelete GhostState\n");
		data->freeGhostState();
		log_log("DeflateCompressor::compress - \tcreate GhostState\n");
		data->createGhost(state_len, lpCompartment->getLocalParameters());
	}
#endif

	log_log("DeflateCompressor::compress - \tupdate GhostState\n");
	data->updateGhost((const uint8_t*)input_ptr, input_size);
	log_log("DeflateCompressor::compress - \tset stateless\n");
	data->setStateless();

	log_log("DeflateCompressor::compress - \t######### END ##########\n");
	//output_buffer.print(2000);
	this->unlock();
	return result;
bail:
	this->unlock();
	return false;
}

__NS_DECLARATION_END__
