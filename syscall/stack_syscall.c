#include <linux/kernel.h>
#include <linux/syscalls.h>

#define STACK_SIZE 10
static long long Stack[STACK_SIZE];
static int top;

SYSCALL_DEFINE0(stack_init){
	printk(KERN_INFO "STACK INITIALIZED\n");
	top = 0;
	return 0;
}       

SYSCALL_DEFINE1(stack_push, long long, val){
	if(top>=10){
		printk(KERN_INFO "STACK OVERFLOW!\n");
		return -1;
	}
	Stack[top++] = val;
	return 0;
}

SYSCALL_DEFINE0(stack_pop){
	if(top==0){
		printk(KERN_INFO "STACK UNDERFLOW!\n");
		return -1;
	}
	return Stack[--top];
}
