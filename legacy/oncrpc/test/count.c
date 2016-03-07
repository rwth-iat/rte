#include <stdio.h>
#include <rpc/rpc.h>
#include "cou.h"

static int count;

int *inc_1 (dummy)
void *dummy;
{
	count++;
	printf("inc called; result: %d\n", count);
	return (&count);
}

int *dec_1 (dummy)
void *dummy;
{
	count--;
	printf("dec called; result: %d\n", count);
	return (&count);
}

int *set_1 (val)
int *val;
{
	int *i = 0;
	count = *val;
	if (!count)
		(*i)++;
	printf("set called; value: %d\n", count);
	return (&count);
}
