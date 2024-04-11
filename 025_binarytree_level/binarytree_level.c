
#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100


typedef struct TreeNode{
	int data;
	struct TreeNode* lchild;
	struct TreeNode* rchild;
} BiNode, *BiTree;

typedef struct queue {

	struct TreeNode* numQueue[MaxSize];
	int front;
	int rear;
} Queue;

Queue  queue;


void initQueue() {
	queue.front = 0;
	queue.rear = 0;
}


void Push(BiTree root) {

	queue.numQueue[++queue.rear] = root;
}

BiTree Pop() {

	return queue.numQueue[++queue.front];

}

int empty() {

	return queue.rear == queue.front;
}


void LevelOrder(BiTree root) {
	BiTree p;
	Push(root);
	while( !empty() ) {
	
	p = Pop();
	printf("%d ",p->data);
	if(p->lchild)
		Push(p->lchild);

	if(p->rchild) 
		Push(p->rchild);
		
	}

}

BiTree createtree() {
	int data;
	scanf("%d",&data);
	BiTree root;

	if (data <= 0){
		return NULL;
	} else {
	
		root = (BiTree)malloc(sizeof(BiNode));
		root->data = data;
		root->lchild = createtree();
		root->rchild = createtree();
	} 
	return root;
}





int main() {


	BiTree root = NULL;
	root = createtree();
	LevelOrder(root);
	printf("\n");
}



