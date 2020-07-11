#include "common.h"

class Sequence {
public:
    //Computes the maximum subsequence sum in the input integer array v.
    //If v contains only negative integers, then the return value is 0.
    int maxArraySum( const vector< int > &v );
    int maxRectangleSum( const vector< vector< int > > & m );
};

int Sequence::maxArraySum( const vector< int > &v ) {
    int sol = 0, maxSofar = 0;
    for( int i : v ) {
        maxSofar = max( i, maxSofar + i );
        sol = max( sol, maxSofar );
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

class SequenceTest {
public:
    void runTest() {
    }
private:

};
