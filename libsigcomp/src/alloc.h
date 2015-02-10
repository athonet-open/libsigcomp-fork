/*****************************************************************************/
/* Copyright (C) 2008-2010 Athonet, SRL. www.athonet.com All rights reserved.*/
/*****************************************************************************/

/*
 * THIS FILE IS PROPRIETARY MATERIAL OF ATHONET SRL, TRIESTE, ITALY.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF ATHONET, SRL.
 * THIS FILE MAY NOT BE DISTRIBUTED.
 */

/**
 * @file alloc.h
 * @date 2015-2-9
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initialize the allocation module
 * 
 * Retrieves actual original references to malloc/free
 */
void alloc_init();
#ifdef __cplusplus
};
#endif

#endif
