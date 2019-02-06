#ifndef AUXQUEUE_H
#define AUXQUEUE_H

/*Circular Dependency between Heap.h and AuxQueue.h*/
struct Heap ;

#include "heap.h"

/*This struct will be the data of each node in the AuxQueue*/
struct aux_struct
{
	float bill;
	float percent;
	char* caller;
};

struct AuxQueueNode
{
	struct AuxQueueNode* next;
	struct aux_struct data;
};


struct AuxQueue
{
	struct AuxQueueNode* front;
	struct AuxQueueNode* rear;
};


/*Creates an AuxQueue*/
void AuxQueueCreate(struct AuxQueue**);

/*returns true if AuxQueue is empty*/
int AuxQueueIsEmpty(struct AuxQueue*);

/*Inserts into the heap all the nodes of the queue*/
void ReInsert(struct AuxQueue*,struct Heap*);

/*Adds a new node to the rear end of the queue*/
void AuxEnqueue(struct AuxQueue* ,char* ,float,float);

/*Deletes a node from the front end of the queue*/
void AuxDequeue(struct AuxQueue*);

/*Destroys the queue*/
void AuxQueueDestroy(struct AuxQueue*);

/*Prints queue nodes*/
void PrintAuxQueue(struct AuxQueue*);

#endif
