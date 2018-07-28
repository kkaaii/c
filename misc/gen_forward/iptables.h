#ifndef	_IPTABLES_H
#define	_IPTABLES_H

void iptables_addpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);
void iptables_delpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);

void iptables_addpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);
void iptables_delpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);

void iptables_add(const char *Vip, const char *Sip, const char *Sport, const char *Fip);
void iptables_del(const char *Vip, const char *Sip, const char *Sport, const char *Fip);

#endif	/* _IPTABLES_H */

