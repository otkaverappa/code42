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

int Sequence::longestDecreasingSubsequence( const vector< int > & v, vector< int > & result ) {
    vector< int > rev( v.rbegin(), v.rend() );
    int maxLen = Sequence::longestIncreasingSubsequence( rev, result );
    reverse( result.begin(), result.end() );
    return maxLen;
}

int Sequence::longestBitonicSubsequence( const vector< int > & v, vector< int > & result ) {
    vector< int > listFromLeft( v.size() ), listFromRight( v.size() );
    vector< int > listBitonic;
    int optLen = 0, optIndex = -1;
    int curLen;

    for( int i = 0; i < v.size(); ++i ) {
        auto iter = upper_bound( listBitonic.begin(), listBitonic.end(), v[i] );
        curLen = iter - listBitonic.begin() + 1;

        if( iter == listBitonic.end() )
            listBitonic.push_back( v[i] );
        else
            *iter = v[i];

        listFromLeft[i] = curLen;
    }
    listBitonic.clear();
    for( int i = v.size() - 1; i >= 0; --i ) {
        auto iter = upper_bound( listBitonic.begin(), listBitonic.end(), v[i] );
        curLen = iter - listBitonic.begin() + 1;

        if( iter == listBitonic.end() )
            listBitonic.push_back( v[i] );
        else
            *iter = v[i];

        listFromRight[i] = curLen;

        curLen = ( listFromLeft[i] > 1 && listFromRight[i] > 1 ) ?
                   listFromLeft[i] + listFromRight[i] - 1 : 0;
        if( optLen < curLen ) {
            optLen = curLen;
            optIndex = i;
        }
    }
    //The way in which we calculate optLen should ensure that it is either 0
    //or at least equal to the minimum possible bitonic sequence length.
    assert( optLen == 0 || optLen >= minBitonicLen );

    if( optLen ) {
        assert( optIndex >= 0 );
        int prevElement = INT_MAX, prevLen = listFromLeft[ optIndex ] + 1;
        for( int i = optIndex; i >= 0; --i ) {
            if( listFromLeft[i] == prevLen - 1 && v[i] < prevElement ) {
                result.push_back( v[i] );
                --prevLen;
                prevElement = v[i];
            }
        }
        reverse( result.begin(), result.end() );
        prevElement = v[optIndex];
        prevLen = listFromRight[ optIndex ];
        for( int i = optIndex + 1; i < v.size(); ++i ) {
            if( listFromRight[i] == prevLen - 1 && v[i] < prevElement ) {
                result.push_back( v[i] );
                --prevLen;
                prevElement = v[i];
            }
        }
    }
    return optLen;
}

int Sequence::longestIncreasingSubsequence( const vector< int > & v, vector< int > & result ) {
    vector< int > incresingSequenceLen;
    vector< int > listLIS;
    int maxLen = 0, endIndex = -1;

    for( int i = 0; i < v.size(); ++i ) {
        int curLen;
        auto iter = upper_bound( listLIS.begin(), listLIS.end(), v[i] );
        curLen = iter - listLIS.begin() + 1;
        if( iter == listLIS.end() ) {
            listLIS.push_back( v[i] );
        }
        else
            *iter = v[i];
        if( curLen > maxLen ) {
                maxLen = curLen;
                endIndex = i;
        }
        incresingSequenceLen.push_back( curLen );
    }
    if( maxLen ) {
        assert( endIndex >= 0 );

        int prevElement = v[endIndex];
        int prevLISLen = maxLen;
        result.push_back( endIndex );

        for( int i = endIndex - 1; i >= 0; --i ) {
            if( incresingSequenceLen[i] == prevLISLen - 1 &&
                v[i] < prevElement ) {
                    result.push_back( i );
                    --prevLISLen;
                    prevElement = v[i];
            }
        }
    }
    reverse( result.begin(), result.end() );
    return maxLen;
}
