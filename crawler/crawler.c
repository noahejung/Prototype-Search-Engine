  #include <unistd.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "hashtable.h"
  #include "bag.h"
  #include "webpage.h"
  #include "mem.h"
  #include "pagedir.h"

  // local functions
  static void parseArgs(const int argc, char* argv[],  char** seedURL, char** pageDirectory, int* maxDepth);
  static void crawl(char* seedURL,  char* pageDirectory, const int maxDepth);
  static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

  // main function, takes args from command line
  int
  main(const int argc, char* argv[]){
    // initialize seedURL, pageDirectory, and maxDepth to be used across
    // crawler.c if parseArgs is succesful
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    return 0;
  }

/**************** parseArgs ****************/
/* see crawler.h for description */
  static void
  parseArgs(const int argc, char* argv[],  char** seedURL, char** pageDirectory, int* maxDepth){
    if (argc != 4){ // of more or less than 3 args provided
      fprintf(stderr, "expected 3 args, received %d\n", argc);
      exit(1);
    } 

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL){ // if any of those 3 args is NULL
      fprintf(stderr, "null arg received\n");
      exit(1);
    }

    // Update seedURL pageDirectory, and maxDepth to the associated arguments 
    *seedURL = argv[1];
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]);

    // create a normalizedURL to check if it is internal and non-null
    char* normalizedURL = normalizeURL(*seedURL);
    if (!isInternalURL(normalizedURL) || normalizedURL == NULL){
      fprintf(stderr, "invalid seedURL\n");
      mem_free(normalizedURL);
      exit(1);
    }
    mem_free(normalizedURL);

    // call pagedir_init to try to create pageDirectory, exit if failed
    if (!pagedir_init(*pageDirectory)){
      fprintf(stderr, "invalid pageDirectory\n");
      exit(1);
    } 

    // check that maxDepth is between 0 and 10
    if (*maxDepth < 0 || *maxDepth > 10 ){
      fprintf(stderr, "invalid maxDepth\n");
      exit(1);
    }
  }

/**************** crawl ****************/
/* see crawler.h for description */
  static void
  crawl(char* seedURL,  char* pageDirectory,  int maxDepth){
    
    // allocate for a copy of seedURL and copy it over
    char* seedURLcpy = mem_malloc(strlen(seedURL) + 1);
    strcpy(seedURLcpy, seedURL);

    int docID = 1; // docID initialized to 0

    hashtable_t* ht = hashtable_new(200); // hashtable initialized with 200 slots
    if (ht == NULL){  // check for NULL hashtable
      fprintf(stderr, "hashtable could not be created\n");
      hashtable_delete(ht, NULL);
      exit(1);
    }
    hashtable_insert(ht, seedURLcpy, ""); // if not NULL, insert seedURL to Hashtable

    bag_t* bag = bag_new(); // bag initialized
    if (bag == NULL){ // check for NULL bag
      fprintf(stderr, "bag could not be created\n");
      bag_delete(bag, webpage_delete);
      exit(1);
    }
    // if not NULL, create webpage_t struct and insert into bag
    webpage_t* page = webpage_new(seedURLcpy, 0, NULL);
    bag_insert(bag, page);

    // initialize 'current' webpage 
    webpage_t* current;
    while ((current = bag_extract(bag)) != NULL){ // loop through and extract pages from bag
      //sleep(1); // sleep 1 second for server handling
      if (webpage_fetch(current)){  // successful fetch will update current->HTML
        // calls pagedir_save on current page, and increments docID
        pagedir_save(current, pageDirectory, docID++); 
        
        // if current-> depth is less than maxDepth, call it to pageScan
        if (webpage_getDepth(current) < maxDepth){  
          pageScan(current, bag, ht);
        }
      }
      // free memory for each current webpage at the end of each loop
      webpage_delete(current); 
    }
    // frees memory allocated for hashtable and bag
    bag_delete(bag, webpage_delete);
    hashtable_delete(ht, NULL);
  }

/**************** pageScan ****************/
/* see crawler.h for description */
 static void
  pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    // check for NULL args
    if (page == NULL || pagesToCrawl == NULL || pagesSeen == NULL){
      fprintf(stderr, "pageScan Error\n");
      exit(1);
    }
    
    // initalize variables
    char* next = NULL;  // will hold each URL found
    int pos = 0; // will hold position on current webpage, 0 starts from the beginning
    int curDepth = webpage_getDepth(page); // holds the depth of the current webpage
    
    if (webpage_getHTML(page) != NULL){ // if the HTML of page is not null
      
      // scan the current webpage's HTML and update 'next' to the next URL it finds, 
      // loop as long as not NULL
      while ((next = webpage_getNextURL(page, &pos)) != NULL){
        char* normalizedURL = normalizeURL(next); // normalize and check URL for internal
        if (normalizedURL != NULL){
          if (isInternalURL(normalizedURL) ){

            // tries to insert into hashtable, true if succesful
            if (hashtable_insert(pagesSeen, normalizedURL, "")){

              // create new webpage based off the current 'next' URL, 
              // depth = original depth + 1, inserts into bag
              webpage_t* webpage = webpage_new(normalizedURL, curDepth+1, NULL);
              bag_insert(pagesToCrawl, webpage);
            } else {
              // if URL was not inserted free it
              mem_free(normalizedURL);
            }
          } else {
            // if URL was not inserted free it
            mem_free(normalizedURL);
          }
        } 
        // free next URL at end of each loop
        mem_free(next);
      }
      // try to insert the original URL just in case, nothing happens if fails
      hashtable_insert(pagesSeen, webpage_getURL(page), "");
    }
  }
  