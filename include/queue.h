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


void queue_create(struct Queue**);

int queue_is_empty(struct Queue*);

void enqueue(struct Queue* ,struct HeapNode*);

struct HeapNode* dequeue(struct Queue*);

void queue_destroy(struct Queue*);

void queue_print(struct Queue*);

#endif
