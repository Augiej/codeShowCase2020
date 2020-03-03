#include <stdio.h>
#include <stdlib.h>
#include "treeList.h"
void generateFreqTable(char* fileName, int charCounts[128][2]);
void createHuffmanTree(Tree* parent, int freqTable[][2]);
int encodeFile(char* fileName, Tree* parent, char* writeFile);
void decodeFile(char* fileCoded, char* fileDecoded, Tree* parent, int size);

//Generate the freq table
void generateFreqTable(char* fileName, int charCounts[][2]){
	FILE *file = fopen(fileName, "r");
	
	char c;
	while((c = fgetc(file)) != EOF){
		charCounts[c][1] += 1;
	}
	if(file == NULL){
		printf("file is null");
	}
	int temp[1][2];
	for(int i = 0; i < 128; i ++){
		//printf("%d, %d \n", charCounts[i][0], charCounts[i][1]);
	}
	// sort the double array of char in the 1st column and count in the second
	for(int i = 0; i < 128; i++){
		temp[0][1] = 0;
		temp[0][0] = 0;
		int minIndex = i;
		for(int j = i + 1; j < 128; j++){
			if(charCounts[j][1] < charCounts[minIndex][1]){
				minIndex = j;
			}
		}
		temp[0][0] = charCounts[minIndex][0];
		temp[0][1] = charCounts[minIndex][1];
			
		charCounts[minIndex][0] = charCounts[i][0];
		charCounts[minIndex][1] = charCounts[i][1];
			
		charCounts[i][0] = temp[0][0];
		charCounts[i][1] = temp[0][1];
	}
}
//create huffman tree
void createHuffmanTree(Tree* parent, int freqTable[][2]){
	int countOfZeros = 0;
	for(int i = 0; i < 128; i++){
		if(freqTable[i][1] == 0){
			countOfZeros ++;
		}
	} 
	//printf("%d \n", countOfZeros);
	//make leaf nodes
	LinkedList* myList = llCreate();
	for(int i = countOfZeros; i < 128; i++){
		Tree* insertTree = (Tree*)malloc(sizeof(Tree));
		insertTree->parent = NULL;
		insertTree->right = NULL;
		insertTree->left = NULL;
		treePopulate(insertTree, freqTable[i][0], freqTable[i][1]);
		llAdd(&myList, insertTree);
		//DFS(insertTree);
	}
	LinkedList* tempOne = llCreate();
	LinkedList* tempTwo = llCreate();
	LinkedList* iterate = myList;
	while(sizeOfList(myList) > 2){
		char check = 0;
		Tree* newTree = (Tree*)malloc(sizeof(Tree));
		if((*myList).next != NULL){
			tempOne = myList;
			//printf("%d \n", (*myList).treeStor->weight);
			tempTwo = (*myList).next;
			iterate = myList;
			for(int j = 0; j < sizeOfList(myList); j ++){
				if(iterate-> next != NULL){
					iterate = iterate->next;
					if((*tempOne).treeStor->weight >= (*iterate).treeStor->weight){
						tempTwo = tempOne;
						tempOne = iterate;
						check = 1;
					}
					else if((*tempTwo).treeStor->weight > (*iterate).treeStor->weight){
						tempTwo = iterate;
						check = 0;
					}
				}
			}
			//printf("%d, %d \n", (*tempOne).treeStor->weight, (*tempTwo).treeStor->weight);
			
			if(check == 0){
				//printf("check 0 \n");
				treeMerge(tempOne, tempTwo, newTree, myList);
			}
			else{
				//printf("check 1 \n");
				treeMerge(tempTwo, tempOne, newTree, myList);
			}
		}
	}
	tempOne = myList;
	tempTwo = (*myList).next;
	//printf("%d, %d \n", (*tempOne).treeStor->weight, (*tempTwo).treeStor->weight);
	treeMerge(tempOne, tempTwo, parent, myList);
	//DFS(parent);
	//merge the nodes to make the tree
}


//encode the file
int encodeFile(char* fileName, Tree* root, char* writeFile){
	//printf("prevNode: %p ", root->parent);
	//printf("encode file \n");
	FILE *file = fopen(fileName, "r");
	FILE *writeToFile = fopen(writeFile, "w");
	char target;
	char buffer = 0;
	Tree** leaf = (Tree**)malloc(sizeof(Tree));
	int index = 7;
	int count = 0;
	int binaryCount = 0;
	int arr[127];
	//setBit(&buffer, index);
	//printf("%d\n", buffer);
	while((target = fgetc(file)) != EOF){
		//printf("%d", target);
		findNode(root, target, leaf); 
		//printf(" Target: %d, Found: %c ", target, (*leaf)->c);
		Tree* prevNode = (*leaf);
		//printf("prevNode: %p ", (*leaf)->parent);
		while((*leaf)->parent != NULL){
			if((*leaf)->isLeft == 1){
				//printf("1 ");
				arr[binaryCount] = 1;
				binaryCount++;
			}
			else{
				//printf("0 ");
				arr[binaryCount] = 0;
				binaryCount++;
			}
			(*leaf) = (*leaf)->parent;
		}
		//printf("Binary Count: %d\n", binaryCount);
		for(int i = binaryCount - 1; i >= 0; i--){
			if(arr[i] == 1){
				setBit(&buffer, index);
				//printByte(buffer);
				//printf("%d ", index);
				index--;
				}
			else{
				clearBit(&buffer, index);
				//printByte(buffer);
				//printf("%d ", index);
				index--;
			}
			if(index < 0){
				count++;
				fputc(buffer, writeToFile);
				buffer = 0;
				
				index = 7;		
				}
			}
			binaryCount = 0;
		}
	fputc(buffer, writeToFile);
	count ++;
	//printf("\ncount %d \n", count);
	fclose(file);
	fclose(writeToFile);
	return count; 
}


//decode the file
void decodeFile(char* fileCoded, char* fileDecoded, Tree* root, int size){
	FILE *file = fopen(fileCoded, "r");
	FILE *writeToFile = fopen(fileDecoded, "w");
	char target;
	int index = 7;
	Tree** iterator = (Tree**)malloc(sizeof(Tree));
	(*iterator) = root;
	//printf("%d \n", isLeaf((*iterator)));
	for(int i = 0; i < size; i ++){
		target = fgetc(file);
		//printf("target: ");
		//printByte(target);
		//printf("\n");
		for(int j = 7; 0<=j; j--){
			//printByte(target);
			//printf("isleaf? %d\n", isLeaf(*iterator));
			if(isLeaf((*iterator)) != 1){
				//printf("getcurrentbit %d\n", getBit(target, j));
				if(getBit(target, j) == 1){
					(*iterator) = (*iterator)->left;
				}
				else{
					(*iterator) = (*iterator)->right;
				}
				if(isLeaf((*iterator)) == 1){
				//printf("%c \n", (*iterator)->c );
				putc((*iterator)->c, writeToFile);
				(*iterator) = root;
				}
			}
			
			else{
				//printf("%c \n", (*iterator)->c );
				putc((*iterator)->c, writeToFile);
				(*iterator) = root;
			}
		}
	}
	//printf("\n");
}




int main(){
	char myFile[] = "decind.txt";
	char writeFile[] = "compressedFile.huf";
	char decodedFile[] = "decompressedFile.txt";
	int myArray [128][2];
	for(int i = 0; i < 128; i ++){
		myArray[i][0] = i;
		myArray[i][1] = 0;
	}
	
	generateFreqTable(myFile, myArray);
	Tree* myTree = (Tree*)malloc(sizeof(Tree));
	createHuffmanTree(myTree, myArray);
	
	int size = encodeFile(myFile, myTree, writeFile);
	
	decodeFile(writeFile, decodedFile, myTree, size);
	return 0;
}