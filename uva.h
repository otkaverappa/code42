#ifndef __UVA_H__
#define __UVA_H__

#include "common.h"
#include "unionfind.h"

class UVA {
public:
    static void runTest();
};

class UVA_599 {
public:
    static ii numberOfTreesAndAcorns( const vector< ii > & edges, const vector< int > & vertices );
public:
    static void runTest();
};

class UVA_10158 {
public:
    UVA_10158( int N );

    bool setFriends( int x, int y );
    bool setEnemies( int x, int y );
    bool areFriends( int x, int y );
    bool areEnemies( int x, int y );
private:
public:
    static void runTest();
};

class UVA_11995 {
public:
    static string containerType( const vector< ii > & input );
private:
    enum { OPERATION_INSERT = 1, OPERATION_REMOVE = 2, };
    enum { CONTAINER_STACK = 0,
           CONTAINER_QUEUE = 1,
           CONTAINER_PRIORITY_QUEUE = 2,
           CONTAINER_NONE = 3,
           CONTAINER_MULTIPLE = 4 };
    static const inline vector< string > tags = {
        "Stack", "Queue", "Priority Queue", "None", " or "
    };
public:
    static void runTest();
};

class UVA_12049 {
public:
    static int minimumDeletions( const vector< int > & list1, const vector< int > & list2 );
public:
    static void runTest();
};

class UVA_978 {
public:
    static string lemmingsBattle( const vector< int > & blueArmy, const vector< int > & greenArmy,
                               int battleRounds, vector< int > & remainingArmy );
private:
    enum { RESULT_TIE = 0, BLUE_LEMMING = 1, GREEN_LEMMING = 2, };
    static const inline vector< string > tags = {
        "None", "Blue Lemming", "Green Lemming"
    };
public:
    static void runTest();
};

#endif // __UVA_H__
