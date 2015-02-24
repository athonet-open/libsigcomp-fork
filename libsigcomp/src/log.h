/**
 * @file log.h
 * @date 2015-2-24
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __LOG_H__
#define __LOG_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void log_init(const char *fname);
void log_log(const char *fmt, ...);

#ifdef __cplusplus
};
#endif

#endif
