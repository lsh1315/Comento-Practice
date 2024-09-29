#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define __NR_stack_init 454
#define __NR_stack_push 455
#define __NR_stack_pop 456

long long StringToLong(char *);
void LongToString(long long, char *);

int main(int argc, char *argv[]){

	long long ret, data;

	if(argc == 2 &&	strcmp(argv[1], "init") == 0){
		syscall(__NR_stack_init);
	}else if(argc == 2 && strcmp(argv[1], "pop") == 0){
		ret = syscall(__NR_stack_pop);
		if(ret == -1){
			printf("POP FAIL\n");
		}else{
			char *str = calloc(8, sizeof(char));
			LongToString(ret, str);
			printf("POP : %s\n", str);
		}
	}else if(argc >= 3 && strcmp(argv[1], "push") == 0){
		for(int i=2; i<argc; i++){
			data = StringToLong(argv[i]);
			ret = syscall(__NR_stack_push, data);
			if(ret == 0){
				printf("%s PUSH SUCCESS!\n", argv[i]);
			}else{
				printf("%s PUSH FAIL\n", argv[i]);
			}
		}
	}else{
		printf("Usage: %s < init | push | pop > [data]\n", argv[0]);
		return -1;
	}

	return 0;
}

long long StringToLong(char *str){

    long long result = 0;
    long long temp;
    
    for(int i=0; i<strlen(str) && i<8; i++){
        temp = *(str+i);                        // 덮어쓰기X, 0으로 채움
        temp = temp<<(8*i);
        result = result|temp;
    }

    return result;
}

void LongToString(long long data, char *str){
    
    long long filter = 0x00000000000000ff;
    long long temp;
    char word;

    for(int i=0;i<8;i++){
        temp = data & filter;
        temp = temp>>(8*i);
        word = temp;            //LSB 1byte 저장
        if(!word){
            *(str+i) = '\0';
            break;
        }
        *(str+i) = word;
        filter = filter<<8;
    }
}