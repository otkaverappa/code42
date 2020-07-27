#include "uva.h"

void UVA_11995::runTest() {
    vector< pair< vector< ii >, string > > testcases = {
        { { {1,1}, {1,2}, {1,3}, {2,1}, {2,2}, {2,3} }, "Queue" },
        { { {1,1}, {1,2}, {1,3}, {2,3}, {2,2}, {2,1} }, "Stack or Priority Queue" },
        { { {1,1}, {2,2} }, "None" },
        { { {1,2}, {1,1}, {2,1}, {2,2} }, "Stack" },
        { { {1,2}, {1,5}, {1,1}, {1,3}, {2,5}, {1,4}, {2,4} }, "Priority Queue" },
        { { {1,1}, {2,1} }, "Stack or Queue or Priority Queue" },
        { { {1,3}, {1,2}, {1,1}, {2,3}, {2,2}, {2,1} }, "Queue or Priority Queue" },
     };

    for( auto & [ inputVec, expectedResult ] : testcases ) {
        string result = UVA_11995::processInputSequence( inputVec );
        if( result != expectedResult ) {
            printf( "Expected [%s] Obtained [%s]\n", expectedResult.c_str(), result.c_str() );
            assert( false );
        }
    }
}

void UVA::runTest() {
    UVA_11995::runTest();
}
