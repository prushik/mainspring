#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tokenizer.h"

void interactive()
{
	char buffer[512];
	
	while (1)
	{
		int len = read(1,buffer,512);
		len = tokenize(buffer, len);
		process_expression(len);
	}
}

int main(int argc, char **argv)
{
	int n;

	if (argc<2)
		interactive();
	else
	{
		n = tokenize(argv[1], strlen(argv[1]));
		process_expression(n);
	}

	return 0;
}
