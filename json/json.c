#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "json.h"

#define	JSON_ARR_HEADER	'['
#define	JSON_ARR_FOOTER	']'

#define	JSON_OBJ_HEADER	'{'
#define	JSON_OBJ_FOOTER	'}'

#define	JSON_DELIMITER	','

typedef char *(*encode_t)(char *buf, void *data, size_t n, json_desc_t *next);

#define	encodeArray(size, expr)					\
	size_t i;									\
												\
	if (n != 1) *buf++ = JSON_ARR_HEADER;		\
	for (i = 0; i < n; ++i) {					\
		if (i != 0) *buf++ = JSON_DELIMITER;	\
		buf = expr;								\
		data = (char *)data + (size);			\
	}											\
	if (n != 1) *buf++ = JSON_ARR_FOOTER;		\
	*buf = '\0';								\
	return buf;									\

static char *encodeBool(char *buf, void *data)
{
	bool value = *(bool *)data;
	return buf += sprintf(buf, value ? "true" : "false");
}

static char *encodeInt8(char *buf, void *data)
{
	int8_t value = *(int8_t *)data;
	return buf += sprintf(buf, "%d", value);
}

static char *encodeInt16(char *buf, void *data)
{
	int16_t value = *(int16_t *)data;
	return buf += sprintf(buf, "%d", value);
}

static char *encodeInt32(char *buf, void *data)
{
	int32_t value = *(int32_t *)data;
	return buf += sprintf(buf, "%d", value);
}

static char *encodeUint8(char *buf, void *data)
{
	uint8_t value = *(uint8_t *)data;
	return buf += sprintf(buf, "%u", value);
}

static char *encodeUint16(char *buf, void *data)
{
	uint16_t value = *(uint16_t *)data;
	return buf += sprintf(buf, "%u", value);
}

static char *encodeUint32(char *buf, void *data)
{
	uint32_t value = *(uint32_t *)data;
	return buf += sprintf(buf, "%u", value);
}

static char *encodeString(char *buf, void *data)
{
	char *value = *(char **)data;
	return buf += sprintf(buf, "\"%s\"", value);
}

static char *encodeBoolArr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (bool), encodeBool(buf, data));
}

static char *encodeInt8Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (int8_t), encodeInt8(buf, data));
}

static char *encodeInt16Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (int16_t), encodeInt16(buf, data));
}

static char *encodeInt32Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (int32_t), encodeInt32(buf, data));
}

static char *encodeUint8Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (uint8_t), encodeUint8(buf, data));
}

static char *encodeUint16Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (uint16_t), encodeUint16(buf, data));
}

static char *encodeUint32Arr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (uint32_t), encodeUint32(buf, data));
}

static char *encodeStringArr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(sizeof (char *), encodeString(buf, data));
}

static char *encodeObject(char *buf, void *data, json_desc_t *next)
{
	encode_t func;

	*buf++ = JSON_OBJ_HEADER;
	if (NULL != next) {
		struct desc *p = &next->descriptors[0];
		size_t i;

		for (i = 0; i < next->count; ++i, ++p) {
			if (i != 0) *buf++ = JSON_DELIMITER;
			buf += sprintf(buf, "\"%s\":", p->name);
			func = (encode_t)(p->type);
			buf = (*func)(buf, (char *)data + p->offs, p->n, p->next);
		}
	}
	*buf++ = JSON_OBJ_FOOTER;
	*buf = '\0';
	return buf;
}

static char *encodeObjectArr(char *buf, void *data, size_t n, json_desc_t *next)
{
	encodeArray(next->size, encodeObject(buf, data, next));
}

char *json(json_desc_t *desc, void *data, char *buf)
{
	encodeObject(buf, data, desc);
	return buf;
}

static struct desc *add(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	struct desc *p = &desc->descriptors[desc->count++];

	p->name = name;
	p->offs = offs;
	p->n    = n;

	return p;
}

void addBoolean(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeBoolArr;
}

void addInt8(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeInt8Arr;
}

void addInt16(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeInt16Arr;
}

void addInt32(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeInt32Arr;
}

void addUint8(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeUint8Arr;
}

void addUint16(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeUint16Arr;
}

void addUint32(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeUint32Arr;
}

void addString(json_desc_t *desc, const char *name, size_t offs, size_t n)
{
	add(desc, name, offs, n)->type = encodeStringArr;
}

void addObject(json_desc_t *desc, const char *name, size_t offs, size_t n,
               json_desc_t *next)
{
	struct desc *p = add(desc, name, offs, n);
	p->type = encodeObjectArr;
	p->next = next;
}

