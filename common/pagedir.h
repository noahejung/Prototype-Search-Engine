/* pagedir.h - pagedir header file for CS50 TSE pagedir
*
* A 'pagedir' provides 2 methods to initialize a page directory and save to page directory
* these methods are to be used externally for the TSE (so far in crawler.c)
* Noah Jung Winter 2024
*/
#ifndef PAGEDIR_H
#define PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "file.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/* Initializes a directory for saving crawled pages.
* Caller provides:
*   pageDirectory name to be initialized
* We guarantee:
*   pageDirectory will only be oepened if successfully allocated
*   if pageDirectory is a valid pathname, a .crawler file will be inserted into the directory
* We return:
*   false on failed allocation or invalid pathname
*   true on succesful creation of directory 
* Note:
*   a succesful directory will contain a .crawler file
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Saves a webpage to a file in the page directory with the document ID as the filename.
* Caller provides:
*   webpage_t page
*   pageDirectory and docID names
* We guarantee:
*   if valid inputs, a file named as docID with the webpage's URL, depth, and HTML is created
*   this file will be in the page directory
*/
void pagedir_save(webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Checks if /.crawler and /1 is in directory
* Caller provides:
*   pageDirectory 
* We guarantee:
*   if /.crawler and /1 is in directory, return true
*/
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Reads text from file and produces webpage 
* Caller provides:
*   File produced by crawler
* We guarantee:
*   If file is openable, and webpage creation works, returned a webpage with URL and HTML from crawler file
*/
webpage_t* pagedir_load(const char* pathname);

#endif // PAGEDIR_H
