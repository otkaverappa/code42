#include "uva.h"

//UVA_599
ii UVA_599::numberOfTreesAndAcorns( const vector< ii > & edges, const vector< int > & vertices ) {
    UnionFind< int > uf( vertices.begin(), vertices.end() );
    for ( const auto & [ u, v ] : edges )
        uf.merge( u, v );
    int acorns = 0, trees;
    for( int v : vertices ) {
        if( uf.findSize( v ) == 1 )
            ++acorns;
    }
    trees = uf.setSize() - acorns;
    return { trees, acorns } ;
}

//UVA_10158
bool UVA_10158::setFriends( int x, int y ) {
}

bool UVA_10158::setEnemies( int x, int y ) {
}

bool UVA_10158::areFriends( int x, int y ) {
}

bool UVA_10158::areEnemies( int x, int y ) {
}

int UVA_12049::minimumDeletions( const vector< int > & list1, const vector< int > & list2 ) {
    int minimumDeletionCount = 0;

    unordered_map< int, int > map1, map2;
    for( int elem : list1 )
        ++map1[elem];
    for( int elem : list2 )
        ++map2[elem];

    for( const auto & [ key, freq ] : map1 ) {
        minimumDeletionCount += abs( freq - map2[key] );
        map2.erase( key );
    }
    for( const auto & [ key, freq ] : map2 )
        minimumDeletionCount += freq;

    return minimumDeletionCount;
}

string UVA_978::lemmingsBattle( const vector< int > & blueArmy, const vector< int > & greenArmy,
                                int battleRounds, vector< int > &remainingArmy ) {
    multiset< int, greater< int > > blue( blueArmy.begin(), blueArmy.end() );
    multiset< int, greater< int > > green( greenArmy.begin(), greenArmy.end() );

    while( blue.size() || green.size() ) {
        if( blue.empty() ) {
            for( auto elem : green )
                remainingArmy.push_back( elem );
            return tags[ GREEN_LEMMING ];
        } else if( green.empty() ) {
            for( auto elem : blue )
                remainingArmy.push_back( elem );
            return tags[ BLUE_LEMMING ];
        }

        auto iterGreen = green.begin();
        auto iterBlue = blue.begin();
        vector< int > greenWinners, blueWinners;
        for( int round = 1; round <= battleRounds; ++round ) {
            if( iterGreen == green.end() || iterBlue == blue.end() )
                break;
            int greenPower = *iterGreen;
            iterGreen = green.erase( iterGreen );
            int bluePower = *iterBlue;
            iterBlue = blue.erase( iterBlue );
            if( greenPower > bluePower ) {
                greenWinners.push_back( greenPower - bluePower );
            } else if( greenPower < bluePower ) {
                blueWinners.push_back( bluePower - greenPower );
            }
        }
        for( int winner : greenWinners ) green.insert( winner );
        for( int winner : blueWinners ) blue.insert( winner );
    }
    return tags[ RESULT_TIE ];
}

string UVA_11995::containerType( const vector< ii > & input ) {
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
