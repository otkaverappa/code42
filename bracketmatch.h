#ifndef __BRACKET_MATCH_H__
#define __BRACKET_MATCH_H__

#include "common.h"

class BracketMatch {
public:
    static bool isBalanced( const string& str );
    static string longestBalancedSubarray( const string& str );

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
    static inline unordered_map< char, char > matchingCloseSymbol = {
        { OPEN_SQUARE_BRACKET, CLOSE_SQUARE_BRACKET },
        { OPEN_ROUND_BRACKET,  CLOSE_ROUND_BRACKET },
    };
     static inline unordered_map< char, char > matchingOpenSymbol = {
        { CLOSE_SQUARE_BRACKET, OPEN_SQUARE_BRACKET },
        { CLOSE_ROUND_BRACKET, OPEN_ROUND_BRACKET },
    };
    static inline unordered_set< char > allBrackets = {
        OPEN_SQUARE_BRACKET, OPEN_CURLY_BRACKET, OPEN_ROUND_BRACKET, OPEN_ANGLE_BRACKET,
        CLOSE_SQUARE_BRACKET, CLOSE_CURLY_BRACKET, CLOSE_ROUND_BRACKET, CLOSE_ANGLE_BRACKET,
    };
    static bool isOpenBracket( char ch ) {
        return matchingCloseSymbol.find( ch ) != matchingCloseSymbol.end() ;
    }
    static bool isCloseBracket( char ch ) {
        return matchingOpenSymbol.find( ch ) != matchingOpenSymbol.end();
    }
};

class BracketMatchTest {
public:
    static void runTest() {
        isBalancedTest();
        longestBalancedSubarrayTest();
    }

private:
    static void isBalancedTest();
    static void longestBalancedSubarrayTest();
};

#endif //__BRACKET_MATCH_H__
