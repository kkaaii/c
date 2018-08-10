#ifndef	_CONFIG_H
#define	_CONFIG_H

struct host {
	const char	*ip;
	const char	*port;
	const char	*back;
	const char	*logdir;
	const char	*fip;
};

struct database {
	const char	*server;
	const char	*port;
	const char	*user;
	const char	*password;
	const char	*database;
};

extern struct host	host;
extern struct database	database;

#endif	/* _CONFIG_H */

