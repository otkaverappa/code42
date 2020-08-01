#ifndef __WORD_LADDER_H__
#define __WORD_LADDER_H__

#include "common.h"
#include "editdistance.h"
#include "graph.h"

class WordLadder {
public:
    static int wordLadder( vector< string > & words, int startWordIndex, int endWordIndex,
                           vector< int > & path );
};

class WordLadderTest {
public:
    static void runTest() {
        sameLengthWordLadderTest();
    }
private:
    static void sameLengthWordLadderTest();
};

#endif // __WORD_LADDER_H__
