#include <stdio.h>
long int factorial(int data1);

int main()
{

	int num;
	
	printf("Enter the number\n");
	scanf("%d",&num);
	printf("ANS = %ld\n", factorial(num));

return 0;
}

long int factorial(int data){
		long int ans = 1;
		for(int i =1; i<=data; i++)
			ans = ans*i;

return ans;
}
