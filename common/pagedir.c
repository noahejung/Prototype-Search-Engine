#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "file.h"
#include "pagedir.h"

/**************** pagedir_init ****************/
/* see pagedir.h for description */
bool
pagedir_init(const char* pageDirectory){
    // allocate memory for page directory and the ".crawler" file to be added
    int pageDirectorySize = ((strlen(pageDirectory) + strlen("/.crawler")) + 1);
    char* pathname = mem_malloc(pageDirectorySize);

    // check if this allocated pathname properly allocated
    if (pathname == NULL){
        return false;
    }
    // if valid pathname allocation, add pageDirectory/.crawler to it
    sprintf(pathname, "%s/.crawler", pageDirectory);
    // open up the pathname as a file to create directory and /.crawler
    FILE* fp = fopen(pathname, "w");
    if (fp == NULL){    // if file open failed free and return false
        mem_free(pathname);
        return false;
    }
    else{
        fclose(fp); // else close file, free and return true
        mem_free(pathname);
        return true;
    }
}


/**************** pagedir_save ****************/
/* see pagedir.h for description */
void
pagedir_save(webpage_t* page, const char* pageDirectory, const int docID){
    // initialize a string for docID and insert docID int as a string into it
    char docIDstr[100];
    sprintf(docIDstr,"/%d", docID);

    if (pageDirectory == NULL || docIDstr == NULL){ // check for NULL strings
        fprintf(stderr,"NULL strings\n");
        exit(1);
    }
    if (page == NULL){  // check for NULL page
        fprintf(stderr,"NULL page\n");
        exit(1);
    }

    // allocate memory for page directory + docID pathname
    int pageDirectorySize = strlen(pageDirectory) + strlen(docIDstr) + 1;
    char* pathname = mem_malloc(pageDirectorySize);
    if (pathname == NULL){  // check for proper allocation
        fprintf(stderr, " Memory error to path\n");
        exit(1);
    }
    // print pageDirectory/docID to the pathname
    snprintf(pathname, pageDirectorySize, "%s/%d", pageDirectory, docID);
    FILE* fp = fopen(pathname, "w"); // open pathname
    if (fp == NULL){ // if failed, free and exit 1
        fprintf(stderr, "Error writing to path\n");
        mem_free(pathname);
        exit(1);
    }
    // get and initialize URL, depth, and HTML from the page
    char* URL = webpage_getURL(page);
    int depth = webpage_getDepth(page);
    char* HTML = webpage_getHTML(page);
    
    if (URL == NULL || HTML == NULL){ //check for NULLS
        fprintf(stderr, "URL, depth, or HTML, is NULL\n");
        fclose(fp);
        mem_free(pathname);
        exit(1);
    }
    // if valid variables, print to file 
    fprintf(fp, "%s\n", URL);
    fprintf(fp, "%d\n", depth);
    fprintf(fp, "%s", HTML);
    // close and free pathname
    fclose(fp);
    mem_free(pathname);
}

bool
pagedir_validate(const char* pageDirectory){
    char* pathname = mem_malloc(strlen(pageDirectory) + strlen("/.crawler") + 1);
    if (pathname == NULL){
        fprintf(stderr, "memory error");
        exit(1);
    }
    
    sprintf(pathname, "%s/.crawler", pageDirectory);
    FILE* fp = fopen(pathname, "r");
    if (fp == NULL){
        fprintf(stderr, "%s not found \n", pathname);
        mem_free(pathname);
        return false;
    }
    fclose(fp);
    mem_free(pathname);
    char* pathname2 = mem_malloc(strlen(pageDirectory) + strlen("/1") + 1);
    if (pathname2 == NULL){
        fprintf(stderr, "memory error");
        exit(1);
    }
    
    
    sprintf(pathname2, "%s/1", pageDirectory);
    FILE* fp2 = fopen(pathname2, "r");
    if (fp2 == NULL){
        fprintf(stderr, "%s not found \n", pathname2);
        mem_free(pathname2);
        return false;
    }
    fclose(fp2);
    mem_free(pathname2);
    return true;
}

webpage_t*
pagedir_load(const char* pathname){
    FILE* fp = fopen(pathname, "r");
    if (fp == NULL){
        return NULL;
    }
    char* URL = file_readLine(fp);
    char* toFree = file_readLine(fp);
    char* HTML = file_readFile(fp);
    
    fclose(fp);
    mem_free(toFree);
    webpage_t* loaded = webpage_new (URL, 0, HTML);
    if (loaded == NULL){
        mem_free(URL);
        mem_free(HTML);
    }
    return loaded;
}

    
