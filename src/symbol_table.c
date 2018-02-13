#include <string.h>
#include "host_config.h"
#include "lang_config.h"
#include "tokenizer.h"
#include "symbols.h"

int add_symbol(struct token *tokens, struct symbol *table, int active, int tok_n)
{
	table[active+1].token_num = tok_n;
	table[active+1].text = tokens[tok_n].text;
	table[active+1].text_len = tokens[tok_n].text_len;
}
