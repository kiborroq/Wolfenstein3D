#include "stdlib.h"
#include "stdio.h"

int main(void)
{
	char *ptr = (char *)malloc(9223372036854775807);
	if (ptr)
		free(ptr);
	perror("");
	return (0);
}

