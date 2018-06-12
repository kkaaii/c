#include <stdio.h>
#include <stdint.h>

#define	BIT(x)	(1u << (x))
#define	MASK(x)	(BIT(x) - 1)

#define	LINE_LEN	16
#define	LINE_MASK	(LINE_LEN - 1)

#define	CAST_PTR(t)	(t)(intptr_t)

typedef union {
	unsigned int	all;
	struct {
		unsigned int	width	: 2;
		unsigned int	endian	: 1;
		unsigned int	align	: 1;
		unsigned int	ascii	: 1;
		unsigned int	addr	: 1;
	};
} Option;

enum {
	eDelimiter	= ' ',
	eSeparator	= ' ',
	eFiller		= '-',
	eAlignment	= ' ',
};

void (*dump_data)(const void *buf, size_t bytes, size_t pre, size_t post);

static void dump_data_le(const void *buf, size_t bytes, size_t pre, size_t post)
{
	const unsigned char	*data = (const unsigned char *)buf + bytes - 1;

	putchar(eSeparator);

	while (post--) {
		putchar(eFiller);
		putchar(eFiller);
	}

	while (bytes--) {
		printf("%02X", *data--);
	}

	while (pre--) {
		putchar(eFiller);
		putchar(eFiller);
	}
}

static void dump_data_be(const void *buf, size_t bytes, size_t pre, size_t post)
{
	const unsigned char	*data = (const unsigned char *)buf;

	putchar(eSeparator);

	while (pre--) {
		putchar(eFiller);
		putchar(eFiller);
	}

	while (bytes--) {
		printf("%02X", *data++);
	}

	while (post--) {
		putchar(eFiller);
		putchar(eFiller);
	}
}

static inline void dump_align(size_t alignment)
{
	putchar(eSeparator);
	while (alignment--) {
		putchar(eAlignment);
		putchar(eAlignment);
	}
}

static void dump_line(const void *buf, size_t bytes, Option option)
{
	const unsigned char	*data = (const unsigned char *)buf;
	size_t			alignment = BIT(option.width);
	size_t			remaining = LINE_LEN;

	dump_data = option.endian ? dump_data_be : dump_data_le;

	if (option.addr) {
		printf("[%012lX]", (intptr_t)(buf));
	}

	if (option.align) {
		size_t	offset = CAST_PTR(size_t)(buf) & LINE_MASK;
		while (offset >= alignment) {
			dump_align(alignment);
			offset -= alignment;
			remaining -= alignment;
		}
		if (offset) {
			size_t	length = alignment - offset;
			size_t	post;
			if (length > bytes) {
				post = length - bytes;
				length = bytes;
			} else {
				post = 0;
			}
			(*dump_data)(data, length, offset, post);
			bytes -= length;
			data += length;
			remaining -= alignment;
		}
	}

	while (bytes >= alignment) {
		(*dump_data)(data, alignment, 0, 0);
		bytes -= alignment;
		data += alignment;
		remaining -= alignment;
	}
	if (bytes) {
		(*dump_data)(data, bytes, 0, alignment - bytes);
		remaining -= alignment;
	}

	while (remaining) {
		dump_align(alignment);
		remaining -= alignment;
	}
}

void dump_ascii(const void *buf, size_t bytes)
{
	const char	*s = (const char *)buf;

	while (bytes--) {
		char	c = *s++;
		putchar((0x20 <= c && c < 0x7F) ? c : '.');
	}
}

void dump_ascii_align(const void *buf, size_t bytes, Option option)
{
	if (option.align) {
		size_t	offset = CAST_PTR(size_t)(buf) & LINE_MASK;
		while (offset--) {
			putchar(eAlignment);
		}
	}

	dump_ascii(buf, bytes);
}

void dump(const unsigned char *data, size_t bytes, Option option)
{
	size_t	length;

	if (option.align) {
		length = -CAST_PTR(size_t)(data) & LINE_MASK;
		if (0 != length) {
			if (length > bytes) {
				length = bytes;
			}
			dump_line(data, length, option);
			if (option.ascii) {
				putchar(eDelimiter);
				dump_ascii_align(data, length, option);
			}
			bytes -= length;
			data += length;
			putchar('\n');
		}
	}

	length = LINE_LEN;
	while (bytes > 0) {
		if (length > bytes) {
			length = bytes;
		}

		dump_line(data, length, option);

		if (option.ascii) {
			putchar(eDelimiter);
			dump_ascii(data, length);
		}
		putchar('\n');
		data += length;
		bytes -= length;
	}
}

int main(int argc, char *argv[])
{
	unsigned char	buf[256];
	unsigned int	i;
	unsigned int	offset = 0;
	unsigned int	length = sizeof buf;
	Option		option;

	for (i = 0; i < sizeof buf; ++i) {
		buf[i] = i;
	}

	if (argc > 1) {
		sscanf(argv[1], "%u", &option.all);
	}

	if (argc > 2) {
		sscanf(argv[2], "%u", &length);
	}

	if (argc > 3) {
		sscanf(argv[3], "%u", &offset);
	}

	printf("offset = %u\n", offset);
	printf("length = %u\n", length);
	printf("option = %Xh\n", option.all);
	printf("\twidth\t= %u\n", option.width);
	printf("\tendian\t= %u\n", option.endian);
	printf("\talign\t= %u\n", option.align);
	printf("\tascii\t= %u\n", option.ascii);
	printf("\taddr\t= %u\n", option.addr);

	if (255 != option.all) {
		dump(&buf[offset], length, option);
	} else {
		for (i = 0; i < 64; ++i) {
			option.all = i;
			dump(&buf[offset], length, option);
		}
	}

	return 0;
}

