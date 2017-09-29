#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "json.h"

#define	DIM(x)	(sizeof (x) / sizeof ((x)[0]))

char buf[1024];

static void testBoolean(void)
{
	bool data0 = 0;
	bool data1 = 1;

	json_desc_t	desc = {0};
	addBoolean(&desc, "bool", 0, 1);

	puts(json(&desc, &data0, buf));
	puts(json(&desc, &data1, buf));
}

static void testBooleanArray(void)
{
	bool data[] = {0, 1};

	json_desc_t	desc = {0};
	addBoolean(&desc, "bool", 0, DIM(data));

	puts(json(&desc, data, buf));
}

static void testInt8(void)
{
	int8_t data0 = 100;
	int8_t data1 = -20;

	json_desc_t	desc = {0};
	addInt8(&desc, "int8", 0, 1);

	puts(json(&desc, &data0, buf));
	puts(json(&desc, &data1, buf));
}

static void testIntArray(void)
{
	int8_t data[16] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15
	};

	{
		json_desc_t	desc = {0};
		addInt8(&desc, "int8", 0, DIM(data) / sizeof(int8_t));
		puts(json(&desc, data, buf));
	}

	{
		json_desc_t	desc = {0};
		addInt16(&desc, "int16", 0, DIM(data) / sizeof(int16_t));
		puts(json(&desc, data, buf));
	}

	{
		json_desc_t	desc = {0};
		addInt32(&desc, "int32", 0, DIM(data) / sizeof(int32_t));
		puts(json(&desc, data, buf));
	}

	{
		json_desc_t	desc = {0};
		addUint32(&desc, "uint32", 0, DIM(data) / sizeof(uint32_t));
		puts(json(&desc, data, buf));
	}
}

static void testString(void)
{
	char s[] = "Hello";
	char *data0 = s, *data1 = "World";

	json_desc_t	desc = {0};
	addString(&desc, "string", 0, 1);

	puts(json(&desc, &data0, buf));
	puts(json(&desc, &data1, buf));
}

#define	offsetof(type,field)	((size_t)&((type *)0)->field)

static void testObject(void)
{
	struct person {
		bool	native;
		uint8_t	age;
		uint16_t birthday[3];
		char *	name[2];
	} data[] = {
		{0, 20, {1997, 7, 1}, {"Mary", "Black"}},
		{0, 35, {1982, 9, 9}, {"Tomas", "Green"}}
	};

	json_desc_t desc = {0};
	json_desc_t	next = {0};
	addBoolean(&next, "native", offsetof(struct person, native), 1);
	addUint8(&next, "age", offsetof(struct person, age), 1);
	addUint16(&next, "birthday", offsetof(struct person, birthday), 3);
	addString(&next, "name", offsetof(struct person, name), 2);

	addObject(&desc, "data", 0, 2, &next);
	puts(json(&desc, data, buf));
}

int main(void)
{
	if (0) testBoolean();
	if (0) testBooleanArray();
	if (1) testInt8();
	if (1) testIntArray();
	if (1) testString();
	if (1) testObject();
}

