telematica-hamming
==================

This directory contains two solutions to the Hamming code challenge of the
Telematics Networks course. The first is a depth-first recursive brute-force
solution that guarantees to find the optimal solution. The second is an
iterative solution that seems to find the optimal one but I can't prove it.
See below for a longer explanation.

To compile (use a `$CXX` with C++11 support for the first version; `g++` 4.8
and `clang++` 3.2 will do):

    $CXX \
      -std=c++11 -O3 \
      -o hammingchallenge hammingchallenge.cpp
    $CXX \
      -O3 \
      -o hammingchallenge-fast hammingchallenge-fast.cpp

To run: `./hammingchallenge bitsize hammingdist` (or use -fast for the second
version)

For example, to find the largest list of code words with bit length 4 and
hamming distance 2:

    $ time ./hammingchallenge 4 2
    0 - Found a new longest list: [0000](1)
    0 - Found a new longest list: [0000,0011](2)
    0 - Found a new longest list: [0000,0011,0101](3)
    0 - Found a new longest list: [0000,0011,0101,0110](4)
    0 - Found a new longest list: [0000,0011,0101,0110,1001](5)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010](6)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100](7)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100,1111](8)
    0 - Exit level 8
    [...]
    27 - Exit level 0
    Longest list first found: [0000,0011,0101,0110,1001,1010,1100,1111](8)
    
    real   0m1.465s
    user   0m1.453s
    sys    0m0.006s
    $ time ./hammingchallenge-fast 4 2
    0 - Found a new longest list: [0000](1)
    0 - Found a new longest list: [0000,0011](2)
    0 - Found a new longest list: [0000,0011,0101](3)
    0 - Found a new longest list: [0000,0011,0101,0110](4)
    0 - Found a new longest list: [0000,0011,0101,0110,1001](5)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010](6)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100](7)
    0 - Found a new longest list: [0000,0011,0101,0110,1001,1010,1100,1111](8)
    
    real   0m0.005s
    user   0m0.001s
    sys    0m0.002s

Both applications will continously output the largest list of code words it can
find, where every code word has the given bitsize as its length, and the
hamming distance of the complete set is at least hammingdist. When their search
space is exhausted, it will exit. The digit at the beginning of each line is
the number of seconds the program has been running.

The algorithm of the first application in pseudo code:

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

I was particularly surprised that the depth-first algorithm used in the first
application seems to find the longest list the very first time it hits its
deepest recursion. It seems to be hard to find every next code word, but once
the first iteration that finds no new code words has passed, I have not seen
the application finding a longer list than it has at that point. Even stronger:
there seems to be no need to backtrack and choose other code words earlier in
the process. The moment a new code word is found, it will be used in the longest
set.

This led me to the second implementation. Here, only one iteration through the
complete list of possible code words is done, instead of re-starting iteration
once a new code word is found. Once one is found with the right Hamming
distance to all earlier code words, it is added to the list and the search
continues. This is exponentially faster than the first implementation, but I
can't provide a proof of it yet. Both applications give a list of code words
that match the Hamming distance, but only the first one is proven to find the
longest list.

-- Sjors Gielen, september 2013
