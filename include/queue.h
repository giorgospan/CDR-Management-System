#ifndef QUEUE_H
#define QUEUE_H


#include "heap.h"

struct Queue
{
	struct QueueNode* front;
	struct QueueNode* rear;
};


struct QueueNode
{
	struct QueueNode* next;
	struct HeapNode* data;
};


void QueueCreate(struct Queue**);

int QueueIsEmpty(struct Queue*);

void Enqueue(struct Queue* ,struct HeapNode*);

struct HeapNode* Dequeue(struct Queue*);

void QueueDestroy(struct Queue*);

void Print(struct Queue*);

#endif
