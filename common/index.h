#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "mem.h"
#include "hash.h"
#include "counters.h"
#include "index.h"

typedef struct index index_t; // opaque to users of the module


/**************** index_new ****************/
/* create a new empty index
* Caller provides:
*   Number of slots to be used for the hashtable held in the index
* We guarantee:
*  empty index initialized returned
* Caller is responsible for:
*   later calling index_delete
*/
index_t* index_new (const int size);


/**************** index_getHash ****************/
/* gets the hashtable stored by index
* Caller provides:
*   valid index pointer
* We guarantee:
*   valid pointer to index->hashSet
*/
hashtable_t* index_getHash(index_t* index);


/**************** hashtable_delete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 * We do:
 *   calls hashtable_delete with counters delete as itemfunc
 */
void index_delete (index_t* index);


/**************** index_save ****************/
/* Insert item, identified by key (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid word string for key, valid int for docID, valid count int (can be NULL).
 * We return:
 *   if count given, word will be stored in index with docID set to the count given
 *   if no count, word will be stored in index and docID frequency incremented by 1 
 * Notes:
 *   if word has not been entered into the index yet, will initialize a new counter to 
 *   hold docID at the word key with count 1 
 */
void index_save (index_t* index, const char* word, const int docID);


/**************** index_iterate ****************/
/* Iterate over all items in the index; in undefined order.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   FILE* of indexFile
 * We do:
 *   hashtable_iterate, output word to indexFile, then 
 *   counters_iterate and output data to indexfile
 */
void index_iterate(index_t* index, FILE* indexFile);

#endif // PAGEDIR_H