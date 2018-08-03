#include <stdio.h>

void case9(FILE *client_sock, const char *Sip)
{
	fprintf(client_sock, "%s", Sip);
}

