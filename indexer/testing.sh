# testing.h     Andy Xu      May 4 2023
#
# Description: tester for indexer

# no arguments
./indexer

# one argument
./indexer 1

# three arguments
./indexer 1 2 3

# more than three arguments
./indexer 1 2 3 4

# invalid pageDirectory (non-existent path)
./indexer ../nonexistentPath ../index_files/1

# invalid pageDirectory (not a crawler directory)
./indexer ../indexer ../index_files/1

# invalid indexFile (non-existent path)
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../nonexistentPath/1

# invalid indexFile (read-only directory)
mkdir ../readOnlyDir
chmod 444 ../readOnlyDir
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../readOnlyDir/1

# invalid indexFile (existing, read-only file)
touch readOnlyIndexFile
chmod 444 readOnlyIndexFile
./indexer ~/cs50-dev/shared/tse/output/letters-1 readOnlyIndexFile

# first valid indexer example (toscrape-1)
mkdir ../index_files
./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../index_files/index_to_scrape1
# running indextest
./indextest ../index_files/index_to_scrape1 ../index_files/index_to_scrape1_tested
# comparing original file with file generated by indextest
~/cs50-dev/shared/tse/indexcmp ../index_files/index_to_scrape1 ../index_files/index_to_scrape1_tested
# comparing generated index file with index file provided in shared
~/cs50-dev/shared/tse/indexcmp ../index_files/index_to_scrape1 ~/cs50-dev/shared/tse/indices/toscrape-1.ndx

# second valid indexer example (wikipedia-2)
./indexer ~/cs50-dev/shared/tse/output/wikipedia_2 ../index_files/index_wikipedia2
# running indextest
./indextest ../index_files/index_wikipedia2 ../index_files/index_wikipedia2_tested
# comparing original file with file generated by indextest
~/cs50-dev/shared/tse/indexcmp ../index_files/index_wikipedia2 ../index_files/index_wikipedia2_tested
# comparing generated index file with index file provided in shared
~/cs50-dev/shared/tse/indexcmp ../index_files/index_wikipedia2 ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx
