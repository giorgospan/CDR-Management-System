#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "hashtable.h"
#include "bucket.h"/*for sizeof(struct CDREntry)*/
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
			opfile = malloc((strlen(argv[i+1])+1)* sizeof(char));
			strcpy(opfile,argv[i+1]);
		}
		else if(!strcmp("-h1",argv[i]))
		{
			ht1_size = atoi(argv[i+1]);
			if (ht1_size<=0) {
				fprintf(stderr, "Error: Hash Table 1 size is not a positive number\n");
				exit(EXIT_FAILURE);
			}
			ht1_provided=1;
		}
		else if(!strcmp("-h2",argv[i]))
		{
			ht2_size = atoi(argv[i+1]);
			if (ht2_size<=0) {
				fprintf(stderr, "Error: Hash Table 2 size is not a positive number\n");
				exit(EXIT_FAILURE);
			}
			ht2_provided=1;
		}
		else if(!strcmp("-s",argv[i]))
		{
			bucket_size = atoi(argv[i+1]);
			if (bucket_size<=sizeof(struct CDREntry)) {
				fprintf(stderr, "Error: Bucket size is too small\n");
				exit(EXIT_FAILURE);
			}
			bsize_provided=1;
		}
		else if(!strcmp("-c",argv[i]))
		{
			confile = malloc((strlen(argv[i+1])+1)* sizeof(char));
			strcpy(confile,argv[i+1]);
		}
	}

	if(!ht1_provided && !ht2_provided && !bsize_provided)
	{
		fprintf(stderr, "Usage:./werhauz -o Operations -h1 Hashtable1NumOfEntries -h2 Hashtable2NumOfEntries -s bucket_size -c config-file\n\n");
		exit(EXIT_FAILURE);
	}


	printf("Caller's hashtable : %d entries\n",ht1_size);
	printf("Callee's hashtable : %d entries\n",ht2_size);
	printf("Bucket's size      : %d bytes\n",bucket_size);
	printf("\n\n");
/********************************************************************************************/

	/* Create Structures */
	struct HashTable* caller_table;
	struct HashTable* callee_table;
	struct Heap* heap;
	ht_create(&caller_table,ht1_size,1,bucket_size);
	ht_create(&callee_table,ht2_size,2,bucket_size);
	heap_create(&heap);

	/* Read Operations File(if provided) */
	if(opfile!=NULL)
	{
		FILE* fp;
		if(!(fp=fopen(opfile,"r")))
		{
			fprintf(stderr,">> Error with opening Operation File \"%s\" << \n\n",opfile);
		}
		else
		{
			parse_opfile(fp,confile,&caller_table,&callee_table,&heap);
			fclose(fp);
		}
	}
	/* Read via prompt */
	parse_prompt(confile,&caller_table,&callee_table,&heap);

	free(opfile);
	free(confile);
	ht_destroy(caller_table);
	ht_destroy(callee_table);
	heap_destroy(heap);
	return 0;

}
