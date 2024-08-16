# CS50 TSE Common Library (Lab 4)
## Noah Jung (noahejung)

## Usage 
Run `make clean` to clean up 

`pagedir.c` takes *webpage* structs defined externally.   `pagedir.c` implements the following methods:

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pathname);
```

`pagedir_init` initializes the page directory with a file ./crawler inside
`pagedir_save` saves pages into the directory
`pagedir_validate` checks if directory has a ./crawler in inside
`pagedir_load` reads from page directory file and returns webpage

`word.c` takes string and returns it in all lowercase.  `word.c` implements the following methods:

```c
char* word_normalize(char* word);
```
`word_normalize` normalizes word to all lowercase;


`index.c` creates an internally defined index struct which holds a hashtable to hold words and counters of the docID's theyre found on.  `index.c` implements the following methods:

```c
index_t* index_new (const int size);
void index_delete (index_t* index);
void index_save (index_t* index, const char* word, const int count);
void index_iterate(index_t* index, FILE* indexFile);
```
`index_new` initializes index
`index_save` saves items to index
`index_delete` deletes index
`index_iterate` iterates through all items in index

## Assumptions

No assumptions beyond those that are clear from the spec.

### Compilation

`make` to compile, also creates `common.a`
`make clean` to clean up
## Files
 
 * `Makefile` - make/compile procedure
 * `pagedir.c` - implementation
 * `pagedir.h` - interface
 * `word.c` - implementation
 * `word.h` - interface
 * `index.c` - implementation
 * `index.h` - interface
 * `.gitignore` - git ignore file