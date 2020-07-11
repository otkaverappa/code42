#ifndef __PARENMATCH_H__
#define __PARENMATCH_H__

#include "common.h"

class ParenMatch {
private:
    static map< char, char > matchingSymbol;
public:
    static bool isValidString( const string& str ) {
        stack< char > s;
        s.push( '*' );
        int N = str.size();

        for( int i = 0; i < N; ++i ) {
            char ch;
            switch( ch = str[i] ) {
            case '[': case '(':
                s.push( matchingSymbol[ ch ] ); break;
            case ']': case ')':
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
    static string longestBalancedSubsequence( const string& str ) {
        map< ii, ii > cache;
        int i = 0, j = str.size() - 1;

        int len = LBSubsequence( str, i, j, cache );
        ii result;
        string left, right;

        while( i < j ) {
            result = cache[ { i, j } ];
            auto & [ u, v ] = result;
            switch( v ) {
                case INCREMENT_LEFT: ++i; break;
                case DECREMENT_RIGHT: --j; break;
                case INC_DEC_BOTH:
                    left += str[ i++ ];
                    right += str[ j-- ];
                    break;
            }
        }
        reverse( right.begin(), right.end() );
        left += right;
        assert( left.size() == len );
        return left;
    }
    static int longestBalancedSubarray( const string& str ) {
    }
private:
    enum { INCREMENT_LEFT = 0, DECREMENT_RIGHT = 1,
           INC_DEC_BOTH = 2 };
    static int LBSubsequence( const string& str, int i, int j, map< ii, ii > &cache ) {
        if( i >= j ) return 0;

        ii elem = { i, j };
        if( cache.count( elem ) > 0 )
            return cache[ elem ].first;

        ii result;
        char ch = matchingSymbol[ str[i] ];
        if( ch == char() ) {
            //The char at index i is not valid. In such a case,
            //LBS(i, j) = LBS(i + 1,j)
            int len = LBSubsequence( str, i + 1, j, cache );
            result = { len, INCREMENT_LEFT };
        } else if( ch == str[j] ) {
            //The char at index i is valid, and its matching char is present at j.
            //In such a case,
            //LBS(i, j) = 2 + LBS(i + 1, j - 1)
            int len = LBSubsequence( str, i + 1, j - 1, cache );
            result = { 2 + len, INC_DEC_BOTH };
        } else {
            //LBS(i, j) = max( LBS(i + 1, j), LBS(i, j - 1) )
            int left = LBSubsequence( str, i + 1, j, cache );
            int right = LBSubsequence( str, i, j - 1, cache );
            if( left > right ) {
                result = { left, INCREMENT_LEFT };
            } else {
                result = { right, DECREMENT_RIGHT };
            }
        }
        cache[ elem ] = result;
        cout << str << " " << i << " " << j << " " << result.first << endl;
        return result.first;
    }
};
map< char, char > ParenMatch::matchingSymbol = {
    { '[', ']' },
    { '(', ')' },
};

class ParenMatchTest {
public:
    static void runTest() {
        vector< pair< string, bool > > testcases = {
            { "[]", true }, { "()", true }, { "([][]())[]", true },
            { "(((()))[[][]]", false, }, { "(((([]))))[[[]]]", true }, { "([)]", false },
            { "([()][]())[()()]()", true }, { "(([]))]", false },
        };
        for( const auto & [ str, result ] : testcases ) {
            bool isValid = ParenMatch::isValidString( str );
            printf( "%-32s %s\n", str.c_str(), isValid ? "True" : "False" );
            assert( isValid == result );
        }

        string stru = ParenMatch::longestBalancedSubsequence( "[][]" );
        cout << stru << endl;
    }
};

#endif //__PARENMATCH_H__
