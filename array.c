#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define __NR_set_array 457
#define __NR_get_array 458

int CharNum2Int(char *);

int main(int argc, char *argv[]){
	int index, val;
	long long ret;
	if(argc==3 && strcmp("get", argv[1])==0){
		index = CharNum2Int(argv[2]);
		ret = syscall(__NR_get_array, index);
		if(ret == -1){
			printf("GET FAILED\n");
		}else{
			printf("Arr[%d] = %d\n", index, ret);
		}
	}else if(argc==4 && strcmp("set", argv[1])==0){
		index = CharNum2Int(argv[2]);
		val = CharNum2Int(argv[3]);
		ret = syscall(__NR_set_array, index, val);
		if(ret == -1){
			printf("SET FAILED\n");
		}
	}else{
		printf("Usage:\n%s set [index] [data]\n%s get [index]\n", argv[0], argv[0]);
	}
	return 0;
}

int CharNum2Int(char *num){
	int i, j, ten;
	int val = 0;
    int sign = 0;
    if(*num == '-'){
        sign = 1;
    }
	for(i=sign;i<strlen(num);i++){
		ten = 1;
		for(j=sign;j<strlen(num)-1-i+sign;j++){
			ten *= 10;
		}	
		val += (*(num+i)-'0')*ten;
	}
    if(sign)
        val *= -1;
	return val;
}