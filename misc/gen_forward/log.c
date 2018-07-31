#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define	MAXBUF	1024

static FILE *logfp = NULL;

int log_init(const char *log)
{
	logfp = fopen(log, "a+");
	return NULL != logfp;
}

void log_exit(void)
{
	fclose(logfp);
	logfp = NULL;
}

static void log_common(const char *format, va_list args)
{
	char	buf[MAXBUF + 1];
	time_t	ctime = time(NULL);

	strftime(buf, MAXBUF, "%Y-%m-%d %H:%M%S ", localtime(&ctime));
	fputs(buf, logfp);

	vsprintf(buf, format, args);
	fputs(buf, logfp);
}

void log_info(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	log_common(format, args);
	va_end(args);

	fflush(logfp);
}

void log_error(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	log_common(format, args);
	va_end(args);

	fputs(strerror(errno), logfp);
	fputc('\n', logfp);

	fflush(logfp);
}

