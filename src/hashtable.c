#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "hashtable.h"
#include "linkedlist.h"
#include "bucket.h"
#include "misc.h"

void ht_create(struct HashTable** HTPtrPtr, int HTSize,int HTType,int bsize)
{
	int i;
	*HTPtrPtr = malloc(sizeof(struct HashTable));
	/*Creating table of struct List*  */
	(*HTPtrPtr) -> table = malloc(HTSize*sizeof(struct List*));
	(*HTPtrPtr) -> size = HTSize;
	(*HTPtrPtr) -> type = HTType;
	(*HTPtrPtr) -> records = 0;

	/*Initializing table*/
	for(i=0;i<HTSize;++i)
	{
		/*Creating a List with buckets of type 1 for each Hashtable entry*/
		list_create((*HTPtrPtr) -> table+i,bsize,1);
	}

	// printf("HT has been created\n");
	// if(HTType==1)printf("Type:Caller's\n");
	// else printf("Callee's\n");
	// printf("Size:%d\n",HTSize);
	// printf("---------------------\n\n");
}

int hashfun(struct HashTable* HTPtr,char* number)
{
	int i;
	int sum=0;
	for(i=0;i<strlen(number);++i)
	{
		sum+=number[i];
	}
	return sum % HTPtr->size ;
}

void ht_insert(struct HashTable* HTPtr,struct CDR* cdr)
{
	int i;
	int hashval;
	char* number;


	number  = malloc((strlen(cdr->hash_key)+1)*sizeof(char));
	strcpy(number,cdr->hash_key);
	hashval = hashfun(HTPtr,number);
	free(number);

	list_insert((HTPtr->table)[hashval],cdr);
	++HTPtr->records;
}

void ht_delete(struct HashTable* HTPtr,char* caller,char* cdr_id)
{
	int hashval;
	int deleted = 0;

	hashval = hashfun(HTPtr,caller);
	deleted = list_delete(HTPtr->table[hashval],caller,cdr_id);

	if(deleted)
	{
		printf("Deleted %s\n",cdr_id);
		--HTPtr->records;
	}
	else printf("DError\n");
}

void ht_find(struct HashTable* HTPtr,char* caller,char* time_range)
{
	int hashval;
	int found;
	hashval = hashfun(HTPtr,caller);
	found   = list_find((HTPtr->table)[hashval],caller,time_range,HTPtr->type);
	if(!found)
	{
		printf("Caller %s could not be found\n",caller);
	}
}

void ht_lookup(struct HashTable* HTPtr,char* callee,char* time_range)
{
	int hashval;
	int found;
	hashval = hashfun(HTPtr,callee);
	found   = list_lookup((HTPtr->table)[hashval],callee,time_range,HTPtr->type);
	if(!found)
	{
		printf("Callee with number %s could not be found\n",callee);
	}
}

void ht_topdest(struct HashTable* HTPtr,char* caller)
{
	int hashval;
	int found;
	hashval = hashfun(HTPtr,caller);
	found = list_topdest((HTPtr->table)[hashval],caller);
	if(!found)
	{
		printf("Caller with number %s could not be found\n",caller);
	}
}

void ht_print(struct HashTable* HTPtr)
{
	int i;
	for(i=0;i<HTPtr->size;++i)
	{
		list_print((HTPtr->table)[i],NULL,HTPtr->type);
	}
}

void ht_dump(struct HashTable* HTPtr,char* dump_file)
{
	int i;
	FILE* fp = fopen(dump_file,"w");
	for(i=0;i<HTPtr->size;++i)
	{
		list_dump(HTPtr->table[i],HTPtr->type,fp,NULL);
	}
	fclose(fp);
}

void ht_destroy(struct HashTable* HTPtr)
{
	int i;
	//BucketSum();

	/*Destroying each list of the table*/
	for(i=0;i<HTPtr->size;++i)list_destroy(HTPtr->table[i]);

	printf("---------------------\n");
	printf("HT has been destroyed\n");

	if(HTPtr->type==1)
	printf("Type   : Caller's\n");
	else
	printf("Type   : Callee's\n");

	printf("Size   : %d\n",HTPtr->size);
	printf("Records: %d\n",HTPtr->records);
	printf("---------------------\n\n");
	free(HTPtr->table);
	free(HTPtr);
}



// void TableIndist(struct HashTable* HTPtr,char* caller1,char* caller2)
// {

	// int hashval;
	// int i;
	// int counter;
	// struct auxiliary_struct* aux=malloc(sizeof(struct auxiliary_struct));
	// aux->counter=0;
	// aux->N=1000;
	// aux->table=malloc(aux->N*sizeof(char*));

	// /*Find every number that have not contacted with both of them*/
	// for(i=0;i<HTPtr->size;++i)
	// {
		// // ListIndist(HTPtr->table[i],caller1,caller2,aux);
	// }
	// printf("| ");
	// for(i=0;i<aux->counter;++i)
	// {
		// printf("%s | ",aux->table[i]);
		// free(aux->table[i]);
	// }
	// printf("\n");
	// free(aux->table);
	// free(aux);
// }
