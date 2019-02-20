#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "misc.h"


struct HashTable
{
	/*Dynamic Table of pointers to struct List*/
	struct List** table;
	int size;
	int type; /*1 for Caller's | 2 for Callee's*/
	int records; /*Number of records inserted in the table*/
};


/*Creates a HashTable*/
/*Calls ListCreate for every list of the table*/
void ht_create(struct HashTable** ,int,int,int);

/*Hash Function*/
int hashfun(struct HashTable* ,char*);

/*Inserts a new CDR */
void ht_insert(struct HashTable*,struct CDR*);

/*Deletes a specific CDR of a given caller*/
int ht_delete(struct HashTable* ,char*,char*,struct CDR*);

/*Finds CDRs of a caller [in a time range optionally]*/
void ht_find(struct HashTable*,char*,char* );

/*Finds CDRs of a callee [in a time range optionally]*/
void ht_lookup(struct HashTable*,char*,char*);


/*Finds Top Destination of a given caller*/
void ht_topdest(struct HashTable*,char*);

/*Not implemented*/
void TableIndist(struct HashTable*,char*,char*);

/*Prints all CDRs*/
/*Calls list_print for every list of the table*/
void ht_print(struct HashTable* );

/*Dumps CDRs into a dump file*/
/*Calls list_dump for every list of the table*/
void ht_dump(struct HashTable*,char* );

/*Calls list_destroy for every list of the table*/
/*Frees memory allocated for the table itself*/
void ht_destroy(struct HashTable*);



#endif
