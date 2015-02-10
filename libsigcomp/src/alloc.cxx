/**
	
	Copyright (C) 2015 Riccardo Manfrin at gmail dot com
	
	This file is part of libSigComp fork project.

    libSigComp fork is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    libSigComp fork is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU Lesser General Public License
    along with libSigComp fork.  
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

class alloc_test_t {
	char *alloc_data;
public:
	alloc_test_t()
	{
		printf("Invoked constructor\n");
		alloc_data = (char *) alloc_malloc(1);
	}
	~alloc_test_t()
	{
		printf("Invoked destructor\n");
		alloc_free(alloc_data);
	}
};


#ifdef __cplusplus
extern "C" {
#endif
	
static void* 	(*orig_malloc)(size_t) = NULL;
static void* 	(*orig_calloc)(size_t nmemb, size_t size);
static void* 	(*orig_realloc)(void *ptr, size_t size);
static void 	(*orig_free)(void *) = NULL;


void
alloc_init() {
	orig_malloc = 	(void* (*)(size_t)) 
			dlsym(RTLD_NEXT, "malloc");
	orig_calloc = 	(void* (*)(size_t nmemb, size_t size)) 
			dlsym(RTLD_NEXT, "calloc");
	orig_realloc = 	(void* (*)(void *ptr, size_t size)) 
			dlsym(RTLD_NEXT, "realloc");
	orig_free = 	(void (*)(void *))
			dlsym(RTLD_NEXT, "free");
}

/**
 * @brief malloc replacement overrides malloc symbol
 */
void*
malloc(size_t __size)
{
	return orig_malloc(__size);
}

void *
calloc(size_t nmemb, size_t size)
{
	return orig_calloc(nmemb, size);
}

void *
realloc(void *ptr, size_t size)
{
	return orig_realloc(ptr, size);
}

/**
 * @brief free replacement overrides free symbol
 */
void
free(void* __ptr)
{
	orig_free(__ptr);
}

#define USE_KAMAILIO_SHM_MEM_MGR
#define SHM_MEM
#define F_MALLOC
#define __CPU_x86_64
#define FAST_LOCK
#define USE_FUTEX
#ifdef USE_KAMAILIO_SHM_MEM_MGR
#include "../../../mem/shm_mem.h"
#endif

void* 
alloc_malloc(size_t size)
{
#ifdef USE_KAMAILIO_SHM_MEM_MGR
	return shm_malloc(size);
#else
	return orig_malloc(size);
#endif
}

void* 
alloc_calloc(size_t nmemb, size_t size)
{
#ifdef USE_KAMAILIO_SHM_MEM_MGR
	void *res = shm_malloc(nmemb * size);
	memset(res,0,nmemb * size);
	return res;
#else
	return orig_calloc(nmemb, size);
#endif
}

void* 
alloc_realloc(void* ptr, size_t size)
{
#ifdef USE_KAMAILIO_SHM_MEM_MGR
	return shm_realloc(ptr, size);
#else
	return orig_realloc(ptr, size);
#endif
}

void
alloc_free(void* ptr)
{
#ifdef USE_KAMAILIO_SHM_MEM_MGR
	shm_free(ptr);
#else
	orig_free(ptr);
#endif
}

void 
alloc_test()
{
	alloc_test_t *t = new alloc_test_t();
	delete t;
}

#ifdef __cplusplus
};
#endif

#include <bits/c++config.h>
#include <cstdlib>
#include <bits/exception_defines.h>
#include "new"


using std::bad_alloc;

void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)
{
	void *p;

	/* malloc (0) is unpredictable; avoid it.  */
	if (sz == 0)
		sz = 1;
	p = (void *) alloc_malloc(sz);

	return p;
}

void
operator delete(void* ptr) _GLIBCXX_USE_NOEXCEPT
{
	alloc_free(ptr);
}
