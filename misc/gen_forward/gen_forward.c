#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "ifcfg.h"
#include "iptables.h"

#pragma GCC diagnostic ignored "-Wpedantic"

#define	FORWARDIP	"10.105.1.200"

#define	MAXBUF		1024
#define	MAXPORT		10

char	V_port[MAXPORT][MAXBUF + 1];

struct global {
	const char	*Fip;
} g = {
	.Fip	= FORWARDIP
};

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

