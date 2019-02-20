#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "linkedlist.h"
#include "bucket.h"
#include "misc.h"

#define CDRBUCKETSIZE 2000


void bucket_create(void** bucket,int type,int slots)
{
	int i;

	/*Create bucket of the first type [Number bucket]*/
	if(type==1)
	{
		*bucket = malloc(slots * sizeof(struct NumberEntry));

		struct NumberEntry* ptr = *bucket;
		for(i=0;i<slots;++i)
		{
			ptr[i].hash_key=NULL;
			/*Create a CDR List */
			list_create(&(ptr[i].CDRList),CDRBUCKETSIZE,2);
		}

	}
	/*Create bucket of the second type [CDR bucket]*/
	else
	{
		*bucket = malloc(slots * sizeof(struct CDREntry));

		struct CDREntry* ptr = *bucket;
		for(i=0;i<slots;++i)
		{
			ptr[i].cdr_uniq_id  = NULL;
			ptr[i].other_number = NULL;
		}
	}

}

void bucket_destroy(void* bucket,int buckettype,int slots)
{
	int i;
	/*Destroy bucket of the first buckettype [Number bucket]*/
	if(buckettype==1)
	{
		struct NumberEntry* ptr = bucket;
		for(i=0;i<slots;++i)
		{
			if(ptr[i].hash_key)free(ptr[i].hash_key);
			list_destroy(ptr[i].CDRList);
		}
	}

	/*Destroy bucket of the second buckettype [CDR bucket]*/
	else
	{
		struct CDREntry* ptr = bucket;
		for(i=0;i<slots;++i)
		{
			if(ptr[i].cdr_uniq_id)
			{
				free(ptr[i].cdr_uniq_id);
				free(ptr[i].other_number);
			}

		}
	}
	free(bucket);
}

/*tabletype is needed in order to know which number to print first.*/
/*if it's 1 , then print first hash_key  [CallerTalbe]*/
/*if it's 2 , then print first the other_number [CalleeTalbe]*/
int cdr_print(struct CDREntry* bucket,char* hash_key,struct tm from_date,struct tm to_date,int flag,int slots,int tabletype)
{
	int found_cdr=0;
	int i;
	for(i=0;i<slots;++i)
	{
		/*Check if bucketslot is empty*/
		if(bucket[i].cdr_uniq_id)
		{
			/*Check if CDR in this slot satisfies the given time range */
			if(compare(from_date,to_date,bucket[i].date,flag))
			{
				found_cdr=1;
				printf("%s ",bucket[i].cdr_uniq_id);
				if(tabletype==1)
				{
					/*Caller Table*/
					printf("%s ",hash_key);
					printf("%s ",bucket[i].other_number);
				}
				else
				{
					/*Callee Table*/
					printf("%s ",bucket[i].other_number);
					printf("%s ",hash_key);
				}
				printf("%02d:%02d ",bucket[i].date.tm_hour,bucket[i].date.tm_min);
				printf("%02d/%02d/%d ",bucket[i].date.tm_mday,bucket[i].date.tm_mon+1,bucket[i].date.tm_year+1900);
				printf("%d %d %d %d\n",bucket[i].duration,bucket[i].type,bucket[i].tarrif,bucket[i].fault_condition);
			}
		}
	}
	return found_cdr;
}

void cdrbucket_print(struct CDREntry* bucket,char* hash_key,int slots,int tabletype)
{
	int i;
	struct CDREntry* ptr=bucket;
	for(i=0;i<slots;++i)
	{
		/*Check if bucketslot is empty*/
		if(ptr[i].cdr_uniq_id!=NULL)
		{

			printf("%s ",ptr[i].cdr_uniq_id);
			if(tabletype==1)
			{
				/*Caller Table*/
				printf("%s ",hash_key);
				printf("%s ",ptr[i].other_number);
			}
			else
			{
				/*Callee Table*/
				printf("%s ",ptr[i].other_number);
				printf("%s ",hash_key);
			}
			printf("%02d:%02d ",ptr[i].date.tm_hour,ptr[i].date.tm_min);
			printf("%02d/%02d/%d ",ptr[i].date.tm_mday,ptr[i].date.tm_mon+1,ptr[i].date.tm_year+1900);
			printf("%d %d %d %d\n",ptr[i].duration,ptr[i].type,ptr[i].tarrif,ptr[i].fault_condition);

		}
	}
}

void cdrbucket_dump(void* bucket,int tabletype,int slots,FILE* fp,char* hash_key)
{
	int i;
	struct CDREntry* ptr=bucket;
	for(i=0;i<slots;++i)
	{
		if(ptr[i].cdr_uniq_id)
		{
			fprintf(fp,"insert ");
			fprintf(fp,"%s;",ptr[i].cdr_uniq_id);
			if(tabletype==1)
			{
				/*Caller Table*/
				fprintf(fp,"%s;",hash_key);
				fprintf(fp,"%s;",ptr[i].other_number);
			}
			else
			{
				/*Callee Table*/
				fprintf(fp,"%s;",ptr[i].other_number);
				fprintf(fp,"%s;",hash_key);
			}
			fprintf(fp,"%02d:%02d;",ptr[i].date.tm_hour,ptr[i].date.tm_min);
			fprintf(fp,"%02d%02d%d;",ptr[i].date.tm_mday,ptr[i].date.tm_mon+1,ptr[i].date.tm_year+1900);
			fprintf(fp,"%d;%d;%d;%d\n",ptr[i].duration,ptr[i].type,ptr[i].tarrif,ptr[i].fault_condition);
		}
	}
}

int bucket_is_empty(struct CDREntry* bucket,int slots)
{
	int i;
	/*Scan every Slot until you find a filled one*/
	for(i=0;i<slots;++i)
	{
		if(bucket[i].cdr_uniq_id!=NULL)return 0;
	}
	return 1;
}


void number_copy(struct NumberEntry* bucket,char* number)
{
	bucket->hash_key= malloc((strlen(number)+1)*sizeof(char));
	strcpy(bucket->hash_key,number);

}

void cdr_copy(struct CDREntry* bucket,struct CDR* cdr)
{
	bucket->cdr_uniq_id = malloc((strlen(cdr->cdr_uniq_id)+1)*sizeof(char));
	strcpy(bucket->cdr_uniq_id,cdr->cdr_uniq_id);

	bucket->other_number = malloc((strlen(cdr->other_number)+1)*sizeof(char));
	strcpy(bucket->other_number,cdr->other_number);

	bucket->date            = cdr->date;
	bucket->duration        = cdr->duration;
	bucket->type            = cdr->type;
	bucket->tarrif          = cdr->tarrif;
	bucket->fault_condition = cdr->fault_condition;
}
