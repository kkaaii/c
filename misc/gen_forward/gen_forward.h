#ifndef	_GEN_FORWARD_H
#define	_GEN_FORWARD_H

#define	MAXBUF		1024

extern MYSQL	*conn;
extern char	sql[MAXBUF + 1];

extern int	(*System)(const char *command);

const char	*iptables_addpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);
const char	*iptables_delpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);
const char	*iptables_addpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);
const char	*iptables_delpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);

void		ParseRequest(const char *req, char *Vtype, char *Sip, char (*Vports)[MAXBUF + 1]);
int		Case0(const char *Sip);

#endif	/* _GEN_FORWARD_H */
