# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md)
and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#implementation-spec)
may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures

We pull from our `indexFilename` output created by our `indexer` and create an internal `index` structure with specific size based on the number lines found in the output file.

## Control flow

The querier is implemented in one file `querier.c`, with a multitude of functions.

Certainly! Here's the pseudo code for the Querier module rewritten in Markdown format:

```markdown
## main

- Call `parseArgs` with `argc`, `argv`, and pointers to `pageDirectory`, `indexFilename`.
- Call `querier_indexBuild` with `indexFilename` to build the index.
- Call `query` with `pageDirectory` and the built index.
- Cleanup and exit.

## parseArgs

- Check if `argc` equals 3. If not, print error and exit.
- Validate `pageDirectory` using `pagedir_validate`. If invalid, print error and exit.
- Check if `indexFile` exists and is readable. If not, print error and exit.
- Assign `argv[1]` to `pageDirectory` and `argv[2]` to `indexFilename`.

## querier_indexBuild

- Open `indexFilename` and determine its size.
- Create a new index with the determined size.
- For each line in `indexFilename`:
  - Parse the word and its document ID/count pairs.
  - For each pair, update the index with the word, document ID, and count.
- Close the file and return the index.

## query

- Find the maximum document ID in `pageDirectory`.
- Loop to read queries from `stdin` until EOF:
  - If `verifyQuery` returns true:
    - Allocate memory for tokens and initialize `wordCount`.
    - Tokenize the query into `tokens`.
    - Clean the query by printing `tokens`.
    - If `validateQuery` returns true:
      - Score pages using `pageScorer`.
      - Rank pages using `pageRank`.
    - Free `tokens` and continue.
  - Free the query string.
- Delete the index to clean up.

## verifyQuery

- Loop through each character in the query string:
  - If a character is not a letter or a space, print error and return false.
- Return true if all characters are valid.

## tokenizeQuery

- Allocate memory for `tempWord` based on query length.
- Loop through the query string:
  - If a space or end of string is encountered and `tempWordLength` > 0:
    - Normalize `tempWord`.
    - Allocate memory for `finalWord` and copy `tempWord` to it.
    - Assign `finalWord` to `tokens` array and reset `tempWord`.
  - Else, append character to `tempWord`.
- Set `wordCount` to count and free `tempWord`.

## cleanQuery

- Loop through `tokens`:
  - Print each token surrounded by '|'.
- Print the closing '|' and a newline.

## findMaxDocID

- Initialize `maxDocID` to 1.
- Loop to construct pathname for each document ID:
  - If the file exists, increment `maxDocID` and continue.
  - If the file does not exist, break the loop.
- Return `maxDocID - 1` as the maximum document ID found.

## validateQuery

- Check if `tokens` array is null or first token is null. If so, print error and return false.
- Check if first or last token is 'and' or 'or'. If so, print error and return false.
- Loop through `tokens` to check for consecutive 'and'/'or'. If found, print error and return false.
- Return true if query is valid.

## pageScorer

- Create a new counters structure for page scores.
- Loop through each document ID:
  - For each token, find its score for the document ID.
  - Compute final score based on 'and'/'or' logic.
  - Update `pageScores` with the computed score for the document ID.
- Return `pageScores`.

## pageRank

- Allocate arrays for `docIDs` and `ranks` based on `maxDocID`.
- Use `pageRank_iterator_helper` to populate `docIDs` and `ranks` from `scores`.
- Sort `docIDs` and `ranks` using `mergeSort`.
- Loop through sorted `docIDs` and `ranks`:
  - For each `docID`, construct the pathname and read URL.
  - Print rank, `docID`, and URL.
- Cleanup allocated memory.

## pageRank_iterator_helper

- If score is greater than 0:
  - Find `docIDs` and `ranks` arrays in `pageRankData`.
  - Update `docIDs` and `ranks` with the document ID and score.

## mergeSort

- If `left` < `right`:
  - Calculate middle index.
  - Recursively sort left and right halves.
  - Merge sorted halves via `merge`

## merge

- Create temporary arrays for left and right halves.
- Copy data to temporary arrays.
- Merge temporary arrays back into original arrays based on score.
```

## Other modules

### pagedir

We create a re-usable module `pagedir.c` to handle initialization and validation of a pageDirectory, as well as reading and writing out page files
Pseudocode for `pagedir_init`:

        construct the pathname for the .crawler file in that directory
        open the file for writing; on error, return false.
    close the file and return true.


Pseudocode for `pagedir_save`:

    construct the pathname for the page file in pageDirectory
    open that file for writing
    print the URL
    print the depth
    print the contents of the webpage
    close the file

Pseudocode for `pagedir_validate`:

    construct the pathname for the pageDirectory and check for the `.crawler` file
    construct the pathname for the pageDirectory and check for the first docID file (this would be a file titled "1")
    if both exists, return true
    
Pseudocode for `pagedir_load`:

    read the first line and retrieve the url
    read the second line and retrieve the toFree/depth
    retrieve the rest of the file for the html
    free the toFree/depth
    lad the webpage with the items
    free the url and html
    return the webpage

### index

We create a module `index.c` to create our datastructure for
the hashtable. The index will store words, their document locations, and their respective frequencies at each document.

Pseudocode for `index_new`:
    
    allocate memory for index and create hashtable
    return pointer to empty index object

Pseudocode for `index_save`:

    check if the word is already in the hashtable
    if no word, create new counter and insert into hashtable with word as key
    Otherwise, increment counter already at the key

Pseudocode for `index_delete`:

    call hashtable delete and call a helper functioner to delete all counters, sets, and hashtables from the index
    free the index

Pseudocode for `index_iterate`:

    call the hashtable iterate and call the helper to iterate through hashtable

### word

We use the module `word.c` to convert any word to all lowercase;


Pseudocode for `normalize_word`:

    iterate through the whole word until you reach null'\0'
        update the pointer at each index of the string array to a lowercased character
    return the word

### libcs50

We leverage the modules of libcs50, most notably `counter`,
`hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t`
objects, to fetch a page from the Internet, and to scan a (fetched)
page for URLs; in that regard, it serves as the *pagefetcher*
described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so
our crawler need not implement that part of the spec.

## Function prototypes

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

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

## Error handling and recovery

All the command-line parameters are rigorously checked before any data
structures are allocated or work begins; problems result in a message
printed to stderr and a non-zero exit status.

We anticipate out-of-memory errors to be rare and thus allow the
program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using
variants of the `mem_assert` functions), e.g., if a function receives
bad parameters.


## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

The `testing.sh` has hardcoded testcases passing invalid and valid arguments into the indexer

To test, simply `make test`.
See `testing.out` for testing details
