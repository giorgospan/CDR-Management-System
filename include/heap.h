#ifndef HEAP_H
#define HEAP_H

struct aux_struct;

#include "auxqueue.h"


struct HeapNode
{
	struct HeapNode* parent;
	struct HeapNode* right;
	struct HeapNode* left;
	char* caller;
	float bill;
};

struct Heap
{
	struct HeapNode* root;
	struct HeapNode* last;
	int counter;
	float revenue;
};


/*Creates Heap*/
void HeapCreate(struct Heap**);


/*Destroys the Heap*/
void HeapDestroy(struct Heap*);

/*Destroys recursively each node*/
void HeapDestroyRecursive(struct HeapNode*);

/*Inserting a  new element into the heap*/
void HeapInsert(struct Heap*,char*,float);

/*Adds a new node */
void HeapAddNode(struct Heap*,struct HeapNode** ,struct HeapNode*,char,char*,float );

/*Extracts Root's value*/
int ExtractRoot(struct Heap* heap,struct aux_struct*);

/*Shifts up elements until the heap property is satisfied*/
void shift_up(struct Heap*,struct HeapNode*);

/*Shifts down elements until the heap property is satisfied*/
void shift_down(struct Heap* heap,struct HeapNode* node);

/*Prints Heap nodes using BFS*/
void PrintHeap(struct Heap*);

/*Prints top subscribers that make top k % of revenue*/
void HeapTop(struct Heap*,float);

/*Swaps two nodes*/
/*Just their stored data [caller and bill] not the whole nodes*/
void swap(struct HeapNode*,struct HeapNode*);

#endif
