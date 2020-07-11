#include <iostream>
#include <cstdlib>
#include <iterator>
#include <cassert>
using namespace std;

namespace Grid2D {

template < unsigned int M, unsigned int N, typename T = int >
class Grid {

private:
    T** g;
public:
    Grid() {
        void* grid = malloc( ( sizeof(T) * M * N ) + ( sizeof(T*) * M ) );
        g = (T**) grid ;
        T* elem = (T*) ( grid + ( sizeof(T*) * M ) );

        for( int i = 0; i < M; ++i, elem += N )
            g[i] = elem;
    }

    const T& getElement( int i, int j ) {
        assert( i >= 0 && i < M && j >=0 && j < N );
        return g[i][j];
    }

    void setElement( int i, int j, const T& data ) {
        assert( i >= 0 && i < M && j >= 0 && j < N );
        g[i][j] = data;
    }

    void populateGrid() {
        istream_iterator< T > eos; //end of stream indicator
        istream_iterator< T > is( cin ); //input stream

        int n = 0;
        while( is != eos ) {
            new ( &g[ n / N ][ n % N  ] ) T( *is );
            ++n;
            if( n == M * N ) break;
            ++is;
        }
        while( n < M * N ) {
            new ( &g[ n / N ][ n % N  ] ) T;
        }
    }

    void printGrid() {
        cout << "Grid Size: " << M << " * " << N << endl;
        for( int i = 0; i < M; ++i )
        {
            for( int j = 0; j < N; ++j )
                cout << g[i][j] << "   ";
            cout << endl;
        }
        cout << "-------------------------" << endl;
    }

    Grid(Grid& src) = delete;
    ~Grid() { free( g ) ; }
};

} //End of namespace Grid2D

namespace GridTest {

    void gridCreationTest();

} //End of namespace GridTest
