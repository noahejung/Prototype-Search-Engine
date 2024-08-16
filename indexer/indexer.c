#include <stdio.h>
#include <stdlib.h>
#include "pagedir.h"
#include "index.h"
#include "word.h"

  static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
  static void indexBuild(char* pageDirectory, char* indexFilename);
  static void indexPage(webpage_t* page, index_t* index, int docID);

int
main(const int argc, char* argv[]){  // main function 
  char* pageDirectory = NULL;
  char* indexFilename = NULL;
  parseArgs(argc, argv, &pageDirectory, &indexFilename);
  indexBuild(pageDirectory, indexFilename);
}
static void
indexBuild(char* pageDirectory, char* indexFilename){  // builds index 
  index_t* index = index_new(900); // initialize w/ size 900
  char* documentfile = mem_malloc((strlen(pageDirectory) + 21) * sizeof(char) + 1); // 21 characters is a sufficient length for possible integers 
  if (documentfile == NULL){
    fprintf(stderr, "memory error");
    exit(1);
  }

  sprintf(documentfile, "%s/%s", pageDirectory, "1");  // create pathname for pageDirectory
  FILE* fp;
  for (int docID = 1; (fp = fopen(documentfile, "r")) != NULL; docID++){ // read pathnames starting at docID pageDirectory/1 and up until these pathnames no longer open 
    webpage_t* page = pagedir_load(documentfile); // call load on each page
    if (page != NULL){
      indexPage(page, index, docID); // call indexPage to sort words
      webpage_delete(page);
    }
    sprintf(documentfile, "%s/%d", pageDirectory, docID); // update the pathname to the next doc
    fclose(fp);
  }
  mem_free(documentfile);
  FILE* indexFile = fopen(indexFilename, "w");
  index_iterate(index, indexFile);
  fclose(indexFile);
  index_delete(index);

}

static void
indexPage(webpage_t* page, index_t* index, int docID){
  int pos = 0;
  char* nextWord; 
  while ((nextWord = webpage_getNextWord(page, &pos)) != NULL){ // loop through all the words in the HTML
    if (strlen(nextWord) >= 3){
        char* word = word_normalize(nextWord);  // normalize to lowercase
        index_save(index, word, docID);   // save to index
    }
    mem_free(nextWord);
  }
}


static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename){    // defensive arg checks
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
  if (!pagedir_validate(argv[1])){
    exit (1);
  }
  *pageDirectory = argv[1];

  FILE* fp = fopen(argv[2], "a");
  if (fp == NULL){
    fprintf(stderr, "indexFile could not be created\n");
    exit (1);
  }
  fclose(fp);


  *indexFilename = argv[2];
}

