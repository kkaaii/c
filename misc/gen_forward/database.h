#ifndef _DATABASE_H
#define	_DATABASE_H

#include "config.h"

void db_init(struct database *db);
void db_exit(void);

void db_query(const char *format, ...);
int db_foreach(void (*callback)(const char **row, va_list args, int n), ...);

#endif	/* _DATABASE_H */

