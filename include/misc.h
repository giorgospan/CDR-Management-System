#ifndef MISC_H
#define MISC_H

#include <time.h>


/*Might use this for indist operation*/
// struct auxiliary_struct
// {
	// int counter;
	// int N;
	// char** table;
// };

struct CDR
{
	char* cdr_uniq_id;
	char* hash_key;/*Hash Key*/
	char* other_number;/*Other Number: Caller or Callee*/
	struct tm date;
	int duration;
	int type;
	int tarrif;
	int fault_condition;
};


struct Date
{
	int day;
	int month;
	int year;
};

struct Time
{
	int hours;
	int minutes;

};

int compare(struct tm ,struct tm,struct tm,int);

#endif
