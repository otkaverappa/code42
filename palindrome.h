#ifndef __PALINDROME_H__
#define __PALINDROME_H__

#include "common.h"
#include "stringsequence.h"

class Palindrome {
public:
    static bool isPalindrome( const string& str ) {
        return isPalindrome( str, 0, str.size() - 1 );
    }
    static bool isPalindrome( const string& str, int startIndex,
                              int endIndex,
                              bool outOfBoundsCheck=false ) {
        if( outOfBoundsCheck )
            assert( endIndex >= startIndex && endIndex >= 0 );
        while( startIndex < endIndex )
            if( str[startIndex++] != str[endIndex--] )
                return false;
        return true;
    }
    static string shortestPalindromicSupersequence( const string& str ) {
        map< ii, ii > cache;
        int i = 0, j = str.size() - 1;
        int len = SPSupersequence( str, i, j, cache );
        ii result;

        string left, right;

        while( i <= j ) {
            result = cache[ {i, j } ];
            auto & [ u, v ] = result;
            switch( v ) {
            case INCREMENT_LEFT:
                left += str[i]; right += str[i]; ++i; break;
            case DECREMENT_RIGHT:
                left += str[j]; right += str[j]; --j; break;
            case INC_DEC_BOTH:
                left += str[i];
                if(j != i)
                    right += str[j];
                ++i; --j; break;
            default:
                assert( false );
            }
        }
        reverse( right.begin(), right.end() );
        left += right;
        assert( left.size() == len );
        return left;
    }
    static string longestPalindromicSubsequence( const string& str ) {
        map< ii, ii > cache;
        int i = 0, j = str.size() - 1;
        int len = LPSubsequence( str, i, j, cache );
        ii result;

        string left, right;

        while( i <= j ) {
            result = cache[ { i, j } ];
            auto & [ u, v ] = result;
            switch( v ) {
                case INCREMENT_LEFT: ++i; break;
                case DECREMENT_RIGHT: --j; break;
                case INC_DEC_BOTH:
                    left += str[i];
                    if(j != i)
                        right += str[j];
                    ++i; --j;
                    break;
                default:
                    assert( false );
            }
        }
        reverse( right.begin(), right.end() );
        left += right;
        assert( left.size() == len );
        return left;
    }
    static int smallestPalindromicDecomposition( const string& str,
                                                 vector< string> & decomp ) {
        map< int, ii > cache;
        int startIndex = 0, endIndex = str.size() - 1;
        int len = SPDecomposition( str, startIndex, endIndex, cache );

        int v = cache[ startIndex ].second;
        while( true ) {
            decomp.emplace_back( str.substr( startIndex, v - startIndex ) );
            if( v == endIndex + 1 ) break;
            startIndex = v;
            v = cache[ v ].second;
        }
        return len;
    }
    static int palindromicDecompositionCount( const string& str ) {
        map< int, int > cache;
        return palindromicDecompositionCount( str, 0, str.size() - 1, cache );
    }
    static int shortestMetaPalindrome( const string& str, vector< int > & metaLen ) {
        map< ii, ii > cache;
        int startIndex = 0, endIndex = str.size() - 1;
        int N = shortestMetaPalindrome( str, startIndex, endIndex, cache );

        int len;
        int copyIndex = -1;
        while( startIndex <= endIndex ) {
            auto& [ minCount, index ] = cache[ { startIndex, endIndex } ];
            len = index - startIndex;
            metaLen.push_back( len );
            startIndex += len;
            endIndex -= len;
            if( minCount != 1 ) ++copyIndex;
        }
        while( copyIndex >= 0 )
            metaLen.push_back( metaLen[ copyIndex-- ] );
        return N;
    }
private:
    enum { INCREMENT_LEFT = 0, DECREMENT_RIGHT = 1,
           INC_DEC_BOTH = 2 };
    static int LPSubsequence( const string& str, int i, int j,
                              map< ii, ii > &cache ) {
        if( i > j ) return 0;

        ii elem = { i, j };
        if( cache.count( elem ) > 0 )
            return cache[ elem ].first;
        if( i == j ) {
            cache[ elem ] = { 1, INC_DEC_BOTH };
            return 1;
        }

        //result.first is the length of the longest palindromic subsequence.
        //result.second stores a code to reconstruct the longest palindromic
        //              subsequence.
        ii result;
        if( str[i] == str[j] ) {
            int len = LPSubsequence( str, i + 1, j - 1, cache );
            result = { 2 + len, INC_DEC_BOTH };
        } else {
            int right = LPSubsequence( str, i + 1, j, cache );
            int left = LPSubsequence( str, i, j - 1, cache );
            if( left > right ) {
                result = { left, DECREMENT_RIGHT };
            } else {
                result = { right, INCREMENT_LEFT };
            }
        }
        cache[ elem ] = result;
        return result.first;
    }
    static int SPSupersequence( const string &str, int i, int j,
                                map< ii, ii > &cache ) {
        if( i > j ) return 0;

        ii elem = { i, j };
        if( i == j ) {
            cache[ elem ] = { 1, INC_DEC_BOTH };
            return 1;
        }
        if( cache.count( elem ) > 0 )
            return cache[ elem ].first;

        ii result;
        if( str[i] == str[j] ) {
            int len = SPSupersequence( str, i + 1, j - 1, cache );
            result = { 2 + len, INC_DEC_BOTH };
        } else {
            int right = SPSupersequence( str, i + 1, j, cache );
            int left = SPSupersequence( str, i, j - 1, cache );
            if( left < right ) {
                result = { 2 + left, DECREMENT_RIGHT };
            } else {
                result = { 2 + right, INCREMENT_LEFT };
            }
        }
        cache[ elem ] = result;
        return result.first;
    }
    static int SPDecomposition( const string& str, int startIndex,
                                int endIndex,
                                map< int, ii > &cache ) {
        if( startIndex > endIndex ) return 0;

        if( cache.count( startIndex ) )
            return cache[ startIndex ].first;

        int decompIndex = startIndex + 1;
        int decompLen = 1 + SPDecomposition( str, decompIndex, endIndex, cache );
        int curLen;
        for( int k = startIndex + 1; k <= endIndex; ++k ) {
            if( isPalindrome( str, startIndex, k, true ) ) {
                curLen = 1 + SPDecomposition( str, k + 1, endIndex, cache );
                //We found a better decomposition; Record its length and index.
                if( curLen < decompLen ) {
                    decompLen = curLen;
                    decompIndex = k + 1;
                }
            }
        }
        cache[ startIndex ] = { decompLen, decompIndex };
        return decompLen;
    }
    static int palindromicDecompositionCount( const string &str, int startIndex,
                                              int endIndex,
                                              map< int, int > &cache ) {
        if( startIndex > endIndex ) return 1;

        if( cache.count( startIndex ) )
            return cache[ startIndex ];
        int totalCount = 0;
        for( int k = startIndex; k <= endIndex; ++k ) {
            if( isPalindrome( str, startIndex, k, true ) ) {
                totalCount += palindromicDecompositionCount( str, k + 1, endIndex, cache );
            }
        }
        cache[ startIndex ] = totalCount;
        return totalCount;
    }
    static int shortestMetaPalindrome( const string &str, int startIndex, int endIndex,
                                       map< ii, ii > &cache ) {
        if( startIndex > endIndex ) return 0;

        ii window = { startIndex, endIndex };
        if( cache.count( window ) )
            return cache[ window ].first;
        if( isPalindrome( str, startIndex, endIndex, true ) )
        {
            cache[ window ] = { 1, endIndex + 1 };
            return 1;
        }

        int N = endIndex - startIndex + 1 ;
        int minCount = INT_MAX, minIndex;
        int posResult;
        for( int k = 1; k <= N / 2 ; ++k ) {
            if( isPalindrome( str, startIndex, startIndex + k - 1, true ) &&
                isPalindrome( str, endIndex - k + 1, endIndex, true) ) {
                    posResult = 2 + shortestMetaPalindrome( str,
                                    startIndex + k, endIndex - k, cache );
                    if( posResult < minCount ) {
                        minCount = posResult;
                        minIndex = startIndex + k;
                    }
                }
        }
        cache[ window ] = { minCount, minIndex };
        return minCount;
    }
};

class PalindromeTest {
public:
    static void runTest() {
        isPalindromeTest();
        longestPalindromicSubsequenceTest();
        shortestPalindromicSupersequenceTest();
        smallestPalindromicDecompositionTest();
        palindromicDecompositionCountTest();
        shortestMetaPalindromeTest();
    }
private:
    static void isPalindromeTest() {
        vector< pair< string, bool > > testcases = {
            { "", true }, { "A", true }, { "AA", true }, { "AB", false },
            { "ABA", true }, { "RACECAR", true }, { "BANANACHIP", false },
            { "ABCTTCBA", true }, { "ABCTTCVA", false },
        };
        for( auto & [ str, expectedResult ] : testcases )
            assert( Palindrome::isPalindrome( str ) == expectedResult );
    }
    static void longestPalindromicSubsequenceTest() {
        vector< pair< string, int > > testcases = {
            { "MAHDYNAMICPROGRAMZLETMESHOWYOUTHEM", 11 },
            { "MALARACEYALAM", 9 },
            { "RACECAR", 7 },
        };
        for( auto & [ str, expectedLen ] : testcases ) {
            string lps = Palindrome::longestPalindromicSubsequence( str );
            assert( lps.size() == expectedLen );
            assert( StringSequence::isSubsequence( str, lps ) );
            //Longest palindromic subsequence of a string S, is equal to
            //the longest common subsequence of S and its reversal.
            string str_reverse = str;
            reverse( str_reverse.begin(), str_reverse.end() );
            string lpsUsinglcs = StringSequence::longestCommonSubsequence( str, str_reverse );
            assert( lpsUsinglcs.size() == expectedLen );
            printf( "Longest palindromic subsequence of (%s) is (%s), length=%d [%s]\n",
                    str.c_str(), lps.c_str(), expectedLen, lpsUsinglcs.c_str() );
        }
    }
    static void shortestPalindromicSupersequenceTest() {
        vector< pair< string, int > > testcases = {
            { "TWENTYONE", 13 },
            { "RACECAR", 7 },
            { "ALMORA", 9 },
        };
        for( auto & [ str, expectedLen ] : testcases ) {
            string sps = Palindrome::shortestPalindromicSupersequence( str );
            assert( sps.size() == expectedLen );
            assert( Palindrome::isPalindrome( sps ) );
            assert( StringSequence::isSubsequence( sps, str ) );
            printf( "Shortest palindromic supersequence of (%s) is (%s), length=%d\n",
                    str.c_str(), sps.c_str(), expectedLen );
        }
    }
    static void shortestMetaPalindromeTest() {
        vector< pair< string, int > > testcases = {
            { "BOBSMAMASEESAUKULELE", 7 },
            { "AAAAA", 1 },
            { "RACECARBOBSBOB", 2 },
            { "RACECARMALAYALAMBOBSBOB", 3},
            { "RACECAR", 1 },
            { "TOPOMAP", 7 },
            { "PALINDROME", 10 },
            { "AB", 2 },
            { "ABBAYOOY", 2 },
            { "ABBCCCDDDDANANAEEEEFFFGGH", 9 },
        };
        vector< int > metaLen;
        for( auto & [ str, count ] : testcases ) {
            int N = Palindrome::shortestMetaPalindrome( str, metaLen );
            assert( N == count );
            assert( N == metaLen.size() );
            int sum = 0;
            for( int i : metaLen ) sum += i;
            assert( str.size() == sum );

            printf( "Shortest meta palindrome sequence length for [%s] = %d\n",
                    str.c_str(), N );
            printf( "Decomposition: {" );
            for( int i : metaLen ) printf( " %d ", i );
            printf( " }\n" );

            metaLen.clear();
        }
    }
    static void palindromicDecompositionCountTest() {
        vector< pair< string, int > > testcases = {
            { "PALINDROME", 1 },
            { "A", 1 },
            { "AAAA", 8 },
            { "ABAB", 3 },
            { "BANANA", 5 },
            { "MALAYALAM", 8 },
        };
        for( auto & [ str, decompCount ] : testcases ) {
            int N = Palindrome::palindromicDecompositionCount( str );
            assert( N == decompCount );
            printf( "Number of palindromic decompositions for [%s] = %d\n",
                    str.c_str(), N );
        }
    }
    static void smallestPalindromicDecompositionTest() {
        vector< pair< string, int > > testcases = {
            { "BUBBASEESABANANA" , 3 },
            { "MALAYALAM", 1 },
            { "MALAYALAMRACECAR", 2 },
            { "ANASTASIA", 7 },
            { "AAAAAAAAAA", 1 },
            { "BOBSMAMASEESAUKULELE", 7 },
        };
        vector< string > decomp;
        for( auto & [ str, expectedVal ] : testcases ) {
            int N = Palindrome::smallestPalindromicDecomposition( str, decomp );
            assert( N == expectedVal );
            assert( N == decomp.size() );
            string concated;
            for( string& frag : decomp ) concated += frag;
            assert( str == concated );

            printf( "Decomposition for [%s] = |", str.c_str() );
            for( string& frag : decomp )
                printf( "%s|", frag.c_str() );
            printf( "\n" );

            decomp.clear();
        }
    }
};

#endif // __PALINDROME_H__
