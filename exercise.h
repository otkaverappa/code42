#include <stdio.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
using namespace std;

typedef pair< int, int > ii ;
enum { FROM_TOP = 0, FROM_LEFT = 1, FROM_RIGHT = 2, FROM_HERE = 3 };

//Vankin's Mile
int vankinsMile( const vector< vector < int > > &grid,
                 vector< ii > *pathVec = nullptr ) {
    int M = grid.size();
    assert( M > 0 );
    int N = grid[0].size();

    vector< vector < ii > > optVal( M, vector < ii > ( N, ii(0, 0) ) );

    for( int i = 0; i < M; ++i ) {
        for( int j = 0; j < N; ++j ) {
            assert( grid[j].size() == N );
            int bestVal;
            int x = grid[i][j];

            if( i == 0 && j == 0 ) bestVal = x;
            else if( i == 0 ) bestVal = max( x, x + optVal[i][j - 1].first );
            else if( j == 0 ) bestVal = max( x, x + optVal[i - 1][j].first );
            else bestVal = max( x, max( x + optVal[i][j - 1].first,
                                        x + optVal[i - 1][j].first ) );

            optVal[i][j].first = bestVal;

            int path = FROM_HERE;
            if( i > 0 && bestVal == ( x + optVal[i - 1][j].first ) )
                path = FROM_TOP;
            if( j > 0 && bestVal == ( x + optVal[i][j - 1].first ) )
                path = FROM_LEFT;

            optVal[i][j].second = path;
        }
    }

    int sol = INT_MIN;
    ii maxPosition;
    for( int col = 0; col < N; ++col ) {
        if( optVal[M - 1][col].first > sol ) {
            sol = optVal[M - 1][col].first;
            maxPosition = { M - 1, col };
        }
    }
    for( int row = 0; row < M; ++row ) {
        if( optVal[row][N - 1].first > sol ) {
            sol = optVal[row][N - 1].first;
            maxPosition = { row, N - 1 };
        }
    }

    if( pathVec != nullptr ) {
        int i = maxPosition.first, j = maxPosition.second;

        assert( i == M - 1 || j == N - 1 ); //maxPosition should be in the last row or last column.
        //Record the final move which pushes the token outside the grid.
        if( i == M - 1 )
            pathVec->push_back( { i + 2, j + 1 } );
        else //j == N - 1
            pathVec->push_back( { i + 1, j + 2 } );

        while( true ) {
            pathVec->push_back( { i + 1, j + 1 } );
            if( optVal[i][j].second == FROM_HERE )
                break;
            switch( optVal[i][j].second ) {
                case FROM_TOP: --i; break;
                case FROM_LEFT: --j; break;
                case FROM_RIGHT: ++j; break;
            }
        }
    }

    return sol;
}

void printVankinsMilePath( const vector< ii > &path ) {
    int r, c, u, v;
    for( int i = path.size(); i > 0; --i ) {
        r = path[i - 1].first;
        c = path[i - 1].second;
        if( i == path.size() )
            printf( "Start from (%2d,%2d)", r, c );
        else {
            printf( ", Move %s", ( r == u + 1 ) ? "DOWN" :
                    ( c == v + 1 ) ? "RIGHT" : "LEFT" );
        }
        //u and v are row and column values in previous iteration.
        u = r; v = c;
    }
    printf( ".\n" );
}

bool vankinsMileTest() {
    vector< vector < int > > v = {
        { -1,  7, -8, 10, -5 },
        { -4, -9,  8, -6,  0 },
        {  5, -2, -6, -6,  7 },
        { -7,  4,  7, -3, -3 },
        {  7,  1, -6,  4, -9 },
    };
    vector< ii > path;
    int maxValue = vankinsMile( v, &path );
    printf( "Maximum possible path value = %d\n", maxValue );
    printVankinsMilePath( path );
}

bool indexWithinBounds( int i, int j, int M, int N ) {
    return i >= 0 && i < M && j >= 0 && j < N;
}

int populateLegalMoves( int i, int j, int M, int N, int step,
                        vector< ii > &result ) {
    int priorSize = result.size();
    for( int u = -1; u <= 1; ++u ) {
        int x = i + u * step, y = j;
        if( indexWithinBounds( x, y, M, N ) )
            result.push_back( { x, y } );
        x = i, y = j + u * step;
        if( indexWithinBounds( x, y, M, N ) )
            result.push_back( { x, y } );
    }
    return result.size() - priorSize;
}

int numberMaze( const vector< vector< int > > &grid,
                vector< ii > *allMoves = nullptr ) {
    int M = grid.size();
    assert( M > 0 );
    int N = grid[0].size();

    int moveCount = 0;
    vector< ii > legalMoves;

    ii startCell = { 0, 0 }, endCell = { M - 1, N - 1 };
    queue< ii > moves;
    moves.push( startCell );

    set< ii > visited;
    map< ii, ii > previousMove;
    ii curMove;

    while( !moves.empty() ) {
        int n = moves.size();
        while( n-- ) {
            curMove = moves.front(); moves.pop();
            //Ignore the move if it has already been processed.
            if( visited.find( curMove ) != visited.end() ) continue;

            if( curMove == endCell ) goto endOfSearch;

            visited.insert( curMove );
            int i = curMove.first, j = curMove.second;
            populateLegalMoves( i, j, M, N, grid[i][j], legalMoves );
            for( ii& legalMove : legalMoves ) {
                if( visited.find( legalMove ) == visited.end() ) {
                    moves.push( legalMove );
                    previousMove[ legalMove ] = curMove;
                }
            }
            legalMoves.clear();
        }
        ++moveCount;
    }

    endOfSearch:
    if( curMove != endCell ) moveCount = 0;

    if( allMoves != nullptr && moveCount > 0 ) {
        ii moveChain = endCell;
        while( true ) {
            allMoves->push_back( moveChain );
            if( moveChain == startCell )
                break;
            moveChain = previousMove[ moveChain ];
        }
    }

    return moveCount;
}

void printNumberMazePath( const vector< ii > &allMoves ) {
    printf("Moves: ");
    for( int i = allMoves.size() - 2; i >= 0; --i ) {
        int u = allMoves[ i + 1 ].first, v = allMoves[ i + 1 ].second;
        int x = allMoves[i].first, y = allMoves[i].second;

        if( x == u ) {
            if( y < v ) printf( "Move Left" );
            else printf( "Move Right" );
        } else {
            assert( y == v );
            if( x < u ) printf( "Move Up" );
            else printf( "Move Down" );
        }

        printf( ( i == 0 ) ? "." : ", " );
    }
    printf( "\n" );
}

void numberMazeTest() {
    vector< vector< int > > grid = {
        { 3, 5, 7, 4, 6 },
        { 5, 3, 1, 5, 3 },
        { 2, 8, 3, 1, 4 },
        { 4, 5, 7, 2, 3 },
        { 3, 1, 3, 2, 0 },
    };
    vector< ii > allMoves;
    int count = numberMaze( grid, &allMoves );
    printf( "Moves needed: %d\n", count );
    printNumberMazePath( allMoves );
}

int vidrachLedaPuzzle( const vector< vector< int > > &grid,
                       vector< pair< ii, ii > > *allMoves = nullptr ) {
    int M = grid.size();
    assert( M > 0 );
    int N = grid[0].size();

    int moveCount = 0;
    ii redTokenStart = { 0, 0 }, blueTokenStart = { M - 1, N - 1 };
    pair< ii, ii > startState = { redTokenStart, blueTokenStart };
    pair< ii, ii > endState = { blueTokenStart, redTokenStart };
    pair< ii, ii > curState, nextState;
    set< pair< ii, ii > > visited;
    vector< ii > legalMoves;
    map< pair< ii, ii >, pair< ii, ii > > previousMove;

    queue< pair< ii, ii > > q;
    visited.insert( startState );
    q.push( startState );

    while( !q.empty() ) {
        int n = q.size();
        while( n-- ) {
            curState = q.front(); q.pop();
            if( curState == endState ) goto endOfSearch;

            ii redPos = curState.first;
            ii bluePos = curState.second;

            legalMoves.clear();
            //Try moving the red token.
            populateLegalMoves( redPos.first, redPos.second, M, N,
                                grid[ bluePos.first ][ bluePos.second ], legalMoves );
            for( ii& legalMove : legalMoves ) {
                if( legalMove == bluePos ) continue;
                nextState = { legalMove, bluePos };
                if( visited.find( nextState ) != visited.end() ) continue;
                visited.insert( nextState );
                previousMove[ nextState ] = curState;
                q.push( nextState );
            }
            //Try moving the blue token.
            legalMoves.clear();
            populateLegalMoves( bluePos.first, bluePos.second, M, N,
                                grid[ redPos.first ][ redPos.second ], legalMoves );
            for( ii& legalMove : legalMoves ) {
                if( legalMove == redPos ) continue;
                nextState = { redPos, legalMove };
                if( visited.find( nextState ) != visited.end() ) continue;
                visited.insert( nextState );
                previousMove[ nextState ] = curState;
                q.push( nextState );
            }
        }
        ++moveCount;
    }

    endOfSearch:
    if( curState != endState )
        moveCount = 0;

    if( allMoves != nullptr && moveCount > 0 ) {
        pair< ii, ii > moveChain = curState;
        while( true ) {
            allMoves->push_back( moveChain );
            if( moveChain == startState ) break;
            moveChain = previousMove[ moveChain ];
        }
    }

    return moveCount;
}

void printTwoTokenPath( const vector< pair< ii, ii > > & allMoves,
                        const char* token1Tag, const char* token2Tag )
{
    ii u, v, x, y;
    int d;
    for( int i = allMoves.size() - 2; i >= 0; --i ) {
        u = allMoves[ i + 1 ].first; v = allMoves[ i + 1 ].second;
        x = allMoves[i].first; y = allMoves[i].second;

        if( x == u ) {
            if( y.first == v.first ) {
                d = y.second - v.second;
                assert( d != 0 );
                printf( "Move the %s token %s.\n", token2Tag, d > 0 ? "right" : "left" );
            } else {
                d = y.first - v.first;
                printf( "Move the %s token %s.\n", token2Tag, d > 0 ? "down" : "up" );
            }
        } else {
            if( x.first == u.first ) {
                d = x.second - u.second;
                assert( d != 0 );
                printf( "Move the %s token %s.\n", token1Tag, d > 0 ? "right" : "left" );
            } else {
                d = x.first - u.first;
                printf( "Move the %s token %s.\n", token1Tag, d > 0 ? "down" : "up" );
            }
        }
    }
}

void vidrachLedaPuzzleTest() {
    vector< vector< int > > grid = {
        { 1, 2, 4, 3 },
        { 3, 4, 1, 2 },
        { 3, 1, 2, 3 },
        { 2, 3, 1, 2 },
    };
    vector< pair< ii, ii > > allMoves;
    int totalMoves = vidrachLedaPuzzle( grid, &allMoves );
    if( totalMoves > 0 ) {
        printf( "Solvable in %d moves.\n", totalMoves );
        printTwoTokenPath( allMoves, "Red", "Blue" );
    } else
        printf( "Not solvable.\n" );
}

int populateLegalMovesForTheDane( ii currentLoc, int M, int N,
                                  const vector< string > &grid,
                                  char target_token, char obstacle_token,
                                  ii other_loc, vector< ii > &result ) {
    int i = currentLoc.first, j = currentLoc.second;
    assert( indexWithinBounds( i, j, M, N ) );
    //current position should not be the target.
    assert( grid[i][j] != target_token );
    int priorSize = result.size();

    int count = 0;
    int u = j;
    //Try moving left
    while( --u >= 0 ) {
        if( grid[i][u] == obstacle_token || other_loc == ii( i, u ) )
            break;
        ++count;
    }
    if( count > 0 ) result.push_back( { i, j - count } );
    //Try moving right
    u = j; count = 0;
    while( ++u < N ) {
        if( grid[i][u] == obstacle_token || other_loc == ii( i, u ) )
            break;
        ++count;
    }
    if( count > 0 ) result.push_back( { i, j + count } );
    //Try moving up
    u = i; count = 0;
    while( --u >= 0 ) {
        if( grid[u][j] == obstacle_token || other_loc == ii( u, j ) )
            break;
        ++count;
    }
    if( count > 0 ) result.push_back( { i - count, j } );
    //Try moving down
    u = i; count = 0;
    while( ++u < M ) {
        if( grid[u][j] == obstacle_token || other_loc == ii( u, j ) )
            break;
        ++count;
    }
    if( count > 0 ) result.push_back( { i + count, j } );

    return result.size() - priorSize;
}

int kanielTheDane( const vector< string > &grid,
                   vector< pair < ii, ii > > *allMoves = nullptr ) {
    int M = grid.size();
    assert( M > 0 );
    int N = grid[0].size();

    const char RED_TOKEN = 'R', GREEN_TOKEN = 'G',
               TARGET_TOKEN = 'T', OBSTACLE_TOKEN = 'X',
               EMPTY_TOKEN = '0';

    ii defaultPos = { -1, -1 };
    ii redTokenPos = defaultPos, greenTokenPos = defaultPos,
       targetPos = defaultPos;

    for( int i = 0; i < M; ++i ) {
        assert( N == grid[i].size() );
        for( int j = 0; j < N; ++j ) {
            switch ( grid[i][j] ) {
                case RED_TOKEN: redTokenPos = { i, j }; break;
                case GREEN_TOKEN: greenTokenPos = { i, j }; break;
                case TARGET_TOKEN: targetPos = { i, j }; break;
                case OBSTACLE_TOKEN:
                case EMPTY_TOKEN: break;
                default: assert( false );
            }
        }
    }

    assert( redTokenPos != defaultPos && greenTokenPos != defaultPos &&
            targetPos != defaultPos );

    pair< ii, ii > startState = { redTokenPos, greenTokenPos };
    pair< ii, ii > curState, newState;
    map< pair< ii, ii >, pair< ii, ii > > previousMove;

    set< pair< ii, ii > > visited;
    vector< ii > legalMoves;

    queue< pair< ii, ii > > moves;
    moves.push( startState );
    visited.insert( startState );

    int moveCount = 0;
    bool solvable = false;

    while( !moves.empty() ) {
        int n = moves.size();
        while( n-- ) {
                curState = moves.front(); moves.pop();

                ii redPos = curState.first;
                ii greenPos = curState.second;
                if( redPos == targetPos || greenPos == targetPos ) {
                    solvable = true;
                    goto endOfSearch;
                }
                //Try to move the red token.
                legalMoves.clear();
                populateLegalMovesForTheDane( redPos, M, N, grid,
                                              TARGET_TOKEN, OBSTACLE_TOKEN,
                                              greenPos, legalMoves );
                for( ii& legalMove : legalMoves ) {
                    newState = { legalMove, greenPos };
                    if( visited.find( newState ) != visited.end() ) continue;
                    visited.insert( newState );
                    moves.push( newState );
                    previousMove[ newState ] = curState;
                }
                //Try to move the green token.
                legalMoves.clear();
                populateLegalMovesForTheDane( greenPos, M, N, grid,
                                              TARGET_TOKEN, OBSTACLE_TOKEN,
                                              redPos, legalMoves );
                for( ii& legalMove : legalMoves ) {
                    newState = { redPos, legalMove };
                    if( visited.find( newState ) != visited.end() ) continue;
                    visited.insert( newState );
                    moves.push( newState );
                    previousMove[ newState ] = curState;
                }
        }
        ++moveCount;
    }

    endOfSearch:
    if( !solvable ) moveCount = 0;

    if( allMoves != nullptr && moveCount > 0 ) {
        pair< ii, ii > moveChain = curState;
        while( true ) {
            allMoves->push_back( moveChain );
            if( moveChain == startState ) break;
            moveChain = previousMove[ moveChain ];
        }
    }

    return moveCount;
}

void kanielTheDaneTest() {
    vector< string > grid = {
        "0R000",
        "0000G",
        "0XT00",
        "00000",
        "000X0",
    };
    vector< pair< ii, ii > > allMoves;
    int totalMoves = kanielTheDane( grid, &allMoves );
    if( totalMoves > 0 ) {
        printf( "Solvable in %d moves.\n", totalMoves );
        printTwoTokenPath( allMoves, "Red", "Green" );
    } else
        printf( "Not solvable.\n" );
}
