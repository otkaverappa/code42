#ifndef __BRACKET_MATCH_H__
#define __BRACKET_MATCH_H__

#include "common.h"

class BracketMatch {
private:
    enum {
        OPEN_SQUARE_BRACKET  = '[',
        CLOSE_SQUARE_BRACKET = ']',
        OPEN_CURLY_BRACKET   = '{',
        CLOSE_CURLY_BRACKET  = '}',
        OPEN_ROUND_BRACKET   = '(',
        CLOSE_ROUND_BRACKET  = ')',
        OPEN_ANGLE_BRACKET   = '<',
        CLOSE_ANGLE_BRACKET  = '>'
    };
    static inline map< char, char > matchingSymbol = {
        { OPEN_SQUARE_BRACKET, CLOSE_SQUARE_BRACKET },
        { OPEN_ROUND_BRACKET,  CLOSE_ROUND_BRACKET },
    };
public:
    static bool isBalanced( const string& str ) {
        stack< char > s;
        s.push( '*' );
        int N = str.size();

        for( int i = 0; i < N; ++i ) {
            char ch;
            switch( ch = str[i] ) {
            case OPEN_SQUARE_BRACKET:
            case OPEN_ROUND_BRACKET:
                s.push( matchingSymbol[ ch ] ); break;
            case CLOSE_SQUARE_BRACKET:
            case CLOSE_ROUND_BRACKET:
                if( s.top() != ch )
                    return false;
                s.pop();
                break;
            default:
                return false;
            }
        }
        s.pop();
        return s.empty();
    }
    static int longestBalancedSubarray( const string& str ) {
    }
private:

};

class BracketMatchTest {
public:
    static void runTest() {
        isBalancedTest();
    }
private:
    static void isBalancedTest() {
        vector< pair< string, bool > > testcases = {
            { "[]", true }, { "()", true }, { "([][]())[]", true },
            { "(((()))[[][]]", false, }, { "(((([]))))[[[]]]", true }, { "([)]", false },
            { "([()][]())[()()]()", true }, { "(([]))]", false },
        };
        for( const auto & [ str, result ] : testcases ) {
            bool isBalanced = BracketMatch::isBalanced( str );
            printf( "%-32s %s\n", str.c_str(), isBalanced ? "True" : "False" );
            assert( isBalanced == result );
        }
    }
};

#endif //__BRACKET_MATCH_H__
