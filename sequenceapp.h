#ifndef __SEQUENCE_APP_H__
#define __SEQUENCE_APP_H__

#include "common.h"
#include "sequence.h"

class Box;

class BoxNesting {
public:
    static int largestNestedBoxSetSize( const vector< Box > & boxes );
public:
    static void runTest();
};

class Envelope;

class EnvelopeNesting {
public:
    static int largestNestedEnvelopeSetSize( const vector< Envelope > & envelopes );
public:
    static void runTest();
};

#endif // __SEQUENCE_APP_H__
