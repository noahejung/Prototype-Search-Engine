/* crawler.h - crawler file for CS50 TSE crawler
*
* A 'crawler' calls a string of commands to validate seedURL, pageDirectory, and maxDepth inputs
* It will scan through the seedURL's HTML content, if additional URLs are found within that content,
* it will scan through those URLS iteratively in a DFS manner until no more are found or maxDepth 
* iterations occur. It will log and create files from each URL and its relevant information printed 
* inside per valid webpage. These files will be stored at pageDirectory
* Noah Jung Winter 2024
*/

#ifndef CRAWLER_H
#define CRAWLER_H

  #include <unistd.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "hashtable.h"
  #include "bag.h"
  #include "webpage.h"
  #include "mem.h"
  #include "pagedir.h"

/**************** functions ****************/


/**************** parseArgs ****************/
/* Parse command line arguments, initialize variables
* Caller provides:
* Command line arguments argc and argv[] as well as pointers for seedURL, pageDirectory, and maxDepth
* We guarantee:
*   NULL seedURL, pageDirectory, or maxDepth causes error 
*   invalid URL address from seedURL causes error
*   invalid pageDirectory address causes error 
*   maxDepth is between 0 and 10 or else error
*   if successful, seedURL, pageDirectory, and maxDepth will be all be updated in main
 */
void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);

/**************** crawl ****************/
/* Executes the crawling process 
* Caller provides:
*   Valid seedURL, pageDirectory, and maxDepth inputs
* We guarantee:
*   initialization of a hashtable and bag to hold webpages to crawl and crawled webpage URLs respectively
*   all URLS embedded from the seedURL up to the iterative depth will be checked 
*   all valid webpages will be saved in pagedir.c's pagedir_save
*   all valid webpages not at maxDepth, will be called to pageScan
*   invalid bag and hashtable creation will return errors
*/
void crawl(const char* seedURL, const char* pageDirectory, const int maxDepth);

/**************** pageScan ****************/
/* Scans a webpage, extracting URLS and processing them
* Caller provides:
*   Valid webpage, bag of pages to crawl, and hashtable of pages seen
* We guarantee:
*   error if any arg is NULL
*   ignores webpages with NULL html content
*   each non-NULL webpage HTML will have webpage_getNextURL from webpage.c called on it
*   all returned URLS that are non-NULL. internal, and not already seen, will be inserted into bag and hashtable
 */
void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

#endif // CRAWLER_H

