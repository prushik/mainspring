
#include "host_config.h"
#include "lang_config.h"
#include "tokenizer.h"
#include "symbols.h"

static inline int is_keyword(struct token *tok)
{
	int i,j;
	for (i = 0; i < RESERVED_KEYWORDS; i++)
	{
		int len = strlen(reserved[i]);
		if (tok->text_len != len)
			continue;
		
		for (j = 0; j < len; j++)
			if (reserved[i][j] != tok->text[j])
				break;
		if (j == len)
			return i;
	}
	return -1;
}

// This function should prepare the token array for parsing
int parse_pass0(struct token *tokens, int tok_len)
{
	int i;
	for (i=0; i<tok_len ; i++)
	{
		if (tokens[i].type == CHAR_TYPE_SYM)
		{
			token[i].sym = is_keyword(tokens[i]);
			if (tokens[i]sym >= 0)
			{
				// Token is a keyword
			}
			else
			{
				// Token must be a symbol or an error
			}
		}
	}
}

int parse_count_symbols(struct token *tokens, int tok_len)
{
	int i;
	for (i=0; i<tok_len ; i++)
	{
		if (tokens[i].type == CHAR_TYPE_SYM)
		{
			if (tokens->sym == -1) ;
		}
	}
}

void parse(struct token *tokens, struct symbol *symbols)
{
	
}
