#ifndef __RACETRACK_H__
#define __RACETRACK_H__

#include "common.h"

class RaceTrack {
public:
    typedef pair< ii, ii > LocationAndVelocity;
private:
    const Bitmap& bitmap;
    set< ii > startPos, endPos;
public:
    RaceTrack( const Bitmap& bitmap, const vector< ii > & startPos,
               const vector< ii > & endPos ) : bitmap( bitmap ) {
        for( const auto & pos : startPos ) {
            if( bitmap.isIndexWithinBounds( pos ) )
                this->startPos.insert( pos );
        }
        for( const auto & pos : endPos ) {
            if( bitmap.isIndexWithinBounds( pos ) )
                this->endPos.insert( pos );
        }
    }

    int minimumSteps( vector< LocationAndVelocity > & allSteps ) {

        map< LocationAndVelocity, LocationAndVelocity > previousLV;
        const ii initialVelocity = {0, 0};
        int steps = 0;
        bool finishLineReached = false;
        queue< LocationAndVelocity > q;

        for( const ii& startLoc : startPos ) {
            LocationAndVelocity lv( startLoc, initialVelocity );
            q.push( lv );
            previousLV[ lv ] = lv;
        }

        LocationAndVelocity curLV;
        while( !q.empty() ) {
            int N = q.size();
            while( N-- ) {
                curLV = q.front(); q.pop();
                auto & [ curLoc, curVelocity ] = curLV;
                if( endPos.count( curLoc ) > 0 ) {
                    finishLineReached = true;
                    goto endOfSearch;
                }
                auto & [ x, y ] = curVelocity;
                for( int u = -1; u <= 1; ++u ) {
                    for( int v = -1; v <= 1; ++v ) {
                        //{x + u, y + v} is the possible new velocity.
                        ii newLoc = { curLoc.first + x + u, curLoc.second + y + v };
                        //Ensure that the new location is within the race track.
                        if( !bitmap.isIndexWithinBounds( newLoc ) ||
                            !bitmap.isSet( newLoc ) ) continue;
                        LocationAndVelocity newLV( newLoc, { x + u, y + v } );
                        //Ensure that the (position, velocity) is not already processed.
                        if( previousLV.count( newLV ) > 0 ) continue;

                        previousLV[ newLV ] = curLV;
                        q.push( newLV );
                    }
                }
            }
            ++steps;
        }
        endOfSearch:
        if( finishLineReached ) {
            while( true ) {
                LocationAndVelocity prevLV;

                allSteps.push_back( curLV );
                prevLV = previousLV[ curLV ];
                if( prevLV == curLV ) break;
                curLV = prevLV;
            }
        } else steps = 0;

        return steps;
    }
};

class RaceTrackTest {
public:
    static void runTest() {
        vector< string > raceTrack = {
            "0000000000000000000000000",
            "0000000000111100000000000",
            "0000000001111110000001111",
            "0000000011111111000011111",
            "0000000011110111101111111",
            "0000000011100111111111111",
            "0000000011000001111100000",
            "0000000011000000111000000",
            "0000000011000000000000000",
            "0000000011100000000000000",
            "0000000011110000000000000",
            "0000000011111000000000000",
            "0000000011111100000000000",
            "0000000011111110000000000",
            "0000000011001110000000000",
            "0000000011001111000000000",
            "0000000111000111000000000",
            "0000000111100111000000000",
            "1000000111111110000000000",
            "1100001111111110000000000",
            "1111111111111100000000000",
            "1111111111110000000000000",
            "1111111111000000000000000",
            "0001111000000000000000000",
            "0000000000000000000000000",
        };
        Bitmap bitmap( raceTrack );
        vector< ii > startPos = { {18,0}, {19,0}, {20,0}, {21,0}, {22,0} };
        vector< ii > endPos = { {2,24}, {3,24}, {4,24}, {5,24}, {6,24} };

        RaceTrack track( bitmap, startPos, endPos );

        vector< RaceTrack::LocationAndVelocity > allSteps;
        track.minimumSteps( allSteps );
        for( int i = allSteps.size(); i > 0; --i ) {
            auto & [ pos, velocity ] = allSteps[ i - 1 ];
            //Translate the position and velocity so that we display the cartesian
            //co-ordinates of the race track.
            printf( "[ %2d, %2d ] (%2d,%2d)\n", pos.second + 1, bitmap.rows() - pos.first,
                    velocity.second, - velocity.first );
        }
    }
};

#endif // __RACETRACK_H__
