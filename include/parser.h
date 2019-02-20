#ifndef INPUT_H
#define INPUT_H

#include <time.h>
#include "heap.h"
#include "hashtable.h"

extern int ht1_size;
extern int ht2_size;
extern int bucket_size;

/*Reading Operations' File*/
void parse_opfile(FILE*,char*,struct HashTable**,struct HashTable**,struct Heap**);

/*Entering data using prompt*/
void parse_prompt(char*,struct HashTable**,struct HashTable**,struct Heap**);

/*Functions for reading each operation*/
void parse_insert(FILE*,char*,struct HashTable*,struct HashTable*,struct Heap*);
void parse_delete(FILE*,char*,struct HashTable*,struct Heap*);
void parse_find(FILE*,struct HashTable*);
void parse_lookup(FILE*,struct HashTable*);
void parse_indist(FILE*,struct HashTable*,struct HashTable*);
void parse_topdest(FILE*,struct HashTable*);
void parse_top(FILE*,struct HashTable*,struct HashTable*,struct Heap*);
void parse_bye(FILE*,struct HashTable**,struct HashTable**,struct Heap**);
void parse_print(FILE*,struct HashTable*,struct HashTable*);
void parse_dump(FILE*,struct HashTable*,struct HashTable*);



/*This function will be used for find and lookup operations*/
void parse_time_range(char*,struct tm*,struct tm*,int*);

/*Computes charge for every call according to configuration file*/
double parse_confile(FILE*,int,int,int );

#endif
