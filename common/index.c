#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "mem.h"
#include "hash.h"
#include "counters.h"
#include "index.h"

static void counters_delete_wrapper(void* item);
static void index_iterate_helper (void* indexFile, const char* key, void* counter);
static void index_counters_iterate_helper (void* indexFile, int key, int count);

typedef struct index {
    hashtable_t* indexHash;   
} index_t;

index_t*
index_new (const int size){
    index_t* index = mem_malloc(sizeof(index_t));
    if (index == NULL){
        fprintf(stderr, "index memory error");
        exit(1);
    }
    
    index->indexHash = hashtable_new(size);
    return index;
}

hashtable_t*
index_getHash(index_t* index){
    return index->indexHash;
}


void
index_delete (index_t* index){
    hashtable_t* indexHash = index->indexHash;
    hashtable_delete(indexHash, counters_delete_wrapper); 
    mem_free(index);
}

static void
counters_delete_wrapper(void* item){    
    counters_delete(item);
}

void
index_save (index_t* index, const char* word, const int docID){
    hashtable_t* indexHash = index->indexHash; // get hashtable
    counters_t* counterCheck = hashtable_find(indexHash, word);
    if (counterCheck != NULL){
        counters_add(counterCheck, docID);
    } else {
        counters_t* newCounter = counters_new(); // create counters to store docID and word ct
        counters_add(newCounter, docID);
        hashtable_insert(indexHash, word, newCounter);
    }
}
 
void
index_iterate (index_t* index, FILE* indexFile){
    hashtable_t* indexHash = index->indexHash;
    hashtable_iterate(indexHash, indexFile, index_iterate_helper); // for indexer pass in index_iterate helper as itemfunc
}

static void
index_iterate_helper (void* indexFile, const char* key, void* counter){
    fprintf(indexFile, "%s", key);
    counters_iterate(counter, indexFile, index_counters_iterate_helper);
    fprintf(indexFile, "\n");
}

static void 
index_counters_iterate_helper (void* indexFile, int key, int count){
    fprintf(indexFile, " %d %d", key, count);
}