# CS50 TSE Querier

### querier

A `querier` reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.

This implementation implements all aspects of functionality described in the assignmen t.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `testing.sh` - tester file
* `testing.out` - result of `make test &> testing.out`
* `fuzzquery.c` - used for testing
*  `queryCommands` - test commands

### Compilation

To compile, simply `make`.

### Testing

The `testing.sh` program tests every method in crawler with normal and edge cases.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.
