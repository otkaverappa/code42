#include "samegame.h"

bool SameGame::setGameState( const vector< char > & rowData ) {
    if( rowData.size() < rows() * cols() )
        return false;
    int index = 0;
    for( int i = 0; i < rows(); ++i ) {
        for( int j = 0; j < cols(); ++j ) {
            grid[i][j] = rowData[ index++ ];
            assert( grid[i][j] >= EMPTY_CELL && grid[i][j] < COLOR_INVALID );
        }
    }
    return true;
}

int SameGame::makeMove( int i, int j ) {
    //To make a move, i and j should be within the grid.
    boundsCheckAndAssert( i, j );
    //If the location (i, j) is already empty, then return.
    if( grid[i][j] == EMPTY_CELL )
        return 0;

    int N = 0; //Size of the cluster of balls of the same color.
    char color = grid[i][j];
    assert( color > EMPTY_CELL && color < COLOR_INVALID );
    queue< ii > q;
    q.push( { i, j } );

    vector< ii > neighborCells;
    while( !q.empty() ) {
        neighborCells.clear();
        fillNeighbors( q.front(), neighborCells );
        ++N; q.pop();
        for( auto & [ u, v ] : neighborCells ) {
            if( grid[u][v] == color ) {
                grid[u][v] = EMPTY_CELL;
                q.push( { u, v } );
            }
        }
    }
    //If the cluster size is at least 2, then we have to remove the
    //ball at location (i, j), and compress the game board by applying
    //shift left and shift right operations.
    if( N > 1 ) {
        grid[i][j] = EMPTY_CELL;
        shiftDown();
        shiftLeft();
        return N;
    }
    return 0;
}

void SameGame::shiftDown() {
    for( int col = 0; col < cols(); ++col ) {
        //Initialize the read and write index.
        int r, w;
        r = w = rows() - 1;
        while( r >= 0 ) {
            if( grid[r][col] != EMPTY_CELL) {
                if( r != w )
                    swap( grid[r][col], grid[w][col] );
                --w;
            }
            --r;
        }
    }
}

bool SameGame::isEmptyColumn( int colNum ) {
    for( int row = 0; row < rows(); ++row ) {
        if( grid[row][colNum] != EMPTY_CELL )
            return false;
    }
    return true;
}

void SameGame::shiftLeft() {
    int c = 0;
    unordered_set< int > emptyColumns;
    for( int col = 0; col < cols(); ++col ) {
        if( isEmptyColumn( col ) )
            emptyColumns.insert( col );
    }
    while( c < cols() ) {
        if( emptyColumns.count( c ) ) {
            int nextNonEmptyCol = c + 1;
            while( nextNonEmptyCol < cols() && emptyColumns.count( nextNonEmptyCol ) )
                ++nextNonEmptyCol;
            //There is no non-empty column to apply shift left operation.
            if( nextNonEmptyCol == cols() )
                return;
            //Swap the empty column with the non-empty column. Update the emptyColumns
            //dictionary.
            for( int i = 0; i < rows(); ++i )
                swap( grid[i][c], grid[i][nextNonEmptyCol] );
            emptyColumns.insert( nextNonEmptyCol );
        }
        ++c;
    }
}

ii SameGameDriver::largestClusterFirst( SameGame & game ) {
    int score = 0, remainingBalls;
    bool finalStateReached = false;
    while( !finalStateReached ) {
        int bestSoFar = 0;
        int r, c; //row and column of the left most and bottom most cell of the best cluster.
        remainingBalls = 0;

        vector< vector< bool > > visited( game.rows(), vector< bool >( game.cols(), false ) );
        for( int i = 0; i < game.rows(); ++i ) {
            for( int j = 0; j < game.cols(); ++j) {
                char color = game.grid[i][j];
                if( color == SameGame::EMPTY_CELL )
                    continue;
                ++remainingBalls;
                if( visited[i][j] )
                    continue;
                //At (i,j) we have a colored ball which has not yet been visited. We do a
                //breadth first search from (i,j).
                int N = 0; //Cluster size.
                //Keep track of the left most column and bottom most row for
                //the current cluster so that we can update "r" and "c".
                int bottomRow = i, leftCol = j;

                queue< ii > q;
                q.push( { i, j } );

                vector< ii > neighbors;
                while( !q.empty() ) {
                    ii curCell = q.front(); q.pop();
                    auto & [u, v] = curCell;
                    if( game.grid[u][v] != color || visited[u][v] )
                        continue;
                    ++N;
                    visited[u][v] = true;
                    //Update bottomRow, leftCol if (u, v) is a better choice.
                    if( v < leftCol ) {
                        bottomRow = u; leftCol = v;
                    } else if( v == leftCol && u >  bottomRow ) {
                        bottomRow = u; leftCol = v;
                    }
                    neighbors.clear();
                    game.fillNeighbors( curCell, neighbors );
                    for( auto & [ x, y ] : neighbors ) {
                        q.push( { x, y } );
                    }
                }
                if( N > bestSoFar) {
                    bestSoFar = N;
                    r = bottomRow; c = leftCol;
                } else if( N == bestSoFar && leftCol < c ) {
                    r = bottomRow; c = leftCol;
                } else if( N == bestSoFar && leftCol == c && bottomRow > r ) {
                    r = bottomRow; c = leftCol;
                }
            }
        }
        if( bestSoFar > 1 ) {
            //We make the move identified by (r, c).
            game.makeMove( r, c );
            score += ( bestSoFar - 2) * ( bestSoFar - 2);
        } else
            finalStateReached = true;
    }
    if( remainingBalls == 0 )
        score += 1000;
    return { score, remainingBalls } ;
}
