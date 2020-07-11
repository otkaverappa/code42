#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <cfloat>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>

#include "stdio.h"

using namespace std;

typedef pair< int, int > ii;

class RectangularGridProperties {
protected:
    int M, N;
public:
    RectangularGridProperties( int rows, int cols ) : M( rows ), N( cols ) {
        assert( M > 0 && N > 0 );
    }
    bool isIndexWithinBounds( int i, int j ) const {
        return ( i >= 0 && i < M && j >= 0 && j < N );
    }
    bool isIndexWithinBounds( const ii & index ) const {
        return isIndexWithinBounds( index.first, index.second );
    }
    void boundsCheckAndAssert( int i, int j ) const {
        assert( isIndexWithinBounds( i, j ) );
    }
    void boundsCheckAndAssert( const ii& index ) const {
        boundsCheckAndAssert( index.first, index.second );
    }
    void fillNeighbors( ii index, vector< ii > &neighbors,
                        bool includeDiagonal = false ) const {
        auto & [ i, j ] = index;
        if( !isIndexWithinBounds( i, j ) ) return;

        for( int u = -1; u <= 1; ++u ) {
            for( int v = -1; v <= 1; ++v ) {
                if( u == 0 && v == 0 ) continue;
                ii pos = { i + u, j + v };
                if( !isIndexWithinBounds( pos ) ) continue;
                if( u == 0 || v == 0 || includeDiagonal )
                    neighbors.push_back( pos );
            }
        }
    }
    int rows() const { return M; }
    int cols() const { return N; }
};

#endif //__COMMON_H__
