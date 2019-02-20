#ifndef BUCKET_H
#define BUCKET_H

#include <time.h>
#include "misc.h"

/* Type 1 bucket entry */
struct NumberEntry
{
	char* hash_key; /*Caller or Callee*/
	struct List* CDRList; /*CDR List for the hash_key above*/
};

/* Type 2 bucket entry */
struct CDREntry
{
	char* cdr_uniq_id;
	char* other_number;/*Caller or Callee*/
	struct tm date;
	int duration;
	int type;
	int tarrif;
	int fault_condition;
};

/*Creates a new bucket*/
void bucket_create(void **,int,int);

/*Destroys a bucket [free memory]*/
void bucket_destroy(void*,int,int);

/*Prints all CDRs*/
void cdrbucket_print(struct CDREntry*,char*,int,int);

/*Prints CDRs in a specific time range*/
int cdr_print(struct CDREntry*,char*,struct tm,struct tm,int,int,int);

/*Dumps all CDRs in a file*/
void cdrbucket_dump(void*,int,int,FILE*,char*);

/*Copies a number to a NumberBucket slot*/
void number_copy(struct NumberEntry*,char*);

/*Copies a cdr to a CDRBucket  slot*/
void cdr_copy(struct CDREntry*,struct CDR*);

/*Returns true if CDRBucket is empty*/
int bucket_is_empty(struct CDREntry*,int);

#endif
