#include <stdio.h>
#include "iptables.h"

#define	IPTABLES	"/sbin/iptables"
#define	MAXBUF		1024

extern int (*System)(const char * command);

void iptables_addpre(char *cmd, const char *Vip, const char *Sip, const char *Sport)
{
	sprintf(cmd, IPTABLES " -t nat -A PREROUTING -p tcp -d %s --dport %s -j DNAT --to %s", Vip, Sport, Sip);
}

void iptables_addpost(char *cmd, const char *Fip, const char *Sip, const char *Sport)
{
	sprintf(cmd, IPTABLES " -t nat -A POSTROUTING -p tcp -d %s --dport %s -j SNAT --to %s", Sip, Sport, Fip);
}

void iptables_delpre(char *cmd, const char *Vip, const char *Sip, const char *Sport)
{
	sprintf(cmd, IPTABLES " -t nat -D PREROUTING -p tcp -d %s --dport %s -j DNAT --to %s", Vip, Sport, Sip);
}

void iptables_delpost(char *cmd, const char *Fip, const char *Sip, const char *Sport)
{
	sprintf(cmd, IPTABLES " -t nat -D POSTROUTING -p tcp -d %s --dport %s -j SNAT --to %s", Sip, Sport, Fip);
}

void iptables_add(const char *Vip, const char *Sip, const char *Sport, const char *Fip)
{
	char	cmd[MAXBUF + 1];

	iptables_addpre(cmd, Vip, Sip, Sport);
	System(cmd);

	iptables_addpost(cmd, Fip, Sip, Sport);
	System(cmd);
}

void iptables_del(const char *Vip, const char *Sip, const char *Sport, const char *Fip)
{
	char	cmd[MAXBUF + 1];

	iptables_delpre(cmd, Vip, Sip, Sport);
	System(cmd);

	iptables_delpost(cmd, Fip, Sip, Sport);
	System(cmd);
}

