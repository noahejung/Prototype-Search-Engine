# CS50 TSE querier (Lab 6)
## Noah Jung (noahejung)

### Usage

`querier.c` makes use of *index* struct defined internally. `querier.c` implements the following methods:

```c
int main (int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* querier_indexBuild(char* indexFilename);
static void query(char* pageDirectory, index_t* index);
static bool verifyQuery(char* query);
static void tokenizeQuery(char* query, char** tokens, int* wordCount);
static void cleanQuery(char** tokens);
static int findMaxDocID(char* pageDirectory);
static bool validateQuery(char** tokens);
static counters_t* pageScorer(index_t* index, char* pageDirectory, char** tokens, int maxDocID);
static void pageRank(counters_t* scores, char* pageDirectory, int maxDocID);
static void pageRank_iterator_helper(void* pageRankData, const int docID, const int score);
static void mergeSort(int docIDs[], int ranks[], int left, int right);
static void merge(int docIDs[], int ranks[], int left, int middle, int right);
```

### Implementation


Give a `pageDirectory` we check if it is a directory made by a crawler. Give a `indexFilename` and check if it exists. If so we create an `index` and scan each line to the index. Similar to the `indexer` module, we keep a reference of frequencies of words that correspond in each docID. We will use this structure to allow the user to input a list of word(s) that may consist of ANDs/ORs and prints out the most useful pages with the most frequency sightings of the word(s).

`querier.c` takes 2 arguments:
char* `pageDirectory` - the Directory where crawled files are stored
char* `indexFile` - the output file of the indexer so you can create an internal index struct 

`parseArgs` will verify these arguments have been provided correctly
`querier_indexBuild` creates the internal index struct
`query` main function that accounts for all querys
`verifyQuery` makes sure the query does not contains any numbers or special characters
`tokenizeQuery` tokenizes the words in the query
`cleanQuery` prints the the query where the spaces are replaced with "|"
`findMaxDocID` locates the highest DocID
`validateQuery` checks for any misusages with "AND" or "OR"
`pageScorer` iterates and calculates the page ranks of the pages respective to the words that the user had put
`pageRank` sorts the pages by their score
`pageRank_iterator_helper` helper function for page rank
`mergeSort` sorting algorithm to merge all items to one list
`merge` main sort algorithm for merge

### Assumptions

pageDirectory has files named 1, 2, 3, ..., without gaps.

The content of files in pageDirectory follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `testing.sh` - test file
* `testing.out` - output of testing.sh from `make test`
* `.gitignore` - git ignore file

### Compilation

`make` to compile.
`make clean` to clean up.
`make test` to create `testing.out` and appropriate directories .

### Testing

The `testing.sh` has hardcoded testcases passing invalid and valid arguments into the indexer

To test, simply `make test`.
See `testing.out` for testing details
