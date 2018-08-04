#ifndef	_GEN_FORWARD_H
#define	_GEN_FORWARD_H

#include "mysql.h"

#define	MAXBUF		1024

extern MYSQL		*conn;
extern struct host	host;

extern char	V_port[][MAXBUF + 1];

extern int	(*System)(const char *command);
/*
const char	*iptables_addpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);
const char	*iptables_delpre(char *cmd, const char *Vip, const char *Sip, const char *Sport);
const char	*iptables_addpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);
const char	*iptables_delpost(char *cmd, const char *Fip, const char *Sip, const char *Sport);
*/
void		ParseRequest(const char *req, char *Vtype, char *Sip, char (*Vports)[MAXBUF + 1]);

void case0(FILE *client_sock, const char *Sip);
void case1(FILE *client_sock, const char *Sip);
void case2(FILE *client_sock, const char *Sip);
void case3(FILE *client_sock, const char *Sip);
void case4(FILE *client_sock, const char *Sip);
void case5(FILE *client_sock, const char *Sip);
void case6(FILE *client_sock, const char *Sip);
void case7(FILE *client_sock, const char *Sip);
void case8(FILE *client_sock, const char *Sip);
void case9(FILE *client_sock, const char *Sip);

#endif	/* _GEN_FORWARD_H */
