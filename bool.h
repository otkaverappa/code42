#ifndef __BOOL_H__
#define __BOOL_H__

#include "common.h"

class BooleanMatch {
public:
    enum {
        OPERATOR_AND = '&',
        OPERATOR_OR  = '|',
        OPERATOR_XOR = '^',
        OPERAND_BINARY_0 = '0',
        OPERAND_BINARY_1 = '1',
    };
public:
    ii booleanMatch( const string& expression ) {
        int N = expression.size();
        //The input expression must be of the form - "1&0&0|1^0". Specifically,
        //N > 0 and N is an odd integer.
        assert( N > 0 && ( N & 1 ) );
        char ch = convertOperand( expression[0] );
        ii baseCase = { ch, 1 - ch };

        for( int index = 1; index < N; index += 2 ) {

        }
    }
private:
    char convertOperand( char ch ) {
        if( ch == OPERAND_BINARY_0 )
            ch = 0;
        else {
            assert( ch == OPERAND_BINARY_1 );
            ch = 1;
        }
        return ch;
    }
    ii baseExpression( ii trueFalseCount, char op, char ch ) {
        ch = convertOperand( ch );
        const auto & [ trueCount, falseCount ] = trueFalseCount;
        ii res;

        switch( op ) {
        case OPERATOR_AND: res = { trueCount * ch, falseCount }; break;
        case OPERATOR_OR:  res = { trueCount, falseCount * ( 1 - ch ) }; break;
        case OPERATOR_XOR: res = { ch == 0 ? trueCount : falseCount,
                                   ch == 0 ? falseCount : trueCount }; break;
        default:
            assert( false );
        }
        return res;
    }
};

class BooleanMatchTest {
public:
    static void runTest() {
        vector< tuple< string, bool, int > > testcases = {
            { "1&1|0|1^0", true, 5 },
        };
    }
};

#endif // __BOOL_H__
