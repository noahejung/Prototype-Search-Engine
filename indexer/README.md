# CS50 TSE indexer (Lab 4)
## Noah Jung (noahejung)

### Usage

`indexer.c` makes use of *index* struct defined externally. `indexer.c` implements the following methods:

```c
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* page, hashtable_t* indicies, int docID);
```

### Implementation


Give a `pageDirectory` we check if it is a directory made by a crawler. If so we create an `index` and scan each file within the directory. These files are turned into webpages which can make use of external `webpage.c` functions to 
read the files' HTML word by word. These words are stored in the index, which works like a hashtable of word keys, with counter items held holding the document id of each file where the word was found and its frequency.

`indexer.c` takes 2 arguments:
char* `pageDirectory` - the Directory where crawled files are stored
char* `indexFile` - the file to output the final index's contents to 

`parseArgs` will verify these arguments have been provided correctly

`indexBuild` will read through each file, and call `pagedir_load` from the external *pagedir.c* module. It will create a webpage from the file so that 'indexPage' can be called on them.

`indexPage` will read each page it receives word by word, and stores each word 3 characters or longer in the index via `index_save` from *index.c*

### Assumptions

pageDirectory has files named 1, 2, 3, ..., without gaps.

The content of files in pageDirectory follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
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
