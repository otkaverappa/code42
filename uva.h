#ifndef __UVA_H__
#define __UVA_H__

#include "common.h"

class UVA {
public:
    static void runTest();
};

class UVA_11995 {
public:
    static string processInputSequence( const vector< ii > & input );
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

#endif // __UVA_H__
