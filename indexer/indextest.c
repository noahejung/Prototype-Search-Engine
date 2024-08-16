#include <stdio.h>
#include <stdlib.h>
#include "index.c"
#include "file.h"

static void parseArgs(const int argc, char* argv[], char** oldIndex, char** newIndex);

int main (int argc, char* argv[]){
    char* oldIndex;
    char* newIndex;
    parseArgs(argc, argv, &oldIndex, &newIndex);

    index_t* index;
    FILE* fp = fopen(oldIndex, "r");

    // find file length
    const int size = file_numLines(fp);
    fclose(fp);
    // create index as the length of file
    index = index_new(size);


    if ((fp = fopen(oldIndex, "r")) != NULL){       // similar to indexer, will loop through each lie of file and extract counter pairs as well as word
        char* line;
        while ((line = file_readLine(fp)) != NULL){
            char* word;
            char* strDocID;
            char* strCount;
            int docID;
            int count;
            counters_t* counter = counters_new();
            word = strtok(line, " ");
            
            while((strDocID = strtok(NULL, " ")) != NULL && ((strCount = strtok(NULL, " "))) != NULL){
                docID = atoi(strDocID);
                count = atoi(strCount);
                counters_set(counter, docID, count);
            }
            hashtable_t* indexHash = index->indexHash;
            hashtable_insert(indexHash, word, counter);
            mem_free(line);
        }
        fclose(fp);
    }
    FILE* newfp = fopen(newIndex, "w");
    index_iterate(index, newfp);
    fclose(newfp);
    index_delete(index);

}


static void
parseArgs(const int argc, char* argv[], char** oldIndex, char** newIndex){
  if (argv == NULL){
    fprintf(stderr, "Parsing error, Null argv received\n");
    exit(1);
  }
  if (argc != 3){
    fprintf(stderr, "expected 2 args, received %d\n", argc - 1);
    exit(1);
  }
  if (argv[1] == NULL || argv[2] == NULL){
    fprintf(stderr, "NULL args received\n");
    exit(1);
  }
  FILE* fp =  fopen(argv[1], "r");
  if (fp  == NULL) {
    fprintf(stderr, "could not open old Index\n");
    exit(1);
    }
  fclose(fp);
  *oldIndex = argv[1];

  FILE* fp2 = fopen(argv[2], "a");
  if (fp2 == NULL) {
    fprintf(stderr, "could not open new Index\n");
    exit(1);
  }
  fclose(fp2);   
  *newIndex = argv[2];
}


