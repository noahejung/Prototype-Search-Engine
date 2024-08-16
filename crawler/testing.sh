#!/bin/bash
# testing.sh
#
# test for crawler

mkdir -p ../testing/

# missing or extra parameters 
./crawler
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../testing/ 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../testing/ 1 extraParameter

# invalid parameters
./crawler 123 123 123 
./crawler abc abc abc
./crawler 1 ~/cs50-dev/tse-noahejung/test/ http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
./ crawler https://en.wikipedia.org/wiki/Science ../testing/ 1

# valid test 1 valgrind
echo "Test 1: Valgrind w/ toscrape"
mkdir -p ../testing/test1
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../testing/test1 1
diff -r ../testingdata-given/toscrape-1 ../testing/test1 1

# test 2 letters
echo "Test 2: Letters, depth 0, 1, 2 , 10"
for ((i = 0; i <= 2; i++)); do
    echo "running letters-${i}"
    mkdir -p ../testing/test2/letters-"${i}"
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../testing/test2/letters-"${i}" $i
done

echo "running letters-10"
mkdir -p ../testing/test2/letters-"10"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../testing/test2/letters-10 10


# test 3 toscrape
echo "Test 3: toscrape, depth 0, 1, 2, 3"
for ((i = 0; i <= 3; i++)); do
    echo "running toscrape-${i}"
    mkdir -p ../testing/test3/toscrape-"${i}"
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../testing/test3/toscrape-"${i}" $i
done

# test 4 Wikipedia
echo "Test 3: wikipedia, depth 0, 1, 2"
for ((i = 0; i <= 2; i++)); do
    echo "running wikipedia-${i}"
    mkdir -p ../testing/test4/wikipedia-"${i}"
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../testing/test4/wikipedia-"${i}" $i
done