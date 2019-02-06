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
void CreateTable(struct HashTable** ,int,int,int);


/*Hash Function*/
int HashFunction(struct HashTable* ,char*);


/*Inserts a new CDR */
void TableInsert(struct HashTable*,struct CDR*);

/*Deletes a specific CDR of a given caller*/
void TableDelete(struct HashTable* ,char*,char*);

/*Finds CDRs of a caller [in a time range optionally]*/
void TableFind(struct HashTable*,char*,char* );

/*Finds CDRs of a callee [in a time range optionally]*/
void TableLookUp(struct HashTable*,char*,char*);


/*Finds Top Destination of a given caller*/
void TableTopDest(struct HashTable*,char*);

/*Not implemented*/
void TableIndist(struct HashTable*,char*,char*);

/*Prints all CDRs*/
/*Calls ListPrint for every list of the table*/
void TablePrint(struct HashTable* );

/*Dumps CDRs into a dump file*/
/*Calls ListDump for every list of the table*/
void TableDump(struct HashTable*,char* );

/*Calls ListDestroy for every list of the table*/
/*Frees memory allocated for the table itself*/
void TableDestroy(struct HashTable*);



#endif