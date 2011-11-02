/*
 * jpeghuffman.c
 *
 * Created: 01/11/2011 17:06:17
 *  Author: mh23g08
 */ 


#include <avr/io.h>
#include <stdlib.h>

#define BOOL int
#define FALSE 0
#define TRUE 1

struct HuffmanNode_t {
	struct HuffmanNode_t* onechild;
	struct HuffmanNode_t* zerochild;
	struct HuffmanNode_t* parent;
	BOOL leafNode;
	char code;
	
};

typedef struct HuffmanNode_t HuffmanNode;

void initHuffmanNode(HuffmanNode* nodeToInit) {
	nodeToInit->onechild = NULL;
	nodeToInit->zerochild = NULL;
	nodeToInit->parent = NULL;
}

HuffmanNode* newHuffmanNode() {
	HuffmanNode* huffmanNode = malloc(sizeof(HuffmanNode));
	initHuffmanNode(huffmanNode);
	return huffmanNode;
}

struct LinkedListNode_t{
	struct LinkedListNode_t* nextItem;
	struct LinkedListNode_t* previousItem;
	void* dataPointer;
};

typedef struct LinkedListNode_t LinkedListNode;

struct LinkedList_t {
	LinkedListNode* rootItem;
	LinkedListNode* endItem;
};

typedef struct LinkedList_t LinkedList;

void insertLinkedListNodeAfter(LinkedList* linkedList, LinkedListNode* itemToAddAfter, LinkedListNode* itemToAdd) {
	LinkedListNode* prevItemToAddAfterNext = itemToAddAfter->nextItem;
	itemToAddAfter->nextItem = itemToAdd;
	itemToAdd->previousItem = itemToAddAfter;
	itemToAdd->nextItem = prevItemToAddAfterNext;
	if (prevItemToAddAfterNext == NULL)
	{
		// was a end node, therefore we can update linkedList
		linkedList->endItem = itemToAdd;
	}
}



void initLinkedListItem(LinkedListNode* nodeToInit) {
	nodeToInit->nextItem = NULL;
	nodeToInit->previousItem = NULL;
	nodeToInit->dataPointer = NULL;
}

LinkedListNode* newLinkedListItem(void* dataPointer) {
	LinkedListNode* llNode = malloc(sizeof(LinkedListNode));
	initLinkedListItem(llNode);
	llNode->dataPointer = dataPointer;
	return llNode;
}

void initLinkedList(LinkedList* linkedList) {
	linkedList->rootItem = NULL;
	linkedList->endItem = NULL;
}

void addLinkedListItemToEnd(LinkedList* linkedList, LinkedListNode* nodeToAdd) {
	if (linkedList->rootItem == NULL) {
		linkedList->rootItem = nodeToAdd;
		linkedList->endItem = nodeToAdd;
	} else {
		insertLinkedListNodeAfter(linkedList, linkedList->endItem, nodeToAdd);
	}
}

void clearLinkedList(LinkedList* linkedList) {
	LinkedListNode* nextItem = linkedList->rootItem;
	linkedList->rootItem = NULL;
	while(nextItem != NULL) {
		LinkedListNode* currentItem = nextItem;
		nextItem = currentItem->nextItem;
		currentItem->nextItem = NULL;
		currentItem->previousItem = NULL;
		free(currentItem);
	}
}

int main(void)
{
	char* huffmanCodes[17];
	int numCodes[17] = { 0, 0, 2, 3, 4, 4, 2, 5, 2, 4, 4, 4, 5, 3, 3, 2, 7 };
	
	char huff2[] = { 0x01, 0x02 };
    huffmanCodes[2] = huff2;
	char huff3[] = { 0x03 };
    huffmanCodes[3] = huff3;
	char huff4[] =  { 0x04, 0x05, 0x11 };
    huffmanCodes[4] = huff4;
	char huff5[] = { 0x00, 0x06, 0x21 };
    huffmanCodes[5] = huff5;
	char huff6[] = { 0x12, 0x31 };
    huffmanCodes[6] = huff6;
	char huff7[] = { 0x07, 0x13, 0x22, 0x41, 0x51 };
    huffmanCodes[7] = huff7;
	char huff8[] = { 0x14, 0x61 };
    huffmanCodes[8] = huff8;
	char huff9[] = { 0x08, 0x32, 0x71, 0x81 };
    huffmanCodes[9] = huff9;
	char huff10[] = { 0x15, 0x23, 0x91, 0xA1 };
    huffmanCodes[10] = huff10;
	char huff11[] = { 0x42, 0xB1, 0xC1, 0xF0 };
    huffmanCodes[11] = huff11;
	char huff12[] = { 0x16, 0x33, 0x52, 0xD1, 0xF1 };
    huffmanCodes[12] = huff12;
	char huff13[] = { 0x09, 0x24, 0xE1 };
    huffmanCodes[13] = huff13;
	char huff14[] = { 0x17, 0x34, 0x43 };
    huffmanCodes[14] = huff14;
	char huff15[] = { 0x62, 0x25 };
    huffmanCodes[15] = huff15;
	char huff16[] = { 0x72, 0x53, 0x18, 0x44, 0x82, 0x92, 0xD2 };
    huffmanCodes[16] = huff16;

	HuffmanNode* rootNode = newHuffmanNode();
	rootNode->leafNode = FALSE;
	
	// make two linked lists, one for storing the branch nodes from the last bitstring length level and one
	// for storing the branch nodes from the current bitstring length level
	LinkedList lastLevelBranchNodes;
	initLinkedList(&lastLevelBranchNodes);

	addLinkedListItemToEnd(&lastLevelBranchNodes, newLinkedListItem(rootNode));
	
	LinkedList currentLevelBranchNodes;
	initLinkedList(&currentLevelBranchNodes);
	
	// it is a bit confusing because linkedLists have nodes and so does the tree
	

	// for each bitstring length (i.e. tree level)
	for (int i = 0; i < 17; i++)
	{
		 clearLinkedList(&currentLevelBranchNodes);
		 int leafNodes = 0;
		 
		 // for each branch node (i.e. node that is not a leaf node) in the last level of nodes
		 LinkedListNode* currentLinkedListItem = lastLevelBranchNodes.rootItem;
		 while(currentLinkedListItem != NULL) {
			 HuffmanNode* currentHuffmanNode = (HuffmanNode*)currentLinkedListItem->dataPointer;
			// we go from zero to one, so zero first
            // ZERO branch
            // see if we have any more leaf nodes to assign
			if (leafNodes < numCodes[i])
			{
				// we have leaf nodes left to assign, so make a new leaf node and assign the current codeword to it
				HuffmanNode* newZeroNode = newHuffmanNode();
				newZeroNode->leafNode = TRUE;
				currentHuffmanNode->zerochild = newZeroNode;
				newZeroNode->parent = currentHuffmanNode;
				newZeroNode->code = huffmanCodes[i][leafNodes];
                leafNodes++;
			} else {
				HuffmanNode* newZeroNode = newHuffmanNode();
				newZeroNode->leafNode = FALSE;
				newZeroNode->parent = currentHuffmanNode;
				currentHuffmanNode->zerochild = newZeroNode;
				addLinkedListItemToEnd(&currentLevelBranchNodes, newLinkedListItem(newZeroNode));
			}
			
			// now we do the one branch
            // ONE branch
            // see if we have any more leaf nodes to assign
			if (leafNodes < numCodes[i])
			{
				// we have leaf nodes left to assign, so make a new leaf node and assign the current codeword to it
				HuffmanNode* newOneNode = newHuffmanNode();
				newOneNode->leafNode = TRUE;
				currentHuffmanNode->onechild = newOneNode;
				newOneNode->parent = currentHuffmanNode;
				newOneNode->code = huffmanCodes[i][leafNodes];
                leafNodes++;
			} else {
				HuffmanNode* newOneNode = newHuffmanNode();
				newOneNode->leafNode = FALSE;
				newOneNode->parent = currentHuffmanNode;
				currentHuffmanNode->onechild = newOneNode;
				addLinkedListItemToEnd(&currentLevelBranchNodes, newLinkedListItem(newOneNode));
			}
					 
			currentLinkedListItem = currentLinkedListItem->nextItem; // iterate to the next node in the list
		 }
		 
		 // copy the linked list over
		clearLinkedList(&lastLevelBranchNodes);
		LinkedListNode* currLLCopyItem = currentLevelBranchNodes.rootItem;
		while(currLLCopyItem != NULL) {
			addLinkedListItemToEnd(&lastLevelBranchNodes, newLinkedListItem(currLLCopyItem->dataPointer));
			currLLCopyItem = currLLCopyItem->nextItem;
		}
	}
}