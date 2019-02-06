/*Queue - Linked List implementation*/

#include <stdio.h>
#include <stdlib.h>


#include "heap.h"
#include "queue.h"



void QueueCreate(struct Queue** queue)
{
	
	*queue=malloc(sizeof(struct Queue));
	(*queue)->front=(*queue)->rear=NULL;
}

int QueueIsEmpty(struct Queue* queue)
{
	return queue->front==NULL;
}

void Enqueue(struct Queue* queue,struct HeapNode* x)
{
	
	struct QueueNode* temp = malloc(sizeof(struct QueueNode));
		
	temp->data =x; 
	temp->next = NULL;
	
	if(queue->front == NULL && queue->rear == NULL)
	{
		queue->front = queue->rear = temp;
		return;
	}
	queue->rear->next = temp;
	queue->rear = temp;
}


struct HeapNode* Dequeue(struct Queue* queue)
{
	struct QueueNode* temp = queue->front;
	struct HeapNode* return_value = queue->front->data;
	if(queue->front == NULL)
	{
		printf("Dequeue: Queue is Empty\n");
		return;
	}
	if(queue->front == queue->rear)
	{
		queue->front = queue->rear = NULL;
	}
	else 
	{
		queue->front = queue->front->next;
	}
	free(temp);
	return return_value;
}

void QueueDestroy(struct Queue* queue)
{
	struct QueueNode* current;
	struct QueueNode* temp;
	
	current=queue->front;
	while(current)
	{
		/*Must NOT free the HeapNode stored in each QueueNode*/
		temp=current;
		current=current->next;
		free(temp);
	}
	free(queue);
}


void Print(struct Queue* queue) 
{
	struct QueueNode* temp = queue->front;
	while(temp != NULL) 
	{
		printf("%p | ",temp->data);
		temp = temp->next;
	}
	printf("\n");
}
