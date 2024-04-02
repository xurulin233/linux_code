/**************************File Header****************************************
* File Name singlelink.c
*
* Copyright (c) 2020- 2030, Inc.
*
* History:
* 2024/04/02 xucheng@zcom.com.cn - fuck it.
****************************************************************************/



#include <stdio.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()


typedef struct node{
	int data;
	struct node* next;
} LinkNode;




void createlisttail(LinkNode** list,int number) 
{
	LinkNode* node;
	LinkNode* r;
	int i;
	srand(time(0));
	*list = (LinkNode*)malloc(sizeof(LinkNode));

	r = *list;

	for(i=0;i<number;i++)
	{
		node = (LinkNode*)malloc(sizeof(LinkNode));
		node->data = rand()% 100 + 1;
		r->next = node;
		r = node;
	}

	r->next = NULL;
}

void displaylist(LinkNode* list) 
{
	LinkNode * p;
	int i = 0;
	p = list;
	while (p->next != NULL)
	{
		p = p->next;
		printf("%d,",p->data);
		i++;
		if(i%10 == 0) printf("\n");
	
	}
	if(i%10 != 0) printf("\n");
	printf("list number is %d\n",i);
}



int main()
{
	LinkNode *linkA;

	createlisttail(&linkA,20);
	displaylist(linkA);

}

