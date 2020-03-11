#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

#include <stdio.h>

void print_std_and_fp(char *msg, FILE *fp)
{
	printf(msg);
	fprintf(fp,msg);
	return ;
}

#endif
