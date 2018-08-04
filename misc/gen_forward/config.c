#include "config.h"

#define	DEFAULT_IP	"127.0.0.1"
#define	DEFAULT_PORT	"80"
#define	DEFAULT_BACK	"100"
#define	DEFAULT_LOGF	"/var/log/ip_forward.log"

#define	FORWARD_IP	"10.105.1.200"

struct host	host = {
	/* .ip		= */DEFAULT_IP,
	/* .port	= */DEFAULT_PORT,
	/* .back	= */DEFAULT_BACK,
	/* .logdir	= */DEFAULT_LOGF,
	/* .fip		= */FORWARD_IP,
};

