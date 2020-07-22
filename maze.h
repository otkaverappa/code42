#ifndef __MAZE_H__
#define __MAZE_H__

#include "common.h"

class NumberMaze {
public:
    static int maxTotalMarkedCellValue( const vector< vector< int > > &grid ) {
        int M = grid.size();
        assert( M > 0 );
        int N = grid[0].size();

        struct State {
            enum { INCLUDES_CELL = 0x1,
                   INCLUDES_CELL_TOP = 0x2,
                   INCLUDES_CELL_LEFT = 0x4,
            };
            // A - maximum value including the cell.
            // B - maximum value not including the cell.
            int A, B;
            unsigned int detail;
            State() : A(0), B(0), detail(0) {}
            int max() { return ::max( A, B ); }
        };
        vector< vector< State > > v( M+1, vector< State > ( N+1, State() ) );

        for( int i = 1; i <= M; ++i ) {
            for( int j = 1; j <= N; ++j ) {
                State curState;
                int cellValue = grid[i-1][j-1];

                //Calculate curState.A, maximum value including the cell.
                int bestValue = v[i][j-1].B + v[i-1][j].B ;


                v[i][j] = curState;
            }
        }

        return v[M][N].max();
    }
    static int solveNumberMaze( const vector< vector < int > > &grid,
                                vector< ii > *allMoves = nullptr ) {
        //startCell - Top Left corner, endCell - Bottom Right corner.
        int M = grid.size();
        assert( M > 0 );
        int N = grid[0].size();
        return solveNumberMaze( grid, {0, 0}, {M-1, N-1}, allMoves );
    }
    static int solveNumberMaze( const vector< vector < int > > &grid,
                                const ii& startCell, const ii& endCell,
                                vector< ii > *allMoves = nullptr ) {
        int M = grid.size();
        assert( M > 0 );
        int N = grid[0].size();
        RectangularGridProperties rgp( M, N );
        rgp.boundsCheckAndAssert( startCell );
        rgp.boundsCheckAndAssert( endCell );

        int moveCount = 0;
        vector< ii > legalMoves;

        queue< ii > moves;
        moves.push( startCell );

        map< ii, ii > previousMove;
        previousMove[ startCell ] = startCell;
        ii curMove;

        while( !moves.empty() ) {
            int n = moves.size();
            while( n-- ) {
                curMove = moves.front(); moves.pop();

                if( curMove == endCell ) goto endOfSearch;

                int i = curMove.first, j = curMove.second;
                rgp.fillNeighbors( { i, j }, legalMoves, grid[i][j] );
                for( ii& legalMove : legalMoves ) {
                    if( previousMove.count( legalMove ) == 0 ) {
                        moves.push( legalMove );
                        previousMove[ legalMove ] = curMove;
                    }
                }
                legalMoves.clear();
            }
            ++moveCount;
        }

        endOfSearch:
        if( curMove != endCell )
            moveCount = 0;

        if( allMoves != nullptr && moveCount > 0 ) {
            while( true ) {
                allMoves->push_back( curMove );
                if( curMove == startCell )
                    break;
                curMove = previousMove[ curMove ];
            }
        }
        return moveCount;
    }
};

class NumberMazeTest {
public:
    static void runTest() {
        testcase1();
        testcase2();
        testcase3();
    }
private:
    static void testcase1() {
        vector< vector< int > > grid = {
            { 3, 5, 7, 4, 6 },
            { 5, 3, 1, 5, 3 },
            { 2, 8, 3, 1, 4 },
            { 4, 5, 7, 2, 3 },
            { 3, 1, 3, 2, 0 },
        };
        vector< ii > allMoves;
        int moveCount = NumberMaze::solveNumberMaze( grid, &allMoves );
        printNumberMazePath( moveCount, allMoves );
    }
    static void testcase2() {
        vector< vector< int > > grid = {
            { 5, 1, 2, 3, 4, 3 },
            { 3, 5, 4, 1, 4, 2 },
            { 2, 4, 1, 3, 4, 2 },
            { 3, 1, 4, 2, 3, 5 },
            { 2, 3, 1, 2, 3, 1 },
            { 1, 4, 3, 2, 4, 5 },
        };
        vector< ii > allMoves;
        int moveCount = NumberMaze::solveNumberMaze( grid, &allMoves );
        printNumberMazePath( moveCount, allMoves );
    }
    static void testcase3() {
        vector< vector< int > > grid = {
            { 4,-5, 1, 6 },
            { 2, 6,-1, 8 },
            { 5, 4, 3, 3 },
            { 1,-1, 7, 4 },
            {-3, 4, 5,-2 },
        };
        int maxValue = NumberMaze::maxTotalMarkedCellValue( grid );
        printf( "Maximum value of marked cells = %d\n", maxValue );
    }
private:
    static void printNumberMazePath( int moveCount, const vector< ii > &allMoves ) {
        if( moveCount <= 0 ) {
            printf( "There is no path through the number maze.\n" );
            return;
        } else {
            printf( "Optimal path length through the number maze = %d\n", moveCount );
        }
        printf( "Moves: " );
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
};

#endif // __MAZE_H__
