#include "uva.h"

string UVA_11995::processInputSequence( const vector< ii > & input ) {
    bool isStack, isQueue, isPriorityQueue;
    int count = 3;
    stack< int > s;
    queue< int > q;
    priority_queue< int > pq;

    isStack = isQueue = isPriorityQueue = true;

    for( auto & [ operation, operand ] : input ) {
            if( operation == OPERATION_INSERT ) {
                s.push( operand );
                q.push( operand );
                pq.push( operand );
            } else if( operation == OPERATION_REMOVE ) {
                if( isStack ) {
                    if( s.empty() || s.top() != operand ) {
                        isStack = false;
                        --count;
                    } else {
                        s.pop();
                    }
                }
                if( isQueue ) {
                    if( q.empty() || q.front() != operand ) {
                        isQueue = false;
                        --count;
                    } else {
                        q.pop();
                    }
                }
                if( isPriorityQueue ) {
                    if( pq.empty() || pq.top() != operand ) {
                        isPriorityQueue = false;
                        --count;
                    } else {
                        pq.pop();
                    }
                }
            }
            else
                assert( false );
    }
    string result;
    switch( count ) {
        case 0: result = tags[ CONTAINER_NONE ]; break;
        case 1: result = isStack ? tags[ CONTAINER_STACK ] :
                         ( isQueue ? tags[ CONTAINER_QUEUE ] : tags[ CONTAINER_PRIORITY_QUEUE ] );
                break;
        default:
            if( isStack ) {
                result += tags[ CONTAINER_STACK ];
                if( --count ) result += tags[ CONTAINER_MULTIPLE ];
            }
            if( isQueue ) {
                result += tags[ CONTAINER_QUEUE ];
                if( --count ) result += tags[ CONTAINER_MULTIPLE ];
            }
            if( isPriorityQueue ) {
                result += tags[ CONTAINER_PRIORITY_QUEUE ];
            }
    }
    return result;
}
