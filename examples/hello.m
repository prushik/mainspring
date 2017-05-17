#define sys_write 1
#define sys_exit 60

section text
{

	syscall(sys_write, 1, msg, 14);

	syscall(sys_exit, 0);

	// functions can be defined with explicit calling conventions
	rax syscall(rax number, rdi arg1, rsi arg2, rdx arg3, r10 arg4, r8 arg5, r9 arg6)
	{
		syscall;
		ret;
	}

	msg: byte "Hello World!\n\0";

}
