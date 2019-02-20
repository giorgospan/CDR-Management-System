#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "misc.h"
#include "bucket.h"
#include "parser.h" /*for parse_time_range()*/


void list_create(struct List** list,int bsize,int t)
{
	*list         = malloc(sizeof(struct List));
	(*list)->head = NULL;

	/*tail is needed for InsertNode and DeleteNode*/
	(*list)->tail            = NULL;
	(*list)->available_slot  = NULL;
	(*list)->bucketType      = t;
	(*list)->empty           = 1;
	(*list)->counter         = 0;
	(*list)->moved_available = 0;

	if(t==1)
	{
		(*list)->bucketSlots = bsize / sizeof(struct NumberEntry);
		(*list)->SlotSize    = sizeof(struct NumberEntry);
	}
	else
	{
		(*list)->bucketSlots = bsize / sizeof(struct CDREntry);
		(*list)->SlotSize    = sizeof(struct CDREntry);
	}
}

void list_add_node(struct List* list)
{
	struct ListNode *node = malloc(sizeof(struct ListNode));
	node->next            = NULL;

	if(list->head == NULL)
	{
		list->head = list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail       = node;
	}
	bucket_create(&node->bucket,list->bucketType,list->bucketSlots);
	/*First available slot :  First slot of the newly created bucket */
	list->available_slot = node->bucket;
}

void list_insert(struct List* list,struct CDR* cdr)
{
	struct NumberEntry* temp;

	/*Checking if I should add a new node*/
	if( (list->empty) || (list->counter % list->bucketSlots == 0 && list->moved_available)  )
	{
		list_add_node(list);
		list->empty=0;
		list->moved_available=0;
	}
	/*Number List */
	if(list->bucketType==1)
	{
		/*If number already exists:
		Insert CDR to the CDR List of this number */
		if(temp=search_number(list,cdr->hash_key))
		{
			list_insert(temp->CDRList,cdr);
			list->moved_available=0;
		}

		/**
		 * If number doesnt exist:
		 *
		 * Insert this number to the first available_slot
		 * Insert this CDR to the CDR List of this number
		 */
		else
		{
			number_copy(((struct NumberEntry*)(list->available_slot)),cdr->hash_key);
			list_insert(((struct NumberEntry*)(list->available_slot))->CDRList,cdr);
			list->available_slot+=list->SlotSize;
			list->moved_available=1;
			++list->counter;
		}
	}
	/*CDR List*/
	/*CDRs will be unique,no need to check if a CDR already exists*/
	else
	{
		cdr_copy((struct CDREntry*)(list->available_slot),cdr);
		list->available_slot+=list->SlotSize;
		list->moved_available=1;
		++list->counter;
	}
}

int list_find(struct List* list,char* caller,char* time_range,int tabletype)
{
	struct NumberEntry* temp;
	int found_cdr = 0;
	int flag;
	struct tm from_date;
	struct tm to_date;
	printf("Time Range:\n");
	parse_time_range(time_range,&from_date,&to_date ,&flag);
	printf("\n");


	if(!(temp=search_number(list,caller)))return 0; /*Number was not found*/
	else
	{
		/*If found : call cdr_print for each CDRBucket of this number*/

		struct ListNode* current=temp->CDRList->head;
		while(current)
		{
			if(cdr_print((struct CDREntry*)(current->bucket),caller,from_date,to_date,flag,temp->CDRList->bucketSlots,tabletype))found_cdr=1;
			current=current->next;
		}

		/*After having scanned each bucket of the CDRList*/
		if(!found_cdr)printf("No CDRs found\n");
		/*Return 1 because given number was found*/
		return 1;
	}
}

int list_lookup(struct List* list,char* callee,char* time_range,int tabletype)
{
	struct NumberEntry* temp;
	int flag;
	int found_cdr=0;
	struct tm from_date;
	struct tm to_date;
	printf("Time Range:\n");
	parse_time_range(time_range,&from_date,&to_date ,&flag);
	printf("\n");


	if(!(temp=search_number(list,callee)))return 0; /*Number was not found*/
	else
	{
		/*If found : call cdr_print for each CDRBucket of this number*/

		struct ListNode* current=temp->CDRList->head;
		while(current)
		{
			if(cdr_print((struct CDREntry*)(current->bucket),callee,from_date,to_date,flag,temp->CDRList->bucketSlots,tabletype))found_cdr=1;
			current=current->next;
		}
		/*After having scanned each bucket of the CDRList*/
		if(!found_cdr)printf("No CDRs found\n");
		/*Return 1 because given number was found*/
		return 1;
	}
}

void list_print(struct List* list,char* hash_key,int tabletype)
{
	struct ListNode* current=list->head;
	int i;

	/*NumberList*/
	if(list->bucketType==1)
	{
		struct NumberEntry* ptr;
		while(current)
		{
			/*Print CDRList for each number in this bucket*/
			ptr= current->bucket;
			for(i=0;i<list->bucketSlots;++i)
			{
				if(ptr[i].hash_key)list_print(ptr[i].CDRList,ptr[i].hash_key,tabletype);
			}
			/*Move to next node*/
			current=current->next;
		}
	}
	/*CDRList */
	else
	{
		while(current)
		{
			/*Print CDRBucket*/
			cdrbucket_print((struct CDREntry*)(current->bucket),hash_key,list->bucketSlots,tabletype);
			/*Move to next node*/
			current=current->next;
		}
	}
}

void list_delete_node(struct List* list,struct ListNode* prev,struct ListNode* node)
{
	/*Deleting head of the list*/
	if(list->head==prev)
	{
		free(list->head->bucket);
		free(list->head);
		list->head=list->tail=NULL;
	}
	/*Deleting tail of list*/
	else if(list->tail==node)
	{
		prev->next=NULL;
		list->tail=prev;
		free(node->bucket);
		free(node);
	}
	/*Deleting a typical node located between two nodes*/
	else
	{
		prev->next=node->next;
		free(node->bucket);
		free(node);
	}
}

int list_delete(struct List* list,char* caller,char* cdr_id,struct CDR* result)
{
	struct NumberEntry* position;
	struct ListNode* previous;
	struct CDREntry* target;
	int empty;

	/*Find given caller*/
	if(position=search_number(list,caller))
	{
		/*Find given cdr_id*/
		if(target=search_cdr(position->CDRList,cdr_id,&previous))
		{

			result->duration        = target->duration;
			result->type            = target->type;
			result->tarrif          = target->tarrif;
			result->fault_condition = target->fault_condition;

			free(target->cdr_uniq_id);
			free(target->other_number);
			target->cdr_uniq_id  = NULL;
			target->other_number = NULL;

			position->CDRList->counter--;
			/*If CDRbucket becomes empty --> Delete this ListNode*/
			if(previous==position->CDRList->head)
				empty = bucket_is_empty( (struct CDREntry*)(position->CDRList->head->bucket)	, position->CDRList->bucketSlots  );
			else
			  empty = bucket_is_empty( (struct CDREntry*)(previous->next->bucket), position->CDRList->bucketSlots  );

			if(empty)
				list_delete_node(position->CDRList,previous,previous->next);

			/*Returns true because we found the given (caller,cdr) pair*/
			return 1;
		}
	}
	return 0;
}

void list_dump(struct List* list,int tabletype,FILE* fp,char* hash_key)
{
	struct ListNode* current=list->head;
	int i;

	/*NumberList*/
	if(list->bucketType==1)
	{
		struct NumberEntry* ptr;
		while(current)
		{
			/*Dump CDRList for each number in this bucket*/
			ptr= current->bucket;
			for(i=0;i<list->bucketSlots;++i)
			{
				if(ptr[i].hash_key)list_dump(ptr[i].CDRList,tabletype,fp,ptr[i].hash_key);
			}
			/*Move to next node*/
			current=current->next;
		}
	}
	/*CDRList */
	else
	{
		while(current)
		{
			/*Dump CDRBucket*/
			cdrbucket_dump(current->bucket,tabletype,list->bucketSlots,fp,hash_key);
			/*Move to next node*/
			current=current->next;
		}
	}
}

int list_topdest(struct List* list,char* caller)
{
	struct NumberEntry* position;
	struct CDREntry* ptr;
	int slots;
	int i;
	int max=-1;
	int topdest;
	int index;
	/*Frequency table for 10^3 different country codes*/
	int *frequency=malloc(1000*sizeof(int));
	for(i=0;i<1000;++i)frequency[i]=0;


	if(position=search_number(list,caller))
	{
		struct ListNode* current;
		current=position->CDRList->head;
		slots=position->CDRList->bucketSlots;

		/*Traverse CDRList of given caller*/
		while(current)
		{
			ptr=current->bucket;
			/*Scan every CDR Bucket*/
			for(i=0;i<slots;++i)
			{
				/*If CDRSlot is not empty*/
				if(ptr[i].cdr_uniq_id)
				{
					sscanf(ptr[i].other_number,"%d[^-]",&index);
					if((++frequency[index])>max){max=frequency[index];topdest=index;}
				}
			}
			current=current->next;
		}
		if(topdest==-1)	printf("No calls found\n");
		else 			printf("Country Code:%03d Calls made:%d\n",topdest,max);
		free(frequency);
		return 1;
	}
	free(frequency);
	return 0;
}

void list_destroy(struct List* list)
{
	struct ListNode* current=list->head;
	struct ListNode* temp;

	while(current)
	{
		/*Destroy Bucket*/
		bucket_destroy(current->bucket,list->bucketType,list->bucketSlots);

		/*Destroy current node*/
		temp=current;
		current=current->next;
		free(temp);
	}
	free(list);
}

struct CDREntry* search_cdr(struct List* list,char* id,struct ListNode** previous)
{
	int i;
	int n=list->bucketSlots;
	struct ListNode* current;
	struct CDREntry* bucket;

	current=list->head;
	while(current)
	{
		*previous=current;

		for(i=0;i<n;++i)
		{
			bucket=(struct CDREntry*)(current->bucket);
			/*If CDRSlot is not empty*/
			if(bucket[i].cdr_uniq_id)
				/*Return bucketslot where this cdr was found*/
				if(!strcmp(bucket[i].cdr_uniq_id,id))return &bucket[i];
		}
		current=current->next;
	}
	/*Return NULL if cdr was not found in any of the CDRBuckets*/
	return NULL;
}

struct NumberEntry* search_number(struct List* list,char* number)
{
	int i;
	int n=list->bucketSlots;
	struct ListNode* current;
	struct NumberEntry* bucket;

	current=list->head;
	while(current)
	{
		for(i=0;i<n;++i)
		{
			bucket=(struct NumberEntry*)(current->bucket);
			if(bucket[i].hash_key)
			{
				if(!strcmp(bucket[i].hash_key,number))return &bucket[i];
			}
			else return NULL; /*As soon as we find the first empty slot return NULL*/
		}
		current=current->next;
	}
	return NULL;
}



/***************************************************************************************************************/

// int ListIndist(struct List* list,char* caller1,char* caller2,struct auxiliary_struct* aux)
// {
	// struct ListNode* current=list->head;
	// struct NumberEntry* bucket;
	// int n=list->bucketSlots;
	// int i,j;
	// int add_number=1;

	// // printf("List Indist:%p \n",list);
	// /*For all number buckets*/
	// while(current)
	// {
		// bucket=(struct NumberEntry*)current->bucket;
		// /*For all numbers in every bucket*/
		// for(i=0;i<n;++i)
		// {
			// /*If this number have called caller1 AND caller 2*/
			// /*Mporei na mhn peire o number ton caller1 alla na phre caller1->number*/
			// if(contact(bucket[i].CDRList,caller1,caller2))
			// {
				// for(j=0;i<aux->counter;++j)
				// {
					// if(communicate(bucket[j].CDRList,aux->table[j]))add_number=0;
				// }
				// /*Current number has not communicated with any of the numbers already stored in the aux*/
				// if(add_number)
				// {
					// /*Check if we need to realloc*/
					// if(aux->counter+1 ==aux->N)
					// {
						// aux->N<<=1;
						// realloc(aux->table,aux->N*sizeof(char*));
					// }
					// aux->table[aux->counter]=malloc((strlen(bucket[i].hash_key)+1)*sizeof(char));
					// strcpy(aux->table[aux->counter],bucket[i].hash_key);
					// ++aux->counter;
				// }
			// }
		// }
		// current=current->next;
	// }
// }


// int communicate(struct List* cdrlist,char* number)
// {
	// struct ListNode* current=cdrlist->head;
	// struct CDREntry* bucket;
	// int n=cdrlist->bucketSlots;
	// int i;

	// while(current)
	// {
		// for(i=0;i<n;++i)
		// {
			// bucket=(struct CDREntry*)(current->bucket);
			// /*If CDRSlot is not empty*/
			// if(bucket[i].cdr_uniq_id)
			// if(!strcmp(bucket[i].other_number,number))
			// return 1;
		// }
		// current=current->next;
	// }
	// return 0;
// }


// int contact(struct List* cdrlist,char* caller1,char* caller2)
// {
	// struct ListNode* current=cdrlist->head;
	// struct CDREntry* bucket;
	// int n=cdrlist->bucketSlots;
	// int i;
	// int found1=0;
	// int found2=0;

	// while(current)
	// {
		// for(i=0;i<n;++i)
		// {
			// bucket=(struct CDREntry*)(current->bucket);
			// /*If CDRSlot is not empty*/
			// if(bucket[i].cdr_uniq_id)
			// if(!strcmp(bucket[i].other_number,caller1))
			// {
				// found1=1;
			// }
			// else if(!strcmp(bucket[i].other_number,caller2))
			// {
				// found2=1;
			// }
			// if(found1 && found2)return 1;
		// }
		// current=current->next;
	// }
	// return 0;
// }
