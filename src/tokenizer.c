#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tokenizer.h"

int id_token_str(const char *token)
{
	int i,j;
	for (i = 0; i < CHARSETS; i++)
	{
		for (j = 0; j < strlen(char_ini[i]); j++)
		{
			if (token[0] == char_ini[i][j])
				return i;
		}
	}
	return CHAR_TYPE_INV;
}

int id_operator(const char *op_str, int len)
{
	int i;
	for (i = 0; i < OPERATORS; i++)
	{
		if (strlen(operator[i]) == len && strncmp(operator[i], op_str, len) == 0)
			return i;
	}
	return OP_COM; // ? I needed something to use for invalid operators
}

int count_tokens(const char *expr, unsigned int len)
{
	int cur_tok,n_tok=0;
	int i,j;
	//Figure out what the first token is
	cur_tok = id_token_str(expr);

	//Step through the string, locate token boundaries, and count tokens
	for (i=0; i < len; i++)
	{
		for (j=0; j < strlen(char_set[cur_tok]); j++)
			if (char_set[cur_tok][j] == expr[i])
				break; // then this is still part of the same token, do nothing.

		if (char_set[cur_tok][j] != expr[i])
		{
			// woo, the previous token is finished, start next token
			cur_tok = id_token_str(&expr[i]);
			// should we make a token for whitespace? for now we will, but later maybe we remove...
//			if (cur_tok != CHAR_TYPE_WHT)
			n_tok++;
		}
	}

	return n_tok;
}

int tokenize(const char *expr, unsigned int len, struct token *token_array)
{
	int cur_tok, n_tok;
	int i,j;

	// Now we know how many tokens exist, lets count them
//	token_array = malloc(sizeof(struct token)*(n_tok+1));
//	printf("Tokens: %d\n",n_tok);

	//We figured out how many tokens there are, now lets figure out what they are and how to handle them
	n_tok = 0;
	int level = 0;
	cur_tok = id_token_str(expr);
	token_array[n_tok].type = cur_tok;
	token_array[n_tok].text = expr;
	token_array[n_tok].level = level;
	for (i=0; i < len; i++)
	{
		for (j=0; j < strlen(char_set[cur_tok]); j++)
			if (char_set[cur_tok][j] == expr[i])
				break;

		if (char_set[cur_tok][j] != expr[i])
		{
//			if (cur_tok != CHAR_TYPE_WHT)
//			{
				token_array[n_tok].text_len = (&expr[i]) - token_array[n_tok].text;

				// Interpret the token to get it into a more usable form
				if (cur_tok == CHAR_TYPE_NUM)
				{
					token_array[n_tok].sym = strtol(token_array[n_tok].text, NULL, 0);
				}
				if (cur_tok == CHAR_TYPE_GRP)
				{
					int k;
					for (k=0;k<token_array[n_tok].text_len;k++)
						if (token_array[n_tok].text[k] == '(')
							level++;
						else
							level--;
				}
				if (cur_tok == CHAR_TYPE_OPR)
				{
					token_array[n_tok].sym = id_operator(token_array[n_tok].text, token_array[n_tok].text_len);
				}
//			}
			cur_tok = id_token_str(&expr[i]); // learn what the next token is
//			if (cur_tok != CHAR_TYPE_WHT)
//			{
				n_tok++;
				token_array[n_tok].type = cur_tok;
				token_array[n_tok].text = &expr[i];
				token_array[n_tok].level = level;
//			}
		}
	}
//	if (cur_tok != CHAR_TYPE_WHT)
//	{
		token_array[n_tok].text_len = (&expr[i]) - token_array[n_tok].text;

		// Interpret the token to get it into a more usable form
		if (cur_tok == CHAR_TYPE_NUM)
		{
			token_array[n_tok].sym = strtol(token_array[n_tok].text, NULL,0);
		}
		if (cur_tok == CHAR_TYPE_GRP)
		{
			int k;
			for (k=0;k<token_array[n_tok].text_len;k++)
				if (token_array[n_tok].text[k] == '(')
					level++;
				else
					level--;
		}
		if (cur_tok == CHAR_TYPE_OPR)
		{
			token_array[n_tok].sym = id_operator(token_array[n_tok].text, token_array[n_tok].text_len);
		}
//	}

	return n_tok;
}

//This function will print an error message and nullify an invalid expression to prevent it from processing
int expr_error(const char *msg, int tok_n, struct token *token_array)
{
	printf("Error related to token #%d (\"",tok_n);
	int i;
	for (i=0;i<token_array[tok_n].text_len;i++)
		printf("%c",token_array[tok_n].text[i]);
	printf("\"): %s\n",msg);
	return -1;
}

// This function should find the "l arg" for an operation
int find_l_arg(int op_n, struct token *token_array)
{
	int i,op_level = token_array[op_n].level;
	for (i=op_n-1; i>=0; i--)
	{
		if (token_array[i].level < op_level)
			return expr_error("Missing lvalue operand", op_n, token_array);

		if (token_array[i].type == CHAR_TYPE_NUM)
		{
			token_array[i].type = CHAR_TYPE_NUL;
			return i;
		}
	}
	return expr_error("Missing lvalue operand", op_n, token_array);
}

// This function should find the "r arg" for an operation
int find_r_arg(int op_n, int len, struct token *token_array)
{
	int i,op_level = token_array[op_n].level;
	for (i=op_n+1; i<=len; i++)
	{
		if (token_array[i].level < op_level)
			return expr_error("Missing rvalue operand", op_n, token_array);

		if (token_array[i].type == CHAR_TYPE_NUM)
		{
			token_array[i].type = CHAR_TYPE_NUL;
			return i;
		}
	}
	return expr_error("Missing rvalue operand", op_n, token_array);
}

//This function should find the appropriate args for an operator, perform the operation, and reduce the expression
int process_operator(int op_n, int len, struct token *token_array)
{
	int l,r;
	int result = token_array[op_n].sym;
	int op = token_array[op_n].sym;

	switch (op)
	{
		case OP_ADD:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym + token_array[r].sym;
			break;
		case OP_SUB:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym - token_array[r].sym;
			break;
		case OP_MUL:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym * token_array[r].sym;
			break;
		case OP_DIV:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym / token_array[r].sym;
			break;
		case OP_MOD:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym % token_array[r].sym;
			break;
		case OP_LSFT:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym << token_array[r].sym;
			break;
		case OP_RSFT:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym >> token_array[r].sym;
			break;
		case OP_AND:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym & token_array[r].sym;
			break;
		case OP_OR:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym | token_array[r].sym;
			break;
		case OP_XOR:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym ^ token_array[r].sym;
			break;
		case OP_NOT:
			r = find_r_arg(op_n, len, token_array);
			result = ~token_array[r].sym; // This is a special case (single arg operator) and should not require or consume lvalue
			break;
		case OP_COM:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[r].sym; //This DOES require an lvalue, but it is ignored
			break;
		case OP_EQU:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym == token_array[r].sym;
			break;
		case OP_LT:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym < token_array[r].sym;
			break;
		case OP_GT:
			r = find_r_arg(op_n, len, token_array);
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym > token_array[r].sym;
			break;
		case OP_INC:
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym +1;
			break;
		case OP_DEC:
			l = find_l_arg(op_n, token_array);
			result = token_array[l].sym - 1;
			break;
		default:
			return expr_error("Unknown operator", op_n, token_array);
			break;
	}
	token_array[op_n].sym = result;
	token_array[op_n].type = CHAR_TYPE_NUM;
}

int process_expression(int len, struct token *token_array)
{
	int i,max_level=0;
	for (i=0;i<=len;i++)
		if (token_array[i].level>max_level)
			max_level=token_array[i].level;
	for (;max_level>=0;max_level--)
		for (i=0;i<=len;i++)
		{
			if (token_array[i].type == CHAR_TYPE_OPR && token_array[i].level == max_level)
			{
				process_operator(i, len, token_array);
			}
		}

	//print result
	for (i=0;i<=len;i++)
	{
//		write(1,token_array[i].text,token_array[i].text_len);
		if (token_array[i].type == CHAR_TYPE_NUM)
			printf("%d %x\n",token_array[i].sym,token_array[i].sym);
	}
}
