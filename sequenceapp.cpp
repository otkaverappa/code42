#include "sequenceapp.h"

class Box {
public:
    unsigned int x, y, z;
public:
    Box( unsigned int a, unsigned int b, unsigned int c ) : x(a), y(b), z(c) {
        //Reorder so that x <= y <= z.
        if( x > y ) swap( x, y );
        if( y > z ) swap( y, z );
        if( x > y ) swap( x, y );
    }

    bool operator < ( const Box & other ) const {
        return x < other.x && y < other.y && z < other.z ;
    }

    uint64_t volume() const { return x * y * z; }
};

int BoxNesting::largestNestedBoxSetSize( const vector< Box > & boxes ) {
    vector< int > result;
    vector< Box > sortedBoxes( boxes.begin(), boxes.end() );

    //Sort the boxes according to their volume.
    sort( sortedBoxes.begin(), sortedBoxes.end(),
          []( const Box & a, const Box & b )
          {
              return a.volume() < b.volume();
          } );

    int maxLen = Sequence::genericLongestIncreasingSubsequence( sortedBoxes, result );
    return maxLen;
}

void BoxNesting::runTest() {

    vector< Box > boxes1 = {
        {145, 472, 812}, {827, 133, 549}, {381, 371, 900}, {271, 389, 128},
        {718, 217, 491},
    };
    printf( "Largest nested box set size = %d\n", largestNestedBoxSetSize( boxes1 ) );

    vector< Box > boxes2 = {
        {432, 123, 139}, {942, 844, 783}, {481, 487, 577}, {677, 581, 701},
    };
    printf( "Largest nested box set size = %d\n", largestNestedBoxSetSize( boxes2 ) );

    vector< Box > boxes3 = {
        {10, 20, 100}, {25, 5, 500}, {10, 80, 50}, {1000, 800, 1},
    };
    printf( "Largest nested box set size = %d\n", largestNestedBoxSetSize( boxes3 ) );
}
