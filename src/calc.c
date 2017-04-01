#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tokenizer.h"

struct token *token_array;

void interactive()
{
	char buffer[512];

	while (1)
	{
		int len = read(0,buffer,512);
		token_array = malloc(sizeof(struct token) * (count_tokens(buffer,len)+1));
		len = tokenize(buffer, len, token_array);
		process_expression(len, token_array);
		free(token_array);
	}
}

int main(int argc, char **argv)
{
	int n;

	if (argc<2)
		interactive();
	else
	{
		n = tokenize(argv[1], strlen(argv[1]), token_array);
		process_expression(n,token_array);
	}

	return 0;
}
