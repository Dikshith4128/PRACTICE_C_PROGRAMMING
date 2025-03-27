#include <stdio.h>

void printf_list(Node *head);

typedef struct node {
	int value;
	struct node *next;
}Node;


int main (){
	Node a, b, c;
	a.value = 5;
	b.value = 5;
	c.value = 5;
	a.next = &b;
	b.next = &c;
	c.next = NULL;


return 0;
}
void printf_list(Node *head){
	Node *current;
	current = he


}
