#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #include "HashTable.h"
#include "heap.h"
#include "parser.h"

int ht1_size;
int ht2_size;
int bucket_size;


int main(int argc,char* argv[])
{
	int i;
	int ht1_provided   = 0;
	int ht2_provided   = 0;
	int bsize_provided = 0;
	char* opfile       = NULL;
	char* confile      = NULL;

	/* Scan command line arguments */
	for(i=0;i<argc;++i)
	{
		if(!strcmp("-o",argv[i]))
		{
			opfile = malloc((strlen(argv[i+1]+1))* sizeof(char));
			strcpy(opfile,argv[i+1]);
		}
		else if(!strcmp("-h1",argv[i]))
		{
			ht1_size = atoi(argv[i+1]);
			ht1_provided=1;
		}
		else if(!strcmp("-h2",argv[i]))
		{
			ht2_size = atoi(argv[i+1]);
			ht2_provided=1;
		}
		else if(!strcmp("-s",argv[i]))
		{
			bucket_size = atoi(argv[i+1]);
			bsize_provided=1;
		}
		else if(!strcmp("-c",argv[i]))
		{
			confile = malloc((strlen(argv[i+1]+1))* sizeof(char));
			strcpy(confile,argv[i+1]);
		}
	}

	if(!ht1_provided && !ht2_provided && !bsize_provided)
	{
		printf("Not enough arguments given\nExiting...\n\n");
		fprintf(stderr, "Usage:./werhauz -o Operations -h1 Hashtable1NumOfEntries -h2 Hashtable2NumOfEntries -s bucket_size -c config-file\n\n");
		exit(EXIT_FAILURE);
	}


	printf("Caller's hashtable:%d entries\n",ht1_size);
	printf("Callee's hashtable:%d entries\n",ht2_size);
	printf("bucket_size:%d bytes\n",bucket_size);
	printf("\n\n");
/********************************************************************************************/

	/* Create Structures */
	struct HashTable* caller_table;
	struct HashTable* callee_table;
	struct Heap* heap;
	// CreateTable(&caller_table,ht1_size,1,bucket_size);
	// CreateTable(&callee_table,ht2_size,2,bucket_size);
	HeapCreate(&heap);

	/* Read Operations File(if provided) */
	if(opfile!=NULL)
	{
		FILE* fp;
		if(!(fp=fopen(opfile,"r")))
		{
			fprintf(stderr,"Error with opening Operation File %s \n",opfile);
		}
		else
		{
			parse_opfile(fp,confile,&caller_table,&callee_table,&heap);
			fclose(fp);
			free(opfile);
		}
	}
	/* Read via prompt */
	parse_prompt(confile,&caller_table,&callee_table,&heap);

	if(confile)free(confile);
	// TableDestroy(caller_table);
	// TableDestroy(callee_table);
	HeapDestroy(heap);
	return 0;

}
