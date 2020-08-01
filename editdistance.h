#ifndef __EDIT_DISTANCE_H__
#define __EDIT_DISTANCE_H__

#include "common.h"

class EditDistance {
public:
    enum { OPERATION_INVALID = -1,

           OPERATION_COPY = 0,
           OPERATION_REPLACE = 1,
           OPERATION_INSERT = 2,
           OPERATION_DELETE = 3,
           OPERATION_TWIDDLE = 4,

           //How many type of operations are supported?
           OPERATION_TOTAL_COUNT = 5,
    };
    static const inline vector< int > defaultEditCosts = { 0, 3, 2, 2, 4 };
    static const inline vector< string > operationTag = {
        "Copy", "Replace", "Insert", "Delete", "Twiddle"
    };
public:
    static int calculateEditDistance( const string& src, const string& dst,
                                      vector< char > & editSequence,
                                      const vector< int > & editCosts = defaultEditCosts );
public:
    //Helper function to print the sequence of optimal edits.
    static void printEditSequence( const string& src, const string& dst,
                                   vector< char > & editSequence,
                                   const vector< int > & editCosts = defaultEditCosts );
    //Helper function for calculating the edit distance with unit weights for
    //copy, replace, insert, and delete operations.
    static int uniformEditDistance( const string& src, const string& dst );
};

class EditDistanceTest {
public:
    static void runTest() {
        editDistanceTest();
    }
private:
    static void editDistanceTest();
};

#endif // __EDIT_DISTANCE_H__
