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
    template <typename T>
    static int genericLongestIncreasingSubsequence( const vector< T > & v,
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

#endif // __SEQUENCE_H__
