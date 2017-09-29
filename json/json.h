/******************************************************************************
** File Name: json.h
** Author:
** Creation Time: Fri Sep 29 01:18:47 2017
*/
#ifndef _JSON_H
#define	_JSON_H

#define	N	100

typedef struct json_desc	json_desc_t;

struct desc {
	void *			type;
	const char *	name;
	size_t			offs;
	size_t			n;
	json_desc_t *	next;
};

struct json_desc {
	size_t			count;
	size_t			size;
	struct desc		descriptors[N];
};

void addBoolean(json_desc_t *desc, const char *name, size_t offs, size_t n);

void addInt8(json_desc_t *desc, const char *name, size_t offs, size_t n);
void addInt16(json_desc_t *desc, const char *name, size_t offs, size_t n);
void addInt32(json_desc_t *desc, const char *name, size_t offs, size_t n);

void addUint8(json_desc_t *desc, const char *name, size_t offs, size_t n);
void addUint16(json_desc_t *desc, const char *name, size_t offs, size_t n);
void addUint32(json_desc_t *desc, const char *name, size_t offs, size_t n);

void addString(json_desc_t *desc, const char *name, size_t offs, size_t n);

void addObject(json_desc_t *desc, const char *name, size_t offs, size_t n,
			   json_desc_t *next);

char *json(json_desc_t *desc, void *data, char *buf);

#endif	/* _JSON_H */

