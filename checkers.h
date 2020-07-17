#ifndef __CHECKERS_H__
#define __CHECKERS_H__

#include "common.h"
#include "bitvector.h"

class CheckersBoard : public RectangularGridProperties {
public:
    set< ii > whitePos, blackPos;
public:
    CheckersBoard( int N, const vector< ii > & whitePos, const vector< ii > & blackPos ) :
                   RectangularGridProperties( N, N ) {
        for( const ii& pos : whitePos ) {
            boundsCheckAndAssert( pos );
            this->whitePos.insert( pos );
        }
        for( const ii& pos : blackPos ) {
            boundsCheckAndAssert( pos );
            //Ensure that there is no attempt to place both pieces in the same
            //position.
            assert( this->whitePos.count( pos ) == 0 );
            this->blackPos.insert( pos );
        }
    }
};

class Checkers {
private:
    class State {
    public:
        int pieceNumber;
        ii curMovePos;
        BitVector bits;
    public:
        State( int pieceNumber, const ii & pos, int numberOfBits ) :
               pieceNumber( pieceNumber ),
               curMovePos( pos ), bits( numberOfBits ) {}
        bool operator < ( const State & other ) const {
            return pieceNumber < other.pieceNumber || curMovePos < other.curMovePos ||
                   bits < other.bits;
        }
    };
public:
    static bool canWhiteWinInSingleTurn( const CheckersBoard & board ) {
        int blackPieceCount = board.blackPos.size();
        if( blackPieceCount == 0 )
            return true;

        int index = 0;
        map< ii, int > blackPiecesIndex;
        for( const ii& pos : board.blackPos )
            blackPiecesIndex[ pos ] = index++;

        queue< State > q;
        set< State > visited;

        int pieceNumber = 0;
        map< ii, int > whitePiecesIndex;
        for( const ii& whitePos : board.whitePos ) {
            whitePiecesIndex[ whitePos ] = pieceNumber;
            q.push( State( pieceNumber++, whitePos, blackPieceCount ) );
        }

        while( !q.empty() ) {
            State curState = q.front(); q.pop();
            if( curState.bits.all() ) {
                return true;
            }
            if( visited.count( curState ) )
                continue;
            else
                visited.insert( curState );

            State newState = curState;
            vector< ii > neighbors;
            board.fillDiagonalNeighbors( curState.curMovePos, neighbors );
            for( ii newPos : neighbors ) {
                //If there is no black piece in the diagonal position, then continue.
                if( board.blackPos.count( newPos ) == 0 ) continue;
                //There is a black piece in the diagonal position. Get it's bit number.
                assert( blackPiecesIndex.count( newPos) );
                int bitNumber = blackPiecesIndex[ newPos ];
                //If we have already crossed this particular black piece, then continue.
                if( curState.bits.isSet( bitNumber ) ) continue;
                int deltaX = newPos.first - curState.curMovePos.first;
                int deltaY = newPos.second - curState.curMovePos.second;
                newPos.first += deltaX; newPos.second += deltaY;

                bool canMoveToNewPos = board.isIndexWithinBounds( newPos );
                //If there is another white piece in the new position, then we can not
                //move.
                if( canMoveToNewPos && board.whitePos.count( newPos ) ) {
                    assert( whitePiecesIndex.count( newPos ) );
                    if( newState.pieceNumber != whitePiecesIndex[ newPos ] )
                        canMoveToNewPos = false;
                }
                if( canMoveToNewPos ) {
                    newState.curMovePos = newPos;
                    newState.bits.set( bitNumber );
                    q.push( newState );
                }
            }
        }
        return false;
    }
};

class CheckersTest {
public:
    static void runTest() {
        canWhiteWinInSingleTurnTest();
    }
private:
    static void canWhiteWinInSingleTurnTest() {
        int N = 10;

        //Test case 1.
        vector< ii > whitePos = {
            {3,8}, {4,5}, {9,0},
        };
        vector< ii > blackPos = {
            {2,3}, {2,5}, {4,1}, {4,3}, {4,7}, {6,1}, {6,3}, {6,7}, {8,1}, {8,3}, {8,5},
        };
        CheckersBoard board( N, whitePos, blackPos );
        bool possible = Checkers::canWhiteWinInSingleTurn( board );
        assert( possible );

        //Test case 2.
        whitePos = { {3,6}, {3,8}, {9,0} };
        CheckersBoard board2( N, whitePos, blackPos );
        possible = Checkers::canWhiteWinInSingleTurn( board2 );
        assert( !possible );

        //Test case 3.
        whitePos = { {3,8}, {4,5}, {9,6} };
        blackPos = { {2,3}, {2,5}, {4,1}, {4,3}, {4,7}, {6,1}, {6,5}, {6,7}, {8,1}, {8,3}, {8,5} };
        CheckersBoard board3( N, whitePos, blackPos );
        possible = Checkers::canWhiteWinInSingleTurn( board3 );
        assert( !possible );
    }
};

#endif // __CHECKERS_H__
