#ifndef __SEQUENCE_APP_H__
#define __SEQUENCE_APP_H__

#include "common.h"
#include "sequence.h"

class Box;

class BoxNesting {
public:
    static int largestNestedBoxSetSize( const vector< Box > & boxes, vector< int > & result );
public:
    static void runTest();
};

class BoxStacking {
public:
    static int largestStackedBoxSetHeight( const vector< Box > & boxes, vector< int > & result );
public:
    static void runTest();
};

class Envelope;

class EnvelopeNesting {
public:
    static ii largestNestedEnvelopeSetSize( const vector< Envelope > & envelopes,
                                            vector< int > & result );
public:
    static void runTest();
};

#endif // __SEQUENCE_APP_H__
