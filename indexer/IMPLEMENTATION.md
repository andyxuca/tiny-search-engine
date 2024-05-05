# CS50 TSE Crawler

## Implementation Spec
In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. Here we focus on the core subset:

* Data structures
* Control flow: pseudo code for overall flow, and for each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan

## Data structures
The key data structure is the 'index', mapping from word to (docID, #occurrences) pairs. The index is a 'hashtable' keyed by word and storing 'counters' as items. The 'counters' is keyed by docID and stores a count of the number of occurrences of that word in the document with that ID. The 'index' starts empty. Since the size (number of slots) is impossible to determine in advance, we use 700.

## Control flow
The indexer is implemented in one file `indexer.c`, with four functions.

### main
The `main` function simply calls `parseArgs` and `indexBuild`, then calls `index_save` and `index_delete` from the index module. Finally, it exits zero.

### parseArgs
Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, ensure that it exists and is crawler produced using pagedir_validate()
* for `indexFileName`, ensure that the file can be opened
* if any trouble is found, print an error stderr and exit non-zero

### indexBuild
Builds the index by looping over all files in the pageDirectory and passsing the loaded webpage and docID to indexPage. Pseudocode:
```
creates a new 'index' object
loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
        passes the webpage and docID to indexPage
```

### indexPage
Counts the number of times each word appears in the given webpage and updates the index accordingly. Pseudocode:
```
steps through each word of the webpage,
    skips trivial words (less than length 3),
    normalizes the word (converts to lower case),
    looks up the word in the index,
        adding the word to the index if needed
    increments the count of occurrences of this word in this docID
```

## Other Modules

### pagedir
We extend `pagedir.c` to handle the `pagedir_validate` (which checks if the pageDirectory exists and contains a file called ".crawler") and `pagedir_load`(which creates a webpage based on the url, depth, and HTML stored in a file) methods.

Pseudocode for `pagedir_validate`
```
create filename for .crawler file in directory
check if file .crawler exists in directory
```

Pseudocode for `pagedir_load`
```
construct the pathname for the file in pageDirectory
open file for reading
read url, depth, and html from file
convert depth to int
create and return webpage
```

### index
We create a re-usable module `index.c` that provides the data structure to represent the in-memory index, and functions to read and write index files. 

Pseudocode for `index_new`
```
Construct and return a new index hashtable with the given number of slots.
```

Pseudocode for `index_insert`
```
Insert the word and counters (using the word as the key
and the counters as the value) into the index using hashtable_insert()
```

Pseudocode for `index_find`
```
Retrieve the counters value for the given word in the index
using  hashtable_find()
```

Pseudocode for `index_save`
```
Save the index into the file called indexFileName by passing 
a local method for printing to hashtable_iterate() 
```

Pseudocode for `index_delete`
```
free the memory for the index using hashtable_delete()
```

Pseudocode for `index_load`
```
create an index 
open the file
read through the file and retrieve each word with its corresponding docID's and counts
store these values into counters 
add words and counters into index
return index
```

### word
We create a re-usable module `word.c` that provides a function to normalize a word.

Pseudocode for `normalizeWord`
```
loop through letters of word and lowercase all of them
```

## Function prototypes 

### indexer
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in indexer.c and is not repeated here.
```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(webpage_t* curPage, int docID, index_t* index);
```

### pagedir
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

### index
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* ind, char* word, counters_t* ctr);
counters_t* index_find(index_t* ind, char* word);
void index_save(index_t* ind, char* indexFileName);
void index_delete(index_t* ind);
index_t* index_load(FILE* fp);
```

### word
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.

```c
char* normalizeWord(char *word);
```

## Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

## Testing plan

### Unit testing
A program *indextest* serves as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

### Regression testing
For regression testing, we create and validate the index for toscrape-1, comparing the result using the *indexcmp* program provided in the cs50-dev shared folder.

### Integration/system testing
We write a script `testing.sh` to test the indexer, as a complete program. We test it by building an index from a pageDirectory, and then the resulting index will be validated by running it through the *indextest*  to ensure it can be loaded and *indexcmp* to ensure that it is accurate.
* We test *indexer* with various invalid arguments
* We run *indexer* on different pageDirectories and validate the results in *indextest* and *indexcmp*
* We run *valgrind* on both indexer and indextest to ensure there are no memory leaks