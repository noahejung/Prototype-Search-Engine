# CS50 TSE Crawler (Lab 4)
## Noah Jung (noahejung)

### Usage

`crawler.c` makes use of *hashtable* and *bag* structs defined externally. `crawler.c` implements the following methods:

```c
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```

### Implementation

We make a `bag` of pages to crawl and a `hashtable` of the URLS of seen pages. Both are initialized empty and the size of the hashtable is initialized to 200  This file also uses the *webpage* struct implemented externally in `webpage.c` and functions provided externally in `pagedir.c`

`crawler.c` takes 3 arguments:
char* `seedURL` - the URL the crawler starts from
char* `pageDirectory` - the Directory where the crawler stores HTML data it crawls
const int `maxDepth` - the maximum iterative depth the crawler will go through

`parseArgs` will verify these arguments have been provided correctly so that

`crawl` will perform a DFS through the hashtable of URLS, starting from the  seedURL, and saves pages within the max depth. These saved pages are inserted into the page directory via `pagedir_save` from `pagedir.c` with a docID as the filename, which is just a counted integer that increments by 1 each time a new page is saved.

`pageScan` will be fed webpages from `crawl` and extracts any URL links found within the HTML of the site that have not been scanned yet. It will update the hashtable via this process

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `crawler.h` - the interface
* `testing.sh` - test file
* `testing.out` - output of testing.sh from `make test`
* `.gitignore` - git ignore file

### Compilation

`make` to compile.
`make clean` to clean up.
`make test` to create `testing.out` and appropriate directories .

### Testing

The `testing.sh` has hardcoded testcases passing invalid and valid arguments into the crawler

To test, simply `make test`.
See `testing.out` for testing details
