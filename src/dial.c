#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "tokenizer.h"

struct token *token_array;

void display_tokens(struct token *tokens, int n_tokens)
{
	int i;
	for (i = 0; i < n_tokens; i=i+1)
	{
		write(1, tokens[i].text, tokens[i].text_len);
		write(1, " ", 1);
	}
}

void interactive()
{
	char buffer[512];

	while (1)
	{
		int len = read(0,buffer,512);
		token_array = malloc(sizeof(struct token) * (count_tokens(buffer,len)+1));
		len = tokenize(buffer, len, token_array)+1;
		display_tokens(token_array, len);
		free(token_array);
	}
}

int main(int argc, char * *argv)
{
	int n;

	if (argc<2)
		interactive();
	else
	{
		int fd;
		struct stat status;

		stat(argv[1], &status);

		if (!(fd = open(argv[1], O_RDONLY, 0)))
			exit(0);

		char *src_code = (char*)mmap(NULL, status.st_size+4096, PROT_READ, MAP_SHARED, fd, 0);

		token_array = malloc(sizeof(struct token) * (count_tokens(src_code, status.st_size)+1));

		n = tokenize(src_code, status.st_size, token_array);
		display_tokens(token_array, n+1);
	}

	return 0;
}
