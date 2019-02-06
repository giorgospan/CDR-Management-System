#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "misc.h"
#include "hashtable.h"
#include "heap.h"

void parse_insert(FILE* fp,FILE* fpconfig,struct HashTable* CallerTable,struct HashTable* CalleeTable,struct Heap* heap)
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
	TableInsert(CallerTable,&record);

	/*Checking if fault condition of type "2XX" */
	if(record.fault_condition>=200 && record.fault_condition<=299)
	{
		charge=parse_confile(fpconfig,record.type,record.tarrif,record.duration);
		HeapInsert(heap,record.hash_key,charge);
	}

	strcpy(record.hash_key,callee_number);
	strcpy(record.other_number,caller_number);
	TableInsert(CalleeTable,&record);

	free(record.cdr_uniq_id);
	free(record.hash_key);
	free(record.other_number);
	free(caller_number);
	free(callee_number);


}

void parse_delete(FILE* fp,struct HashTable* CallerTable)
{
	char* caller_num = malloc(50 * sizeof(char));
	char* cdr_id     = malloc(50 * sizeof(char));
	fscanf(fp,"%49s %49s",caller_num,cdr_id);

	printf("----------------------------------------------\n");
	printf("Delete cdr %s of number %s\n",cdr_id,caller_num);
	printf("----------------------------------------------\n\n");
	TableDelete(CallerTable,caller_num,cdr_id);
	printf("\n");
	free(cdr_id);
	free(caller_num);
}

void parse_find(FILE* fp,struct HashTable* CallerTable)
{
	char* caller_num = malloc(50 * sizeof(char));
	char* buffer     = malloc(50 * sizeof(char));

	fscanf(fp,"%49s",caller_num);
	fgets(buffer,49,fp);
	printf("--------------------------\n");
	printf("Find Caller:%s\n",caller_num);
	printf("--------------------------\n\n");
	TableFind(CallerTable,caller_num,buffer);
	printf("\n");

	free(caller_num);
	free(buffer);
}

void parse_lookup(FILE* fp,struct HashTable* CalleeTable)
{
	char* callee_num = malloc(50 * sizeof(char));
	char* buffer     = malloc(50 * sizeof(char));
	fscanf(fp,"%49s",callee_num);
	fgets(buffer,50,fp);

	printf("----------------------------\n");
	printf("LookUp Callee:%s\n",callee_num);
	printf("----------------------------\n\n");
	TableLookUp(CalleeTable,callee_num,buffer);
	printf("\n");

	free(callee_num);
	free(buffer);
}

void parse_indist(FILE* fp,struct HashTable* CallerTable,struct HashTable* CalleeTable)
{
	char* caller1 = malloc(50 * sizeof(char));
	char* caller2 = malloc(50 * sizeof(char));
	fscanf(fp,"%49s %49s",caller1,caller2);
	printf("------------------------------------------------------------------\n");
	printf("Show everyone that has contacted %s and %s\n",caller1,caller2);
	printf("------------------------------------------------------------------\n\n");

	/* Not implemented */
	//TableIndist(CallerTable,caller1,caller2);

	free(caller1);
	free(caller2);
}

void parse_topdest(FILE* fp,struct HashTable* CallerTable)
{
	char* caller_num = malloc(50 * sizeof(char));
	fscanf(fp,"%49s",caller_num);
	printf("----------------------------------\n");
	printf("Top Destination for %s\n",caller_num);
	printf("----------------------------------\n\n");
	TableTopDest(CallerTable,caller_num);
	printf("\n");
	free(caller_num);
}

void parse_top(FILE* fp,struct HashTable* CallerTable,struct HashTable* CalleeTable,struct Heap* heap)
{
	float percentage;
	fscanf(fp,"%f",&percentage);
	printf("----------------------\n");
	printf("Top %.1f %% subscribers\n",percentage);
	printf("----------------------\n\n");
	// PrintHeap(heap);
	HeapTop(heap,percentage);
	// PrintHeap(heap);
	printf("\n");
}

void parse_bye(FILE* fp,struct HashTable** CallerTable,struct HashTable** CalleeTable,struct Heap** heap)
{
	printf("--------------------------------------\n");
	printf("All data structures will be destroyed\n");
	printf("New ones will be created automatically\n");
	printf("--------------------------------------\n\n");

	TableDestroy(*CallerTable);
	TableDestroy(*CalleeTable);
	HeapDestroy(*heap);

	CreateTable(CallerTable,ht1_size,1,bucket_size);
	CreateTable(CalleeTable,ht2_size,2,bucket_size);
	HeapCreate(heap);
}

void parse_print(FILE* fp,struct HashTable* CallerTable,struct HashTable* CalleeTable)
{
	char* tablestring = malloc(50 * sizeof(char));
	int tablenumber;
	fscanf(fp,"%49s",tablestring);
	tablenumber = atoi(&tablestring[strlen(tablestring)-1]);

	printf("---------------------------\n");
	printf("Hashtable %d will be printed\n",tablenumber);
	printf("---------------------------\n\n");

	if(tablenumber==1)TablePrint(CallerTable);
	else TablePrint(CalleeTable);
	printf("\n");
	free(tablestring);
}
void parse_dump(FILE* fp,struct HashTable* CallerTable,struct HashTable* CalleeTable)
{
	int tablenumber;
	char* tablestring = malloc(50 * sizeof(char));
	char* dump_file   = malloc(50 * sizeof(char));

	fscanf(fp,"%49s %49s",tablestring,dump_file);
	tablenumber = atoi(&tablestring[strlen(tablestring)-1]);

	printf("-----------------------------------------\n");
	printf("Hashtable %d will be written to file:%s\n",tablenumber,dump_file);
	printf("-----------------------------------------\n\n");


	if(tablenumber==1)TableDump(CallerTable,dump_file);
	else TableDump(CalleeTable,dump_file);
	printf("\n");
	free(dump_file);
	free(tablestring);
}

void parse_opfile(FILE* fp,char* confile,struct HashTable** CallerTable,struct HashTable** CalleeTable,struct Heap** heap)
{


	char* operation = malloc(50*sizeof(char));

	/*Read until EOF*/
	while(fscanf(fp,"%49s",operation))
	{
		if(feof(fp))break;

		fgetc(fp); /*extracting space after operation's name*/

		if(!strcmp(operation,"insert"))
		{
			FILE* fpconfig;
			if(!(fpconfig=fopen(confile,"r")))
			{
				printf("Error with opening config file.Exiting...\n");
				exit(1);
			}
			parse_insert(fp,fpconfig,*CallerTable,*CalleeTable,*heap);
			fclose(fpconfig);
		}
		else if(!strcmp(operation,"delete"))
		{
			parse_delete(fp,*CallerTable);
		}
		else if(!strcmp(operation,"find"))
		{
			parse_find(fp,*CallerTable);
		}
		else if(!strcmp(operation,"lookup"))
		{
			parse_lookup(fp,*CalleeTable);
		}

		else if(!strcmp(operation,"indist"))
		{
			parse_indist(fp,*CallerTable,*CalleeTable);
		}
		else if(!strcmp(operation,"topdest"))
		{
			parse_topdest(fp,*CallerTable);
		}
		else if(!strcmp(operation,"top"))
		{
			parse_top(fp,*CallerTable,*CalleeTable,*heap);
		}
		else if(!strcmp(operation,"bye"))
		{
			parse_bye(fp,CallerTable,CalleeTable,heap);
		}
		else if(!strcmp(operation,"print"))
		{
			parse_print(fp,*CallerTable,*CalleeTable);
		}
		else if(!strcmp(operation,"dump"))
		{
			parse_dump(fp,*CallerTable,*CalleeTable);
		}
		else
		{
			printf("Operation was not valid.Exiting...\n\n");
			return;
		}
	}
	free(operation);
}

void parse_prompt(char* confile,struct HashTable** CallerTable,struct HashTable** CalleeTable,struct Heap** heap)
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

		scanf("%d",&choice);
		switch(choice)
		{
			case 0:
				printf("Exiting...See ya !\n\n\n");
				break;
			case 1:
				if(!(fpconfig=fopen(confile,"r")))
				{
					printf("Error with opening config file.Exiting...\n");
					exit(1);
				}
				printf("Enter the number of CDRs you would like to insert\n");
				scanf("%d",&inserts);
				printf("Enter CDR unique ID,caller's number,callee number,date,time,duration,type,tarrif,fault condition\n");
				for(i=0;i<inserts;++i)parse_insert(stdin,fpconfig,*CallerTable,*CalleeTable,*heap);
				fclose(fpconfig);
				break;
			case 2:
				printf("Enter CDR unique ID,caller's number seperated by space\n");
				parse_delete(stdin,*CallerTable);
				break;
			case 3:
				printf("Enter caller's number and time range [optionally]\n");
				parse_find(stdin,*CallerTable);
				break;
			case 4:
				printf("Enter callee's number and time range [optionally]\n");
				parse_find(stdin,*CalleeTable);
				break;
			case 5:
				printf("Enter number for caller1 and caller2\n");
				parse_indist(stdin,*CallerTable,*CalleeTable);
				break;
			case 6:
				printf("Enter caller's number\n");
				parse_topdest(stdin,*CallerTable);
				break;
			case 7:
				printf("Enter percentage (%%)\n");
				parse_top(stdin,*CallerTable,*CalleeTable,*heap);
				break;
			case 8:
				parse_bye(stdin,CallerTable,CalleeTable,heap);
				break;
			case 9:
				printf("Enter \"hashtableX\" [X = 1 or 2]\n");
				parse_print(stdin,*CallerTable,*CalleeTable);
				break;
			case 10:
				printf("Enter \"hashtableX\" [X = 1 or 2]  and filename\n");
				parse_dump(stdin,*CallerTable,*CalleeTable);
				break;
			default:
				printf("Error: Your choice was not valid.Try again !\n");
				/* Collecting garbage from stdin*/
				scanf("%s",trash);
				break;
		}
	}
}

void parse_time_range(char* buffer, struct tm* from_date,struct tm* to_date ,int*flag)
{
	/*	0:	no time range provided
		1:	time1 -> time2
		2:	year1 -> year2
		3:	time1,year1  --> time2,year2
	*/

	if(buffer[0]=='\n')
	{
		/*Time range was not provided*/
		printf("No time range\n");
		*flag=0;
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
			sscanf(buffer,"%02d : %02d  %02d : %02d",&from_date->tm_hour,&from_date->tm_min,&to_date->tm_hour,&to_date->tm_min);
			printf("%02d:%02d",from_date->tm_hour,from_date->tm_min);
			printf(" ---> ");
			printf("%02d:%02d\n",to_date->tm_hour,to_date->tm_min);
			*flag=1;
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
			return;
		}
	}
	/*time1,year1 -> time2,year2*/
	else if(tokens==4)
	{
		sscanf(buffer,"%02d : %02d %02d %02d %4d %02d : %02d %02d %02d %4d ",&from_date->tm_hour,&from_date->tm_min,&from_date->tm_mday,&from_date->tm_mon,&from_date->tm_year,&to_date->tm_hour,&to_date->tm_min,&to_date->tm_mday,&to_date->tm_mon,&to_date->tm_year);
		printf("%02d/%02d/%d | %02d:%02d",from_date->tm_mday,from_date->tm_mon,from_date->tm_year,from_date->tm_hour,from_date->tm_min);
		printf(" ---> ");
		printf("%02d/%02d/%d | %02d:%02d\n",to_date->tm_mday,to_date->tm_mon,to_date->tm_year,to_date->tm_hour,to_date->tm_min);

		from_date->tm_year-=1900;
		to_date->tm_year-=1900;
		--from_date->tm_mon;
		--to_date->tm_mon;
		*flag=3;
		return;
	}
	free(temp);

}


double parse_confile(FILE* fpconfig,int type,int tarrif,int duration)
{
	int typ,tarr;
	float cost;

	/*SMS: standard charge*/
	if(type==0 && tarrif==0)return 0.1;

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
	/* (Type,Tarrif) combination was not found*/
	return 0;
}
