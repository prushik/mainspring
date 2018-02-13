#define PARSE_STATE_OPEN 0
#define PARSE_STATE_ENTERING_DIRECTIVE 1
#define PARSE_STATE_ENTERING_SECTION 2
#define PARSE_STATE_IN_BLOCK 3
#define PARSE_STATE_IN_EXPRESSION 4
#define PARSE_STATE_DECLARATION 5
#define PARSE_STATE_IDENTIFIER 6

struct state
{
	qword current_state;
	qword previous_state;
	dword block_level;
	dword group_level;
	qword token_num;
};

// this represents a parse tree for a sentence
struct parse_sentence
{
	qword start_tok; // first token of sentence in token stream
	qword end_tok; // last token of sentence in token stream
	dword block_level;
};

#define PARSE_NODE_BLOCK 1
#define PARSE_NODE_DECLARATION 2
#define PARSE_NODE_DEFINITION 4
#define PARSE_NODE_EXPRESSION 8
#define PARSE_NODE_STATEMENT 16
#define PARSE_NODE_ 0


// A node of the parse tree. I suppose that this should replace sentence.
// or, can't we just like the token structures be the leaves and this be 
// the sentence, and than we have a "parse tree", right?
struct parse_node
{
	qword start_tok; // first token of sentence in token stream
	qword end_tok; // last token of sentence in token stream
	qword node_type;
	struct parse_node *parent;
	struct parse_node *children[2];
};
