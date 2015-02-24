/**
 * @file log.cxx
 * @date 2015-2-24
 * @author Riccardo Manfrin at gmail dot com
 * @brief
 */
#ifndef __LOG_C__
#define __LOG_C__

#include <stdarg.h>
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

static FILE *libsigcomp_fout = NULL;

void log_init(const char* fname)
{
	 libsigcomp_fout = fopen(fname, "a+");
}

void log_log(const char *fmt, ...)
{
	if(libsigcomp_fout) {
		va_list args;
		va_start(args,fmt);
		vfprintf(libsigcomp_fout, fmt, args);
		va_end(args);
		fflush(libsigcomp_fout);
	}
}

#ifdef __cplusplus
};
#endif

#endif

