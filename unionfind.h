#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__

#include "common.h"

template <typename T>
class UnionFind {
private:
    struct ElementNode {
        T data;
        int count;
        ElementNode* parent;

        ElementNode( const T& data ) : data( data ),
                                       count( 1 ), parent( nullptr ) {}
        void setParent( ElementNode* newParent ) { parent = newParent; }
    };
    map< T, ElementNode* > elementMap;
    int numSets;
public:
    UnionFind() : numSets( 0 ) {}

    template <typename Iterator>
    UnionFind( Iterator s, Iterator e ) : numSets( 0 ) {
        while( s != e ) {
            ElementNode * elem = new ElementNode( *s );
            elementMap[ *s ] = elem;
            ++numSets;
            ++s;
        }
    }
    bool addSet( const T& x ) {
        if( findElement( x ) != nullptr )
            return false;
        ElementNode* elem = new ElementNode( x );
        elementMap[ x ] = elem;
        ++numSets;
        return true;
    }
    int merge( const T& x, const T& y ) {
        ElementNode* n1 = findParent( x );
        ElementNode* n2 = findParent( y );
        int count;
        if( n1 == n2 )
            return n1->count;
        --numSets;
        if( n1->count >= n2->count ) {
            n2->parent = n1;
            count = n1->count += n2->count;
        } else {
            n1->parent = n2;
            count = n2->count += n1->count;
        }
        return count;
    }
    T find( const T& x ) {
        ElementNode* elem = findElement( x );
        if( elem == nullptr ) return x;
        while( elem->parent != nullptr )
            elem = elem->parent;
        return elem->data;
    }
    int findSize( const T& x ) {
        ElementNode* elem = findParent( x );
        return elem == nullptr ? 0 : elem->count;
    }
    int setSize() { return numSets; }
    ~UnionFind() {
    }
private:
    ElementNode* findParent( const T& x ) {
        ElementNode* elem = findElement( x );
        while( elem && elem->parent != nullptr )
            elem = elem->parent;
        return elem;
    }
    ElementNode* findElement( const T& x ) {
        return elementMap[ x ];
    }
};

class UnionFindTest {
public:
    static void run() {
        unionFindBasicTest();
    }
private:
    static void unionFindBasicTest() {
        vector< int > v = { 10, 20, 30, 40, 50, 60, 70 };
        int N = v.size();

        UnionFind< int > uf( v.begin(), v.end() );

        assert( uf.setSize() == N );
        for( int i : v )
            assert( uf.find( i ) == i );
        uf.merge( 10, 20 );
        uf.merge( 10, 70 );
        assert( uf.setSize() == N - 2 );
        assert( uf.find( 10 ) == uf.find( 20 ) &&
                uf.find( 10 ) == uf.find( 70 ) );
        assert( uf.findSize( 10 ) == 3 );
        uf.merge( 30, 40 );
        uf.merge( 50, 60 );
        uf.merge( 30, 50 );
        assert( uf.setSize() == 2 );
        assert( uf.findSize( 30 ) == 4 );
        uf.merge( 10, 50 );
        assert( uf.setSize() == 1 );
        assert( uf.findSize( 10 ) == N );
    }
};

#endif //__UNION_FIND_H__
