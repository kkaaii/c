#ifndef	_LOG_H
#define	_LOG_H

#define	LOG	log_info
#define	ERR	log_error

int  log_init(const char *log);
void log_exit(void);

void log_info(const char *format, ...);
void log_error(const char *format, ...);

#endif	/* _LOG_H */

