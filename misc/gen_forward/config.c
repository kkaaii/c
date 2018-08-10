#include "config.h"

#define	DEFAULT_IP	"127.0.0.1"
#define	DEFAULT_PORT	"80"
#define	DEFAULT_BACK	"100"
#define	DEFAULT_LOGF	"/var/log/ip_forward.log"

#define	FORWARD_IP	"10.105.1.200"

#define	DB_SERVER	"127.0.0.1"
#define	DB_PORT		"3306"
#define	DB_USER		"ip_forward"
#define	DB_PASSWORD	"********"
#define	DB_DATABASE	"ip_forward"

struct host	host = {
	/* .ip		= */DEFAULT_IP,
	/* .port	= */DEFAULT_PORT,
	/* .back	= */DEFAULT_BACK,
	/* .logdir	= */DEFAULT_LOGF,
	/* .fip		= */FORWARD_IP,
};

struct database	database = {
	/* .server	= */DB_SERVER,
	/* .port	= */DB_PORT,
	/* .user	= */DB_USER,
	/* .password	= */DB_PASSWORD,
	/* .database	= */DB_DATABASE,
};

