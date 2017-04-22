#define STATE_OPEN 0
#define STATE_ENTERING_DIRECTIVE 1
#define STATE_ENTERING_SECTION 2
#define STATE_IN_BLOCK 3
#define STATE_IN_EXPRESSION 4
#define STATE_DECLARATION 5
#define STATE_IDENTIFIER 6


struct state
{
	qword current_state;
	qword previous_state;
	dword block_level;
	dword group_level;
	qword token_num;
};
