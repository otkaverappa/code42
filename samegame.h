#ifndef __SAME_GAME_H__
#define __SAME_GAME_H__

#include "common.h"

class SameGame : public RectangularGridProperties {
public:
    SameGame( int rows, int cols ) :
              RectangularGridProperties( rows, cols ),
              grid( rows, vector< char > ( cols, EMPTY_CELL ) ) {
    }

    //Fill the game board with contents from the vector "rowData".
    //"rowData" should have at least (rows*cols) elements and each element
    //should be a valid color.
    bool setGameState( const vector< char > & rowData );

    //If (i, j) is a valid move, then the move is made, and the number of balls
    //removed from the board is returned. If the move is invalid, the return value is 0.
    int makeMove( int i, int j );

    //Is the game board in its final state? (i.e. no legal moves should be possible
    //from the final state.)
    //Returns false if the game board is not in its final state. Otherwise, returns true
    //and populates "remainingBalls" with the number of remaining balls in the game board.
    bool isFinalState( int * remainingBalls=nullptr ) const;
private:
    vector< vector< char > > grid;

    //When a move is made, we have to rearrange the grid so that
    //the balls are shifted down and then shifted left.
    void shiftDown();
    void shiftLeft();
    bool isEmptyColumn( int colNum );
public:
    enum { EMPTY_CELL  = 0,
           COLOR_RED   = 1,
           COLOR_GREEN = 2,
           COLOR_BLUE  = 3,
           COLOR_INVALID = 4
         };
    //Tests for the class "SameGame" will be implemented by the class "SameGameTest".
    friend class SameGameTest;
    //SameGameDriver implements the functionality of choosing a move.
    friend class SameGameDriver;
};

class SameGameDriver {
public:
    static ii largestClusterFirst( SameGame & game );
};

class SameGameTest {
public:
    static void runTest() {
        classSameGameTest();
        classSameGameDriverTest();
    }
private:
    static void classSameGameTest();
    static void classSameGameDriverTest();
private:
    static bool verifyCurrentGameState( const SameGame & game, const vector< char > & rowData );
    static void printGameState( const SameGame & game );
};

#endif // __SAME_GAME_H__
