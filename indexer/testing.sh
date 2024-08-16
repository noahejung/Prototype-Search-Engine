#!/bin/bash
# testing.sh
#
# test for indexer

mkdir -p ../indexertesting/

# missing or extra parameters 
./indexer
./indexer ../testing/test2/letters-2
./indexer ../testing/test2/letters-2 ./indexertest1 extra

# invalid parameters
./indexer 123 123 123 
./indexer abc abc abc
./indexer ./indexertest1/ ..testing/test2/letters-2/


# not a crawler page directory
./indexer ../testingfake/testingfake ./indexertest1
./indexer ../testing/test2 ./indexertest1
chmod -w ../testing/test2/letters-2
./indexer ../testing/test2/letters-2 ./indexertest1


# not a valid indexFile
./indexer ..testing/test2/letters-2 ../testingfake/testingfake
touch indexertest1
chmod -w indexertest1
./indexer ../testing/test2/letters-2 ./indexertest1
./indexer ../testing/test/letters-1 ..testing/test/letters-2
chmod +w indexertest1
chmod +w ../testing/test2/letters-2

# valid test 1 
echo "Test 1: letters-2"

./indexer ../testing/test2/letters-2 ./indexertest1
./indextest indexertest1 indexertest1check
if diff <(sort indexertest1) <(sort indexertest1check) > /dev/null; then
    echo "The files have the same contents, ignoring order."
else
    echo "The files do not have the same contents."
fi

# valid test 2 
echo "Test 2: toscrape-1"

/indexer ../testing/test3/toscrape-1/ ./indexertest2
./indextest indexertest2 indexertest2check
if diff <(sort indexertest2) <(sort indexertest2check) > /dev/null; then
    echo "The files have the same contents, ignoring order."
else
    echo "The files do not have the same contents."
fi

# valgrind test 3:
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../testing/test2/letters-2/ ./indexertest1/
valgrind --leak-check=full --show-leak-kinds=all ./indextest indexertest1 indexertest1check