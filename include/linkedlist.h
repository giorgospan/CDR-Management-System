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
void list_create(struct List**,int,int);


/*Adds a new node to the tail of the list*/
void list_add_node(struct List* );


/*Deletes a node from the list*/
void list_delete_node(struct List*,struct ListNode* ,struct ListNode* );


/*Number List: Inserts a new number to the first available slot  */
/*CDR List   : Inserts a new CDR to first available slot*/
void list_insert(struct List*,struct CDR* );


/*Deletes a CDR of a specific caller*/
/*Returns 1 if caller has been found, else 0*/
int list_delete(struct List*,char*,char*,struct CDR* cdr);

/*Finds CDRs of a caller [in a time range optionally]*/
int list_find(struct List*,char*,char*,int);

/*Finds calls to a callee [in a time range optionally]*/
int list_lookup(struct List*,char*,char*,int);

/*Not implemented*/
// int ListIndist(struct List*,char*,char*,struct auxiliary_struct*);

/*Number List: Calls list_dump for the CDRList of each number in the current list */
/*CDR    List: Calls cdrbucket_dump for each CDR Bucket in the list */
void list_dump(struct List*,int,FILE*,char*);

/*Number List: Calls list_print for the CDRList of each number in the current list */
/*CDR    List: Calls cdrbucket_print for each CDR Bucket in the list */
void list_print(struct List*,char*,int);



/*Finds top destination for a given caller*/
int list_topdest(struct List*,char*);


/*Destroys all nodes [free]*/
void list_destroy(struct List*);


/*Searches in  a list for a given number. Returns its bucketslot*/
/*Stops searching upon finding an empty slot or finding the given number*/
struct NumberEntry* search_number(struct List*,char*);



/*Searches in a list for a given CDR.Returns its bucketslot*/
/*Stops searching as soon as it finds the given CDR or as soon as it reaches the end of list*/
struct CDREntry* search_cdr(struct List* list,char*,struct ListNode**);




/****************************************************************************/
/*Those fucntions will be used for indist operation*/


/*Returns true if caller1 and caller2 are in the given CDR List*/
int contact(struct List*,char*,char*);

/*Returns true if number is in the given CDR List*/
/*Similar to search_number but this one applies to a CDRList not a NumberList */
int communicate(struct List*,char*);

#endif
