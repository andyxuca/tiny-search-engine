/*
* index.h     Andy Xu     May 2, 2023
*
* Description: This file is the header for the index module.
*/

#ifndef __INDEX_H__
#define __INDEX_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "counters.h"
#include "hashtable.h"

/*** global types ***/
typedef hashtable_t index_t;

/*** functions ***/

/*
* index_new() - creates a new index with a given number of slots
*
* Caller provides an int number of slots
* index_new() constructs and returns a new index hashtable with the number of slots.
*/
index_t* index_new(const int num_slots);

/*
* index_insert() - inserts a new word and counter into the index
*
* Caller provides a functional index, a string word, and a counters
* index_insert() inserts the word and counters (using the word as the key
* and the counters as the value) into the index using hashtable_insert()
*/
bool index_insert(index_t* ind, char* word, counters_t* ctr);

/*
* index_find() - finds a given word in the index
*
* Caller provides a functional index and a string word
* index_find() retrieves the counters value for the given word in the index
* using  hashtable_find()
*/
counters_t* index_find(index_t* ind, char* word);

/*
* index_save() - saves a given index into a given file
*
* Caller provides a functional index and a string indexFileName
* index_save() saves the index into the file called indexFileName by passing
* a local method for printing to hashtable_iterate() 
*/
void index_save(index_t* ind, char* indexFileName);

/*
* index_delete() - deletes a given index
*
* Caller provides an index
* index_delete() frees the memory for the index using hashtable_delete()
*/
void index_delete(index_t* ind);

/*
* index_load() - loads and returns an index based a given file
*
* Caller provides a file with index details
* index_load() creates an index by reading through the file, retrieving each word with its
* corresponding docID's and counts, storing these values into appropriate counters, and adding
* all of this into the index. The index is returned at the end.
*/
index_t* index_load(FILE* fp);


#endif //  __INDEX_H__