#ifndef INPUT_H
#define INPUT_H

#include <time.h>
#include "heap.h"
#include "hashtable.h"

extern int ht1_size;
extern int ht2_size;
extern int bucket_size;



/*Reading Operations' File*/
void read_opfile(FILE*,char*,struct HashTable**,struct HashTable**,struct Heap**);

/*Entering data using prompt*/
void read_prompt(char*,struct HashTable**,struct HashTable**,struct Heap**);

/*Functions for reading each operation*/
void read_insert(FILE*,FILE*,struct HashTable*,struct HashTable*,struct Heap*);
void read_delete(FILE*,struct HashTable*);
void read_find(FILE*,struct HashTable*);
void read_lookup(FILE*,struct HashTable*);
void read_indist(FILE*,struct HashTable*,struct HashTable*);
void read_topdest(FILE*,struct HashTable*);
void read_top(FILE*,struct HashTable*,struct HashTable*,struct Heap*);
void read_bye(FILE*,struct HashTable**,struct HashTable**,struct Heap**);
void read_print(FILE*,struct HashTable*,struct HashTable*);
void read_dump(FILE*,struct HashTable*,struct HashTable*);



/*This function will be used for find and lookup operations*/
void read_time_range(char*,struct tm*,struct tm*,int*);

/*Computes charge for every call according to configuration file*/
double compute_billing(FILE*,int,int,int );

#endif
