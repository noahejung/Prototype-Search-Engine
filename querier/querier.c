#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"
#include "counters.h"
#include "mem.h"

/* querier.c: queries user via stdin and returns a list of urls with the most frequent results
 * Author: Noah Jung
 */

static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* querier_indexBuild(char* indexFilename);
static void query(char* pageDirectory, index_t* index);
static bool verifyQuery(char* query);
static void tokenizeQuery(char* query, char** tokens, int* wordCount);
static void cleanQuery(char** queryWords);
static int findMaxDocID(char* pageDirectory);
static bool validateQuery(char** tokens);
static counters_t* pageScorer(index_t* index, char* pageDirectory, char** tokens, int maxDocID);
static void pageRank(counters_t* scores, char* pageDirectory, int maxDocID);
static void pageRank_iterator_helper(void* pageRankData, const int docID, const int score);
static void mergeSort(int docIDs[], int ranks[], int left, int right);
static void merge(int docIDs[], int ranks[], int left, int middle, int right);

int
main (int argc, char* argv[]){
  char* pageDirectory = NULL;
  char* indexFilename = NULL;
  parseArgs(argc, argv, &pageDirectory, &indexFilename);
  index_t* index = querier_indexBuild(indexFilename);
  query(pageDirectory, index);
}

/* parses command line arguments and initializes variables:
* Caller provides: command line arguments argc, argv[], and pointers to pageDirectory, and indexFilename
* Guarantees:
*  if NULL argv[], argv[1], or argv[2], prints to stderr and exits 1
*  if argc != 3 aka more or less than 2 command line arguments provided, print to stderr and exits 1
*  if pageDirectory is an invalid crawler-produced directory, print to stderr and exits 1
*  if indexFile does not exist and cannot be created at the specified path, print stderr and exits 1
*  Otherwise initialize pointers to pageDirectory and indexFilename from given argv[] command line inputs
*/
static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename){ 

  // defensive arg checks for NULLS or extraneous amount of inputs 
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

  // validate pagedirectory
  if (!pagedir_validate(argv[1])){
    fprintf(stderr, "error validating pageDirectory as a crawler directory\n");
    exit (1);
  }

  // verify or create indexFile
  FILE* fp = fopen(argv[2], "r");
  if (fp == NULL){
    fprintf(stderr, "indexFile could not be read\n");
    exit (1);
  }
  fclose(fp);

  // if passes checks initialize pageDirectory and indexFilename 
  *pageDirectory = argv[1];
  *indexFilename = argv[2];
}


/* reads indexFile and creates index structure provided by index.h :
* Caller provides: valid readable indexFilename
* Guarantees:
*   return of an index structure with words parsed from the indexFilename stored appropriately
*/

static index_t* 
querier_indexBuild(char* indexFilename){
  // find file length
  FILE* fp = fopen(indexFilename, "r");
  const int size = file_numLines(fp);
  fclose(fp);
  
  // create index as the length of file
  index_t* index;
  index = index_new(size);
  
  char* line;
  fp = fopen(indexFilename, "r");  // similar to indexer, will loop through each line of file and extract counter pairs as well as word
  while ((line = file_readLine(fp)) != NULL){
      char* word;
      char* strDocID;
      char* strCount;
      int docID;
      int count;
      counters_t* counter = counters_new();
      word = strtok(line, " ");
        
      while((strDocID = strtok(NULL, " ")) != NULL && ((strCount = strtok(NULL, " "))) != NULL){  // iterates through indexer file following the counter count pattern
          docID = atoi(strDocID);
          count = atoi(strCount);
          counters_set(counter, docID, count);
      }
      hashtable_t* indexHash = index_getHash(index);
      hashtable_insert(indexHash, word, counter);
      mem_free(line);
  }
  fclose(fp);
  return index;
}

/* calls the query for user to input words and handles them:
* Caller provides: command line arguments fed into standard input
* Guarantees:
*   query will be verified
*   if verified will be tokenized and cleaned 
*   if tokens are validated, calls pageScorer and pageRank
*   frees all used storage memory for tokens, query, and index
*/
static void 
query(char* pageDirectory, index_t* index){
  char* query;
  int maxDocID = findMaxDocID(pageDirectory);

  while ((query = file_readLine(stdin)) != NULL){
    if (verifyQuery(query)){
      char** tokens = mem_calloc(strlen(query), sizeof(char*)); 
      int wordCount = 0;
      tokenizeQuery(query, tokens, &wordCount);
      cleanQuery(tokens);
      if (validateQuery(tokens)){
        counters_t* pageScores = pageScorer(index, pageDirectory, tokens, maxDocID);
        pageRank(pageScores, pageDirectory, maxDocID);
        }
      for (int i = 0; i < wordCount; i++){    // free each individual slot of tokens[]
          mem_free(tokens[i]);
      }
      mem_free(tokens);
    }
     mem_free(query);
  }
  index_delete(index);
}


/* 
* Caller provides: char* string 
* Guarantees:
*   return false if any character in the string is not a letter or a space
*   return true otherwise
*/
static bool
verifyQuery(char* query){
  for (int i = 0; query[i] != '\0'; i++){
    char c = query[i];
    if((!isalpha(c)) && !isspace(c)){
      fprintf(stderr, "invalid query syntax\n");
      return false;
    }
  }
  return true;
}



/* splits query into individual words
* Caller provides:  allocated query string, allocated array to hold tokens, and pointer to an integer of wordCount
* Guarantee:
*   exit non-zero on memory error
*   will parse through the whole string, inserting words delimited by spaces into the token array
*   memory will be allocated for each word held in the array, caller is responsible for later freeing
*/
static void
tokenizeQuery(char* query, char** tokens, int* wordCount){
  int tempWordLength = 0;
  int count = 0;
  char* tempWord = mem_calloc(strlen(query), sizeof(char));
  if (tempWord == NULL){
    fprintf(stderr, "memory error");
    exit(1);
  }
  for (int i = 0; i <= strlen(query); i++){
    char c = query[i];
    if (isspace(c) || c == '\n' || c == '\0'){ 
      if (tempWordLength > 0){
        tempWord = word_normalize(tempWord);
        char* finalWord = mem_malloc(tempWordLength * sizeof(char) + 1);
        if (finalWord == NULL){
          fprintf(stderr, "memory error");
          exit (1);
        }
        strcpy(finalWord, tempWord);
        tokens[count] = finalWord;
         
        for (int i = 0; i < tempWordLength; i++){
          tempWord[i] = '\0';
        }
        count++;
        tempWordLength = 0;
      }
    }
    else {
      tempWord[tempWordLength] = c; // append character to the temp word
      tempWordLength++;
    }
  }
  *wordCount = count;
  mem_free(tempWord);
}


/* prints each word from the token array */
static void
cleanQuery(char** tokens){
  for (int i = 0; tokens[i] != NULL; i++){
    printf("| %s ", tokens[i]);
  }
  printf("|\n");
}



/* finds highest document ID in directory */
static int
findMaxDocID(char* pageDirectory){
  int maxDocID = 1;
  char* docIDcheck = mem_malloc((strlen(pageDirectory)+ 21) * sizeof(char) + 1);  // 21 is a sufficient number for integer length
  sprintf(docIDcheck, "%s/%d", pageDirectory, maxDocID);
  FILE* fp;
  while ((fp = fopen(docIDcheck, "r")) != NULL){
    fclose(fp);
    maxDocID++;
    sprintf(docIDcheck, "%s/%d", pageDirectory, maxDocID);
  }
  mem_free(docIDcheck);
  return maxDocID;
}


/* handles logic to validate the content of the query,
Caller provides:
    properly allocated array of words
Guarantees:
    False if and or or is at beginning/end of query, or if and/or occur consecutively 
 */
static bool
validateQuery(char** tokens){
  if (tokens == NULL || tokens [0] == NULL){
    fprintf(stderr, "NULL tokens received\n");
    return false;
  }
  if (strcmp(tokens[0], "and") == 0 || strcmp(tokens[0], "or") == 0){
    fprintf(stderr, "cannot have 'and' or 'or' at the beginning of the query\n");
    return false;
  }
  char* prev = NULL;
  char* curr;
  for (int i = 0; tokens[i] != NULL; i++){
    curr = tokens[i];
    if (prev != NULL && (strcmp(curr, "and") == 0 || strcmp(curr, "or") == 0)){
      if (strcmp(prev, "and") == 0 || strcmp(prev, "or") == 0){
        fprintf(stderr, "cannot have 'and' and/or 'or' back to back\n");
        return false;
      }
    }
    prev = curr;
  }
  if (strcmp(prev, "and") == 0 || strcmp(prev, "or") == 0){
    fprintf(stderr, "cannot have 'and' or 'or' at the end of the query\n");
    return false;
  }
  return true;
}


/* Scores pages based on and and or logic operators w/ frequency of query tokens
Caller provides:
  Index with properly indexed words, counts, and docIDs
  Valid page Directory
  Valid tokens array
  Integer for the highest Doc ID
Guarantees:
  Documents will be ranked in unspecified order within a counters object
*/
static counters_t*
pageScorer(index_t* index, char* pageDirectory, char** tokens, int maxDocID){
  counters_t* pageScores = counters_new();
  hashtable_t* hashtable = index_getHash(index);

  // create a pageRank for each doc
  for (int docID = 1; docID <= maxDocID; docID++){    // loop through docID's
    int finalScore = 0;
    for (int i = 0; tokens[i] != NULL; i++){  // for each docID, check query arguments and add to pageRank accordingly          
      int tempFinalScore = maxDocID;
      while (tokens[i] != NULL && strcmp(tokens[i], "or") != 0){
        char* current = tokens[i];
        int tempScore;
        
        counters_t* tokenMatch = hashtable_find(hashtable, current); 
        if (tokenMatch == NULL){
          tempScore = 0;
        }
        else{
          tempScore = counters_get(tokenMatch, docID);
        }
        
        if (tempScore < tempFinalScore){
          tempFinalScore = tempScore;
        }
        i++;
      }
      if (tempFinalScore != maxDocID){
        finalScore += tempFinalScore;
      }
    }
    counters_set(pageScores, docID, finalScore);
  }
  return pageScores;
}

/* Scores pages based on and and or logic operators w/ frequency of query tokens
Caller provides:
  Counters with correlated scores to docID's provided by page Scorer
  Valid page Directory
  Integer for the highest Doc ID
Guarantees:
  Documents will be ranked in descending order by score and printed to std output accordingly
*/
static void
pageRank(counters_t* scores, char* pageDirectory, int maxDocID){
  int* docIDs = mem_calloc(maxDocID, sizeof(int));  // holds docID's
  int* ranks = mem_calloc(maxDocID, sizeof(int));  // holds scores 


  set_t* pageRankData = set_new();
  set_insert(pageRankData, "docIDs", &docIDs);
  set_insert(pageRankData, "ranks", &ranks);
  
  counters_iterate(scores, pageRankData, &pageRank_iterator_helper);
  set_delete(pageRankData, NULL);

  mergeSort(docIDs, ranks, 0, maxDocID -1);

  for (int i = 0; i < maxDocID; i++){         
    if (docIDs[0] == 0){      // if first input of docIDsis empty, no documents 
      printf("No documents match\n");
      break;
    }
    if (docIDs[i] == 0){    // ignore NULL docID values
      continue;
    }
    char* pathname = mem_malloc((strlen(pageDirectory) + 21) * sizeof(char) + 1);
    if (pathname == NULL){
      fprintf(stderr,"memory error\n");
      exit(1); 
    }
    sprintf(pathname, "%s/%d", pageDirectory, docIDs[i]);
    FILE* fp = fopen(pathname, "r");
    if (fp == NULL){
       fprintf(stderr, "error reading crawler file from %s\n", pathname);
       mem_free(pathname);
       continue;
    }
    char* URL = file_readLine(fp);    // URL on first line of fp

    printf("rank: %d | docID: %d | URL: %s\n", ranks[i], docIDs[i], URL);

    fclose(fp);
    mem_free(pathname);
    mem_free(URL);
  }
  counters_delete(scores);
  mem_free(docIDs);
  mem_free(ranks);
}


/* item func for iterating through counters and appropriately using docID and score data
*/
static void
pageRank_iterator_helper(void* pageRankData, const int docID, const int score){
  if (score > 0){
    int** docIDs = set_find(pageRankData, "docIDs");
    int** ranks = set_find(pageRankData, "ranks");
    (*docIDs)[docID - 1] = docID;
    (*ranks)[docID - 1] = score;
  }
}

/* Recursively sorts docIDs and ranks via mergsort algorithm, uses merge to combine segments*/ 
static void 
mergeSort(int docIDs[], int ranks[], int left, int right){
  if (left < right){
    int middle = (left + right - 1) / 2;  
    mergeSort(docIDs, ranks, left, middle);
    mergeSort(docIDs, ranks, middle + 1, right);
    merge(docIDs, ranks, left, middle, right);
  }
}


/* merges two segments of docIDs and ranks into one*/
static void
merge(int docIDs[], int ranks[], int left, int middle, int right){
  int n1 = middle - left + 1;
  int n2 = right - middle;

  // Temporary arrays
  int leftDocIDs[n1], rightDocIDs[n2];
  int leftScores[n1], rightScores[n2];

  // Copy data to temporary arrays
  for (int i = 0; i < n1; i++) {
      leftDocIDs[i] = docIDs[left + i];
      leftScores[i] = ranks[left + i];
  }
  for (int j = 0; j < n2; j++) {
      rightDocIDs[j] = docIDs[middle + 1 + j];
      rightScores[j] = ranks[middle + 1 + j];
  }

  // Merge the temporary arrays back into the original arrays
  int i = 0; // Initial index of first subarray
  int j = 0; // Initial index of second subarray
  int k = left; // Initial index of merged subarray

  while (i < n1 && j < n2) {
      if (leftScores[i] >= rightScores[j]) {
          docIDs[k] = leftDocIDs[i];
          ranks[k] = leftScores[i];
          i++;
      } else {
          docIDs[k] = rightDocIDs[j];
          ranks[k] = rightScores[j];
          j++;
      }
      k++;
  }

  // Copy the remaining elements of left[], if there are any
  while (i < n1) {
      docIDs[k] = leftDocIDs[i];
      ranks[k] = leftScores[i];
      i++;
      k++;
  }

  // Copy the remaining elements of right[], if there are any
  while (j < n2) {
      docIDs[k] = rightDocIDs[j];
      ranks[k] = rightScores[j];
      j++;
      k++;
  }
}