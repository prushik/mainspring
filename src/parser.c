#include <string.h>
#include "host_config.h"
#include "lang_config.h"
#include "tokenizer.h"
#include "parser.h"
#include "symbols.h"

// This function actually returns the index of the keyword, or -1 if 
//no keyword is matched
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
// This function is hacky. This is really the preprocessors job, just like in C. but doing it in the parser is fine for now I suppose
// basically, this function removes whitespace and comments.
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
			i += -1; //because the next symbol moved into the current position
		}
	}
	return tok_len;
}

inline int is_definition_keyword(int sym)
{
	switch (sym)
	{
		case KEY_SIZE_BYTE:
		case KEY_SIZE_WORD:
		case KEY_SIZE_DWORD:
		case KEY_SIZE_QWORD:
			return 1;
	}
	if (sym >= KEY_REG_RANGE_BEGIN &&
		sym <= KEY_REG_RANGE_END)
		return 1;
	else
		return 0;
}

// this tries to count symbols to find out how much memory we will need for the symbol table. however, it sucks.
int parse_count_symbols(struct token *tokens, int tok_len)
{
	int i, count=0;
	for (i=0; i<tok_len ; i++)
	{
		if (tokens[i].type == CHAR_TYPE_SYM)
		{
			// If this looks like a symbol, check if it also looks like 
			// a declaration;
			if (tokens[i].sym == -1)
			{
				if ((tokens[i-1].type == CHAR_TYPE_SYM && is_definition_keyword(tokens[i-1].sym))
					|| (tokens[i+1].type == CHAR_TYPE_SPE && tokens[i+1].sym == PUNC_LABL))
				{
					count++;
				}
			}
		}
	}

	return count;
}

// this should divvy the work out to specific construct functions, although I wonder if it makes sense to do that or if enumerating all possible language constructs here is not great
int parse_identify_sentence(struct token *tokens, int tok_n, struct symbol *symbols)
{
	if (tokens[tok_n+1].type == CHAR_TYPE_SPE && tokens[tok_n+1].sym == PUNC_LABL)
	{
		parse_label(tokens, tok_n, symbols);
		return PARSE_NODE_DECLARATION;
	}
	if (tokens[tok_n].type == CHAR_TYPE_SYM && is_definition_keyword(tokens[tok_n].sym))
	{
		parse_declaration(tokens, tok_n, symbols);
		return PARSE_NODE_DECLARATION;
	}
}

static parse_state = PARSE_STATE_OPEN;

int parse(struct token *tokens, int tok_len, struct symbol *symbols)
{
	int i, sym_cur=0;
	for (i=0; i<tok_len ; i++)
	{
		if (parse_state & PARSE_STATE_OPEN|PARSE_STATE_IN_BLOCK)
		{
			parse_identify_sentence(tokens, i, symbols);
		}
//		if (tokens[i].type == CHAR_TYPE_SYM)
//		{
			// If this looks like a symbol, check if it also looks like 
			// a declaration;
//			if (tokens[i].sym == -1)
//			{
//				if ((tokens[i-1].type == CHAR_TYPE_SYM && is_declaration_keyword(tokens[i-1].sym))
//					|| (tokens[i+1].type == CHAR_TYPE_OPR && tokens[i+1].sym == OP_LAB))
//				{
//					add_symbol(tokens, symbols, sym_cur, i);
//				}
//			}
//		}
	}

	return 0;
}

int parse_block(struct token *tokens, int tok_n)
{
	
}

// Declarations exclusively impart knowledge to the parser.
int parse_declaration(struct token *tokens, int tok_n, struct symbol *symbols)
{
	int i;
	for (i = 0; i < 2; i++)
	{
		if (tokens[tok_n+i].type == CHAR_TYPE_SYM && (!is_definition_keyword(tokens[tok_n+i].sym)))
		{
			int j;
			for (j = 0; j < tokens[tok_n+i].text_len; j++)
			{
				printf("%c", tokens[tok_n+i].text[j]);
			}
			printf(" <-- declaration\n");

			add_symbol(tokens, symbols, 0, tok_n);

			return tok_n+i;
		}
	}
}

// Definitions are basically anything that would generate a linkable symbol
int parse_definition(struct token *tokens, int tok_n, struct symbol *symbols)
{
	
}

int parse_label(struct token *tokens, int tok_n, struct symbol *symbols)
{
	int i;
	for (i = 0; i < tokens[tok_n].text_len; i++)
	{
		printf("%c", tokens[tok_n].text[i]);
	}
	printf(" <-- label\n");

	add_symbol(tokens, symbols, 0, tok_n);

	return tok_n;
}
