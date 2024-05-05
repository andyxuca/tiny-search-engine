/*
* querier.c     Andy Xu     May 9, 2023
*
* Description: This file implements the querier, which reads the index produced 
* by the Indexer and the page files produced by the Crawler, to interactively 
* answer written queries entered by the user.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "file.h"
#include "mem.h"
#include "pagedir.h"
#include "webpage.h"
#include "hashtable.h"
#include "index.h"
#include "word.h"

/*** function prototypes ***/
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
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
int fileno(FILE *stream);


//stores doc with max score
struct max{
  int maxDoc;
  int maxScore;
};

//stores two counters for merging
struct twoCnt {
  counters_t* andSeq;
  counters_t* newWord;
};

/*
* main() - calls parseArgs and readQueries and exits 0
*/
int main(const int argc, char* argv[])
{
    char* pageDirectory = NULL;
    char* indexFileName = NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFileName);

    //load the index from indexFilename into an internal data structure
    FILE* fp = fopen(indexFileName, "r");
    index_t* ind = mem_assert(index_load(fp), "cannot load index");

    //read search queries from stdin, one per line, until EOF
    readQueries(pageDirectory, ind);

    index_delete(ind);
    exit(0);
}

/*
* parseArgs() - extracts and validates function parameters given the initial parameters from main
*/
static 
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
    if (argc != 3) {
        fprintf(stderr, "Error: wrong number of arguments. Must only have two arguments: pageDirectory and indexFilename.");
        exit(1);
    }

    *pageDirectory = argv[1];
    if (!pagedir_validate(*pageDirectory)) {
        fprintf(stderr, "Error: pageDirectory is invalid, either does not exist or is not crawler produced.");
        exit(2);
    }

    *indexFilename = argv[2];
    FILE* fp = fopen(*indexFilename, "r");
    if (fp != NULL) { 
        fclose(fp);
    }
    else {
        fprintf(stderr, "Error: indexFilename is invalid.");
        exit(3);
    }
}

/*
* readQueries() - given a pageDirectory and index, readQueries accepts the inputted query
* calls all the necessary methods to create a counters of scores for the query, and prints it out.
*/
static void 
readQueries(char* pageDirectory, index_t* index)
{
    char line[200];
	
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
	while (fgets(line, 200, stdin) != NULL) {
		char* words[strlen(line)];
		int numWords = getWords(line, words);
		if (numWords != 0 && words != NULL){
            if (validate(words, numWords)) {
			    counters_t* scores = processQuery(words, numWords, index);
                int numDocs = 0;
                counters_iterate(scores, &numDocs, numMatchDocs);
                printf("%d\n", numDocs);

                //no documents satisfy query
                if (numDocs == 0) {
                    printf("%s\n", "No documents match.");
                }
                else {
                    //rank resulting docs according to score and print out details
                    printf("Matches %d documents (ranked):\n", numDocs);

                    //loop through all docs
                    for (int i = 0; i<numDocs; i++) {
                        struct max* curMax = (struct max*)mem_malloc(sizeof(struct max));
                        curMax->maxDoc = 0;
                        curMax->maxScore = 0;
                        counters_iterate(scores, (void*) curMax, getMaxScore);

                        //print document details
                        webpage_t* getPage = pagedir_load(pageDirectory, curMax->maxDoc);
                        char* url = webpage_getURL(getPage);
                        printf("score %d  doc %d: %s\n", curMax->maxScore, curMax->maxDoc, url);
                        mem_free(url);
                        counters_set(scores, curMax->maxDoc, 0);
                        mem_free(curMax);
                    }
                }
                printf("------------------------------\n");
                counters_delete(scores);
            }
		}
	
	    if (isatty(fileno(stdin))) {
            printf("\nQuery? ");
        }
	}
	
	printf("\n");
}

/*
* getWords() - given a string query and an array of strings, getWords checks that
* all characters in the query are spaces or letters, then parses the query to store
* each word in array. It returns the number of words.
*/
static int
getWords(char* line, char** words)
{
    int numWords = 0;
    int len = strlen(line);

    //check that input has no special characters
    for (int i = 0; i<len; i++) {
        if (!isspace(line[i]) && !isalpha(line[i])) {
            fprintf(stderr, "invalid character in query\n");
            return 0;
        }
    }

    //store separate words into array and calculate number of words
	char* word = NULL;
	char* rest = NULL;
    for (int i = 0; i < len; i++) {
		if (isalpha(line[i])) {
			word = &line[i]; 

            rest = word;
            while (isalpha(*rest)) {
                rest++;
                i++;
            }
            *rest = '\0';

            //normalize word
			words[numWords] = normalizeWord(word);
			numWords++;
		}
	}
    
    return numWords;
}

/*
* validate() - validates if query of words does not start or end with an operator 
* and does not contain adjacent operators. Prints out the query if valid.
*/
static bool
validate(char** words, int wordCount)
{
    //check if first or last word is an operator
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 || strcmp(words[wordCount - 1], "and") == 0 || strcmp(words[wordCount - 1], "or") == 0) {
        fprintf(stderr, "first or last word cannot be an operator\n");
        return false;
    } 

    //check if there are any adjacent operators
    for (int i = 0; i < wordCount - 1; i++) {
        if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
            if (strcmp(words[i + 1], "and") == 0 || strcmp(words[i + 1], "or") == 0) {
                fprintf(stderr, "operators cannot be adjacent to each other\n");
                return false;
            }
        }
    }

    // print the normalized query
    printf("%s", "Query:");
    for (int i = 0; i < wordCount; i++) {
        printf(" %s", words[i]);
    }
    printf("\n");
    return true;
}

/*
* processQuery() - given an array of words representing the query, the number of words, and an index, 
* processQuery() goes through each word and returns a counter of documents and their corresponding scores
*/
static counters_t*
processQuery(char** words, int wordCount, index_t* index)
{
    counters_t *andSeq = NULL;
    counters_t *orSeq = NULL;
    bool stop = false;

    //loop over words
    for (int i = 0; i<wordCount; i++) {
        //if word is "or", merge to andSeq
        if (strcmp(words[i], "or") == 0) {
            merge(&andSeq, &orSeq);
            stop = false;
            continue;
        }

        if (stop) {
            continue;
        }

        //if word is "and", continue to next word
        if (strcmp(words[i], "and") == 0) {
            continue;
        }
        
        //get documents that match with word
        counters_t* pages = index_find(index, words[i]);
        if (pages == NULL) {
            stop = true;                
            if (andSeq != NULL) {
                counters_delete(andSeq); 
                andSeq = NULL;
            }
        }
        else {
            if (andSeq == NULL) {
                andSeq = mem_assert(counters_new(), "cannot create counter");
                //add matching documents to andSeq
                unionSeq(andSeq, pages);
            }
            else {
                //intersect andSeq and matching documents for word
                intersect(andSeq, pages);
            }
        }
    } 
    
    //merge andSeq and orSeq for final scores for all words
    merge(&andSeq, &orSeq);
    return orSeq;
}

/*
* merge() - combines the scores of two counters
*/
static void 
merge(counters_t** andSeq, counters_t** orSeq)
{
  if (*andSeq != NULL) {
    if (*orSeq == NULL) {
      *orSeq = mem_assert(counters_new(), "");
    }
    unionSeq(*orSeq, *andSeq);  
    counters_delete(*andSeq);
    *andSeq = NULL;
  }
}

/*
* unionSeq() - iterates through counters using unionFunc
*/
static void 
unionSeq(counters_t* orSeq, counters_t* newWord)
{
  mem_assert(orSeq, "cannot create memory for counter");
  mem_assert(newWord, "cannot create memory for counter");
  counters_iterate(newWord, orSeq, unionFunc);
}

/*
* unionFunc() - combines the scores of two counters by adding the two
*/
static void
unionFunc(void* arg, int key, const int count)
{
  counters_t* useArg = arg;
  int wordCount = counters_get(useArg, key);
  counters_set(useArg, key, count + wordCount);
}

/*
* intersect() - intersects scores for two counters
*/
static void
intersect(counters_t* andSeq, counters_t* newWord)
{
    mem_assert(andSeq, "cannot create memory for counter");
    mem_assert(newWord, "cannot create memory for counter");
    struct twoCnt args = {andSeq, newWord};
    counters_iterate(andSeq, &args, intersectFunc);
}

/*
* intersectFunc() - combines the scores of two counters by finding the minimum
* of the two
*/
static void
intersectFunc(void* arg, int key, const int count)
{
    struct twoCnt* args = (struct twoCnt*)arg;
    counters_set(args->andSeq, key, min(count, counters_get(args->newWord, key)));
}

/*
* min() - returns the minimum of two integers
*/
static int 
min(const int a, const int b)
{
  return (a > b) ? b : a;
}

/*
* numMatchDocs() - given a counters with scores, returns the number of matching docs 
*/
static void
numMatchDocs(void* arg, const int key, const int count)
{
    int* cnt = arg;
    if (count > 0) {
        (*cnt)++;
    } 
}

/*
* maxScore() - replaces the max score if given value is greater
*/
static void 
getMaxScore(void* arg, const int key, const int count)
{
  struct max* curMax = arg;
  if (count > curMax->maxScore) {
    curMax->maxDoc = key;
    curMax->maxScore = count;
  }
}