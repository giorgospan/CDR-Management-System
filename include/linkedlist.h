#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "misc.h"


struct ListNode
{
	void* bucket;
	struct ListNode* next;
};

struct List
{
	struct ListNode* head;
	struct ListNode* tail;
	void* available_slot;/*first available BucketSlot*/
	int bucketSlots;
	int bucketType;
	int empty;
	int SlotSize;
	int counter;
	int moved_available;
};


/* "available_slot" is needed in order to find in O(1) time an available slot a CDR*/
/*However, this cannot be achieved for the number buckets  because I need to search until I find the given number or until I reach the first empty slot*/




/*Creates a struct List*/
void CreateList(struct List**,int,int);


/*Adds a new node to the tail of the list*/
void ListAddNode(struct List* );


/*Deletes a node from the list*/
void ListDeleteNode(struct List*,struct ListNode* ,struct ListNode* );


/*Number List: Inserts a new number to the first available slot  */
/*CDR List   : Inserts a new CDR to first available slot*/
void ListInsert(struct List*,struct CDR* );


/*Deletes a CDR of a specific caller*/
/*Returns 1 if caller has been found, else 0*/
int ListDelete(struct List*,char*,char*);

/*Finds CDRs of a caller [in a time range optionally]*/
int ListFind(struct List*,char*,char*,int);

/*Finds calls to a callee [in a time range optionally]*/
int ListLookUp(struct List*,char*,char*,int);

/*Not implemented*/
// int ListIndist(struct List*,char*,char*,struct auxiliary_struct*);

/*Number List: Calls ListDump for the CDRList of each number in the current list */
/*CDR    List: Calls DumpCDRBucket for each CDR Bucket in the list */
void ListDump(struct List*,int,FILE*,char*);

/*Number List: Calls ListPrint for the CDRList of each number in the current list */
/*CDR    List: Calls PrintCDRBucket for each CDR Bucket in the list */
void ListPrint(struct List*,char*,int);



/*Finds top destination for a given caller*/
int ListTopDest(struct List*,char*);


/*Destroys all nodes [free]*/
void ListDestroy(struct List*);


/*Searches in  a list for a given number. Returns its bucketslot*/
/*Stops searching upon finding an empty slot or finding the given number*/
struct NumberBucket* search_number(struct List*,char*);



/*Searches in a list for a given CDR.Returns its bucketslot*/
/*Stops searching as soon as it finds the given CDR or as soon as it reaches the end of list*/
struct CDRBucket* search_cdr(struct List* list,char*,struct ListNode**);

/*Prints sum of different numbers in all number lists*/
void NumberSum();





/****************************************************************************/
/*Those fucntions were supposed to be used for indist operation*/


/*Returns true if caller1 and caller2 are in the given CDR List*/
int contact(struct List*,char*,char*);

/*Returns true if number is in the given CDR List*/
/*Similar to search_number but this one applies to a CDRList not a NumberList */
int communicate(struct List*,char*);

#endif