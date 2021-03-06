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

#include <iostream>
#include "global_config.h"

#include "SigCompUDVM.h"

__NS_DECLARATION_BEGIN__

/**
*/
void SigCompUDVM::createNackInfo(uint8_t reasonCode, SigCompBuffer* lpDetails, int16_t memory_address_of_instruction /*= -1*/)
{
#define NACK_HEADER_INDEX		0
#define NACK_VERSION_INDEX		(NACK_HEADER_INDEX+2)
#define NACK_REASON_CODE_INDEX	(NACK_VERSION_INDEX+1)
#define NACK_OPCODE_INDEX		(NACK_REASON_CODE_INDEX+1)
#define NACK_PC_INDEX			(NACK_OPCODE_INDEX+1)
#define NACK_SHA1_INDEX			(NACK_PC_INDEX+2)
#define NACK_DETAILS_INDEX		(NACK_PC_INDEX+SHA1HashSize)
	/*  0   1   2   3   4   5   6   7
        +---+---+---+---+---+---+---+---+
        | 1   1   1   1   1 | T |   0   |
        +---+---+---+---+---+---+---+---+
        |                               |
        :    returned feedback item     :
        |                               |
        +---+---+---+---+---+---+---+---+
        |         code_len = 0          |
        +---+---+---+---+---+---+---+---+
        | code_len = 0  |  version = 1  |
        +---+---+---+---+---+---+---+---+
        |          Reason Code          |
        +---+---+---+---+---+---+---+---+
        |  OPCODE of failed instruction |
        +---+---+---+---+---+---+---+---+
        |   PC of failed instruction    |
        |                               |
        +---+---+---+---+---+---+---+---+
        |                               |
        : SHA-1 Hash of failed message  :
        |                               |
        +---+---+---+---+---+---+---+---+
        |                               |
        :         Error Details         :
        |                               |
        +---+---+---+---+---+---+---+---+
	*/

	uint16_t mem_add_instruction = (memory_address_of_instruction >=0) ? memory_address_of_instruction : this->last_memory_address_of_instruction;

	this->lpResult->getNackInfo()->allocBuff( (NACK_SHA1_INDEX + SHA1HashSize) );
	*this->lpResult->getNackInfo()->getBuffer(NACK_HEADER_INDEX) = 0xf8;
	*this->lpResult->getNackInfo()->getBuffer(NACK_VERSION_INDEX) = NACK_VERSION;
	*this->lpResult->getNackInfo()->getBuffer(NACK_REASON_CODE_INDEX) = reasonCode;
	*this->lpResult->getNackInfo()->getBuffer(NACK_OPCODE_INDEX) = *this->memory.getBuffer(mem_add_instruction);
	*this->lpResult->getNackInfo()->getBuffer(NACK_PC_INDEX) = (mem_add_instruction >> 8);
	*this->lpResult->getNackInfo()->getBuffer(NACK_PC_INDEX+1) = (mem_add_instruction & 0x00ff);
	
	//
	// SHA-1 computation
	//
	SHA1Context sha;
	::SHA1Reset(&sha);
	::SHA1Input(&sha, this->sigCompMessage->startPtr, this->sigCompMessage->totalSize);
	::SHA1Result(&sha, this->lpResult->getNackInfo()->getBuffer(NACK_SHA1_INDEX));
	
	//
	// Details
	//
	if(lpDetails && lpDetails->getSize())
	{
		if(!this->lpResult->getNackInfo()->appendBuff(lpDetails->getBuffer(), lpDetails->getSize())) {
			std::cout << "Buffer not allocated\n";
		}
	}
	else if(reasonCode == BYTECODES_TOO_LARGE)
	{
		size_t memorySize = this->memory.getSize();
		if(!this->lpResult->getNackInfo()->appendBuff(&memorySize, 2)) {
			std::cout << "Buffer not allocated\n";
		}
	}
	else if(reasonCode == CYCLES_EXHAUSTED)
	{
		uint8_t cpb = this->stateHandler->getSigCompParameters()->getCpbValue();
		if (!this->lpResult->getNackInfo()->appendBuff(&cpb, 1)) {
			std::cout << "Buffer not allocated\n";
		}
	}

	lpResult->setIsNack(true);
}

__NS_DECLARATION_END__
