#include <stdio.h>
#include <string.h>

#define	BIT64(x)	(1ull << (x))

#define	SCORE2		3
#define	SCORE3		6
#define	SCORE4		12

#define	THRESHOLD8	60

typedef	unsigned long long	UINT64;

struct {
	char	name[20];
	UINT64	link2;
	UINT64	link3;
	UINT64	link4;
} data[64];

unsigned int	n = 0;

unsigned int get_key(const char *name)
{
	unsigned int	i;

	for (i = 0; i < n; ++i) {
		if (0 == strcmp(data[i].name, name)) {
			return i;
		}
	}

	strcpy(data[n].name, name);
	return n++;
}

static inline void add_link(UINT64 *link, const char *name)
{
	unsigned int	key = get_key(name);
	*link |= BIT64(key);
}

static inline void add_link2(unsigned int key, const char *name)
{
	add_link(&data[key].link2, name);
}

static inline void add_link3(unsigned int key, const char *name)
{
	add_link(&data[key].link3, name);
}

static inline void add_link4(unsigned int key, const char *name)
{
	add_link(&data[key].link4, name);
}

static char *get_name(char **line)
{
	char	*name = *line;
	char	*p = strchr(name, ',');
	*p = '\0';
	*line = p + 1;
	return name;
}

UINT64 getNextLarger(UINT64 x);

int main(void)
{
	char		line[256];
	char		*p;
	unsigned int	key;
	unsigned int	i;

	memset(data, 0, sizeof data);

	while (!feof(stdin)) {
		p = fgets(line, sizeof line - 1, stdin);
		if (NULL == p || '\n' == *p) {
			continue;
		}

		key = get_key(get_name(&p));
		add_link2(key, get_name(&p));

		if ('\n' != *p) {
			add_link3(key, get_name(&p));
			add_link3(key, get_name(&p));
		}

		if ('\n' != *p) {
			add_link4(key, get_name(&p));
			add_link4(key, get_name(&p));
			add_link4(key, get_name(&p));
		}
	}

	for (i = 0; i < n; ++i) {
		printf("[%2d] %-20s %016LX %016LX %016LX\n", i,
			data[i].name, data[i].link2, data[i].link3, data[i].link4);
	}

	UINT64	x = BIT64(8) - 1;
	while (x < BIT64(n + 1)) {
		unsigned int	score = 0;
		for (i = 0; i < n; ++i) {
			if (0 == (x & BIT64(i)))
				continue;

			UINT64	mask = x ^ BIT64(i);
			if (data[i].link2 != 0 && data[i].link2 == (data[i].link2 & mask))
				score += SCORE2;
			if (data[i].link3 != 0 && data[i].link3 == (data[i].link3 & mask))
				score += SCORE3;
			if (data[i].link4 != 0 && data[i].link4 == (data[i].link4 & mask))
				score += SCORE4;
		}

		if (score >= THRESHOLD8) {
			printf("======== %u ========\n", score);
			for (i = 0; i < n; ++i) {
				if (0 == (x & BIT64(i))) continue;
				puts(data[i].name);
			}
		}

		x = getNextLarger(x);
	}

	return 0;
}

