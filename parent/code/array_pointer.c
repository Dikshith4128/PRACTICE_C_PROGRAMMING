#include <stdio.h>


int main(){

	int arr[5] = {4,2,9,5,6};
	int *ptr = arr;

	for (int i =0; i < 5; i++){
		printf("Address : %p, value: %d\n",(ptr + i), *(ptr + i));
	}



return 0;	
}
