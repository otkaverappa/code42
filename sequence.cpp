#include "sequence.h"

int Sequence::maxArraySum( const vector< int > &v ) {
    int sol = 0, maxSofar = 0;
    for( int i : v ) {
        maxSofar = max( i, maxSofar + i );
        sol = max( sol, maxSofar );
    }
    return sol;
}

uint64_t Sequence::maxArrayProduct( const vector< int > &v ) {
    uint64_t sol = 1;
    int64_t maxSofar = 1, minSofar = 1;
    for( int i : v ) {
        if( i == 0 ) {
            maxSofar = minSofar = 1;
        } else if( i > 0 ) {
            maxSofar = i * maxSofar;
            minSofar = min( int64_t(1), i * minSofar );
        } else {
            int64_t maxSofar_ = maxSofar; //We need the old value for calculating the minimum product so far.
            maxSofar = max( int64_t(1), i * minSofar );
            minSofar = i * maxSofar_;
        }
        sol = max( sol, uint64_t(maxSofar) );
    }
    return sol;
}

int Sequence::maxRectangleSum( const vector< vector< int > > & m ) {
    int rows = m.size(), cols;
    if( rows == 0 || ( ( cols = m[0].size() ) == 0 ) )
        return 0;

    vector< vector< int > > cumulativeSum( rows, vector< int > ( cols, 0 ) );
    for( int i = 0; i < rows; ++i ) {
        assert( m[i].size() == cols );
        for( int j = 0; j < cols; ++j ) {
            cumulativeSum[i][j] = m[i][j] +
            ( i > 0 ? cumulativeSum[i-1][j] : 0 );
        }
    }
    int sol = 0;
    vector< int > v;
    for( int topRow = 0; topRow < rows; ++topRow ) {
        for( int botRow = topRow; botRow < rows; ++botRow ) {
            for(int col = 0; col < cols; ++col ) {
                int value = cumulativeSum[botRow][col] -
                ( topRow > 0 ? cumulativeSum[topRow - 1][col] : 0 );
                v.push_back( value );
            }
            sol = max( sol, maxArraySum( v ) );
            v.clear();
        }
    }
    return sol;
}

int Sequence::largestSquare( const vector< vector< int > > & m ) {
    int rows = m.size(), cols;
    if( rows == 0 || ( ( cols = m[0].size() ) == 0 ) )
        return 0;

    int maxSoFar = 0;
    vector < int > prevRow( cols, 0 ), curRow( cols, 0 );
    vector < int > *prevRowPtr = &prevRow, *curRowPtr = &curRow;

    for( int i = 0; i < rows; ++i ) {
        assert( m[i].size() == cols );
        for( int j = 0; j < cols; ++j ) {
                int optSize;
                switch( m[i][j] ) {
                    case 0: optSize = (*curRowPtr)[j] = 0; break;
                    case 1:
                        optSize = (*prevRowPtr)[j];
                        optSize = min( optSize, j > 0 ? (*prevRowPtr)[j-1] : 0 );
                        optSize = min( optSize, j > 0 ? (*curRowPtr)[j-1] : 0 );
                        (*curRowPtr)[j] = ++optSize;
                        break;
                    default:
                        assert( false );
                }
                maxSoFar = max( maxSoFar, optSize );
        }
        swap( prevRowPtr, curRowPtr );
    }
    return maxSoFar;
}

void SequenceTest::largestSquareTest() {
}

void SequenceTest::maxArraySumTest() {
}

void SequenceTest::maxArrayProductTest() {
    vector< pair< vector< int >, uint64_t > > testcases = {
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
