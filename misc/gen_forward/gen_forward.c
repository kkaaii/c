#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "gen_forward.h"
#include "log.h"
#include "database.h"
#include "ifcfg.h"
#include "iptables.h"

#pragma GCC diagnostic ignored "-Wpedantic"

#define	MAXBUF		1024
#define	MAXPORT		10

char	V_port[MAXPORT][MAXBUF + 1];

#if 1
int (*System)(const char * command) = NULL;
#else
int (*System)(const char * command) = system;
#endif

static const char *GetVtype(char *Vtype, const char *req)
{
	if ('V' != *req++)
		return NULL;

	if ('=' != *req++)
		return NULL;

	if ('\0' != *req)
		*Vtype = *req++;

	return req;
}

static const char *GetSip(char *Sip, const char *req)
{
	if ('I' != *req++)
		return NULL;

	if ('=' != *req++)
		return NULL;

	while ('\0' != *req && 'P' != *req) {
		*Sip++ = *req++;
	}
	*Sip = '\0';

	return req;
}

static void GetVports(char (*Vports)[MAXBUF + 1], const char *req)
{
	char	*p;

	if ('\0' != *req)
		++req;

	while ('\0' != *req) {
		++req;
		p = *Vports++;
		while ('\0' != *req && '+' != *req) {
			*p++ = *req++;
		}
		*p = '\0';
	}
}

void ParseRequest(const char *req, char *Vtype, char *Sip, char (*Vports)[MAXBUF + 1])
{
	if (NULL == (req = GetVtype(Vtype, req)))
		return;

	if (NULL == (req = GetSip(Sip, req)))
		return;

	GetVports(Vports, req);
}

void Response(FILE *client_sock, const char *request)
{
	char	Vtype;
	char	Sip[MAXBUF + 1];
	static	void (*cases[])(FILE *, const char *) = {
		case0, case1, case2, case3, case4,
		case5, case6, case7, case8, case9,
	};

	fputs("HTTP/1.1 200 OK\r\n"
		"Server: Ip Forward 1.1 by Shluo\r\n"
		"\r\n\r\n"
		"<html><head><meta HTTP-EQUIV='Pragma' CONTENT='no-cache'><title>Success</title></head>"
		"<body>", client_sock);

	ParseRequest(request, &Vtype, &Sip[0], V_port);
	if ('0' <= Vtype && Vtype <= '9') {
		db_init(&database);
		(*cases[Vtype - '0'])(client_sock, Sip);
		db_exit();
	}

	fputs("</body></html>", client_sock);
}

int Main(void)
{
	struct sockaddr_in addr;
	struct sockaddr	*address = (struct sockaddr *)&addr;
	socklen_t	addrlen;
	int		serverfd;
	int		clientfd;
	char		buf[MAXBUF + 1];
	ssize_t		len;

	if ((serverfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		ERR("socket(): ");
	}

	addrlen = 1;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &addrlen, sizeof addrlen);

	addr.sin_family = AF_INET;
	addr.sin_port = htons((uint16_t)atoi(host.port));
	addr.sin_addr.s_addr = inet_addr(host.ip);

	addrlen = sizeof (struct sockaddr_in);
	if (bind(serverfd, address, addrlen) < 0) {
		ERR("bind(): ");
	}

	if (listen(serverfd, atoi(host.back)) < 0) {
		ERR("listen(): ");
	}

	for (;;) {
		addrlen = sizeof (struct sockaddr_in);
		if ((clientfd = accept(serverfd, address, &addrlen)) < 0) {
			ERR("accept(): ");
		}

		if (!fork()) {
			if ((len = recv(clientfd, buf, MAXBUF, 0)) > 0) {
				FILE *fp = NULL;
				char *p;

				if (NULL == (fp = fdopen(clientfd, "w"))) {
					ERR("fdopen(): ");
				}

				if (NULL != (p = strstr(buf, " HTTP"))) {
					*p = '\0';
				}

				p = &buf[4];	/* "GET %s HTTP" */
				LOG("request='%s'\n", p);
				Response(fp, p);

				fclose(fp);
			}

			exit(0);
		}

		close(clientfd);
	}

	close(serverfd);
	return 0;
}

