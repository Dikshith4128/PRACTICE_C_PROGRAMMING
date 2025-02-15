#include <stdio.h>

int main(){

	int num = 100;
	int *mm = &num;

	printf("num %d\n",num);
	printf("num %d\n",*mm);
	printf("memory address %p\n",&num);
	printf("nenory address of num is using mm %p\n",mm);
	

return 0;
}
