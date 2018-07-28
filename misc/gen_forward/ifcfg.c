#include <stdio.h>
#include "ifcfg.h"

#define	MAXBUF		128

extern int (*System)(const char * command);

void ifcfg_add(const char *Vip, const char *Netmask)
{
	char	cmd[MAXBUF + 1];

	sprintf(cmd, IFCFG " add %s/%s", Vip, Netmask);
	System(cmd);
}

void ifcfg_del(const char *Vip, const char *Netmask)
{
	char	cmd[MAXBUF + 1];

	sprintf(cmd, IFCFG " del %s/%s", Vip, Netmask);
	System(cmd);
}

