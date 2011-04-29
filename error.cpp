#include "error.h"

#ifdef __cplusplus 
extern "C" {
#endif

static void err_doit(int ,const char *,va_list );

void err_ret(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(errno,fmt,ap);
	va_end(ap);
	return;
}

void err_quit(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(errno,fmt,ap);
	va_end(ap);
	exit(1);
}

#define	MAXLINE	4096

static void err_doit(int errnoflag,const char *fmt,va_list ap)
{
	int errno_save;
	char buf[MAXLINE];

	errno_save = errno;
	memset(buf,0,MAXLINE);
	vsprintf(buf,fmt,ap);
	if(errnoflag)
		sprintf(buf+strlen(buf),": %s",strerror(errno_save));
	strcat(buf,"\n");
	fflush(stdout);
	fputs(buf,stderr);
	fflush(NULL);
	return;
}

#ifdef __cplusplus 
}
#endif
