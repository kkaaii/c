#ifndef	_IFCFG_H
#define	_IFCFG_H

#define	IFCFG		"/sbin/ifcfg eth0"

void ifcfg_add(char cmd[], const char *Vip, const char *Netmask);

void ifcfg_del(char cmd[], const char *Vip, const char *Netmask);

#endif	/* _IFCFG_H */

