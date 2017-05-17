#include <string.h>
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
	int i,j,k,l;
	for (i=0; i<tok_len ; i++)
	{
		if (tokens[i].type == CHAR_TYPE_SYM)
		{
			tokens[i].sym = is_keyword(&tokens[i]);
			if (tokens[i].sym >= 0)
			{
				// Token is a keyword
			}
			else
			{
				// Token must be a symbol or an error
			}
		}
		else if (tokens[i].type == CHAR_TYPE_OPR)
		{
			//check if this is a comment
			if (tokens[i].text_len>1 && tokens[i].text[0] == '/' && tokens[i].text[1] == '/')
			{
				for (j = i; j < tok_len; j++)
				{
					if (tokens[j].type == CHAR_TYPE_WHT)
					{
						for (k = 0; k < tokens[j].text_len; k++)
							if (tokens[j].text[k] == '\n')
								break;
						if (tokens[j].text[k] == '\n')
						{
							// then we need to merge i-j into i and mark i as a comment
							for (k = i; k < j; k++)
								tokens[i].text_len += tokens[k].text_len;
							for (k = i+1; k < j; k++)
							{
								for (l = i+1; l < tok_len-1; l++)
									memcpy(&tokens[l], &tokens[l+1], sizeof(struct token));
								tok_len += -1;
							}
							tokens[i].type = CHAR_TYPE_COM;
							break;
						}
					}
				}
			}
		}
		else if (tokens[i].type == CHAR_TYPE_WHT)
		{
			// Remove whitespace tokens
			for (j = i; j < tok_len-1; j++)
				memcpy(&tokens[j], &tokens[j+1], sizeof(struct token));
			tok_len += -1;
			i += -1; //because now we the next symbol moved into the current position
		}
	}
	return tok_len;
}

int parse_count_symbols(struct token *tokens, int tok_len)
{
	int i;
	for (i=0; i<tok_len ; i++)
	{
		if (tokens[i].type == CHAR_TYPE_SYM)
		{
			if (tokens[i].sym == -1) ;
		}
	}
}

void parse(struct token *tokens, struct symbol *symbols)
{
	
}
