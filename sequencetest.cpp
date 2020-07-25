#include "sequence.h"

void SequenceTest::largestSquareTest() {
}

void SequenceTest::maxArraySumTest() {
}

void SequenceTest::maxArrayProductTest() {
    vector< pair< vector< int >, uint64_t > > testcases = {
        { { 10 }, 10 },
        { { -10 }, 1 },
        { { 1, 2, 3, 4 }, 24 },
        { { 1, 2, 3, 4, 0, 5 }, 24 },
        { { 1, 2, 3, 4, 0, 5, 5, -2 }, 25 },
        { { 1, 2, 3, 4, 0, 5, 5, -2, -1 }, 50 },
        { { 1, 2, 3, 4, 0, 5, 5, -2, 1, 100 }, 100 },
    };
    uint64_t result;
    printf( "%s\n", __FUNCTION__ );
    for( auto & [ vec, expectedResult ] : testcases ) {
        result = Sequence::maxArrayProduct( vec );
        printf( "maxArrayProduct = %3d ", result );
        printf( "[ " );
        for( int i : vec )
            printf( " %2d ", i );
        printf( "]\n" );
        assert( expectedResult == result );
    }
}

void SequenceTest::maxRectangleSumTest() {
}
