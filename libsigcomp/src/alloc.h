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

/**
 * @brief verifies new / delete overridden behavior
 */
void alloc_test();


void *alloc_malloc(size_t size);
void alloc_free(void * ptr);
void *alloc_calloc(size_t nmemb, size_t size);
void *alloc_realloc(void *ptr, size_t size);

#ifdef __cplusplus
};
#endif

#endif
