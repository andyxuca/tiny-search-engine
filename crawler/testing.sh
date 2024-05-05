# testing.h     Andy Xu      April 28, 2023
#
# Description: tester for crawler

#zero arguments
./crawler

#one argument
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/

#four arguments
./crawler 1 2 3 4

#create directory for documents
mkdir ../data

#invalid URL (not internal)
./crawler https://home.dartmouth.edu/ ../data -100

#nonexistent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../blob -100

#maxDepth less than 0
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data -100

#maxDepth greater than 10
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data 100

#letters with depth 0
mkdir ../data/letters0
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters0 0

#letters with depth 1
mkdir ../data/letters1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters1 1

#letters with depth 2
mkdir ../data/letters2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters2 2

#letters with depth 1 but start from different seed page
mkdir ../data/letters1_new_seed 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../data/letters1_new_seed 1

#toscrape with depth 0
mkdir ../data/toscrape0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape0 0

#toscrape with depth 1
mkdir ../data/toscrape1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape1 1

#toscrape with depth 2
#mkdir ../data/toscrape2
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape2 2

#toscrape with depth 3
#mkdir ../data/toscrape3
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape3 3

#wikipedia with depth 0
mkdir ../data/wikipedia0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia0 0

#wikipedia with depth 1
mkdir ../data/wikipedia1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia1 1