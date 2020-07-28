#include "sequence.h"

void SequenceTest::longestBitonicSubsequenceTest() {
    vector< pair< vector< int >, int > > testcases = {
        { { 8, 1, 9, 8, 3, 4, 6, 1, 5, 2 }, 6 },
        { { 1, 2, 3, 4, 5 }, 0 },
        { { 5, 4, 3, 2, 1 }, 0 },
        { { 1, 2, 3, 4, 10, 9, 8, 7, 6, 5 }, 10 },
        { { 10, 9, 8, 7, 8, 9, 10, 11, 12 }, 0 },
        { { 10, 9, 8, 7, 8, 7, 10, 11, 12 }, 3 },
        { { 1, -1 }, 0 },
        { { }, 0 },
    };
    vector< int > result;
    printf( "Longest Bitonic Subsequence Test\n" );
    for( auto & [ vec, expectedLen ] : testcases ) {
        int bitonicLen = Sequence::longestBitonicSubsequence( vec, result );

        printf( "Sequence: [ ");
        for( int i : vec ) printf( " %2d ", i );
        printf( "] Longest Bitonic Sequence Length = %d [ ", bitonicLen );
        for( int i : result ) printf( " %2d ", i );
        printf( "]\n" );

        assert( bitonicLen == 0 || bitonicLen >= Sequence::minBitonicLen );
        assert( bitonicLen == expectedLen );
        bool increasing = true;
        for( int j = 1; j < result.size(); ++j ) {
            assert( result[j] != result[j-1] );
            if( result[j] > result[j-1] ) {
                assert( increasing );
            } else {
                assert( j > 1 );
                increasing = false;
            }
        }
        result.clear();
    }
}

void SequenceTest::longestIncreasingSubsequenceTest() {
    vector< pair< vector< int >, int > > testcases = {
        { { 8, 1, 9, 8, 3, 4, 6, 1, 5, 2 }, 4 },
        { { 1, 2, 3, 4, 5 }, 5 },
        { { 5, 4, 3, 2, 1 }, 1 },
        { { 1, 2, 3, 4, 10, 9, 8, 7, 6, 5 }, 5 },
        { { 1, -1 }, 1 },
        { { }, 0 },
    };
    vector< int > result;
    printf( "Longest Increasing Subsequence Test\n" );
    for( auto & [ vec, expectedLen ] : testcases ) {
        int LISLen = Sequence::longestIncreasingSubsequence( vec, result );

        printf( "Sequence: [ ");
        for( int i : vec ) printf( " %2d ", i );
        printf( "] LIS Length = %d [ ", LISLen );
        for( int i : result ) printf( " %2d ", i );
        printf( "]\n" );

        assert( LISLen == expectedLen );
        for( int j = 1; j < result.size(); ++j )
            assert( result[j] > result[j-1] );
        result.clear();
    }
}

void SequenceTest::longestDecreasingSubsequenceTest() {
    vector< pair< vector< int >, int > > testcases = {
        { { 8, 1, 9, 8, 3, 4, 6, 1, 5, 2 }, 5 },
        { { 1, 2, 3, 4, 5 }, 1 },
        { { 5, 4, 3, 2, 1 }, 5 },
        { { 1, 2, 3, 4, 10, 9, 8, 7, 6, 5 }, 6 },
        { { 1, 2 }, 1 },
        { { }, 0 },
    };
    vector< int > result;
    printf( "Longest Decreasing Subsequence Test\n" );
    for( auto & [ vec, expectedLen ] : testcases ) {
        int LDSLen = Sequence::longestDecreasingSubsequence( vec, result );

        printf( "Sequence: [ ");
        for( int i : vec ) printf( " %2d ", i );
        printf( "] LDS Length = %d [ ", LDSLen );
        for( int i : result ) printf( " %2d ", i );
        printf( "]\n" );

        assert( LDSLen == expectedLen );
        for( int j = 1; j < result.size(); ++j )
            assert( result[j] < result[j-1] );
        result.clear();
    }
}

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
    printf( "Maximum Product SubArray Test\n" );
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
