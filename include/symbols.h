#define SYM_TYPE_LABEL 1
#define SYM_TYPE_REGISTER 2
#define SYM_TYPE_FUNCTION 3
#define SYM_TYPE_VARIABLE 4
#define SYM_TYPE_KEYWORD 5
#define SYM_TYPE_UNKNOWN 6

struct symbol
{
	int token_num; // token number in stream
	const char *text; // text of identifier name
	int text_len; // length of identifier name
	native addr; // address in memory
	native reg; // register designated to hold data
	int size; // size in bits (or 0 for unknown)
	int type; // the type
};
