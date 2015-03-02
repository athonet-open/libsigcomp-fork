/*****************************************************************************/
/* Copyright (C) 2008-2010 Athonet, SRL. www.athonet.com All rights reserved.*/
/*****************************************************************************/

/*
 * THIS FILE IS PROPRIETARY MATERIAL OF ATHONET SRL, TRIESTE, ITALY.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF ATHONET, SRL.
 * THIS FILE MAY NOT BE DISTRIBUTED.
 */

/**
 * @file reliability_mgmt_ext.h
 * @date 2015-3-2
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __RELIABILITY_MGMT_EXT_H__
#define __RELIABILITY_MGMT_EXT_H__

#include "rfc3174_us-sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	SHA_INVALID,
	SHA_VALID
}state_sha_valid_flag_t;
	
typedef struct {
	/** @brief value of the SHA */
	char value[SHA1HashSize];
	/** @brief Specifies if the SHA is to be considered valid */
	state_sha_valid_flag_t valid :1;
}state_sha_t;

#ifdef __cplusplus
};
#endif

#endif
