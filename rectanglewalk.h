#ifndef __RECTANGLE_WALK_H__
#define __RECTANGLE_WALK_H__

#include "common.h"

class RectangleWalk {
private:
    typedef pair< ii, ii > Rectangle;
    const Bitmap& bitmap;
    SolidBlockComputation compute;
    ii startPos, endPos;
public:
    RectangleWalk( const Bitmap& bitmap ) : bitmap( bitmap ),
                   compute( bitmap ) {}

    int rectangleWalk( const ii& startPos, const ii& endPos ) {
        bitmap.boundsCheckAndAssert( startPos );
        bitmap.boundsCheckAndAssert( endPos );
        assert( bitmap.isSet( startPos ) && bitmap.isSet( endPos ) );

        int totalMoves = 0;
        bool searchSuccessful = false;
        //startState and endState are unit rectangles.
        Rectangle startState( startPos, startPos ), endState( endPos, endPos );
        map< Rectangle, Rectangle > processedMoves;
        processedMoves[ startState ] = startState;
        queue< Rectangle > q;
        q.push( startState );

        while( !q.empty() ) {
            int N = q.size();
            while( N -- ) {
                Rectangle & curState = q.front(); q.pop();
                Rectangle newState;
                const auto & [ u, v ] = curState.first;
                const auto & [ x, y ] = curState.second;
                //(u, v) is the topLeft corner of the rectangle and (x, y) is the
                //bottomRight corner.
                if( curState == endState ) {
                    searchSuccessful = true;
                    goto endOfSearch;
                }
                //Generate rectangles obtained by expanding.
                int topRow = u, bottomRow = x;
                while( topRow - 1 >= 0 &&
                       compute.isSolidWhiteBlock( { topRow - 1, v }, { x, y } ) ) {
                    --topRow;
                    newState = Rectangle( { topRow, v }, { x, y } );
                    evaluateState( newState, curState, processedMoves, q );
                }
                while( bottomRow + 1 < bitmap.rows() &&
                       compute.isSolidWhiteBlock( { u, v }, { bottomRow + 1, y } ) ) {
                    ++bottomRow;
                    newState = Rectangle( { u, v }, { bottomRow, y } );
                    evaluateState( newState, curState, processedMoves, q );
                }
                for( int startRow = u; startRow >= topRow; --startRow ) {
                    for( int endRow = x; endRow <= bottomRow; ++endRow ) {
                        int startCol = v, endCol = y;
                        while( startCol - 1 >= 0 &&
                               compute.isSolidWhiteBlock( { startRow, startCol - 1 },
                                                          { endRow, y } ) ) {
                            --startCol;
                            newState = Rectangle( { startRow, startCol }, { endRow, y } );
                            evaluateState( newState, curState, processedMoves, q );
                        }
                        while( endCol + 1 < bitmap.cols() &&
                               compute.isSolidWhiteBlock( { startRow, v },
                                                          { endRow, endCol + 1 } ) ) {
                            ++endCol;
                            newState = Rectangle( { startRow, v }, { endRow, endCol } );
                            evaluateState( newState, curState, processedMoves, q );
                        }

                        for( ; startCol < v; ++startCol ) {
                            for( ; endCol > y; --endCol ) {
                                if( compute.isSolidWhiteBlock( { startRow, startCol },
                                                               { endRow, endCol } ) ) {
                                    newState = Rectangle( { startRow, startCol },
                                                          { endRow, endCol } );
                                    evaluateState( newState, curState, processedMoves, q );
                                }
                            }
                        }
                    }
                }

                //Generate rectangles obtained by shrinking.
                for( int startRow = u; startRow <= x; ++startRow ) {
                    for( int startCol = v; startCol <= y; ++startCol ) {
                        for( int endRow = startRow; endRow <= x; ++endRow ) {
                            for( int endCol = startCol; endCol <= y; ++endCol ) {
                                newState = Rectangle( { startRow, startCol },
                                                      { endRow, endCol } );
                                evaluateState( newState, curState, processedMoves, q );
                            }
                        }
                    }
                }
            }
            ++totalMoves;
        }

        endOfSearch:
        if( !searchSuccessful ) totalMoves = -1;
        return totalMoves;
    }
private:
    void evaluateState( const Rectangle& newState, const Rectangle& curState,
                        map< Rectangle, Rectangle > & processedMoves, queue< Rectangle > & q ) {
        if( processedMoves.count( newState ) > 0 )
            return;
        processedMoves[ newState ] = curState;
        q.push( newState );
    }
};

class RectangleWalkTest {
public:
    static void runTest() {
        vector< string > grid = {
            "111111111111011111111111",
            "111111110001101000111111",
            "111111111110110110110111",
            "111111111111111110101011",
            "111111111111111101011100",
            "101111111111011011110111",
            "011101111111110101111011",
            "101110111111111011111111",
            "111001111111111011111110",
            "111110111111110101111101",
            "110111001111110111111111",
            "111111111111111011111111",
            "111111111111111011111111",
            "101111111111111011001111",
            "101111111111110111011111",
            "111111111101110111011111",
            "111111111010010111011111",
            "101111110111000011011111",
            "111111111111111111011101",
            "011111111111111110011110",
            "101111100111111111011101",
            "110110011010110110111111",
            "111001111101111101111111",
            "111111111111011011101111",
        };

        Bitmap bitmap( grid );
        RectangleWalk walk( bitmap );
        ii startPos = { 0, 0 };
        ii endPos = { grid.size() - 1, grid[0].size() - 1 };

        int pathLen = walk.rectangleWalk( startPos, endPos );
        printf( "Path Length = %d\n", pathLen );
    }
};

#endif // __RECTANGLE_WALK_H__
