#ifndef __MULTIVECTOR_H__
#define __MULTIVECTOR_H__

#include "common.h"

template <typename T>
class Multivector {
private:
    vector< T > v;
    vector< int > dimVector;

    bool isIndexWithinBounds( const vector< int > & dim,
                              int * index=nullptr ) {
        int idx = 0;
        if( dim.size() != dimVector.size() ) return false;
        for( int i = 0; i < dim.size(); ++i ) {
            if( dim[i] < 0 || dim[i] >= dimVector[i] )
                return false;
            idx = idx * dimVector[i] + dim[i];
        }
        if( index != nullptr ) *index = idx;
        return true;
    }
public:
    Multivector( initializer_list< int > dim, T initialValue=T() ) {
        int total = 1;
        for( auto d : dim ) {
            assert( d > 0 );
            dimVector.push_back( d );
            total = total * d;
        }
        v.resize( total, initialValue );
    }

    bool get( const vector< int > & dim, T & data ) {
        int index;
        if( isIndexWithinBounds( dim, &index ) ) {
                data = v[index];
                return true;
        }
        return false;
    }

    bool set( const vector< int > & dim, T data ) {
        int index;
        if( isIndexWithinBounds( dim, &index ) ) {
            v[index] = data;
            return true;
        }
        return false;
    }

    size_t size() {
        return v.size();
    }
};

class MultivectorTest {
public:
    static void runTest() {
        const int M = 10, N = 20, U = 30, V = 40;
        int initValue, testValue;

        initValue = 0; testValue = 42;
        Multivector< int > vec( { M }, initValue );
        assert( vec.size() == M );
        for( int i = 0; i < M; ++i ) {
            int element;
            assert( vec.get( { i }, element ) && element == initValue );
            assert( vec.set( { i }, testValue ) );
            assert( vec.get( { i }, element ) && element == testValue );
        }

        initValue = 0xFF; testValue = 1;
        Multivector< int > matrix( { M, N }, initValue );
        assert( matrix.size() == M * N );
        for( int i = 0; i < M; ++i ) {
            for( int j = 0; j < N; ++j ) {
                int element;
                assert( matrix.get( { i, j }, element ) && element == initValue );
                assert( matrix.set( { i, j }, testValue ) );
                assert( matrix.get( { i, j }, element ) && element == testValue );
            }
        }

        initValue = 0xABCD; testValue = ~initValue;
        Multivector< int > tensor ( { M, N, U, V }, initValue );
        assert( tensor.size() == M * N * U * V );
        for( int i = 0; i < M; ++i ) {
            for( int j = 0; j < N; ++j ) {
                for( int u = 0; u < U; ++u ) {
                    for( int v = 0; v < V; ++v ) {
                        int element;
                        assert( tensor.get( { i, j, u, v }, element ) && element == initValue );
                        assert( tensor.set( { i, j, u, v }, testValue ) );
                        assert( tensor.get( { i, j, u, v }, element ) && element == testValue );
                    }
                }
            }
        }
    }
};

#endif // __MULTIVECTOR_H__
