#include "sequenceapp.h"

class Box {
public:
    unsigned int x, y, z;
    int index;
public:
    Box( unsigned int a, unsigned int b, unsigned int c, bool sortDimensions=true ) :
         x(a), y(b), z(c), index(0) {
        if( sortDimensions ) {
            //Reorder so that x <= y <= z.
            if( x > y ) swap( x, y );
            if( y > z ) swap( y, z );
            if( x > y ) swap( x, y );
        }
    }

    bool operator < ( const Box & other ) const {
        return x < other.x && y < other.y && z < other.z ;
    }

    uint64_t volume() const { return x * y * z; }
};

class Envelope {
public:
    unsigned int width, height;
    int index;
public:
    Envelope( unsigned int w, unsigned int h, bool sortDimensions=true ) :
              width( w ), height( h ) {
        if( sortDimensions && width > height ) {
            swap( width, height );
        }
    }

    bool operator < ( const Envelope& other ) const {
        return width < other.width && height < other.height ;
    }

    uint64_t area() const { return width * height; }
};

int BoxNesting::largestNestedBoxSetSize( const vector< Box > & boxes, vector< int > & result ) {
    vector< Box > sortedBoxes( boxes.begin(), boxes.end() );
    for ( int i = 0; i < sortedBoxes.size(); ++i )
        sortedBoxes[i].index = i;

    //Sort the boxes according to their volume.
    sort( sortedBoxes.begin(), sortedBoxes.end(),
          []( const Box & a, const Box & b )
          {
              return a.volume() < b.volume();
          } );

    ii sol = Sequence::genericLongestIncreasingSubsequence( sortedBoxes, result );
    auto & [ maxLen, disjointSequenceCount ] = sol;
    //Translate index in the result vector so that they are applicable
    //to the original unsorted boxes.
    for( int i = 0; i < result.size(); ++i )
        result[i] = sortedBoxes[ result[i] ].index;
    return maxLen;
}

ii EnvelopeNesting::largestNestedEnvelopeSetSize( const vector< Envelope > & envelopes,
                                                  vector< int > & result ) {
    vector< Envelope > sortedEnvelopes( envelopes.begin(), envelopes.end() );
    for( int i = 0; i < sortedEnvelopes.size(); ++i )
        sortedEnvelopes[i].index = i;

    //Sort the envelopes according to their area.
    sort( sortedEnvelopes.begin(), sortedEnvelopes.end(),
          []( const Envelope & a, const Envelope & b )
          {
              return a.area() < b.area();
          } );

    ii sol = Sequence::genericLongestIncreasingSubsequence( sortedEnvelopes, result );
    //Translate index in the result vector so that they are applicable
    //to the original unsorted envelopes.
    for( int i = 0; i < result.size(); ++i )
        result[i] = sortedEnvelopes[ result[i] ].index;
    return sol;
}

void BoxNesting::runTest() {

    vector< vector< Box > > testcases = {
        {
            {145, 472, 812}, {827, 133, 549}, {381, 371, 900}, {271, 389, 128},
            {718, 217, 491}
        },
        {
            {432, 123, 139}, {942, 844, 783}, {481, 487, 577}, {677, 581, 701}
        },
        {
            {10, 20, 100}, {25, 5, 500}, {10, 80, 50}, {1000, 800, 1}
        },
        {
            {3, 9, 9}, {1, 4, 10}, {5, 10, 11}, {3, 9, 3}, {1, 5, 3}, {7, 12, 1}
        }
    };

    for( auto & boxSet : testcases ) {
        vector< int > result;
        printf( "Largest nested box set size = %d\n", largestNestedBoxSetSize( boxSet, result ) );
        for( int i = 0; i < result.size(); ++i ) {
            int index = result[i];
            printf( "[ %2d, %2d, %2d ] ", boxSet[index].x, boxSet[index].y, boxSet[index].z );
        }
        printf( "\n" );
    }
}

void EnvelopeNesting::runTest() {

    vector< vector< Envelope > > testcases = {
        {
            {5,4}, {6,4}, {6,7}, {2,3}
        }
    };

    for( auto & envelopeSet : testcases ) {
        vector< int > result;
        ii sol = largestNestedEnvelopeSetSize( envelopeSet, result );
        auto & [ maxLen, disjointSequenceCount ] = sol;
        printf( "Largest nested envelope set size = %d, disjoint groups = %d\n",
                maxLen, disjointSequenceCount );
        for( int i = 0; i < result.size(); ++i ) {
            int index = result[i];
            printf( "[ %2d, %2d ] ", envelopeSet[index].width, envelopeSet[index].height );
        }
        printf( "\n" );
    }
}
