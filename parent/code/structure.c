#include <stdio.h>
#include <string.h>

struct mystruct{

	int num;
	char ch;
	char arr[10];

};

int main(){

	struct mystruct s1, s2;
	
	printf("Enter num,ch,arr for s1\n");
	scanf("%d",&(s1.num));
	getchar();
	scanf("%c",&(s1.ch));
	scanf("%s",s1.arr);
	
	printf("Enter num,ch,arr for s2\n");
	scanf("%d",&(s2.num));
	getchar();
	scanf("%c",&(s2.ch));
	scanf("%s",s2.arr);
	
	printf("S1: %d, %c, %s\nS2: %d, %c, %s\n",s1.num,s1.ch,s1.arr,s2.num,s2.ch,s2.arr);
	
	

return 0;
}

