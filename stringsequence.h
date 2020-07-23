#ifndef __STRINGSEQUENCE_H__
#define __STRINGSEQUENCE_H__

#include "common.h"

class StringSequence {
public:
    static string longestCommonSubstring( const string& a, const string& b ) {
        int substrLen = 0, bestEndIndex;
        string lcs;

        int M = a.size(), N = b.size();
        vector< vector< int > > lcsMatrix( M + 1, vector< int > ( N + 1, 0 ) );
        for( int i = 1; i <= M; ++i ) {
            for( int j = 1; j <= N; ++j ) {
                lcsMatrix[i][j] = ( a[i-1] == b[j-1] ) ? 1 + lcsMatrix[i-1][j-1] : 0;
                if( substrLen < lcsMatrix[i][j] ) {
                    substrLen = lcsMatrix[i][j];
                    bestEndIndex = i;
                }
            }
        }
        if( substrLen > 0 )
            lcs = a.substr( bestEndIndex - substrLen, substrLen );
        return lcs;
    }
    static string longestCommonSubsequence( const string& a, const string& b ) {
        int M = a.size(), N = b.size();
        string lcs;
        vector< vector< int > > lcsMatrix( M + 1, vector< int > ( N + 1, 0 ) );
        for( int i = 0; i < M; ++i ) {
            for( int j = 0; j < N; ++j ) {
                if( a[i] == b[j] )
                    lcsMatrix[i + 1][j + 1] = 1 + lcsMatrix[i][j];
                else
                    lcsMatrix[i + 1][j + 1] = max( lcsMatrix[i][j + 1],
                                                   lcsMatrix[i + 1][j] );
            }
        }
        int i = M, j = N;
        while( i + j > 0 ) {
            if( i > 0 && j > 0 && a[i - 1] == b[j - 1] ) {
                lcs += b[j - 1]; --i; --j;
            } else if( j > 0 && lcsMatrix[i][j] == lcsMatrix[i][j - 1] ) {
                --j;
            } else {
                assert( i > 0 );
                --i;
            }
        }
        reverse( lcs.begin(), lcs.end() );
        return lcs;
    }
    static string shortestCommonSupersequence( const string& a, const string& b ) {
        int M = a.size(), N = b.size();
        string scs;
        vector< vector < int > > scsMatrix( M + 1, vector< int > ( N + 1, 0 ) );

        for( int r = 1; r <= M; ++r ) scsMatrix[r][0] = 1 + scsMatrix[r - 1][0];
        for( int c = 1; c <= N; ++c ) scsMatrix[0][c] = 1 + scsMatrix[0][c - 1];

        for( int i = 0; i < M; ++i ) {
            for( int j = 0; j < N; ++j ) {
                if( a[i] == b[j] )
                    scsMatrix[i + 1][j + 1] = 1 + scsMatrix[i][j];
                else
                    scsMatrix[i + 1][j + 1] = 1 + min( scsMatrix[i][j + 1],
                                                       scsMatrix[i + 1][j] );
            }
        }
        int i = M, j = N;
        while( i + j > 0 ) {
            if( i > 0 && j > 0 && a[i - 1] == b[j - 1] ) {
                scs += b[j - 1]; --i; --j;
            } else if( j > 0 && scsMatrix[i][j] == 1 + scsMatrix[i][j - 1] ) {
                scs += b[j - 1]; --j;
            } else {
                assert( i > 0 );
                scs += a[i - 1]; --i;
            }
        }
        reverse( scs.begin(), scs.end() );
        return scs;
    }
    //Is string "c" a shuffle of strings "a" and "b" ?
    static bool isShuffle( const string& a, const string& b, const string& c ) {
        int M = a.size(), N = b.size(), K = c.size();
        if( K != M + N ) return false;

        vector< bool > shuffle( K + 1, false );

        for( int i = 0; i <= M; ++i ) {
            for( int j = 0; j <= N; ++j ) {
                if( i + j == 0 ) {
                    shuffle[ i + j ] = true;
                    continue;
                }
                if( i > 0 && a[i - 1] == c[ i + j - 1 ] )
                    shuffle[ i + j ] = shuffle[ i + j - 1 ] & true;
                if( j > 0 && b[j - 1] == c[ i + j - 1 ] )
                    shuffle[ i + j ] = shuffle[ i + j - 1 ] & true;
            }
        }
        return shuffle[ K ];
    }
    //A smooth shuffle of "a" and "b" is a shuffle of "a" and "b" that never
    //uses two consecutive symbols of either string. Is "c" a smooth shuffle
    //of "a" and "b" ?
    static bool isSmoothShuffle( const string& a, const string& b, const string& c ) {
        int M = a.size(), N = b.size(), K = c.size();
        if( K != M + N ) return false;

        enum { SHUFFLE_MATCH_NONE = 0x00,
               SHUFFLE_MATCH_A    = 0x01,
               SHUFFLE_MATCH_B    = 0x02,
               SHUFFLE_MATCH_AB   = SHUFFLE_MATCH_A | SHUFFLE_MATCH_B,
             };

        vector< char > tag( K + 1, SHUFFLE_MATCH_NONE );
        tag[ 0 ] = SHUFFLE_MATCH_AB;

        for( int i = 0; i <= M; ++i ) {
            for( int j = 0; j <= N; ++j ) {
                if( i + j == 0 ) {
                    continue;
                }
                if( i > 0 && a[i - 1] == c[ i + j - 1 ] ) {
                    tag[ i + j ] |= SHUFFLE_MATCH_A;
                }
                if( j > 0 && b[j - 1] == c[ i + j - 1 ] ) {
                    tag[ i + j ] |= SHUFFLE_MATCH_B;
                }
            }
        }
        if( tag[ K ] == SHUFFLE_MATCH_NONE ) return false;
    }
    //Is string "b" a subsequence of string "a" ?
    static bool isSubsequence( const string& a, const string& b ) {
        int M = a.size(), N = b.size();
        if( N > M ) return false;

        auto iter = b.begin();
        int i = 0;

        while( iter != b.end() && i < M ) {
            if( a[i++] == *iter ) ++iter;
        }
        return iter == b.end();
    }
    static int longestContiguousForwardsBackwardsSubstring( const string& s ) {
        int maxLen = 0;
        map< ii, int > cache;
        longestContiguousForwardsBackwardsSubstring_( s, 0, s.size() - 1, cache, maxLen );
        return maxLen;
    }
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
        //isSubsequenceTest();
        //isShuffleTest();
        //longestContiguousForwardsBackwardsSubstringTest();
        //isSmoothShuffleTest();
        longestCommonSubsequenceTest();
        shortestCommonSupersequenceTest();
        longestCommonSubstringTest();
    }
private:
    typedef vector< tuple< string, string, string > > StringTestcaseList;
    typedef function< string( const string&, const string& ) > StringFunction;
    typedef function< void( const string&, const string&, const string& ) > StringFunctionVerify;

    static void longestCommonSubstringTest() {
        StringTestcaseList testcases = {
            { "", "", "" },
            { "DYNAMICPROGRAMMING", "", "" },
            { "", "DYNAMICPROGRAMMING", "" },
            { "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING" },
            { "DYNAMICPROGRAMMING", "DYNAMIC", "DYNAMIC" },
            { "DYNAMICPROGRAMMING", "PROGRAMMING", "PROGRAMMING" },
            { "DYNAMICPROGRAMMING", "ABMICPROBB", "MICPRO" },
            { "DYNAMICPROGRAMMING", "ABMICPROBBDYNAMICCC", "DYNAMIC" },
        };
        printf( "Longest common Substring Test\n" );
        function verificationFunc =
                 []( const string& a, const string& b, const string& result ) {
                 };
        generic_test( testcases, StringSequence::longestCommonSubstring, verificationFunc );
    }
    static void longestCommonSubsequenceTest() {
        StringTestcaseList testcases = {
            { "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING" },
            { "DYNAMICPROGRAMMING", "", "" },
            { "", "DYNAMICPROGRAMMING", "" },
            { "", "", "" },
            { "PINEAPPLE", "INAPPLEORANGE", "INAPPLE" },
            { "PINEAPPLE", "PINE", "PINE" },
            { "PINEAPPLE", "APPLECART", "APPLE" },
        };
        printf( "Longest Common Subsequence Test\n" );
        function verificationFunc =
                 []( const string& a, const string& b, const string& result ) {
                     assert( StringSequence::isSubsequence( a, result ) );
                     assert( StringSequence::isSubsequence( b, result ) );
                 };
        generic_test( testcases, StringSequence::longestCommonSubsequence, verificationFunc );
    }
    static void shortestCommonSupersequenceTest() {
        StringTestcaseList testcases = {
            { "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING" },
            { "DYNAMICPROGRAMMING", "", "DYNAMICPROGRAMMING" },
            { "", "DYNAMICPROGRAMMING", "DYNAMICPROGRAMMING" },
            { "", "", "" },
            { "EATAPPLEFRUIT", "EATORANGEFRUIT", "EATORAPPLNGEFRUIT" },
        };
        printf( "Shortest Common Supersequence Test\n" );
        function verificationFunc =
                 []( const string& a, const string &b, const string &result ) {
                     assert( StringSequence::isSubsequence( result, a ) );
                     assert( StringSequence::isSubsequence( result, b ) );
                 };
        generic_test( testcases, StringSequence::shortestCommonSupersequence, verificationFunc );
    }
    static void generic_test( StringTestcaseList & testcases, StringFunction implFunc,
                              StringFunctionVerify verificationFunc ) {
        for( auto testcase : testcases ) {
            const string& a = get< 0 > ( testcase );
            const string& b = get< 1 > ( testcase );
            const string& expectedResult = get< 2 > ( testcase );

            string result = implFunc( a, b );
             printf( "[%s] [%s] = [%s] length = %d\n", a.c_str(), b.c_str(), result.c_str(),
                     result.size() );
            assert( result.size() == expectedResult.size() );

            verificationFunc( a, b, result );
        }
    }
    static void longestContiguousForwardsBackwardsSubstringTest() {
        vector< tuple< string, int > > testcases = {
            { "DYNAMICPROGRAMMINGMANYTIMES", 4 },
            { "ALGORITHM", 0 }, { "RECURSION", 1 }, { "REDIVIDE", 3 },
            { "", 0 }, { "A", 0 }, { "AAAA", 2 }, { "AAA", 1 },
            { "BUBBACANNOTSPELLNACBUBBA", 3 },
        };
        for( auto testcase : testcases ) {
            const string& s = get< 0 > ( testcase );
            int expectedResult = get< 1 > ( testcase );

            int result = StringSequence::longestContiguousForwardsBackwardsSubstring( s );
            printf( "[%s] Length of contiguous forwards backward substring = %d\n",
                    s.c_str(), result );
            assert( result == expectedResult );
        }
    }
    static void isSubsequenceTest() {
        vector< tuple< string, string, bool > > testcases = {
            { "PENPINEAPPLEAPPLEPEN", "PPAP", true },
            { "PENPINEAPPLEAPPLEPEN", "PINEAPPLE", true },
            { "PENPINEAPPLEAPPLEPEN", "", true },
            { "PENPINEAPPLEAPPLEPEN", "PENPINEAPPLEAPPLEPEN", true },
            { "PENPINEAPPLEAPPLEPEN", "PENPINEAPPLEAPPLEPENEXTRA", false },
            { "", "PENPINEAPPLEAPPLEPEN", false },
            { "", "", true },
            { "PENPINEAPPLEAPPLEPEN", "PENPINEORANGE", false },
        };
        for( auto testcase : testcases ) {
            const string& a = get< 0 > ( testcase );
            const string& b = get< 1 > ( testcase );
            bool expectedResult = get< 2 > ( testcase );

            bool isSubsequence = StringSequence::isSubsequence( a, b );
            printf( "[%s] [%s] isSubsequence: %s\n", a.c_str(), b.c_str(), isSubsequence ? "True" : "False" );
            assert( expectedResult == isSubsequence );
        }
    }
    static void isShuffleTest() {
        vector< tuple< string, string, string, bool > > testcases = {
            { "BANANA", "ANANAS", "BANANAANANAS", true },
            { "ANANAS", "BANANA", "BANANAANANAS", true },
            { "BANANA", "ANANAS", "BANANA", false },
            { "ANANAS", "BANANA", "BAANANNANAAS", true },
            { "ANANAS", "BANANA", "BANANAANANAQ", false },
            { "BANANA", "", "BANANA", true },
            { "", "BANANA", "BANANA", true },
            { "", "", "BANANA", false },
            { "", "", "", true },
        };
        for( auto testcase : testcases ) {
            const string& a = get< 0 > ( testcase );
            const string& b = get< 1 > ( testcase );
            const string& c = get< 2 > ( testcase );
            bool expectedResult = get< 3 > ( testcase );

            bool isShuffle = StringSequence::isShuffle( a, b, c );
            printf( "[%s] [%s] [%s] isShuffle: %s\n", a.c_str(), b.c_str(), c.c_str(),
                    isShuffle ? "True" : "False" );
            assert( expectedResult == isShuffle );
        }
    }
    static void isSmoothShuffleTest() {
        vector< tuple< string, string, string, bool > > testcases = {
            { "BANANA", "", "BANANA", false },
            { "", "BANANA", "BANANA", false },
            { "", "", "BANANA", false },
            { "", "", "", true },
            { "DYNAMIC", "PROGRAMMING", "PRDOYGNARAMMMIICNG", true },
            { "DYNAMIC", "PROGRAMMING", "DYPRNOGRAAMMMICING", false },
            { "DYNAMIC", "PROGRAMMING", "GREEDYALGORITHM", false },
            { "XXXXXXX", "XXXXXXXXXXX", "XXXXXXXXXXXXXXXXXX", true },
            { "XXXX", "XXXXXXXXXXXX", "XXXXXXXXXXXXXXXX", false },
        };
        for( auto testcase : testcases ) {
            const string& a = get< 0 > ( testcase );
            const string& b = get< 1 > ( testcase );
            const string& c = get< 2 > ( testcase );
            bool expectedResult = get< 3 > ( testcase );

            bool isSmoothShuffle = StringSequence::isSmoothShuffle( a, b, c );
            printf( "[%s] [%s] [%s] isSmoothShuffle: %s\n", a.c_str(), b.c_str(), c.c_str(),
                    isSmoothShuffle ? "True" : "False" );
            assert( expectedResult == isSmoothShuffle );
        }
    }
};

#endif //__STRINGSEQUENCE_H__
