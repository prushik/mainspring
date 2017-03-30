#define TOK_TYPE_SYMBOL 1
#define TOK_TYPE_NUMBER 2
#define TOK_TYPE_STRING 4
#define TOK_TYPE_OPERAT 8
#define TOK_TYPE_KEYWRD 16
#define TOK_TYPE_GROUPS 32
#define TOK_TYPE_WHTSPC 64

struct token
{
	int type;
	int level;
	int sym;
	int text_len;
	char *text;
	int argc;
	struct token *argv[6];
};

static const char *reserved[] = {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","static","struct","switch","typedef","union","unsigned","void","volitile","while","_Packed"};

#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_DIV 3
#define OP_MOD 4
#define OP_LSFT 5
#define OP_RSFT 6
#define OP_SET 7
#define OP_EQU 8
#define OP_NEQ 9
#define OP_AND 10
#define OP_OR 11
#define OP_LAND 12
#define OP_LOR 13
#define OP_LNO 14
#define OP_XOR 15
#define OP_NOT 16
#define OP_GT 17
#define OP_LT 18
#define OP_COM 19
static const char *operator[] = {"+","-","*","/","%","<<",">>","=","==","!=","&","|","&&","||","!","^","~",">","<",","};

#define CHAR_TYPE_INV -1
#define CHAR_TYPE_NUM 0
#define CHAR_TYPE_STR 1
#define CHAR_TYPE_WHT 2
#define CHAR_TYPE_GRP 3
#define CHAR_TYPE_OPR 4
#define CHAR_TYPE_NUL 5
static const char *char_ini[] = {"0123456789","_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"," \r\n\t\0","([{}])","+-*/%><=!&|,^~"};
static const char *char_set[] = {"0123456789xabcdefABCDEF","_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"," \r\n\t\0","([{}])","+-*/%><=!&|,^~"};

int id_token_str(const char *token);
int id_operator(const char *op_str, int len);
int tokenize(const char *expr, unsigned int len);
int expr_error(const char *msg, int tok_n);
int find_l_arg(int op_n);
int find_r_arg(int op_n, int len);
int process_operator(int op_n, int len);
int process_expression(int len);
