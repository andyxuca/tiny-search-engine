
/*
* index.c      Andy Xu      May 2, 2023
*
* Description: This file implements the index module as specified in index.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "file.h"
#include "mem.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"

/*** local functions ***/
static void display_word(void* fp, const char* word, void* ctr);
static void display_count(void* fp, const int docID, const int cnt);
static void delete_count(void* item);

/*** global types ***/
typedef hashtable_t index_t;

/*** local functions ***/

/*
* display_word() - used to print a word followed by the docID keys and counts
* based on its corresponding counters 
*/
static void 
display_word(void* fp, const char* word, void* ctr)
{
    fprintf((FILE*)fp, "%s ", word);
    counters_iterate((counters_t*)ctr, fp, display_count);
    fprintf((FILE*)fp, "\n");
}

/*
* display_count() - prints a given docID followed by a given count
*/
static void 
display_count(void* fp, const int docID, const int cnt)
{
  fprintf((FILE*)fp, "%d %d ", docID, cnt);
}

/*
* delete_count() deletes a counter
*/
static void 
delete_count(void* item)
{
  counters_t* ctr = item;
  counters_delete(ctr);
}

/*** global functions ***/

/*
* index_new() - see index.h for description
*/
index_t*
index_new(const int num_slots)
{
  return (index_t*)(hashtable_new(num_slots));
}

/*
* index_insert() - see index.h for description
*/
bool 
index_insert(index_t* ind, char* word, counters_t* ctr)
{
  return hashtable_insert(ind, word, ctr);
}

/*
* index_find() - see index.h for description
*/
counters_t* 
index_find(index_t* ind, char* word)
{
  return (counters_t*)(hashtable_find(ind, word));
}

/*
* index_save() - see index.h for description
*/
void 
index_save(index_t* ind, char* indexFileName)
{
    FILE* fp = NULL;
    fp = fopen(indexFileName, "w");
    if (ind != NULL && fp != NULL) {
       hashtable_iterate(ind, fp, display_word);
    }
    fclose(fp);
}


/*
* index_delete() - see index.h for description
*/
void 
index_delete(index_t* ind)
{
    hashtable_delete(ind, delete_count);
}

/*
* index_load - see index.h for description
*/
index_t* 
index_load(FILE* fp)
{
  if (fp == NULL) {
    fprintf(stderr, "file pointer is null");
    return NULL;
  }
  else {
    //create index
    int numLines = file_numLines(fp);
    index_t* index = index_new(2*numLines);
    if (index == NULL) {
      fprintf(stderr, "cannot create index");
      return NULL;
    }

    //read through file and update index
    char* curWord = NULL;
    while ((curWord = file_readWord(fp)) != NULL) {
      //create new counter for word
      counters_t* counter = mem_assert(counters_new(), "cannot create counter");

      //retrieve docID's and counts
      int docID;
      int count;
      while ((fscanf(fp, " %d %d", &docID, &count) == 2)) {
        if (!counters_set(counter, docID, count)) {
          fprintf(stderr, "counters set failed");
        }
      }
      if (!hashtable_insert(index, curWord, counter)) {
        fprintf(stderr, "hashtable insert failed");
      }

      mem_free(curWord);
    }

    //close file
    fclose(fp);
    return index;
  }
}


