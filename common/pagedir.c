/*
* pagedir.c     Andy Xu      April 26, 2023
*
* Description: Implements the functions described in pagedir.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "file.h"
#include "mem.h"
#include "webpage.h"

/*
* pagedir_init() - see pagedir.h for description
*/
bool 
pagedir_init(const char* pageDirectory)
{
    //construct the pathname for the .crawler file in that directory
    char *path = mem_malloc_assert((strlen(pageDirectory) + strlen("/.crawler") + 1), "can not make memory for pathname");
    if (path == NULL) {
        fprintf(stderr, "can not create pathname\n");
        exit(1);
    }
    strcpy(path, pageDirectory);
    strcat(path, "/.crawler");

    // open the file for writing
    FILE *fp = NULL;
    fp = fopen(path, "w");
    if (fp == NULL) {
        mem_free(path);
        return false;
    }

    // close the file and return true
    fclose(fp);
    mem_free(path);
    return true;
}

/*
* pagedir_save) - see pagedir.h for description
*/
void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    //construct the pathname for the page file in pageDirectory
    char *stringID = mem_malloc(10);  
    sprintf(stringID, "%d", docID);
    char *path = mem_malloc_assert((strlen(pageDirectory) + 10), "can not create filename");
    strcpy(path, pageDirectory);
    strcat(path, "/");
    strcat(path, stringID);
    mem_free(stringID);
    
    //open that file for writing
    FILE *fp = NULL;
    fp = fopen(path, "w");
    if (fp == NULL) {
        mem_free(path);
        fprintf(stderr, "can not open file");
        exit(2);
    }

    //print the URL
    char *url = webpage_getURL(page);
    if (url == NULL) {
        fprintf(stderr, "can not open URL");
        fclose(fp);
        exit(3);
    }
    else {
        fprintf(fp, "%s\n", url);
    }

    //print the depth
    fprintf(fp, "%d\n", webpage_getDepth(page));

    //print the contents of the webpage
    char *webCont = webpage_getHTML(page);
    if (webCont == NULL) {
        fprintf(stderr, "can not get html content");
        fclose(fp);
        exit(4);
    }
    else {
        fprintf(fp, "%s", webCont);
    }
    
    mem_free(path);

    //close the file
    if (fclose(fp) != 0) {
        fprintf(stderr, "error while closing the file %d\n", docID);
        exit(5);
    }
}

/*
* pagedir_validate() - see pagedir.h for description
*/
bool 
pagedir_validate(const char* pageDirectory)
{
    //create filename for .crawler file in directory
    char *fileCheck = mem_malloc_assert((strlen(pageDirectory) + strlen("/.crawler") + 1), "cannot allocate memory for filename");
    strcpy(fileCheck,pageDirectory);
    strcat(fileCheck, "/.crawler");

    //check if file .crawler exists in directory
    FILE *fp = NULL;
    fp = fopen(fileCheck, "r'");
    if (fp != NULL) {
        fclose(fp);
        mem_free(fileCheck);
        return true;
    }
    else {
        mem_free(fileCheck);
        return false;
    }
}

/*
* pagedir_load() - see pagedir.h for description
*/
webpage_t*
pagedir_load(const char* pageDirectory, const int docID)
{
     //construct the pathname for the file in pageDirectory
    char *stringID = mem_malloc(10);  
    sprintf(stringID, "%d", docID);
    char *path = mem_malloc_assert((strlen(pageDirectory) + 10), "can not create filename");
    strcpy(path, pageDirectory);
    strcat(path, "/");
    strcat(path, stringID);
    mem_free(stringID);

    //open file for reading
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp != NULL) {
        //read contents from file
        char* url = file_readLine(fp);
        char* depthRead = file_readLine(fp);
        char* html = file_readFile(fp);
        fclose(fp);
        mem_free(path);

        //convert depth to int
        int depth;
        sscanf(depthRead, "%d", &depth);
        mem_free(depthRead);

        //create webpage
        webpage_t* page = mem_assert(webpage_new(url, depth, html), "cannot create webpage");
        return page;
    }
    else {
        mem_free(path);
        return NULL;
    }
}