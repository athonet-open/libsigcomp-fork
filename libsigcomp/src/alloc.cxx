/*****************************************************************************/
/* Copyright (C) 2008-2010 Athonet, SRL. www.athonet.com All rights reserved.*/
/*****************************************************************************/

/*
 * THIS FILE IS PROPRIETARY MATERIAL OF ATHONET SRL, TRIESTE, ITALY.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF ATHONET, SRL.
 * THIS FILE MAY NOT BE DISTRIBUTED.
 */

/**
 * @file alloc.cxx
 * @date 2015-2-9
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "alloc.h"
#ifdef __cplusplus
extern "C" {
#endif

static void* (*orig_malloc)(size_t) = NULL;
static void (*orig_free)(void *) = NULL;

void
alloc_init() {
    orig_malloc = (void* (*)(size_t)) dlsym(RTLD_NEXT, "malloc");
	orig_free = (void (*)(void *)) dlsym(RTLD_NEXT, "free");
}

/**
 * @brief malloc replacement overrides malloc symbol
 */
void*
malloc(unsigned long int __size)
{
	return orig_malloc(__size);
}
/**
 * @brief free replacement overrides free symbol
 */
void
free(void* __ptr)
{
	orig_free(__ptr);
}

#ifdef __cplusplus
};
#endif