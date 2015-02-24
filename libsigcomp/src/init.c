/*****************************************************************************/
/* Copyright (C) 2008-2010 Athonet, SRL. www.athonet.com All rights reserved.*/
/*****************************************************************************/

/*
 * THIS FILE IS PROPRIETARY MATERIAL OF ATHONET SRL, TRIESTE, ITALY.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF ATHONET, SRL.
 * THIS FILE MAY NOT BE DISTRIBUTED.
 */

/**
 * @file init.c
 * @date 2015-2-24
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __INIT_C__
#define __INIT_C__

#include "init.h"
#include "log.h"
#include "alloc.h"
void libsigcomp_init()
{
	log_init("/tmp/libsigcomp.log");
	alloc_init();
}

#endif
