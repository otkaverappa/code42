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
};

class SequenceTest {
public:
    static void runTest() {
        maxArraySumTest();
        maxArrayProductTest();
        maxRectangleSumTest();
        largestSquareTest();
    }
private:
    static void maxArraySumTest();
    static void maxArrayProductTest();
    static void maxRectangleSumTest();
    static void largestSquareTest();
};

#endif // __SEQUENCE_H__
