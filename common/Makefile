# Makefile for common library
# Builds and compiles pagedir functions into a library.
#
# Andy Xu, April 2023

L = ../libcs50

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

all: $(LIB)

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $L/webpage.h $L/file.h $L/mem.h
index.o: index.h $L/counters.h $L/hashtable.h $L/file.h $L/mem.h
word.o: word.h

# index.o: index.c 
# 	$(CC) $(CFLAGS) -c $^

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
