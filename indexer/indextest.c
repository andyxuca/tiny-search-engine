/*
* indextest.c     Andy Xu     May 4, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "file.h"
#include "mem.h"
#include "webpage.h"
#include "counters.h"
#include "hashtable.h"

int 
main(const int argc, char* argv[])
{
    //file names
    char* readFile = NULL;
    char* writeFile = NULL;

    //get inputted file names
    if (argc != 3) {
        fprintf(stderr, "please enter 2 file names.");
        exit(1);
    }
    else {
        readFile = argv[1];
        writeFile = argv[2];
    }

    //open file to read
    FILE *fpRead = fopen(readFile, "r");
    if (fpRead == NULL){
        fprintf(stderr, "cannot open file to read");
        exit(1);
    }
/*
    FILE *fpWrite = fopen(*writeFile, "r");
    if (fpWrite == NULL){
        fprintf(stderr, "cannot open file to write");
        exit(2)
    }
*/
    //load index
    index_t* index = index_load(fpRead);
    if (index == NULL) {
        fprintf(stderr, "cannot load index");
        exit(4);
    }

    //write index to new file
    index_save(index, writeFile);

    //delete index
    index_delete(index);
}