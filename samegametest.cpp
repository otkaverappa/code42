#include "samegame.h"

void SameGameTest::printGameState( const SameGame & game ) {
    const string table = ".RGB*" ;
    for( int i = 0; i < game.rows(); ++i ) {
        for( int j = 0; j < game.cols(); ++j ) {
            printf( "%c", table[ game.grid[i][j] ] );
        }
        printf( "\n" );
    }
}

#define R SameGame::COLOR_RED,
#define G SameGame::COLOR_GREEN,
#define B SameGame::COLOR_BLUE,
#define _ SameGame::EMPTY_CELL,
void SameGameTest::classSameGameDriverTest() {

    vector< char > data1 = {
        R G G B B G G R B R R G G B G
        R B G R B G R B G R B G R B G
        R R R R G B B B R G G R B B B
        G G R G B G G B R R G G G B G
        G B G G R R R R R B G G R R R
        B B B B B B B B B B B B B B B
        B B B B B B B B B B B B B B B
        R R R R R R R R R R R R R R R
        R R R R R R G G G G R R R R R
        G G G G G G G G G G G G G G G
    };
    vector< char > data2 = {
        R R R R R R R R R R R R R R R
        R R R R R R R R R R R R R R R
        G G G G G G G G G G G G G G G
        G G G G G G G G G G G G G G G
        B B B B B B B B B B B B B B B
        B B B B B B B B B B B B B B B
        R R R R R R R R R R R R R R R
        R R R R R R R R R R R R R R R
        G G G G G G G G G G G G G G G
        G G G G G G G G G G G G G G G
    };
    vector< char > data3 = {
        R B G R B G R B G R B G R B G
        B G R B G R B G R B G R B G R
        G R B G R B G R B G R B G R B
        R B G R B G R B G R B G R B G
        B G R B G R B G R B G R B G R
        G R B G R B G R B G R B G R B
        R B G R B G R B G R B G R B G
        B G R B G R B G R B G R B G R
        G R B G R B G R B G R B G R B
        R B G R B G R B G R B G R B G
    };

    int rows = 10, cols = 15;

    SameGame game1( rows, cols );
    game1.setGameState( data1 );
    printGameState( game1 );
    ii result1 = SameGameDriver::largestClusterFirst( game1 );
    printf( "Score = %d Remaining Balls = %d\n", result1.first, result1.second );

    SameGame game2( rows, cols );
    game2.setGameState( data2 );
    printGameState( game2 );
    ii result2 = SameGameDriver::largestClusterFirst( game2 );
    printf( "Score = %d Remaining Balls = %d\n", result2.first, result2.second );

    SameGame game3( rows, cols );
    game3.setGameState( data3 );
    printGameState( game3 );
    ii result3 = SameGameDriver::largestClusterFirst( game3 );
    printf( "Score = %d Remaining Balls = %d\n", result3.first, result3.second );
}

void SameGameTest::classSameGameTest() {

    vector< char > initialState = {
        R R R G B B
        G R R R R R
        R R R B G R
        G G R R G R
        B B R B G G
    };
    vector< char > intermediateState = {
        _ _ _ G B B
        B _ _ _ _ _
        _ _ _ B G _
        G G _ _ G _
        B B _ B G G
    };
    vector< char > afterShiftDown = {
        _ _ _ _ _ _
        _ _ _ _ B _
        B _ _ G G _
        G G _ B G B
        B B _ B G G
    };
    vector< char > afterShiftLeft = {
        _ _ _ _ _ _
        _ _ _ B _ _
        B _ G G _ _
        G G B G B _
        B B B G G _
    };

    int M = 5, N = 6;
    SameGame game( M, N );
    //Set the initial state of the game, and ensure that the set operation
    //is successful.
    assert( game.setGameState( initialState ) );
    assert( verifyCurrentGameState( game, initialState ) );

    //Set the intermediate state, and ensure that the set operation is
    //successful.
    assert( game.setGameState( intermediateState ) );
    assert( verifyCurrentGameState( game, intermediateState ) );
    //Apply shiftDown and verify the game state.
    game.shiftDown();
    assert( verifyCurrentGameState( game, afterShiftDown ) );
    //Apply shiftLeft and verify the game state.
    game.shiftLeft();
    assert( verifyCurrentGameState( game, afterShiftLeft ) );
    //Verify that the game is not yet in the final state.

    int i = M-1, j = 0;
    //Verify that, if we make a move at (i, j) 3 times, the game reaches the final
    //state.
    for( int k = 1; k <= 3; ++k ) {
        printGameState( game );
        assert( game.makeMove( i, j ) );
    }
}
#undef _
#undef B
#undef G
#undef R

bool SameGameTest::verifyCurrentGameState( const SameGame & game,
                                           const vector< char > & rowData ) {
    if( rowData.size() < game.rows() * game.cols() )
        return false;
    int index = 0;
    for( int i = 0; i < game.rows(); ++i ) {
        for( int j = 0; j < game.cols(); ++j ) {
            char cell = rowData[index++];
            assert( cell >= SameGame::EMPTY_CELL && cell < SameGame::COLOR_INVALID );
            if( game.grid[i][j] != cell )
                return false;
        }
    }
    return true;
}
