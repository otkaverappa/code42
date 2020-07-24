#ifndef __STRINGSEQUENCE_H__
#define __STRINGSEQUENCE_H__

#include "common.h"
#include "multivector.h"

class StringSequence {
public:
    //Longest Common Substring between "a" and "b".
    static string longestCommonSubstring( const string& a, const string& b );

    static string longestCommonSubsequence( const string& a, const string& b );

    static string shortestCommonSupersequence( const string& a, const string& b );

    //Is string "c" a shuffle of strings "a" and "b" ?
    static bool isShuffle( const string& a, const string& b, const string& c );

    //A smooth shuffle of "a" and "b" is a shuffle of "a" and "b" that never
    //uses two consecutive symbols of either string. Is "c" a smooth shuffle
    //of "a" and "b" ?
    static bool isSmoothShuffle( const string& a, const string& b, const string& c );

    //Is string "b" a subsequence of string "a" ?
    static bool isSubsequence( const string& a, const string& b );

    static int longestContiguousForwardsBackwardsSubstring( const string& s );

    //Longest Common Palindromic Subsequence between "a" and "b".
    static string longestCommonPalindromicSubsequence( const string& a, const string& b );

private:
    static int longestContiguousForwardsBackwardsSubstring_( const string& s, int i, int j,
                                                             map< ii, int >& cache,
                                                             int& maxLen ) {
        if( i >= j ) return 0;
        ii elem = { i, j };
        if( cache.count( elem ) > 0 )
            return cache[ elem ];
        int len;
        if( s[i] == s[j] ) {
            len = 1 + longestContiguousForwardsBackwardsSubstring_( s, i + 1, j - 1,
                                                                    cache, maxLen );
            maxLen = max( maxLen, len );
        } else {
            longestContiguousForwardsBackwardsSubstring_( s, i + 1, j, cache, maxLen );
            longestContiguousForwardsBackwardsSubstring_( s, i, j - 1, cache, maxLen );
            len = 0;
        }
        cache[ elem ] = len;
        return len;
    }
};

class StringSequenceTest {
public:
    static void runTest() {
        isSubsequenceTest();
        isShuffleTest();
        longestContiguousForwardsBackwardsSubstringTest();
        //isSmoothShuffleTest();
        longestCommonSubsequenceTest();
        shortestCommonSupersequenceTest();
        longestCommonSubstringTest();
        longestCommonPalindromicSubsequenceTest();
    }
private:
    typedef vector< tuple< string, string, string > > StringTestcaseList;
    typedef function< string( const string&, const string& ) > StringFunction;
    typedef function< void( const string&, const string&, const string& ) > StringFunctionVerify;

    static void longestCommonPalindromicSubsequenceTest();
    static void longestCommonSubstringTest();
    static void longestCommonSubsequenceTest();
    static void shortestCommonSupersequenceTest();
    static void generic_test( StringTestcaseList & testcases, StringFunction implFunc,
                              StringFunctionVerify verificationFunc );
    static void longestContiguousForwardsBackwardsSubstringTest();
    static void isSubsequenceTest();
    static void isShuffleTest();
    static void isSmoothShuffleTest();
};

#endif //__STRINGSEQUENCE_H__
