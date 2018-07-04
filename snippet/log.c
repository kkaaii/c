#include <stdio.h>

#define	_XNAME(_0, _1, _2, _3, _4, name, ...)	name

#define	_LOG(...)	_XNAME(__VA_ARGS__, Log4, Log3, Log2, Log1, Log0)(__VA_ARGS__)

void Log0(const char *prompt)
{
	puts(prompt);
}

void Log1(const char *prompt, unsigned p1)
{
	puts(prompt);
	printf("\tparameter: %u\n", p1);
}

void Log2(const char *prompt, unsigned p1, unsigned p2)
{
	puts(prompt);
	printf("\tparameter 1: %u\n", p1);
	printf("\tparameter 2: %u\n", p2);
}

void Log3(const char *prompt, unsigned p1, unsigned p2, unsigned p3)
{
	puts(prompt);
	printf("\tparameter 1: %u\n", p1);
	printf("\tparameter 2: %u\n", p2);
	printf("\tparameter 3: %u\n", p3);
}

int main(void)
{
	_LOG("Log 0");
	_LOG("Log 1", 1);
	_LOG("Log 2", 1, 2);
	_LOG("Log 3", 1, 2, 3);
}

