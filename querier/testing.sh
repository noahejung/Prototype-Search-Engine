# test for the 'querier' module

# test case with missing parameters
echo "missing parameters"
echo "breadth and first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 
echo "breadth and first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test case with wrong incorrect parameters
echo "incorrect parameters"
echo "breadth and first" | ./querier ~/cs50-dev/shared/tse/output/-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "breadth and first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikndex

# test with incorrect usages of and/or
echo "misusage or and/or"
echo "and dog and first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "breadth or" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "dog and and cat" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test with ands
echo "ands"
echo "breadth and first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "computer and science and department" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "sports and soccer and game and ball" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test with ors
echo "ors"
echo "breadth or first" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "cat or dog or mouse" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "jelly or peanut or butter" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test with no ands/ors
echo "no and/or"
echo "breadth first search" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "cat dog" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "just kidding       " | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test with both ands/ors
echo "both and/or"
echo "breadth and first or search" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "computer and science or cat and dogs" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "beauty and the beast or pizza slice" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index

# test with numbers and special characters
echo "specials"
echo "??? okay" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "the 7 deadly sins" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index
echo "my name is ****" | ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index


# test with valgrind
echo "valgrind"
echo "this and that" | valgrind --leak-check=full --track-origins=yes ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/output/wikipedia-2.index