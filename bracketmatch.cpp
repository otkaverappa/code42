#include "bracketmatch.h"
#include "stringsequence.h"

bool BracketMatch::isBalanced( const string& str ) {
    int N = str.size();
    if( N & 0x1 )
        return false;

    stack< char > s;
    s.push( '*' );

    for( int i = 0; i < N; ++i ) {
        char ch = str[i];
        if( isOpenBracket( ch ) ) {
            s.push( matchingCloseSymbol[ ch ] );
        } else if( isCloseBracket( ch ) ) {
            if( s.top() != ch )
                return false;
            s.pop();
        } else {
            return false;
        }
    }
    s.pop();
    return s.empty();
}

bool BracketMatch::balancedComponent( const string& str, int startIndex,
                                      int endIndex, int& len ) {
    if( startIndex >= endIndex || ( ( endIndex - startIndex ) & 0x1 ) == 0 )
        return false;
    int len_ = 0;

    stack< char > s;
    for( int i = startIndex; i <= endIndex; ++i ) {
        char ch = str[i];
        if( isOpenBracket( ch ) ) {
            s.push( matchingCloseSymbol[ ch ] );
        } else if( isCloseBracket( ch ) ) {
            if( s.empty() || s.top() != ch )
                return false;
            s.pop();
        }
        ++len_;
        if( s.empty() ) {
            len = len_;
            return true;
        }
    }
    return false;
}

bool BracketMatch::isBalancedPalindrome( const string& str ) {
    int N = str.size();
    if( N == 0 )
        return true;
    int startIndex = 0, endIndex = N-1;
    int len;
    while( balancedComponent( str, startIndex, endIndex, len ) ) {
        int curLen = endIndex - startIndex + 1;
        if( len == curLen )
            return true;
        else if( len + len > curLen )
            return false;

        string balancedComp = str.substr( startIndex, len );
        if( !StringSequence::isSubarray( str, balancedComp, endIndex - len + 1 ) )
            return false;
        if( len + len == curLen )
            return  true;
        startIndex += len;
        endIndex -= len;
    }
    return false;
}

string BracketMatch::longestBalancedSubarray( const string& str ) {
    int N = str.size();
    string result;

    vector< int > lbsLen( N, 0 );
    int maxLen = 0, bestIndex;

    for( int i = 0; i < N; ++i ) {
        int optLen = 0;
        char ch = str[i];

        if( isCloseBracket( ch ) ) {
            if( i > 0 && str[i-1] == matchingOpenSymbol[ ch ] ) {
                optLen = 2 + ( i-2 >= 0 ? lbsLen[i-2] : 0 );
            } else if( i > 0 && lbsLen[i-1] > 0 ) {
                int index = i - lbsLen[i-1] - 1;
                if( index >= 0 && str[index] == matchingOpenSymbol[ ch ] )
                    optLen = 2 + lbsLen[i-1];
                    if( index - 1 >= 0 )
                        optLen += lbsLen[index - 1];
            }
        }
        lbsLen[i] = optLen;
        if( maxLen < lbsLen[i] ) {
            maxLen = lbsLen[i];
            bestIndex = i;
        }
    }
    if( maxLen > 0 ) {
        result = str.substr( bestIndex - maxLen + 1, maxLen );
    }
    return result;
}

void BracketMatchTest::isBalancedTest() {
    vector< pair< string, bool > > testcases = {
        { "[]", true }, { "()", true }, { "([][]())[]", true },
        { "(((()))[[][]]", false, }, { "(((([]))))[[[]]]", true }, { "([)]", false },
        { "([()][]())[()()]()", true }, { "(([]))]", false },
    };
    printf( "IsBalanced ?\n" );
    for( const auto & [ str, result ] : testcases ) {
        bool isBalanced = BracketMatch::isBalanced( str );
        printf( "%-32s %s\n", str.c_str(), isBalanced ? "True" : "False" );
        assert( isBalanced == result );
    }
}

void BracketMatchTest::isBalancedPalindromeTest() {
    vector< pair< string, bool > > testcases = {
        { "", true }, { "[]", true }, { "()", true },
        { "[][][][]", true }, { "()()()()", true },
        { "([][]())[]", false }, { "(((()))[[][]]", false, },
        { "(((([]))))[[[]]]", false },
        { "(((([]))))[[[]]](((([]))))", true },
        { "([)]", false },
        { "([()][]())[()()]()", false },
        { "[[[(([]))]]]", true },
        { "()[]()[[[[([])]]]]()[]()", true },
    };
    printf( "IsBalancedPalindrome ?\n" );
    for( const auto & [ str, result ] : testcases ) {
        bool isBalancedPalindrome = BracketMatch::isBalancedPalindrome( str );
        printf( "%-32s %s\n", str.c_str(), isBalancedPalindrome ? "True" : "False" );
        assert( isBalancedPalindrome == result );
    }
}

void BracketMatchTest::longestBalancedSubarrayTest() {
    vector< pair< string, string > > testcases = {
        { "[][][][][][]", "[][][][][][]" },
        { "()()()()()()", "()()()()()()" },
        { "[((([][])))]", "[((([][])))]" },
        { "[][][](())()", "[][][](())()" },
        { "[][)]()()[]", "()()[]" },
        { "[[[[[[]]]]]]([][]", "[[[[[[]]]]]]" },
        { "[[[[[[]]]]]]([][])", "[[[[[[]]]]]]([][])" },
        { "[[[[[[]]]]]]([][]))))))", "[[[[[[]]]]]]([][])" },
        { ")))))(((((", "" },
        { "]]]]][[[[[", "" },
        { "", "" },
    };
    printf( "Longest Balanced Subarray\n" );
    for ( const auto & [ str, result ] : testcases ) {
        string lbs = BracketMatch::longestBalancedSubarray( str );
        printf( "%-32s --> %-32s [ Length = %d ]\n", str.c_str(), lbs.c_str(), result.size() );
        assert( result.size() == lbs.size() );
        assert( BracketMatch::isBalanced( lbs ) );
        if( BracketMatch::isBalanced( str ) )
            assert( str.size() == lbs.size() );
    }
}
