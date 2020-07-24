#include "stringsequence.h"
#include "palindrome.h"

string StringSequence::longestCommonSubstring( const string& a, const string& b ) {
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

string StringSequence::longestCommonSubsequence_3( const string& a, const string& b,
                                                   const string& c ) {
    int M = a.size(), N = b.size(), P = c.size();
    string lcs;
    Multivector< int > lcsMatrix( { M+1, N+1, P+1 }, 0 );
    for( int i = 1; i <= M; ++i ) {
        for( int j = 1; j <= N; ++j ) {
            for( int k = 1; k <= P; ++k ) {
                int optVal;
                if( a[i-1] == b[j-1] && b[j-1] == c[k-1] ) {
                    lcsMatrix.get( { i-1, j-1, k-1 }, optVal );
                    ++optVal;
                } else {
                    int A, B, C;
                    lcsMatrix.get( { i-1, j, k }, A );
                    lcsMatrix.get( { i, j-1, k }, B );
                    lcsMatrix.get( { i, j, k-1 }, C );
                    optVal = max( max( A, B ), C );
                }
                lcsMatrix.set( { i, j, k }, optVal );
            }
        }
    }
    int i = M, j = N, k = P;
    int optVal;
    lcsMatrix.get( { i, j, k }, optVal );
    while( i > 0 && j > 0 && k > 0 ) {
        int A, B, C, D;
        lcsMatrix.get( { i-1, j, k }, A );
        lcsMatrix.get( { i, j-1, k }, B );
        lcsMatrix.get( { i, j, k-1 }, C );
        lcsMatrix.get( { i-1, j-1, k-1 }, D );
        if( optVal == D + 1 ) {
            lcs += a[i-1];
            --i; --j; --k;
            optVal = D;
        }
        else if( optVal == A )
            --i;
        else if( optVal == B )
            --j;
        else if( optVal == C )
            --k;
    }
    reverse( lcs.begin(), lcs.end() );
    return lcs;
}

string StringSequence::longestCommonSubsequence( const string& a, const string& b ) {
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

string StringSequence::longestCommonPalindromicSubsequence( const string& a, const string& b ) {
    enum { INCREMENT_LEFT_A = 0, DECREMENT_RIGHT_A = 1,
           INCREMENT_LEFT_B = 2, DECREMENT_RIGHT_B = 3,
           INC_DEC_BOTH = 4 };
    int M = a.size(), N = b.size();
    string left, right;
    if( M == 0 || N == 0 ) return left;
    Multivector< int > cache( { M, M, N, N }, 0 );
    Multivector< int > pos( { M, M, N, N }, 0 );

    for( int d1 = 0; d1 < M; ++d1 ) {
        for( int i = 0; i < M - d1; ++i ) {
            int j = i + d1;
            for( int d2 = 0; d2 < N; ++d2 ) {
                for( int u = 0; u < N - d2; ++u ) {
                    int v = u + d2;

                    int optVal, optMove;
                    if( i == j && u == v ) {
                        optVal = ( a[i] == b[u] ) ? 1 : 0;
                        optMove = ( optVal == 1 ) ? INC_DEC_BOTH : INCREMENT_LEFT_A;
                    }
                    else if( i == j ) {
                        int A, B;
                        cache.get( { i, j, u+1, v }, A );
                        cache.get( { i, j, u, v-1 }, B );
                        optVal = max( A, B );
                        optMove = optVal == A ? INCREMENT_LEFT_B : DECREMENT_RIGHT_B;
                    }
                    else if( u == v ) {
                        int A, B;
                        cache.get( { i+1, j, u, v }, A );
                        cache.get( { i, j-1, u, v }, B );
                        optVal = max( A, B );
                        optMove = optVal == A ? INCREMENT_LEFT_A : DECREMENT_RIGHT_A;
                    }
                    else if( a[i] == a[j] && b[u] == b[v] && a[i] == b[u] ) {
                        int len;
                        cache.get( { i+1, j-1, u+1, v-1 }, len );
                        optVal = 2 + len;
                        optMove = INC_DEC_BOTH;
                    } else {
                        int A, B, C, D;
                        cache.get( { i+1, j, u, v }, A );
                        cache.get( { i, j-1, u, v }, B );
                        cache.get( { i, j, u+1, v }, C );
                        cache.get( { i, j, u, v-1 }, D );
                        optVal = max( max( A, B ), max( C, D ) );
                        optMove = optVal == A ? INCREMENT_LEFT_A : (
                                  optVal == B ? DECREMENT_RIGHT_A : (
                                  optVal == C ? INCREMENT_LEFT_B : DECREMENT_RIGHT_B ) );
                    }
                    cache.set( { i, j, u, v }, optVal );
                    pos.set( { i, j, u, v }, optMove );
                }
            }
        }
    }
    int i = 0, j = M-1, u = 0, v = N-1;
    while( i <= M-1 && j >= 0 && u <= N-1 && v >= 0 ) {
        int optMove;
        pos.get( { i, j, u, v }, optMove );
        switch( optMove ) {
            case INCREMENT_LEFT_A: ++i; break;
            case INCREMENT_LEFT_B: ++u; break;
            case DECREMENT_RIGHT_A: --j; break;
            case DECREMENT_RIGHT_B: --v; break;
            case INC_DEC_BOTH:
                left += a[i];
                if( j != i ) right += a[j];
                ++i; --j; ++u; --v; break;
            default: assert( false );
        }
    }
    reverse( right.begin(), right.end() );
    return left + right;
}

string StringSequence::shortestCommonSupersequence( const string& a, const string& b ) {
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

bool StringSequence::isShuffle( const string& a, const string& b, const string& c ) {
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

bool StringSequence::isSmoothShuffle( const string& a, const string& b, const string& c ) {
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

bool StringSequence::isSubarray( const string& a, const string& b, int index ) {
    if( index < 0 || a.size() - index < b.size() )
        return false;
    for( int i = 0; i < b.size(); ++i ) {
        if( a[index + i] != b[i] )
            return false;
    }
    return true;
}

bool StringSequence::isSubsequence( const string& a, const string& b ) {
    int M = a.size(), N = b.size();
    if( N > M ) return false;

    auto iter = b.begin();
    int i = 0;

    while( iter != b.end() && i < M ) {
        if( a[i++] == *iter ) ++iter;
    }
    return iter == b.end();
}

int StringSequence::longestContiguousForwardsBackwardsSubstring( const string& s ) {
    int maxLen = 0;
    map< ii, int > cache;
    longestContiguousForwardsBackwardsSubstring_( s, 0, s.size() - 1, cache, maxLen );
    return maxLen;
}

void StringSequenceTest::longestCommonPalindromicSubsequenceTest() {
    StringTestcaseList testcases = {
        { "", "", "" },
        { "RACECAR", "", "" },
        { "", "RACECAR", "" },
        { "RACECAR", "RACECAR", "RACECAR" },
        { "RACECAR", "MALAYALAMRACECARLION", "RACECAR" },
        { "MALAYALAMRACECARLION", "RACECAR", "RACECAR" },
        { "RACECAR", "MALAYALAM", "AA" },
    };
    printf( "Longest Common Palindromic Subsequence Test\n" );
    function verificationFunc =
             []( const string& a, const string& b, const string& result ) {
                 assert( StringSequence::isSubsequence( a, result ) );
                 assert( StringSequence::isSubsequence( b, result ) );
                 assert( Palindrome::isPalindrome( result ) );
             };
    generic_test( testcases, StringSequence::longestCommonPalindromicSubsequence, verificationFunc );
}

void StringSequenceTest::longestCommonSubstringTest() {
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
    printf( "Longest Common Substring Test\n" );
    function verificationFunc =
             []( const string& a, const string& b, const string& result ) {
             };
    generic_test( testcases, StringSequence::longestCommonSubstring, verificationFunc );
}

void StringSequenceTest::longestCommonSubsequenceTest() {
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

void StringSequenceTest::longestCommonSubsequence_3Test() {
    vector< tuple< string, string, string, string > > testcases = {
        { "", "", "", "" },
        { "APPLE", "", "", "" },
        { "", "APPLE", "", "" },
        { "", "", "APPLE", "" },
        { "APPLE", "APPLEORANGE", "APPLEPINE", "APPLE" },
        { "REDAPPLE", "GREENAPPLE", "NOAPPLE", "APPLE" },
        { "XXRXAXCXEXCXAXRXX", "YYRYAYCYEYCYAYRYY", "ZZRZAZCZEZCZAZRZZ", "RACECAR" },
    };
    printf( "Longest Common Subsequence (3 strings) Test\n" );
    for( auto testcase : testcases ) {
        const string& a = get< 0 > ( testcase );
        const string& b = get< 1 > ( testcase );
        const string& c = get< 2 > ( testcase );
        const string& expectedResult = get< 3 > ( testcase );

        string result = StringSequence::longestCommonSubsequence_3( a, b, c );
        printf( "[%s] [%s] [%s] = [%s] length = %d\n", a.c_str(), b.c_str(), c.c_str(),
                result.c_str(), result.size() );
        assert( result.size() == expectedResult.size() );

        assert( StringSequence::isSubsequence( a, result ) );
        assert( StringSequence::isSubsequence( b, result ) );
        assert( StringSequence::isSubsequence( c, result ) );
    }
}

void StringSequenceTest::shortestCommonSupersequenceTest() {
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

void StringSequenceTest::generic_test( StringTestcaseList & testcases, StringFunction implFunc,
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

void StringSequenceTest::longestContiguousForwardsBackwardsSubstringTest() {
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

void StringSequenceTest::isSubsequenceTest() {
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

void StringSequenceTest::isSubarrayTest() {
    vector< tuple< string, string, int, bool > > testcases = {
        { "PINEAPPLE", "PINE", 0, true },
        { "PINEAPPLE", "PINE", 1, false },
        { "PINEAPPLE", "PINE", 10, false },
        { "PINEAPPLE", "", 0, true },
        { "PINEAPPLE", "APPLE", 4, true },
        { "PINEAPPLE", "PINEAPPLE", 0, true },
        { "PINEAPPLE", "PINEAPPLE", 1, false },
        { "PINEAPPLE", "PINEAPPLES", 0, false },
    };
    for( auto testcase : testcases ) {
        const string& a = get< 0 > ( testcase );
        const string& b = get< 1 > ( testcase );
        int index = get< 2 > ( testcase );
        bool expectedResult = get< 3 > ( testcase );

        bool isSubarray = StringSequence::isSubarray( a, b, index );
        printf( "[%s] [%s] [from %d] isSubarray: %s\n", a.c_str(), b.c_str(), index,
                isSubarray ? "True" : "False" );
        assert( expectedResult == isSubarray );
    }
}

void StringSequenceTest::isShuffleTest() {
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

void StringSequenceTest::isSmoothShuffleTest() {
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
