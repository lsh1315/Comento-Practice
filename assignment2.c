#include <linux/kernel.h>
#include <linux/syscalls.h>

#define ARRAY_SIZE 256*8/32

static int Arr[ARRAY_SIZE];

SYSCALL_DEFINE2(set_array, int, index, int, val){
	if(index<0 || index>=ARRAY_SIZE){
		printk(KERN_INFO "WORNG INDEX\n");
		return -1;
	}
	Arr[index]=val;
	return 0;
}

SYSCALL_DEFINE1(get_array, int, index){
	if(index<0 || index>=ARRAY_SIZE){
		printk(KERN_INFO "WORNG INDEX\n");
		return -1;
	}
	return Arr[index];
}
