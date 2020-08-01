#include "wordladder.h"

int WordLadder::wordLadder( vector< string > & words, int startWordIndex, int endWordIndex,
                            vector< int > & path ) {

    int N = words.size();
    Graph g( N, false );

    for( int i = 0; i < N; ++i ) {
        for( int j = i+1; j < N; ++j ) {
            if( EditDistance::isUnitEditDistance( words[i], words[j] ) ) {
                    g.addEdge( i, j );
            }
        }
    }
    return GraphFunctions::bidirectionalBFS( g, startWordIndex, endWordIndex, path );
}

void WordLadderTest::sameLengthWordLadderTest() {
    vector< int > path;
    vector< pair< vector< string >, ii > > testcases = {
        { { "hit", "hot", "dot", "dog", "lot", "log", "cog" }, { 0, 6 } },
        { { "fool", "pool", "poll", "pole", "pale", "sale", "sage", "page", "cage", "rage",
            "maze", "cool" }, { 0, 9 } },
    };

    for( auto & [ words, startEndIndex ] : testcases ) {
        auto & [ startIndex, endIndex ] = startEndIndex;
        path.clear();
        int len = WordLadder::wordLadder( words, startIndex, endIndex, path );
        printf( "Length = %d from [%s] to [%s]\n", len, words[ startIndex ].c_str(),
                words[ endIndex ].c_str() );
        for( int i = 0; i < len; ++i ) {
            printf( "%s %s ", words[ path[i] ].c_str(), i == len - 1 ? "" : "-->" );
        }
        printf( "\n" );
    }
}
