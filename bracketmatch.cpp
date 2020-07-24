#include "bracketmatch.h"

bool BracketMatch::isBalanced( const string& str ) {
    stack< char > s;
    s.push( '*' );
    int N = str.size();

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
