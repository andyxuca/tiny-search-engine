# CS50 TSE Crawler

### crawler

A `crawler` crawls through a website based on a URL, extracts and stores any embedded URLs it finds on the website, and continues to crawl each of the new URLs until a specified max depth. Every website the crawler crawls through is documented in a separate file.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `testing.sh` - tester file
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make`.

### Testing

The `testing.sh` program tests every method in crawler with normal and edge cases.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.
