# testing.h     Andy Xu      May 12 2023
#
# Description: tester for querier

# no arguments
./querier

# one argument
./querier 1

# three arguments
./querier 1 2 3

# more than three arguments
./querier 1 2 3 4

# invalid pageDirectory (non-existent path)
./querier ../nonexistentPath ../index_files/1

# invalid pageDirectory (not a crawler directory)
./querier ../indexer ../index_files/1

# invalid indexFile (non-existent path)
./querier ~/cs50-dev/shared/tse/output/letters-1 ../nonexistentPath/1

# test various query commands in bash file
./querier ~/cs50-dev/shared/tse/output/wikipedia_2 ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx < queryCommands

# test using fuzzquery
./fuzzquery ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx 10 0 | ./querier ~/cs50-dev/shared/tse/output/wikipedia_2 ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx
