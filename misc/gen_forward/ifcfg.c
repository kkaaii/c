#include <stdio.h>
#include "ifcfg.h"

void ifcfg_add(char cmd[], const char *Vip, const char *Netmask)
{
	sprintf(cmd, IFCFG " add %s/%s", Vip, Netmask);
}

void ifcfg_del(char cmd[], const char *Vip, const char *Netmask)
{
	sprintf(cmd, IFCFG " del %s/%s", Vip, Netmask);
}

