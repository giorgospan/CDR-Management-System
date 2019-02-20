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
void heap_create(struct Heap**);

/*Destroys the Heap*/
void heap_destroy(struct Heap*);

/*Destroys recursively each node*/
void heap_destroy_rec(struct HeapNode*);

/*Inserts a  new element into the heap*/
void heap_insert(struct Heap*,char*,float);

/*Deletes an element from the heap*/
void heap_delete(struct Heap*,char*,float);

/*Adds a new node */
void heap_add_node(struct Heap*,struct HeapNode** ,struct HeapNode*,char,char*,float );

/*Extracts Root's value*/
int extract_root(struct Heap* heap,struct aux_struct*);

/*Shifts up element until the heap property is satisfied*/
void shift_up(struct Heap*,struct HeapNode*);

/*Shifts down element until the heap property is satisfied*/
void shift_down(struct Heap* heap,struct HeapNode* node);

/*Prints Heap nodes using BFS*/
void heap_print(struct Heap*);

/*Prints top subscribers that make top k % of revenue*/
void heap_top(struct Heap*,float);

/*Swaps two nodes*/
/*Just their stored data [caller and bill] not the whole nodes*/
void swap(struct HeapNode*,struct HeapNode*);

#endif
