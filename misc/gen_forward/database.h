#ifndef _DATABASE_H
#define	_DATABASE_H

#include <stdarg.h>

struct database {
	const char	*server;
	const char	*port;
	const char	*user;
	const char	*password;
	const char	*database;
};

void db_init(struct database *db);
void db_exit(void);

void db_query(void);
int  db_foreach(void (*callback)(const char **row, va_list args), ...);

#endif	/* _DATABASE_H */

