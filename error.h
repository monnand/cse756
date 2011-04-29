#ifndef __MONNAND_ERROR_H
#define __MONNAND_ERROR_H

/* This lib is changed from APUE written by W. Richard Stevens */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
typedef enum debug_level_e {
	DEBUG_SILENT = 0,
	DEBUG_ERROR_MSG,
	DEBUG_TRACE_MSG,
	DEBUG_MAX_LEVEL
} debug_level_e;

#if (DEBUG_LEVEL > DEBUG_ERROR_MSG - 1)
*/
#ifdef DEBUG_ERROR
#define ERROR(msg)	do {	\
	fprintf(stderr, "\tERROR %s(%s):%d\n", __FILE__, __func__, __LINE__);	\
	err_quit msg;	\
} while(0)
#else
#define ERROR(msg)
#endif

/*
#if (DEBUG_LEVEL > DEBUG_TRACE_MSG - 1)
*/
#ifdef DEBUG_TRACE
#define TRACE(msg)	do {	\
	fprintf(stderr, "\tTRACE %s(%s):%d\n", __FILE__, __func__, __LINE__);	\
	err_ret msg;	\
} while (0)
#else
#define TRACE(msg)
#endif

/* for portability matters, __VA_ARGS__ will not be used. 
 * so we have to call these macros like this:
 * TRACE(("test %s", "just a test"));
 */
/*
#define DEF_TRACE_ERROR_MACRO(macro)	\
#ifdef macro	\
#define TRACE(msg)	do {	\
	err_ret("%s(%s %d): ", __FILE__, __func__, __LINE__);	\
	err_ret msg;	\
} while (0)	\
#else	\
#define TRACE(msg)	\
#endif	\
#define ERROR(msg)	do {	\
	err_quit("%s(%s %d): ", __FILE__, __func__, __LINE__);	\
	err_quit msg;	\
} while(0)
*/

void err_quit(const char *, ...);
void err_ret(const char *, ...);

/*
   +--------------------+-----------------------+---------------+
   |	Function	|strerror(errno)?	|Terminate?	|
   +--------------------+-----------------------+---------------+
   |	err_ret		|	yes		|return;	|
   |	err_sys		|	yes		|exit(1);	|
   |	err_dump	|	yes		|abort();	|
   |	err_msg		|	no		|return;	|
   |	err_quit	|	no		|exit(1);	|
   +--------------------+-----------------------+---------------+
   |	log_ret		|	yes		|return;	|
   |	log_sys		|	yes		|exit(2);	|
   |	log_msg		|	no		|return;	|
   |	log_quit	|	no		|exit(2);	|
   +--------------------+-----------------------+---------------+
*/

#endif
