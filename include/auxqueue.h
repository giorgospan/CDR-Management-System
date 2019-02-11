#ifndef AUXQUEUE_H
#define AUXQUEUE_H

/*Circular Dependency between heap.h and auxqueue.h*/
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
void aux_queue_create(struct AuxQueue**);

/*returns true if AuxQueue is empty*/
int aux_queue_is_empty(struct AuxQueue*);

/*Inserts into the heap all the nodes of the queue*/
void re_insert(struct AuxQueue*,struct Heap*);

/*Adds a new node to the rear end of the queue*/
void aux_enqueue(struct AuxQueue* ,char* ,float,float);

/*Deletes a node from the front end of the queue*/
void aux_dequeue(struct AuxQueue*);

/*Destroys the queue*/
void aux_queue_destroy(struct AuxQueue*);

/*Prints queue nodes*/
void aux_queue_print(struct AuxQueue*);

#endif
