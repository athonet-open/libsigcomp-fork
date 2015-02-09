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

#include <stddef.h>

void* operator new(size_t size);

void operator delete(void *);

void *malloc (size_t __size);
void free (void * __ptr);

#endif
