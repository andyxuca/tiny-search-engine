/*
* crawler.c      Andy Xu     April 25, 2023
*
* Description: The crawler crawls websites beginning from the given seedURL
* and repeats the process for websites found on each site to a specified max depth.
* It stores the HTML code of each site in a specified folder.
*
* Usage: ./crawler seedURL pageDirectory maxDepth
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagedir.h"
#include "mem.h"
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"

/*** function prototypes ***/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/*
* main() - calls parseArgs and crawl and then exits 0
*/
int 
main(const int argc, char* argv[])
{
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    exit(0);
}

/*
* parseArgs() - extracts function parameters given the initial parameters from main
*/
static void 
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
    //check if number of arguments is correct
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments entered.");
        exit(1);
    }
    
    //normalize seed URL
    *seedURL = argv[1];
    char* normURL = NULL;
    normURL = normalizeURL(*seedURL);
    if (normURL == NULL || !isInternalURL(normURL)) {
        mem_free(normURL);
        fprintf(stderr, "URL is invalid.");
        exit(2);
    }
    mem_free(normURL);
        
    //check page directory
     *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Page directory is invalid.");
        exit(3);
    }

    //check that max depth is in range
    char* maxDepthInput = argv[3];
    //convert maxDepth to int using scanf
    char nextchar;
    if (sscanf(maxDepthInput, "%d%c", maxDepth, &nextchar) != 1) {
        fprintf(stderr, "Max Depth is not an a valid integer.");
        exit(4);
    }
    else {
        if(*maxDepth < 0 || *maxDepth > 10) {
            fprintf(stderr, "Max Depth is not in range. It must be between 0 and 10 inclusive.");
            exit (5);
        }
    }
}

/*
* crawl() - recieves seedURL, pageDirectory and maxDepth. Crawls from seedURL to maxDepth and saves
* pages in pageDirectory.
*/
static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth) 
{
    //Copy the URL 
    char* copySeed = mem_malloc(strlen(seedURL) + 1);
    strcpy(copySeed, seedURL);

    //initialize the hashtable and add the seedURL
    hashtable_t* ht = mem_assert(hashtable_new(200), "Hashtable cannot be created.");
    if (!hashtable_insert(ht, seedURL, "")) {
        fprintf(stderr, "SeedURL cannot be inserted into the hashtable.");
        exit(5);
    }

    //initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* toCrawl = mem_assert(bag_new(), "Bag can not be created.");
    bag_insert(toCrawl, webpage_new(copySeed, 0, NULL));

    webpage_t* nextPage = NULL;
    int depth = 0;
    while ((nextPage = (webpage_t*)bag_extract(toCrawl)) != NULL) {
        //fetch the HTML for the webpage
        if (webpage_fetch(nextPage)) {
            pagedir_save(nextPage, pageDirectory, ++depth); 
            if(webpage_getDepth(nextPage) < maxDepth) {
                pageScan(nextPage, toCrawl, ht);
            }
        }
        //delete webpage
        webpage_delete(nextPage);
    }

    //delete hashtable and bag
    hashtable_delete(ht, NULL);
    bag_delete(toCrawl, webpage_delete);
}

/*
* pageScan() - scans the given webpage to extract any links, adding them to the 
* bag of pagesToCrawl and marking ones it has scanned in the hashtable of pagesSeen.
*/
static void 
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    char* nextURL = NULL;
    int depth = 0;
    //while there is another URL in the page
    while ((nextURL = webpage_getNextURL(page, &depth)) != NULL) {
        char* normURL = normalizeURL(nextURL);
        if (normURL != NULL && isInternalURL(normURL)) {
            //insert webpage into hashtable
            if (hashtable_insert(pagesSeen, normURL, "")) {
                //create a webpage_t for it
                webpage_t* nextWebPage = webpage_new(normURL, webpage_getDepth(page)+1, NULL);

                //insert webpage into bag
                bag_insert(pagesToCrawl, nextWebPage);
            }
        }
        else {
            fprintf(stderr, "URL is invalid.");
        }
    }
    
    mem_free(nextURL); //free the URL
}





