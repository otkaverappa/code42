#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include "common.h"

class Sequence {
public:
    //Computes the maximum subsequence sum in the input integer array v.
    //If v contains only negative integers, then the return value is 0.
    static int maxArraySum( const vector< int > & v );

    //Computes the maximum subsequence product in the input integer array v.
    //The minimum possible product is 1, corresponding to the empty subarray.
    static uint64_t maxArrayProduct( const vector< int > & v );

    //Given a matrix m, computes the maximum sum in any rectangle within
    //the matrix m. If m contains only negative integers, then the return value is 0.
    static int maxRectangleSum( const vector< vector< int > > & m );

    //Width of the largest square of 1's in the bitmap m containing 1's and 0's.
    static int largestSquare( const vector< vector< int > > & m );

    //Populate the longest increasing subsequence of v, in result, and return
    //its length.
    //Time complexity - O(n log k) where k is the size of the longest increasing
    //subsequence.
    static int longestIncreasingSubsequence( const vector< int > & v,
                                             vector< int > & result );
    //Populate the longest decreasing subsequence of v, in result, and return
    //its length.
    static int longestDecreasingSubsequence( const vector< int > & v,
                                             vector< int > & result );

    //Populate the longest bitonic sequence of v, in result, and return its
    //length.
    static int longestBitonicSubsequence( const vector< int > & v, vector< int > & result );

    //Generic Longest Increasing Subsequence - result is populated with the index of
    //elements of v, which form a longest increasing subsequence.
    //Returns the pair (a, b) where "a" is the length of the longest increasing subsequence
    //and "b" is the number of disjoint subsequences.
    //Time complexity - O(n^2)
    template <typename T>
    static ii genericLongestIncreasingSubsequence( const vector< T > & v,
                                                    vector< int > & result );

public:
    //The shortest bitonic sequence is of length 3. (a, b, c) where a < b and b > c.
    static inline const int minBitonicLen = 3;
};

class SequenceTest {
public:
    static void runTest() {
        maxArraySumTest();
        maxArrayProductTest();
        maxRectangleSumTest();
        largestSquareTest();
        longestIncreasingSubsequenceTest();
        longestDecreasingSubsequenceTest();
        longestBitonicSubsequenceTest();
    }
private:
    static void maxArraySumTest();
    static void maxArrayProductTest();
    static void maxRectangleSumTest();
    static void largestSquareTest();
    static void longestIncreasingSubsequenceTest();
    static void longestDecreasingSubsequenceTest();
    static void longestBitonicSubsequenceTest();
};

template <typename T>
ii Sequence::genericLongestIncreasingSubsequence( const vector< T > & v,
                                                  vector< int > & result ) {
    vector< int > incresingSequenceLen;
    int maxLen = 0, endIndex = -1;
    int disjointSequenceCount = 0;

    for( int i = 0; i < v.size(); ++i ) {
        int curLen = 1;

        for( int j = i - 1; j >= 0; --j ) {
            if( v[j] < v[i] ) {
                curLen = max( curLen, 1 + incresingSequenceLen[j] );
            }
        }
        incresingSequenceLen.push_back( curLen );

        if( maxLen < curLen ) {
            maxLen = curLen;
            endIndex = i;
        }
    }
    if( maxLen ) {
        assert( endIndex >= 0 );
        T prevElement = v[endIndex];
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
    return { maxLen, disjointSequenceCount } ;
}

#endif // __SEQUENCE_H__
