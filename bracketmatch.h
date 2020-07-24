#ifndef __BRACKET_MATCH_H__
#define __BRACKET_MATCH_H__

#include "common.h"

class BracketMatch {
public:
    static bool isBalanced( const string& str );
    static bool isBalancedPalindrome( const string& str );

    static string longestBalancedSubarray( const string& str );
    static string longestBalancedSubsequence( const string& str );
    static string longestPalindromicBalancedSubarray( const string& str );
    static string longestPalindromicBalancedSubsequence( const string& str );

    static string longestCommonBalancedSubarray( const string& a, const string& b );
    static string longestCommonBalancedSubsequence( const string& a, const string& b );
    static string longestCommonPalindromicBalancedSubarray( const string& a, const string& b );
    static string longestCommonPalindromicBalancedSubsequence( const string& a, const string& b );

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
    static bool balancedComponent( const string& str, int startIndex, int endIndex,
                                   int& len );
};

class BracketMatchTest {
public:
    static void runTest() {
        isBalancedTest();
        isBalancedPalindromeTest();
        longestBalancedSubarrayTest();
    }

private:
    static void isBalancedTest();
    static void isBalancedPalindromeTest();
    static void longestBalancedSubarrayTest();
};

#endif //__BRACKET_MATCH_H__
