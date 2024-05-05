# Makefile for 'indexer'
#
# Andy Xu, May 2023

C = ../common
L = ../libcs50
OBJS = 
LIBS =
LLIBS = $C/common.a $L/libcs50.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

# executable depends on object files
indexer: indexer.o $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

# object files depend on include files
indexer.o: $C/pagedir.h $C/index.h $C/word.h 
indexer.o: $L/webpage.h $L/mem.h $L/file.h $L/hashtable.h 

indextest.o: $C/index.h $L/mem.h $L/file.h $L/hashtable.h

.PHONY: all test valgrind clean

test: indexer testing.sh
	bash -v testing.sh

valgrind: indexer indextest
	$(VALGRIND)  ./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../index_to_scrape1_valgrind
	$(VALGRIND) ./indextest ../index_to_scrape1_valgrind ../index_to_scrape1_valgrind_tested

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest