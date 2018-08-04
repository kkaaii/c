#ifndef	_CONFIG_H
#define	_CONFIG_H

struct host {
	const char	*ip;
	const char	*port;
	const char	*back;
	const char	*logdir;
	const char	*fip;
};

extern struct host	host;

#endif	/* _CONFIG_H */

