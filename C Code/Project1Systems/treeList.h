#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h>


typedef struct treeNode {
  int weight;
  struct treeNode* left;
  struct treeNode* right;
  struct treeNode* parent;
  char c;
  char isLeft;
} Tree;

typedef struct node {
  struct node* next;
  struct node* prev;
  struct treeNode* treeStor;
} LinkedList;



Tree* treeCreate();
int treeIsEmpty(Tree* ll);
void treePopulate(Tree* myTree, char c, int weight);
void treeMerge(LinkedList* one, LinkedList* two, Tree* parent, LinkedList* head);
LinkedList* llCreate();
void treeAdd(Tree* left, Tree* right, Tree* parent);
void llDisplay(LinkedList* ll);
void llAdd(LinkedList** ll, Tree* newValue);
void llFree(LinkedList* ll);
int sizeOfList(LinkedList* head);
void DFS(Tree* top);
Tree* treeCreate() {
  return NULL;
}

void treePopulate(Tree* myTree, char c, int weight){
	myTree->weight = weight;
	myTree->c = c;
}
void treeListMerge(Tree* one, Tree* two, Tree* parent){
	parent->left = one;
	parent->left->parent = parent;
	parent->left->isLeft = 1;
	parent->right = two;
	parent->right->parent = parent;
	parent->right->isLeft = 0;
	parent->parent = NULL;
}

void treeMerge(LinkedList* one, LinkedList* two, Tree* parent, LinkedList* head){
	int count = sizeOfList(head);
	//printf("count: %d \n", count);
	//llDisplay(head);
	treeListMerge((*one).treeStor, (*two).treeStor, parent); 
	parent->weight = parent->left->weight + parent->right->weight;
	(*one).treeStor = parent;
	if(count > 1){
		if(two->next == NULL){
			two->prev->next = NULL;
		}
		else if(two->prev != NULL){
			two->next->prev = two->prev;
		}
		else if(two->prev == NULL){
			two->next->prev = NULL;
		}
		if(two->next != NULL){
			two->prev->next = two->next;
		}
		/*printf("%p \n", (*one).treeStor->right);
		printf("%p \n", (*two).treeStor->left);*/
		/*printf("%p \n", (*parent).left);
		printf("%p \n", (*parent).right);
		printf("%d \n", (*parent).left->weight);
		printf("%d \n", (*parent).right->weight);
		printf("%p \n", (*parent).left->left);
		printf("%p \n", (*parent).left->right);
		printf("%p \n", (*parent).right->left);
		printf("%p \n", (*parent).right->right);
		if((*parent).left->left != NULL){
			printf("%d \n", (*parent).left->left->weight);
		}
		if((*parent).left->right != NULL){
			printf("%d \n", (*parent).left->right->weight);
		}
		if((*parent).right->left != NULL){
			printf("%d \n", (*parent).right->left->weight);
		}
		if((*parent).right->right != NULL){
			printf("%d \n", (*parent).right->right->weight);
		}*/
	}
	else{
		one->next = NULL;
		one->prev = NULL;
		/*printf("%d \n", count);
		printf("%p \n", (*parent).left);
		printf("%p \n", (*parent).right);*/
	}
}

LinkedList* llCreate() {
  return NULL;
}

int llIsEmpty(LinkedList* ll);


int llIsEmpty(LinkedList* ll) {
  return (ll == NULL);
}

void llDisplay(LinkedList* ll) {
  
  LinkedList* p = ll;

  printf("[");
  
  while (p != NULL) {
    printf("%d:%d, ", (*p).treeStor->c, (*p).treeStor->weight);
    p = p->next;
  }

  printf("]\n");
}

void treeAdd(Tree* left, Tree* right, Tree* parent){
	parent->left = left;
	parent->right = right;
	
}
void llAdd(LinkedList** ll, Tree* newValue) {

  // Create the new node
  LinkedList* newNode = (LinkedList*)malloc(1 * sizeof(LinkedList));
  newNode->treeStor = newValue;
  newNode->next = NULL;
  newNode->prev = NULL;
  // Find the end of the list
  LinkedList* p = *ll;
  LinkedList* previous = *ll;

  if (p == NULL) {  // Add first element
    *ll = newNode;  // This is why we need ll to be a **

  } else {

    while (p->next != NULL) {
		previous = p;
		p = p->next;
    }
    
    // Attach it to the end
    p->next = newNode;
	p->prev = previous;
  }

}

void llFree(LinkedList* ll) {
  LinkedList* p = ll;

  while (p != NULL) {
    LinkedList* oldP = p;
    p = p->next;
    free(oldP);
  }
  
}


int sizeOfList(LinkedList* ll){
	int count = 0;
	LinkedList* p = ll;
	
	while (p != NULL) {
		p = p->next;
		 count++;
	}
	return count;
}

void DFS(Tree* top){
	if(top->left != NULL){
		DFS(top->left);
	}
	printf("%d, %c, %d, %d \n", (*top).weight, (*top).c, (*top).c, (*top).isLeft );
	if(top->right != NULL){
		DFS(top->right);
	}
}
void findNode(Tree* top, char target, Tree** found){
	
	if(top->left != NULL){
		findNode(top->left, target, found);
	}
	if(top->right != NULL){
		findNode(top->right, target, found);
	}
	if(top->c == target){
		*found = top;
		//printf("target: %d, found: %d ", target, (*found)->c);
	}
}
void setBit(char* bs, int index){
	*bs |= ((unsigned short int)1 << index);
}
void clearBit(char* bs, int index){
	*bs &= ~((unsigned short int)1 << index);
}
char isLeaf(Tree* myTree){
	if(myTree->right == NULL && myTree->left == NULL){
		return 1;
	}
	else{
		return 0;
	}
}
void printByte(char byte){
	for(int i = 7; 0 <= i; i --)
        printf("%d", (byte >> i) & 0x01);
	printf("\n");
}
int getBit(char byte, int index){
	int myValue = (byte >> index) & 0x01;
	return myValue;
}






