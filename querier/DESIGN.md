# CS50 TSE Crawler

## Design Spec
According to the Querier Requirements Spec, the TSE querier is a standalone program that reads the document files produced by the TSE crawler and index files produced by the TSE indexer, and answerswritten queries entered by the user.

## User interface
The querier's only interface with the user is on the command-line; it must always have two arguments.
```
querier pageDirectory indexFilename
```

For example if `letters` example, if letters is a pageDirectory in `../data`,
```
indexer ../data/letters ../data/letters.index
```

## Inputs and Outputs
Input: the querier loads the index given in the arguments and continues to answer
queries from the user.

Output: the querier prints out a sorted list of documents based on scores with words matching those from the query.

## Functional Decompostion into Modules
We anticipate the following modules or functions:
1. main, which simply calls `parseArgs` and `readQueries` and exits zero.
2. parseArgs, which given arguments from the command line, extract them into the function parameters; return only if successful.
3. readQueries, which accepts the query input from user and calls necessary methods to generate and print out matching documents with scores.
4. getWords, which checks that all characters in the query are spaces or letters, then parses the query to store each word in array. It returns the number of words.
5. validate which validates if query of words does not start or end with an operator and does not contain adjacent operators. Prints out the query if valid.
6. processQuery, which goes through each word and returns a counter of documents and their corresponding scores
7. merge, which combines the scores of two counters
8. unionSeq, which uses counters_iterate to add scores
9. unionFunc, which is a helper function for unionSeq. Combines the scores of two counters by adding the two
10. intersect, which uses counters_iterate to intersects scores for two counters
11. intersectFunc, which is a helper function for intersect. Combines the scores of two counters by finding the minimum.
12. min, which returns minimum of two integers.
13. numMatchDocs, which given a counters with scores, returns the number of matching docs.
14. getMaxScore, which replaces the max score if given value is greater

## Pseudocode
See pseudocode for methods in IMPLEMENTATION.md

## Major data structures
The key data structure is the 'index' that is loaded based on the indexFileName argument. Different counters are also used to keep track of the scores of documents containing words in the query. We also create structs max and twoCnt, that are documented in querier.c

## Testing plan
1. Test `querier` using various invalid arguments, incuding no arguments, one argument, three or more arguments, invalid pageDirectory (non-existent path), invalid pageDirectory (not a crawler directory), and invalid indexFileName (non-existent path)
2. Use fuzzquery to test `querier` with randomly generated prompts.
3. Test `querier` using various queries in queryCommands
4. Run make valgrind to ensure no memory is lost.