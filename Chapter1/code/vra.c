#include<stdio.h>

int main (){
	int i = 0;
	char ch = 'C';
	float f = 22.22;
	printf("%d\n%c\n%0.2f\n",i,ch,f);
	printf("\n");
	int  ii;
	char chh;
	float ff;
	ii = i+20;
	chh = ch;
	ff = f+.01;
	printf("%d\n%c\n%f\n",ii,chh,ff);

	while ( i <= ii){
        	printf("%d\t",i);
		i+=1;
	}
	printf("\n");
	char st [10] = "aaa";
	printf("%s",st);	
	printf("\n");
return 0;
}
