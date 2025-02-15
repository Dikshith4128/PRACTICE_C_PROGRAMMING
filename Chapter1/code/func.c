#include <stdio.h>

int summ(int a,int b);
int subb(int c,int d);
int multi(int e,int f);
int divv(int g,int h);

int main(){

	int num1, num2, choice;
	
	printf("ENter num1 & num2 \n");
	scanf("%d",&num1);
	scanf("%d",&num2);
	printf("\n SELECT_YOUR_choice\n1.addition\n2.subtraction\n3.multiplication\n4.division\n");
	scanf("%d",&choice);
	switch(choice){

	case 1:
		printf("ANS: %d\n",summ(num1,num2));
		break;
	
	case 2:
		printf("ANS: %d\n",subb(num1,num2));
		break;
	
	case 3:
		printf("ANS: %d\n",multi(num1,num2));
		break;
	
	case 4:
		printf("ANS: %d\n",divv(num1,num2));
		break;
	
	default:
		break;

	}
return 0;
}



int summ(int a,int b){
	
	int sum;

	sum = a + b;
	return sum;
}

int subb(int a,int b){
	
	int sum;

	sum = a - b;
	return sum;
}
int multi(int a,int b){
	
	int sum;

	sum = a * b;
	return sum;
}
int divv(int a,int b){
	
	int sum;

	sum = a / b;
	return sum;
}
