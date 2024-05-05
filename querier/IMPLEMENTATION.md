# CS50 TSE Crawler

## Implementation Spec
In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. Here we focus on the core subset:

* Data structures
* Control flow: pseudo code for overall flow, and for each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan

## Data structures
The key data structure is the 'index' that is loaded based on the indexFileName argument. Different counters are also used to keep track of the scores of documents containing words in the query. We also create structs max and twoCnt, that are documented in querier.c

## Control flow
The querier is implemented in one file `querier.c`, with 14 functions.

### main
The `main` function simply calls `parseArgs` and `readQueries` and exits zero.

### parseArgs
Given arguments from the command line, extract them into the function parameters; return only if successful.
* for `pageDirectory`, ensure that it exists and is crawler produced using pagedir_validate()
* for `indexFileName`, ensure that the file can be opened
* if any trouble is found, print an error stderr and exit non-zero

## readQueries
Accepts the query input from user and calls necessary methods to generate and print out matching documents with scores.
```
while there is query input
    get words in query
    validate query
        process words in query to scores
        get word count
        if word count is greater than zero
            display scores in sorted order
```
## getWords
Checks that all characters in the query are spaces or letters, then parses the query to store each word in array. It returns the number of words.
```
loops over letters
    check that input has no special characters
loops over letters
    skip spaces
    set pointer to first letter of word
    set pointer to end of word
    normalize word
    add word to array
    increment word count
return word count
```
## validate
Validates if query of words does not start or end with an operator and does not contain adjacent operators. Prints out the query if valid.
```
check if first or last word is "and" or "or"
loop through words
    check if there are adjecent operators
```

## processQuery
Goes through each word and returns a counter of documents and their corresponding scores
```
create counters for andSequence and orSequence
create boolean for when to stop 
loop over words
    if word is or
        merge andSequence and orSequence
        set stop to false
    if stop is true
        continue
    if word is and
        continue
    else
        get scores for word  
        add to andSequence  
merge andSequence and orSequence
return result
```
## merge
Combines the scores of two counters
```
call unionSeq
```
## unionSeq
Uses counters_iterate to add scores

## unionFunc
Helper function for unionSeq. Combines the scores of two counters by adding the two

## intersect
Uses counters_iterate to intersects scores for two counters

## intersectFunc
Helper function for intersect. Combines the scores of two counters by finding the minimum.

## min
Returns minimum of two integers.

## numMatchDocs
Given a counters with scores, returns the number of matching docs.

## getMaxScore
Replaces the max score if given value is greater

## Function prototypes 

### querier
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in querier.c and is not repeated here.
```c
static void readQueries(char* pageDirectory, index_t* index);
static int getWords(char* line, char** words);
static bool validate(char** words, int wordCount);
static counters_t* processQuery(char** words, int wordCount, index_t* index);
static void merge(counters_t** andSeq, counters_t** orSeq);
static void unionSeq(counters_t* orSeq, counters_t* newWord);
static void unionFunc(void* arg, int key, const int count);
static void intersect(counters_t* andSeq, counters_t* newWord);
static void intersectFunc(void* arg, int key, const int count);
static int min(const int a, const int b);
static void numMatchDocs(void* arg, const int key, const int count);
static void getMaxScore(void* arg, const int key, const int count);
```

## Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

## Testing plan
For testing, we write a script `testing.sh` that tests the querier using various
prompts from `queryCommands`. These prompts include error-inducing prompts with bad parameters,
normal prompts using the index for wikipedia-2, and fuzzquery prompts.