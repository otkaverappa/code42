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
        string result = UVA_11995::containerType( inputVec );
        if( result != expectedResult ) {
            printf( "Expected [%s] Obtained [%s]\n", expectedResult.c_str(), result.c_str() );
            assert( false );
        }
    }
}

void UVA_10158::runTest() {
}

void UVA_12049::runTest() {
}

void UVA_978::runTest() {
}

void UVA_599::runTest() {
    vector< int > vertices = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N' };
    vector< ii > edges = {
        { 'A', 'B' }, { 'B', 'C' }, { 'B', 'D' }, { 'D', 'E' }, { 'E', 'F' }, { 'B', 'G' },
        { 'G', 'H' }, { 'G', 'I' }, { 'J', 'K' }, { 'K', 'L' }, { 'K', 'M' },
    };
    ii result = UVA_599::numberOfTreesAndAcorns( edges, vertices );
    printf( "Trees = %d Acorns = %d\n", result.first, result.second );

    vertices = { 'A', 'B', 'C', 'D', 'F' };
    edges = { { 'A', 'B' }, { 'A', 'C' }, { 'C', 'F' } };
    result = UVA_599::numberOfTreesAndAcorns( edges, vertices );
    printf( "Trees = %d Acorns = %d\n", result.first, result.second );
}

void UVA::runTest() {
    UVA_599::runTest();
    UVA_10158::runTest();
    UVA_11995::runTest();
    UVA_978::runTest();
    UVA_12049::runTest();
}
