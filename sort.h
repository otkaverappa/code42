#ifndef __SORT_H__
#define __SORT_H__

#include "common.h"

class Sort {
    public:
    static int selectRank( const vector< int > &a, const vector< int > &b, int k ) {
        int M = a.size(), N = b.size();
        assert( k >= 1 && k <= M + N );
        //return selectRankRecursive( k, a, 0, M - 1, b, 0, N - 1 );
    }
    static bool binarySearch( const vector< int > &v, int key, int& index ) {
        int i = 0, j = v.size();
        if( j == 0 || key < v[0] || key > v[j - 1] )
            return false;
        --j;
        while( i <= j ) {
            int m = i + (j - i)/2;
            if( key < v[m] ) j = m - 1;
            else if( key > v[m] ) i = m + 1;
            else {
                index = m;
                return true;
            }
        }
        return false;
    }
    static bool lowerBound( const vector< int > &v, int key, int& index ) {
        int i = 0, j = v.size();
        if( j == 0 || key < v[0] ) {
            index = j;
            return false;
        }
        --j;
        while( i < j ) {
            int m = i + (j - i)/2;
            if( key <= v[m] ) j = m;
            else i = m + 1;
        }
        assert( i == j );
        index = i;
        return key == v[i];
    }
    static bool upperBound( const vector< int > &v, int key, int& index ) {
        int i = 0, j = v.size();
        if( j == 0 || key > v[j - 1] ) {
            index = j;
            return false;
        }
        --j;
        while( i < j ) {
            int m = i + (j - i)/2 + 1;
            if( key >= v[m] ) i = m;
            else j = m - 1;
        }
        assert( i == j );
        index = i + 1;
        return key == v[i];
    }
    static bool equalRange( const vector< int > &v, int key, ii& index ) {
        bool l = lowerBound( v, key, index.first );
        bool r = upperBound( v, key, index.second );
        if( l ) {
            assert( r );
            return true;
        }
        return false;
    }
    private:
};

class SortTest {
public:
    static void runTest() {
        //selectRankTest();
        binarySearchTest();
        binarySearchBoundsTest();
    }
private:
    static void binarySearchTest() {
        int M = 1000;
        vector< int > v( M );

        int limit = 10000;
        for( int i = 0; i < M; ++i )
            v[i] = rand() % limit;
        sort( v.begin(), v.end() );

        int index;
        for( int key = 0; key < limit; ++key ) {
            if( Sort::binarySearch( v, key, index ) )
                assert( v[index] == key );
            else {
                assert( ! binary_search( v.begin(), v.end(), key ) );
            }
        }
    }
    static void binarySearchBoundsTest() {
        int M = 1000;
        vector< int > v( M );

        int limit = 50; //Ensure that the vector v will have many duplicates.
        for( int i = 0; i < limit; ++i )
            v[i] = i;
        for( int i = limit; i < M; ++i )
            v[i] = rand() % limit;
        sort( v.begin(), v.end() );

        for( int key = 0; key < limit; ++key ) {
            ii index = { -1, -1 };
            bool found = Sort::equalRange( v, key, index );
            printf( "Search element = %d, found = %s [ %d, %d ]\n", key, found ? "true" : "false",
                    index.first, index.second );
            assert( found );
            assert( index.first >= 0 && index.first < M &&
                    index.second > 0 && index.second <= M );
            auto range = equal_range( v.begin(), v.end(), key );
            assert( range.second - range.first == ( index.second - index.first ) );
        }
        for( int key = -10; key < 0; ++key ) {
            int index;
            Sort::lowerBound( v, key, index );
        }
    }
    static void selectRankTest() {
        int M = 30, N = 20;
        vector< int > a( M ), b ( N ), c( M + N );

        for( int i = 0; i < M + N; ++i ) c[i] = i + 1;
        random_shuffle( c.begin(), c.end() );
        for( int i = 0; i < M + N; ++i ) {
            if( i < M )
                a[i] = c[i];
            else
                b[i - M] = c[i];
        }
        sort( a.begin(), a.end() );
        sort( b.begin(), b.end() );

        for( int r = 1; r <= M + N; ++r ) {
            //assert( Sort::selectRank( r, a, b ) == r );
        }
    }
};

#endif //__SORT_H__
