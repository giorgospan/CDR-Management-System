#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "hashtable.h"
#include "linkedlist.h"
#include "bucket.h"
#include "misc.h"

void ht_create(struct HashTable** ht, int ht_size,int ht_type,int bsize)
{
	int i;
	*ht = malloc(sizeof(struct HashTable));
	/*Creating table of struct List*  */
	(*ht) -> table = malloc(ht_size*sizeof(struct List*));
	(*ht) -> size = ht_size;
	(*ht) -> type = ht_type;
	(*ht) -> records = 0;

	/*Initializing table*/
	for(i=0;i<ht_size;++i)
	{
		/*Creating a List with buckets of type 1 for each Hashtable entry*/
		list_create((*ht) -> table+i,bsize,1);
	}

	// printf("HT has been created\n");
	// if(ht_type==1)printf("Type:Caller's\n");
	// else printf("Callee's\n");
	// printf("Size:%d\n",ht_size);
	// printf("---------------------\n\n");
}

int hashfun(struct HashTable* ht,char* number)
{
	int i;
	int sum=0;
	for(i=0;i<strlen(number);++i)
	{
		sum+=number[i];
	}
	return sum % ht->size ;
}

void ht_insert(struct HashTable* ht,struct CDR* cdr)
{
	int i;
	int hashval;
	char* number;

	number  = malloc((strlen(cdr->hash_key)+1)*sizeof(char));
	strcpy(number,cdr->hash_key);
	hashval = hashfun(ht,number);
	free(number);

	list_insert((ht->table)[hashval],cdr);
	++ht->records;
}

int ht_delete(struct HashTable* ht,char* caller,char* cdr_id,struct CDR* cdr)
{
	int hashval;
	int deleted = 0;

	hashval = hashfun(ht,caller);
	deleted = list_delete(ht->table[hashval],caller,cdr_id,cdr);

	if(deleted)
	{
		printf("Deleted %s\n",cdr_id);
		--ht->records;
		return 1;
	}
	else
	{
		printf("CDR %s with caller %s was not found\n",cdr_id,caller);
		return 0;
	}
}

void ht_find(struct HashTable* ht,char* caller,char* time_range)
{
	int hashval;
	int found;
	hashval = hashfun(ht,caller);
	found   = list_find((ht->table)[hashval],caller,time_range,ht->type);
	if(!found)
	{
		printf("Caller %s could not be found\n",caller);
	}
}

void ht_lookup(struct HashTable* ht,char* callee,char* time_range)
{
	int hashval;
	int found;
	hashval = hashfun(ht,callee);
	found   = list_lookup((ht->table)[hashval],callee,time_range,ht->type);
	if(!found)
	{
		printf("Callee with number %s could not be found\n",callee);
	}
}

void ht_topdest(struct HashTable* ht,char* caller)
{
	int hashval;
	int found;
	hashval = hashfun(ht,caller);
	found = list_topdest((ht->table)[hashval],caller);
	if(!found)
	{
		printf("Caller with number %s could not be found\n",caller);
	}
}

void ht_print(struct HashTable* ht)
{
	int i;
	for(i=0;i<ht->size;++i)
	{
		list_print((ht->table)[i],NULL,ht->type);
	}
}

void ht_dump(struct HashTable* ht,char* dump_file)
{
	int i;
	FILE* fp = fopen(dump_file,"w");
	for(i=0;i<ht->size;++i)
	{
		list_dump(ht->table[i],ht->type,fp,NULL);
	}
	fclose(fp);
}

void ht_destroy(struct HashTable* ht)
{
	int i;
	//BucketSum();

	/*Destroying each list of the table*/
	for(i=0;i<ht->size;++i)list_destroy(ht->table[i]);

	printf("---------------------\n");
	printf("HT has been destroyed\n");

	if(ht->type==1)
	printf("Type   : Caller's\n");
	else
	printf("Type   : Callee's\n");

	printf("Size   : %d\n",ht->size);
	printf("Records: %d\n",ht->records);
	printf("---------------------\n\n");
	free(ht->table);
	free(ht);
}

// void TableIndist(struct HashTable* ht,char* caller1,char* caller2)
// {

	// int hashval;
	// int i;
	// int counter;
	// struct auxiliary_struct* aux=malloc(sizeof(struct auxiliary_struct));
	// aux->counter=0;
	// aux->N=1000;
	// aux->table=malloc(aux->N*sizeof(char*));

	// /*Find every number that have not contacted with both of them*/
	// for(i=0;i<ht->size;++i)
	// {
		// // ListIndist(ht->table[i],caller1,caller2,aux);
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
