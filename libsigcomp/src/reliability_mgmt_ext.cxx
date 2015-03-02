/*****************************************************************************/
/* Copyright (C) 2008-2010 Athonet, SRL. www.athonet.com All rights reserved.*/
/*****************************************************************************/

/*
 * THIS FILE IS PROPRIETARY MATERIAL OF ATHONET SRL, TRIESTE, ITALY.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF ATHONET, SRL.
 * THIS FILE MAY NOT BE DISTRIBUTED.
 */

/**
 * @file reliability_mgmt_ext.cxx
 * @date 2015-3-2
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __RELIABILITY_MGMT_EXT_CXX__
#define __RELIABILITY_MGMT_EXT_CXX__

#include <string.h>
#include "reliability_mgmt_ext.h"

void 
state_sha_set(state_sha_t* out, uint8_t* buffer, state_sha_valid_flag_t valid)
{
	out->valid = valid;
	if( (valid == SHA_VALID) && buffer )
		memcpy(out->value, buffer, SHA1HashSize);
	/*else
	 * //Safe clear ?
	 * memset(out->value, 0, SHA1HashSize);*/
}




#endif
