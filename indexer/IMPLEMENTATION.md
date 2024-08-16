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

We use the data structure of 'index', which is a modified version of our 'hashtable' as it will now take the 'counter' data structure to record the count of a specific word (that being implemented as a key in the greater hashtable structure) and recording with current document ID (the counter will have it down as (documentID, count)). The size of the index is impossible to know, thus we use 900 for the index/hashtable.

## Control flow

The Indexer is implemented in one file `indexer.c`, with four functions.

### main

The `main` function calls `parseArgs` and `indexBuild`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function
parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()` to check if the directory is crawler-produced
* for `indexFilename`, ensure that the file can be created.
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Begin to index through the directory and build onto your index structure.

Pseudocode:
    creates a new 'index' object
    loops over document ID numbers, counting from 1
        loads a webpage from the document file 'pageDirectory/id'
        if successful,
            passes the webpage and docID to indexPage

### indexPage

This function iterates through the words of the file and creates or increments a count pertaining to them. To do so, the word has to be of length no less than 3 then the word will be "normalized". In other words, the word will then be converted to all lowercase.

Pseudocode:
    steps through each word of the webpage,
        skips trivial words (less than length 3),
        normalizes the word (converts to lower case),
        looks up the word in the index,
            adding the word to the index if needed
        increments the count of occurrences of this word in this docID

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

//### word

We use the module `word.c` to convert any word to all lowercase;


Pseudocode for `normalize_word`:

    iterate through the whole word until you reach null'\0'
        update the pointer at each index of the string array to a lowercased character
    return the word

//### libcs50

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

### indexer

Detailed descriptions of each function's interface is provided as a
paragraph comment prior to each function's implementation in
`indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* page, index_t* index, int docID);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
```

### pagedir

Detailed descriptions of each function's interface is provided as a
paragraph comment prior to each function's declaration in `pagedir.h`
and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pathname);
```

### index

Detailed descriptions of each function's interface is provided as a
paragraph comment prior to each function's declaration in `index.h`
and is not repeated here.

```c
index_t* index_new (const int size);
void index_delete (index_t* index);
void index_save (index_t* index, const char* word, const int docID);
void index_iterate(index_t* index, FILE* indexFile);
static void counters_delete_wrapper(void* item);
static void index_iterate_helper (void* indexFile, const char* key, void* counter);
static void index_counters_iterate_helper (void* indexFile, int key, int count);
```

### word

Detailed descriptions of each function's interface is provided as a
paragraph comment prior to each function's declaration in `word.h` and
is not repeated here.

```c
char* word_normalize(char* word);
```

//## Error handling and recovery

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

There are four units (indexer, pagedir, word, and index).
The indexer represents the whole system and is covered below.
The pagedir unit is tiny; it could be tested using a small C 'driver'
to invoke its functions with various arguments, but it is likely
sufficient to observe its behavior during the system test.
The word is sufficient since it simply changes the word to lowecase.
The index unit is small; it could be tested with a small C 'driver' to
invoke its function with various arguments, but it is lkely
sufficient.

//### Regression testing

The indexer may take a second to print out its index structure out.
For routine regression tests, we crawl the results of the `crawler`
function and its directory and iterate through each file in that
directory and add it to the hashtable and set the counter for that
specific word.

//### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times,
with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing
each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case.
Run that script with `bash -v testing.sh` so the output of indexer is
intermixed with the commands used to invoke the indexer.
Verify correct behavior by studying the output, and by sampling the
files created in the respective indexFiles.