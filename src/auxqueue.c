/*Queue - Linked List implementation*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auxqueue.h"
#include "heap.h"

void AuxQueueCreate(struct AuxQueue** queue)
{

	*queue=malloc(sizeof(struct AuxQueue));
	(*queue)->front=(*queue)->rear=NULL;
}

int AuxQueueIsEmpty(struct AuxQueue* queue)
{
	return queue->front==NULL;
}

void AuxEnqueue(struct AuxQueue* queue,char* caller,float bill,float percent)
{

	struct AuxQueueNode* temp = malloc(sizeof(struct AuxQueueNode));

	temp->data.bill = bill;
	temp->data.percent=percent;
	temp->data.caller=malloc((strlen(caller)+1)*sizeof(char));
	strcpy(temp->data.caller,caller);
	temp->next = NULL;

	if(queue->front == NULL && queue->rear == NULL)
	{
		queue->front = queue->rear = temp;
		return;
	}
	queue->rear->next = temp;
	queue->rear = temp;
}

void ReInsert(struct AuxQueue* queue,struct Heap* heap)
{
	struct AuxQueueNode* current;
	current=queue->front;

	while(current)
	{
		HeapInsert(heap,current->data.caller,current->data.bill);
		current=current->next;
		Dequeue(queue);
	}
}

void AuxDequeue(struct AuxQueue* queue)
{
	struct AuxQueueNode* temp = queue->front;
	if(queue->front == NULL)
	{
		printf("Dequeue: AuxQueue is Empty\n");
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
	free(temp->data.caller);
	free(temp);
}

void AuxQueueDestroy(struct AuxQueue* queue)
{
	struct AuxQueueNode* current;
	struct AuxQueueNode* temp;

	current=queue->front;
	while(current)
	{
		temp=current;
		current=current->next;
		free(temp->data.caller);
		free(temp);
	}
	free(queue);
}


void PrintAuxQueue(struct AuxQueue* queue)
{
	float s=0;
	printf("Subscribers:\n");
	struct AuxQueueNode* temp = queue->front;
	while(temp != NULL)
	{
		printf("%s: ",temp->data.caller);
		printf("%.2f euros\t| ",temp->data.bill);
		printf("%.2f %%\n",temp->data.percent);
		s+=temp->data.percent;
		temp = temp->next;
	}
	printf("Total percent:\t\t\t|%.2f %%\n",s);
}
