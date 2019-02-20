#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "heap.h"
#include "queue.h"
#include "auxqueue.h"


void heap_create(struct Heap** heap)
{
	*heap=malloc(sizeof(struct Heap));
	(*heap)->root= NULL;
	(*heap)->last=NULL;
	(*heap)->counter=0;
	(*heap)->revenue=0;
}

void heap_destroy_rec(struct HeapNode* node)
{
	if(node==NULL)return;

	heap_destroy_rec(node->left);
	heap_destroy_rec(node->right);

	free(node->caller);
	free(node);
}

void heap_destroy(struct Heap* heap)
{
	heap_destroy_rec(heap->root);
	free(heap);
}

void heap_add_node(struct Heap* heap,struct HeapNode** new_node,struct HeapNode* parent,char position,char* caller,float charge)
{
	(*new_node)         = malloc(sizeof(struct HeapNode));
	(*new_node)->bill   = charge;
	(*new_node)->caller = malloc((strlen(caller)+1)*sizeof(char));
	strcpy((*new_node)->caller,caller);


	(*new_node)->parent = parent;
	(*new_node)->left   = (*new_node)->right=NULL;

	/*position indicates where to attach the (*new_node) [right or left of the parent]*/
	if(position=='l')parent->left=(*new_node);
	else parent->right=(*new_node);

	heap->last=(*new_node);/*New node is now the last node in heap*/
	++heap->counter;
	heap->revenue+=charge;
}

/*Swaps ONLY the data of max and node*/
/*This function does not swap the whole nodes!!!!*/
void swap(struct HeapNode* max,struct HeapNode* node)
{
	struct HeapNode* temp;
	char* temp_string;
	float temp_bill;

	/*just swap data of max and node*/

	/*Swap caller number*/
	temp_string=malloc((strlen(max->caller)+1)*sizeof(char));
	strcpy(temp_string,max->caller);
	strcpy(max->caller,node->caller);
	strcpy(node->caller,temp_string);
	free(temp_string);

	/*Swap bills*/
	temp_bill=max->bill;
	max->bill=node->bill;
	node->bill=temp_bill;
}

/*node is the parent of the mutated node*/
void shift_up(struct Heap* heap,struct HeapNode* node)
{
	if(node==NULL)return;

	float max_val=node->bill;
	struct HeapNode* max_node=node;

	if(node->left!=NULL )
	{
		if(node->left->bill>max_val)
		{
			max_node=node->left;
			max_val=node->left->bill;
		}
		else max_node=node;
	}
	if(node->right!=NULL)
	{
		if(node->right->bill>max_val)
		{
			max_node=node->right;
		}
	}


	if(max_node!=node)
	{

		swap(max_node,node);

		/*Recursively calling shift_up after swap*/
		shift_up(heap,node->parent);
	}
}

void shift_down(struct Heap* heap,struct HeapNode* node)
{
	if(node==NULL)return;

	float max_val=node->bill;
	struct HeapNode* max_node=node;

	if(node->left!=NULL )
	{
		if(node->left->bill>max_val)
		{
			max_node=node->left;
			max_val=node->left->bill;
		}
		else max_node=node;
	}
	if(node->right!=NULL)
	{
		if(node->right->bill>max_val)
		{
			max_node=node->right;
		}
	}

	if(max_node!=node)
	{
		swap(max_node,node);

		/*Recursively calling shift_down after swap*/
		shift_down(heap,max_node);
	}
}

/*O(n) : linear search required*/
void heap_insert(struct Heap* heap,char* caller,float charge)
{
	char position;
	int found       = 0;
	int found_empty = 0;
	struct HeapNode* parent_of_empty;
	struct HeapNode* parent;
	struct HeapNode* new_node;
	struct HeapNode* current=heap->root;

	/*If Heap is empty*/
	if(heap->counter==0)
	{
		/*No need to call shift_up*/
		new_node=malloc(sizeof(struct HeapNode));
		new_node->caller=malloc((strlen(caller)+1)*sizeof(char));
		strcpy(new_node->caller,caller);
		new_node->bill=charge;
		new_node->left=new_node->right=NULL;
		new_node->parent=NULL;/*parent of root is NULL*/

		heap->root=new_node;
		heap->revenue+=charge;
		++heap->counter;
		return ;
	}

	/*BFS until the caller has been found or until the end of Heap has been reached*/
	struct Queue* queue;
	queue_create(&queue);
	enqueue(queue,heap->root);
	while(!queue_is_empty(queue))
	{
		current = dequeue(queue);
		parent  = current->parent;

		/*If found: Stop search*/
		if(!strcmp(current->caller,caller))
		{
			found=1;
			break;
		}

		/*Need to hold the first node found without a child[either left or right]  in case the given caller is not in the heap */
		/*Otherwise,I will just update the given caller*/
		if(current->left != NULL)enqueue(queue,current->left);
		else if(!found_empty)
		{
			new_node        = current->left;
			parent_of_empty = current;
			position        = 'l';
			found_empty     = 1;
		}
		if(current->right!=NULL)enqueue(queue,current->right);
		else if(!found_empty)
		{
			parent_of_empty = current;
			new_node        = current->right;
			position        = 'r';
			found_empty     = 1;
		}
	}
	queue_destroy(queue);

	/*If found: Update and shift_up starting from parent of found node*/
	if(found)
	{
		current->bill+=charge;
		heap->revenue+=charge;
		shift_up(heap,parent);
	}
	/*Else: Create a new node and shift_up starting from parent of the new node*/
	else
	{
		heap_add_node(heap,&new_node,parent_of_empty,position,caller,charge);
		shift_up(heap,parent_of_empty);
	}
}

/*O(n) : linear search required in order to find where the caller resides inside the heap */
void heap_delete(struct Heap* heap,char* caller,float charge)
{
	char position;
	int found       = 0;
	int found_empty = 0;
	struct HeapNode* parent_of_empty;
	struct HeapNode* parent;
	struct HeapNode* new_node;
	struct HeapNode* current = heap->root;

	/*If Heap is empty*/
	if(heap->counter==0)
		return;

	/*BFS until the caller has been found or until the end of Heap has been reached*/
	struct Queue* queue;
	queue_create(&queue);
	enqueue(queue,heap->root);
	while(!queue_is_empty(queue))
	{
		current=dequeue(queue);
		parent=current->parent;

		/*If found: Stop search*/
		if(!strcmp(current->caller,caller))
		{
			found=1;
			break;
		}

		/*Need to hold the first node found without a child[either left or right]  in case the given caller is not in the heap */
		if(current->left != NULL)enqueue(queue,current->left);
		else if(!found_empty)
		{
			new_node        = current->left;
			parent_of_empty = current;
			found_empty     = 1;
		}
		if(current->right!=NULL)enqueue(queue,current->right);
		else if(!found_empty)
		{
			parent_of_empty = current;
			new_node        = current->right;
			found_empty     = 1;
		}
	}
	queue_destroy(queue);

	/*If found: Update and start shifting downwards until heap property holds */
	if(found)
	{
		current->bill-=charge;
		heap->revenue-=charge;
		shift_down(heap,parent);
	}
	else
		return;
}

int extract_root(struct Heap* heap,struct aux_struct* return_value)
{
	/*Heap is empty*/
	if(heap->counter==0)return 0;
	// heap_print(heap);
	float root_bill=heap->root->bill;
	/*this struct will contain root's (caller,bill) pair*/
	return_value->bill=root_bill;
	return_value->caller=malloc((strlen(heap->root->caller)+1)*sizeof(char));

	strcpy(return_value->caller,heap->root->caller);

	/*Heap with at least two nodes*/
	if(heap->counter>1)
	{
		/*BFS in order to find last node in the heap*/
		struct HeapNode* current;
		struct HeapNode* last_node;
		struct HeapNode* parent_of_last_node;
		struct Queue* queue;
		queue_create(&queue);
		enqueue(queue,heap->root);
		while(!queue_is_empty(queue))
		{
			current=dequeue(queue);
			last_node=current;
			parent_of_last_node=last_node->parent;
			if(current->left != NULL)enqueue(queue,current->left);
			if(current->right != NULL)enqueue(queue,current->right);
		}
		queue_destroy(queue);


		/*Swap data of last and root*/
		swap(heap->root,last_node);

		/*Delete the last node*/
		free(last_node->caller);
		free(last_node);
		if(parent_of_last_node->left==last_node)parent_of_last_node->left=NULL;
		else parent_of_last_node->right=NULL;

		/*Shift Down the new root*/
		shift_down(heap,heap->root);
	}
	/*Heap has only one node*/
	else
	{
		free(heap->root->caller);
		free(heap->root);
	}
	heap->revenue-=return_value->bill;
	heap->counter--;
	return 1;

}

void heap_top(struct Heap* heap,float  percentage)
{
	/*Keeping starting revenue because im gonna extract some nodes from the heap*/
	float starting_rev=heap->revenue;

	/*Empty Heap OR percentage give is zero*/
	if(percentage==0 || heap->root==NULL)
	{
		printf("No calls found\n");
		return;
	}
	/*Heap with just one item*/
	else if(heap->counter==1)
	{
		printf("%s: ",heap->root->caller);
		printf("%.2f euros | ",heap->root->bill);
		printf("%.2f %%\n",heap->root->bill/starting_rev);
		return ;
	}

	/*We should reach total*/
	float total = heap->revenue*percentage/100;
	printf("Revenue:%.2f\nTop %.1f %% equals to: %.2f euros\n\n",heap->revenue,percentage,total);
	float current_sum=0;
	struct aux_struct* extracted;

	/*This queue will contain the (caller,bill) pairs we are searching for*/
	struct AuxQueue* queue;
	aux_queue_create(&queue);


	/*Fill this Queue with the extracted roots until we reach total*/
	while(current_sum<total)
	{
		extracted=malloc(sizeof(struct aux_struct));
		/*Heap has no more nodes*/
		if(!extract_root(heap,extracted))break;

		current_sum+=extracted->bill;
		/*Insert extracted data into the AuxQueue*/
		aux_enqueue(queue,extracted->caller,extracted->bill,extracted->bill/starting_rev*100);

		free(extracted->caller);
		free(extracted);
	}
	/*Print Result*/
	aux_queue_print(queue);

	/*Insert in the heap the extracted nodes that were temporarily stored in AuxQueue*/
	re_insert(queue,heap);
	/*Destroy AuxQueue*/
	aux_queue_destroy(queue);
}

void heap_print(struct Heap* heap)
{
	printf("heap_print:\n----------\n");
	if(heap->root==NULL){printf("Heap is empty\n");return;}
	struct HeapNode* current=heap->root;

	/*Breadth First Traversal of the heap[using a queue]*/
	struct Queue* queue;
	queue_create(&queue);
	enqueue(queue,heap->root);

	while(!queue_is_empty(queue))
	{
		current=dequeue(queue);
		printf("Caller:%s | Bill:%f\n",current->caller,current->bill);

		if(current->left != NULL)enqueue(queue,current->left);
		if(current->right!=NULL)enqueue(queue,current->right);
	}
	queue_destroy(queue);
	printf("===============================\n");
	printf("Nodes:%d\nRevenue:%f euros\n\n",heap->counter,heap->revenue);
}
