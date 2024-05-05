# CS50 TSE Crawler

### indexer

An `indexer` reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `indextest.c` - program that tests implementation by loading index and creating a new file based on index
* `testing.sh` - tester file for indexer
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make`.

### Testing

The `testing.sh` program tests every method in indexer with normal and edge cases.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.