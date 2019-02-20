#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "misc.h"
#include "hashtable.h"
#include "heap.h"

void parse_insert(FILE* fp,char* confile,struct HashTable* caller_table,struct HashTable* callee_table,struct Heap* heap)
{
	double charge;
	struct CDR record;
	record.cdr_uniq_id  = malloc(50 * sizeof(char));
	record.hash_key     = malloc(50 * sizeof(char));
	record.other_number = malloc(50 * sizeof(char));

	char* caller_number = malloc(50 * sizeof(char));
	char* callee_number = malloc(50 * sizeof(char));

	/**
	 * White space is essential for avoiding storing
	 * newline from previous input when reading from prompt
	 */
	fscanf(fp," %49[^;] ;",record.cdr_uniq_id);
	fscanf(fp,"%49[^;] ;",caller_number);
	fscanf(fp,"%49[^;] ;",callee_number);
	fscanf(fp,"%2d %2d %4d ;",&record.date.tm_mday,&record.date.tm_mon,&record.date.tm_year);
	fscanf(fp,"%d : %d ;",&record.date.tm_hour,&record.date.tm_min);
	fscanf(fp,"%d ;%d ;%d; %d",&record.duration,&record.type,&record.tarrif,&record.fault_condition);
	record.date.tm_mon--;
	record.date.tm_year-=1900;
	record.date.tm_sec=0;

	strcpy(record.hash_key,caller_number);
	strcpy(record.other_number,callee_number);
	ht_insert(caller_table,&record);

	/*Checking if fault condition of type "2XX" */
	if(record.fault_condition>=200 && record.fault_condition<=299)
	{
		if(confile)
		{
			FILE* fpconfig;
			if(!(fpconfig=fopen(confile,"r")))
			{
				fprintf(stderr,"Error with opening config file.Exiting...\n");
				exit(EXIT_FAILURE);
			}
			charge = parse_confile(fpconfig,record.type,record.tarrif,record.duration);
			fclose(fpconfig);
		}
		/* No configuration file given - Apply standard charge */
		else
			charge = 0.1*record.duration;

		heap_insert(heap,record.hash_key,charge);
	}

	strcpy(record.hash_key,callee_number);
	strcpy(record.other_number,caller_number);
	ht_insert(callee_table,&record);

	free(record.cdr_uniq_id);
	free(record.hash_key);
	free(record.other_number);
	free(caller_number);
	free(callee_number);
}

void parse_delete(FILE* fp,char* confile,struct HashTable* caller_table,struct Heap* heap)
{
	char* caller_num = malloc(50 * sizeof(char));
	char* cdr_id     = malloc(50 * sizeof(char));
	int found = 0;
	struct CDR record;
	float charge;

	/**
	 * White space is essential for avoiding storing
	 * newline from previous input when reading from prompt
	 */
	fscanf(fp,"%49s %49s",caller_num,cdr_id);

	printf("------------------------------------------------\n");
	printf("Delete cdr %s with caller %s\n",cdr_id,caller_num);
	printf("------------------------------------------------\n\n");
	found = ht_delete(caller_table,caller_num,cdr_id,&record);

	/* If the given cdr_id with the given caller number has been found */
	if(found)
		/*Checking if fault condition of type "2XX" */
		if(record.fault_condition>=200 && record.fault_condition<=299)
		{
			if(confile)
			{
				FILE* fpconfig;
				if(!(fpconfig=fopen(confile,"r")))
				{
					fprintf(stderr,"Error with opening config file.Exiting...\n");
					exit(EXIT_FAILURE);
				}
				charge = parse_confile(fpconfig,record.type,record.tarrif,record.duration);
				fclose(fpconfig);
			}
			/* No configuration file given */
			else
				charge = 0.1*record.duration;
			heap_delete(heap,caller_num,charge);
		}

	printf("\n");
	free(cdr_id);
	free(caller_num);
}

void parse_find(FILE* fp,struct HashTable* caller_table)
{
	char* caller_num = malloc(50 * sizeof(char));
	char* buffer     = malloc(50 * sizeof(char));
	strcpy(buffer,"");

	fscanf(fp,"%49s",caller_num);
	fgets(buffer,49,fp);
	printf("--------------------------\n");
	printf("Find Caller:%s\n",caller_num);
	printf("--------------------------\n\n");
	ht_find(caller_table,caller_num,buffer);
	printf("\n");

	free(caller_num);
	free(buffer);
}

void parse_lookup(FILE* fp,struct HashTable* callee_table)
{
	char* callee_num = malloc(50 * sizeof(char));
	char* buffer     = malloc(50 * sizeof(char));
	strcpy(buffer,"");

	fscanf(fp,"%49s",callee_num);
	fgets(buffer,50,fp);
	printf("----------------------------\n");
	printf("LookUp Callee:%s\n",callee_num);
	printf("----------------------------\n\n");
	ht_lookup(callee_table,callee_num,buffer);
	printf("\n");

	free(callee_num);
	free(buffer);
}

void parse_indist(FILE* fp,struct HashTable* caller_table,struct HashTable* callee_table)
{
	char* caller1 = malloc(50 * sizeof(char));
	char* caller2 = malloc(50 * sizeof(char));
	fscanf(fp,"%49s %49s",caller1,caller2);
	printf("------------------------------------------------------------------\n");
	printf("Show everyone that has contacted %s and %s\n",caller1,caller2);
	printf(">>> Not implemented <<<\n");
	printf("------------------------------------------------------------------\n\n");

	/* Not implemented */
	//TableIndist(caller_table,caller1,caller2);

	free(caller1);
	free(caller2);
}

void parse_topdest(FILE* fp,struct HashTable* caller_table)
{
	char* caller_num = malloc(50 * sizeof(char));
	fscanf(fp,"%49s",caller_num);
	printf("----------------------------------\n");
	printf("Top Destination for %s\n",caller_num);
	printf("----------------------------------\n\n");
	ht_topdest(caller_table,caller_num);
	printf("\n");
	free(caller_num);
}

void parse_top(FILE* fp,struct HashTable* caller_table,struct HashTable* callee_table,struct Heap* heap)
{
	float percentage;
	fscanf(fp,"%f",&percentage);
	printf("----------------------\n");
	printf("Top %.1f %% subscribers\n",percentage);
	printf("----------------------\n\n");
	// heap_print(heap);
	heap_top(heap,percentage);
	// heap_print(heap);
	printf("\n");
}

void parse_bye(FILE* fp,struct HashTable** caller_table,struct HashTable** callee_table,struct Heap** heap)
{
	printf("--------------------------------------\n");
	printf("All data structures will be destroyed\n");
	printf("New ones will be created automatically\n");
	printf("--------------------------------------\n\n");

	ht_destroy(*caller_table);
	ht_destroy(*callee_table);
	heap_destroy(*heap);

	ht_create(caller_table,ht1_size,1,bucket_size);
	ht_create(callee_table,ht2_size,2,bucket_size);
	heap_create(heap);
}

void parse_print(FILE* fp,struct HashTable* caller_table,struct HashTable* callee_table)
{
	char* tablestring = malloc(50 * sizeof(char));
	int tablenumber;
	fscanf(fp,"%49s",tablestring);
	tablenumber = atoi(&tablestring[strlen(tablestring)-1]);

	printf("---------------------------\n");
	printf("Hashtable %d will be printed\n",tablenumber);
	printf("---------------------------\n\n");

	if(tablenumber==1)ht_print(caller_table);
	else ht_print(callee_table);
	printf("\n");
	free(tablestring);
}

void parse_dump(FILE* fp,struct HashTable* caller_table,struct HashTable* callee_table)
{
	int tablenumber;
	char* tablestring = malloc(50 * sizeof(char));
	char* dump_file   = malloc(50 * sizeof(char));

	fscanf(fp,"%49s %49s",tablestring,dump_file);
	tablenumber = atoi(&tablestring[strlen(tablestring)-1]);

	printf("-----------------------------------------\n");
	printf("Hashtable %d will be written to file:%s\n",tablenumber,dump_file);
	printf("-----------------------------------------\n\n");


	if(tablenumber==1)ht_dump(caller_table,dump_file);
	else ht_dump(callee_table,dump_file);
	printf("\n");
	free(dump_file);
	free(tablestring);
}

void parse_opfile(FILE* fp,char* confile,struct HashTable** caller_table,struct HashTable** callee_table,struct Heap** heap)
{


	char* operation = malloc(50*sizeof(char));

	/*Read until EOF*/
	while(fscanf(fp,"%49s",operation))
	{
		if(feof(fp))break;

		fgetc(fp); /*extracting space after operation's name*/

		if(!strcmp(operation,"insert"))
		{
			parse_insert(fp,confile,*caller_table,*callee_table,*heap);
		}
		else if(!strcmp(operation,"delete"))
		{
			parse_delete(fp,confile,*caller_table,*heap);
		}
		else if(!strcmp(operation,"find"))
		{
			parse_find(fp,*caller_table);
		}
		else if(!strcmp(operation,"lookup"))
		{
			parse_lookup(fp,*callee_table);
		}
		else if(!strcmp(operation,"indist"))
		{
			parse_indist(fp,*caller_table,*callee_table);
		}
		else if(!strcmp(operation,"topdest"))
		{
			parse_topdest(fp,*caller_table);
		}
		else if(!strcmp(operation,"top"))
		{
			parse_top(fp,*caller_table,*callee_table,*heap);
		}
		else if(!strcmp(operation,"bye"))
		{
			parse_bye(fp,caller_table,callee_table,heap);
		}
		else if(!strcmp(operation,"print"))
		{
			parse_print(fp,*caller_table,*callee_table);
		}
		else if(!strcmp(operation,"dump"))
		{
			parse_dump(fp,*caller_table,*callee_table);
		}
		else
		{
			printf("Operation was not valid.Exiting...\n\n");
			return;
		}
	}
	free(operation);
}

void parse_prompt(char* confile,struct HashTable** caller_table,struct HashTable** callee_table,struct Heap** heap)
{
	char trash[200];
	int i;
	int inserts;
	FILE* fpconfig;
	int choice=-1;

	while(choice != 0)
	{
		printf("---------------------\n");
		printf("Select an operation  \n");
		printf("---------------------\n");

		printf(	"0.Exit\n"
			"1.Insert\n"
			"2.Delete\n"
			"3.Find\n"
			"4.Lookup\n"
			"5.Indist\n"
			"6.TopDest\n"
			"7.Top\n"
			"8.Bye\n"
			"9.Print\n"
			"10.Dump\n\n");

		scanf(" %d",&choice);
		switch(choice)
		{
			case 0:
				printf("Exiting...See ya !\n\n\n");
				break;
			case 1:
				printf("Enter the number of CDRs you would like to insert\n");
				scanf("%d",&inserts);
				printf("Enter CDR_unique_ID;caller_num;callee_num;date;time;duration;type;tarrif;fault_cond\n");
				for(i=0;i<inserts;++i)parse_insert(stdin,confile,*caller_table,*callee_table,*heap);
				break;
			case 2:
				printf("Enter caller's number and CDR unique ID seperated by whitespace\n");
				parse_delete(stdin,confile,*caller_table,*heap);
				break;
			case 3:
				printf("Enter caller's number and time range [optionally]\n");
				parse_find(stdin,*caller_table);
				break;
			case 4:
				printf("Enter callee's number and time range [optionally]\n");
				parse_find(stdin,*callee_table);
				break;
			case 5:
				printf("Enter number for caller1 and caller2\n");
				parse_indist(stdin,*caller_table,*callee_table);
				break;
			case 6:
				printf("Enter caller's number\n");
				parse_topdest(stdin,*caller_table);
				break;
			case 7:
				printf("Enter percentage (%%)\n");
				parse_top(stdin,*caller_table,*callee_table,*heap);
				break;
			case 8:
				parse_bye(stdin,caller_table,callee_table,heap);
				break;
			case 9:
				printf("Enter \"hashtableX\" [X = 1 or 2]\n");
				parse_print(stdin,*caller_table,*callee_table);
				break;
			case 10:
				printf("Enter \"hashtableX\" [X = 1 or 2]  and filename\n");
				parse_dump(stdin,*caller_table,*callee_table);
				break;
			default:
				printf(">>>Error: Your choice was not valid.Try again<<<\n\n");
				/* Collecting garbage(i.e: invalid input) from stdin*/
				if(choice==EOF)
					scanf("%s",trash);
				break;
		}
	}
}

double parse_confile(FILE* fpconfig,int type,int tarrif,int duration)
{
	int typ,tarr;
	float cost;

	/*SMS: standard charge*/
	if(type==0 && tarrif==0)return 0.1*duration;

	/*Otherwise*/
	char* line = malloc(200*sizeof(char));
	while(fgets(line,200,fpconfig)!=NULL)
	{

		if(line[0]=='#')continue;/*comment*/
		else
		{
			sscanf(line,"%d;%d;%f",&typ,&tarr,&cost);
			if(typ==type && tarrif==tarr)
			{
				free(line);
				return duration * cost;
			}
		}
	}
	free(line);
	/* (Type,Tarrif) combination was not found */
	/* Thus, return standard charge */
	return 0.1*duration;
}

void parse_time_range(char* buffer, struct tm* from_date,struct tm* to_date ,int* flag)
{

	/**
	 * 0:	no time range provided
	 * 1:	time1 -> time2
	 * 2:	year1 -> year2
	 * 3:	time1,year1  --> time2,year2
	 */

	*flag = 0;
	if(!strcmp(buffer,"\n") || !strcmp(buffer,""))
	{
		/*Time range was not provided*/
		printf("No time range\n");
		return;
	}
	from_date->tm_sec = to_date->tm_sec=0;
	int length;
	int tokens = 0;
	char* temp = malloc(50 * sizeof(char));
	char* pch;

	strcpy(temp,buffer);
	pch = strtok(temp," ");
	length = strlen(pch);
	while (pch != NULL)
	{
		pch = strtok (NULL, " ");
		++tokens;
	}

	if(tokens==2)
	{
		/*time1 -> time2 */
		if(length==5)
		{
			sscanf(buffer,"%2d : %2d  %2d : %2d",&from_date->tm_hour,&from_date->tm_min,&to_date->tm_hour,&to_date->tm_min);
			printf("%02d:%02d",from_date->tm_hour,from_date->tm_min);
			printf(" ---> ");
			printf("%02d:%02d\n",to_date->tm_hour,to_date->tm_min);
			*flag=1;
			free(temp);
			return;
		}
		/*year1 -> year2*/
		else if(length==8)
		{
			sscanf(buffer,"%2d %2d %4d %2d %2d %4d",&from_date->tm_mday,&from_date->tm_mon,&from_date->tm_year,&to_date->tm_mday,&to_date->tm_mon,&to_date->tm_year);
			printf("%02d/%02d/%d",from_date->tm_mday,from_date->tm_mon,from_date->tm_year);
			printf(" ---> ");
			printf("%02d/%02d/%d\n",to_date->tm_mday,to_date->tm_mon,to_date->tm_year);

			from_date->tm_year-=1900;
			to_date->tm_year-=1900;
			--from_date->tm_mon;
			--to_date->tm_mon;
			*flag=2;
			free(temp);
			return;
		}
	}
	/*time1,year1 -> time2,year2*/
	else if(tokens==4)
	{
		sscanf(buffer,"%2d : %2d %2d %2d %4d %2d : %2d %2d %2d %4d ",&from_date->tm_hour,&from_date->tm_min,&from_date->tm_mday,&from_date->tm_mon,&from_date->tm_year,&to_date->tm_hour,&to_date->tm_min,&to_date->tm_mday,&to_date->tm_mon,&to_date->tm_year);
		printf("%02d/%02d/%d | %02d:%02d",from_date->tm_mday,from_date->tm_mon,from_date->tm_year,from_date->tm_hour,from_date->tm_min);
		printf(" ---> ");
		printf("%02d/%02d/%d | %02d:%02d\n",to_date->tm_mday,to_date->tm_mon,to_date->tm_year,to_date->tm_hour,to_date->tm_min);

		from_date->tm_year-=1900;
		to_date->tm_year-=1900;
		--from_date->tm_mon;
		--to_date->tm_mon;
		*flag=3;
		free(temp);
		return;
	}
	free(temp);
}
