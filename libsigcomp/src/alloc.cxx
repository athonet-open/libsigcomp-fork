/**
	
	Copyright (C) 2009 Riccardo Manfrin at gmail dot com
	
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

#include <bits/c++config.h>
#include <cstdlib>
#include <bits/exception_defines.h>
#include "new"

using std::new_handler;
using std::bad_alloc;
#if _GLIBCXX_HOSTED
using std::malloc;
#else
// A freestanding C runtime may not provide "malloc" -- but there is no
// other reasonable way to implement "operator new".
extern "C" void *malloc (std::size_t);
#endif

// new_handler get_new_handler();

_GLIBCXX_WEAK_DEFINITION void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)
{
  void *p;

  /* malloc (0) is unpredictable; avoid it.  */
  if (sz == 0)
    sz = 1;
  p = (void *) malloc (sz);

  return p;
}
