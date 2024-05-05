# Makefile for 'crawler'
#
# Andy Xu, April 2023

L = ../libcs50
C = ../common
OBJS = crawler.o
LIBS =
LLIBS = $L/libcs50.a $C/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: crawler

# executable depends on object files
crawler: $(OBJS) $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

# object files depend on include files
crawler.o: $C/pagedir.h $L/webpage.h $L/mem.h $L/bag.h $L/hashtable.h

.PHONY: all test valgrind clean

test: crawler testing.sh
	bash -v testing.sh

valgrind: crawler
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data 0

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler