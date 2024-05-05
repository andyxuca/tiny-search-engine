# Makefile for 'querier'
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

all: querier fuzzquery

# executable depends on object files
querier: querier.o $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: fuzzquery.o $(LLIBS)
			 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

# object files depend on include files
querier.o: $C/pagedir.h $C/index.h $C/word.h $L/webpage.h $L/file.h
fuzzquery.o: $L/mem.h $L/file.h

test: querier testing.sh
	bash -v testing.sh

valgrind: querier
	$(VALGRIND) ./fuzzquery ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx 1 0 | ./querier ~/cs50-dev/shared/tse/output/wikipedia_2 ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f querier
	rm -f fuzzquery