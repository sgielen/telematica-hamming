telematica-hamming
==================

This is a depth-first recursive brute-force solution to the Hamming code
challenge of the Telematics Networks course.

To compile (use a `$CXX` with C++11 support):

    $CXX \
      -std=c++11 -O3 \
      -o hammingchallenge hammingchallenge.cpp

To run: `./hammingchallenge bitsize hammingdist`

For example, to find the largest list of code words with bit length 4 and
hamming distance 2:

    $ ./hammingchallenge 4 2
    0 - Found a new longest list: [0000](1)
    0 - Found a new longest list: [0000,0011](2)
    0 - Found a new longest list: [0000,0011,0101](3)
    0 - Found a new longest list: [0000,0011,0101,0110](4)
    0 - Found a new longest list: [0000,0011,0101,0110,1001](5)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010](6)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100](7)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100,1111](8)
    0 - Exit level 8
    0 - Exit level 7
    0 - Exit level 6
    0 - Exit level 5
    0 - Exit level 4
    0 - Exit level 3
    0 - Exit level 2
    2 - Exit level 1
    27 - Exit level 0
    Longest list first found: [0000,0011,0101,0110,1001,1010,1100,1111](8)

It will continously output the largest list of code words it can find, where
every code word has the given bitsize as its length, and the hamming distance
of the complete set is at least hammingdist. When its search space is
exhausted, it will exit. The digit at the beginning of each line is the number
of seconds the program has been running.

The algorithm in pseudo code:

    LongestListCalculator(codeWordList) {
      for every possible codeword "codeword":
        newCodeWordList <- codeWordList + codeword
        if hamming distance of newCodeWordList is high enough:
          if newCodeWordList is longer than currentLongestList:
            currentLongestList <- newCodeWordList
            print "New longest list: ", newCodeWordList
          endif
          LongestListCalculator(newCodeWordList)
        endif
      endfor
    }
    currentLongestList = []
    LongestListCalculator([])
    print "Longest list found: ", currentLongestList

Some notes: I'm particularly surprised that the depth-first algorithm seems to
find the longest list the very first time it hits its deepest recursion. It
seems to be hard to find every next code word, but once the first iteration
that finds no new code words has passed, I have not seen the application
finding a longer list than it has at that point. This makes it apparant that
finding the next code word faster is the most important optimization to do. Or,
maybe it would be possible to take another theoretical look at the problem, and
find a much faster implementation.

-- Sjors Gielen, september 2013
