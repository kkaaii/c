/******************************************************************************
** File Name: main.c
** Author:
** Creation Time: Mon Oct 16 06:46:48 2017
*/
#include <stdio.h>
#include "config.h"
#include "core.h"
#include "print.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
		return -1;

	init();
	load_config(argv[1]);
	print();

	return 0;
}

