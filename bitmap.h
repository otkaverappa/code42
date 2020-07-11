#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "common.h"
#include "unionfind.h"

class Bitmap : public RectangularGridProperties {
private:
    enum { N = sizeof( int ) * 8 };
    int colsInternal;
    vector< vector< int > > bmap;

    void setAll( bool setBits=false ) {
        for( int i = 0; i < rows(); ++i )
            for( int j = 0; j < colsInternal; ++j )
                bmap[ i ][ j ] = ( setBits ) ? ~0 : 0 ;
    }
    void resizeBitmap( bool setBits=false ) {
        colsInternal = cols() / N;
        if( cols() % N ) ++colsInternal;

        bmap.resize( rows() );
        for( int i = 0; i < rows(); ++i )
            bmap[ i ].resize( colsInternal, setBits ? ~0 : 0 );
    }
public:
    Bitmap( int rows, int cols, bool setBits=false ) :
            RectangularGridProperties( rows, cols ) {
        resizeBitmap( setBits );
    }
    Bitmap( const vector< string > &grid ) :
            RectangularGridProperties( grid.size(), grid[0].size() ) {
        resizeBitmap();

        for( int i = 0; i < rows(); ++i ) {
            assert( grid[i].size() == cols() );
            for( int j = 0; j < cols(); ++j ) {
                switch( grid[i][j] ) {
                    case '0': reset(i, j); break;
                    case '1': set(i, j); break;
                    assert( false );
                }
            }
        }
    }
    void setAll() {
        setAll( true );
    }
    void resetAll() {
        setAll( false );
    }
    void set( int i, int j ) {
        boundsCheckAndAssert( i, j );
        bmap[i][j/N] |= ( 1 << (j%N) );
    }
    void reset( int i, int j ) {
        boundsCheckAndAssert( i, j );
        bmap[i][j/N] &= ~( 1 << (j%N) );
    }
    bool isSet( int i, int j ) const {
        boundsCheckAndAssert( i, j );
        return bmap[i][j/N] & ( 1 << (j%N) );
    }
    int valueAt( int i, int j ) const {
        return isSet( i, j ) ? 1 : 0 ;
    }

    void set( const ii& index ) { set( index.first, index.second ); }
    void reset( const ii& index ) { reset( index.first, index.second ); }
    bool isSet( const ii& index ) const { return isSet( index.first, index.second ); }
    int valueAt( const ii& index ) const { return valueAt( index.first, index.second ); }

    void print() {
        printf( "Bitmap @%p Rows=%2d Cols=%2d\n", this, rows(), cols() );
        for( int i = 0; i < rows(); ++i ) {
            for( int j = 0; j < cols(); ++j )
                printf( "%d", isSet( i, j ) ? 1 : 0 );
            printf( "\n" );
        }
        printf( "\n" );
    }
};

class BlackenConnectedComponent {
private:
    RectangularGridProperties rgp;
    Bitmap grid;
    UnionFind <ii> uf; //ii is pair< int, int >
    int maxSetSize;
public:
    BlackenConnectedComponent( int M, int N ) : grid( M, N, true ), rgp( M, N ),
                                                maxSetSize( 0 ) {
    }
    int blacken( ii index ) {
        //(i,j) is the cell specified by index.
        //If the cell at (i,j) is already 0, then return the maximum
        //set size recorded so far.
        if( !grid.isSet( index ) )
            return maxSetSize;
        //Add pixel (i,j) to the union find data structure, setup
        //maxSetSize, and set the pixel value to 0.
        assert( uf.addSet( index ) );
        if( maxSetSize == 0 )
            maxSetSize = 1;
        grid.reset( index );
        //For each neighboring cell (u,v) which is 0, merge (u,v)
        //and (i,j).
        vector< ii > neighbors;
        rgp.fillNeighbors( index, neighbors );
        for( const ii& n : neighbors ) {
            if( grid.isSet( n ) ) continue;
            maxSetSize = max( maxSetSize, uf.merge( n, index ) );
        }
        return maxSetSize;
    }
    static int LargestConnectedBlackComponent( const Bitmap& grid ) {
        int result = INT_MIN;
        int M = grid.rows(), N = grid.cols();
        vector< vector< bool > > visited( M, vector< bool > ( N, false ) );
        for( int i = 0; i < M; ++i ) {
            for( int j = 0; j < N; ++j ) {
                if( grid.isSet( i, j ) || visited[i][j] ) continue;
                result = max( result, sizeOfConnectedComponent( i, j, grid, visited ) );
            }
        }
        return result;
    }
private:
    static int sizeOfConnectedComponent( int i, int j, const Bitmap& grid,
                                         vector< vector< bool > > &visited ) {
        queue< ii > q;
        q.push( { i, j } );
        visited[i][j] = true;
        int count = 0;

        vector< ii > neighbors;
        while( !q.empty() ) {
            ii currentIndex = q.front(); q.pop(); ++count;
            grid.fillNeighbors( currentIndex, neighbors );
            for( const ii& n : neighbors ) {
                auto & [ u, v ] = n;
                if( grid.isSet( u, v ) || visited[u][v] ) continue;
                q.push( n );
                visited[u][v] = true;
            }
        }
        return count;
    }
};

class SolidBlockComputation {
private:
    const Bitmap& bitmap;
    vector< vector < int > > partialSum;
public:
    SolidBlockComputation( const Bitmap& bitmap ) : bitmap( bitmap ) {
        preprocessBitmap();
    }
    pair< bool, int > isSolidBlock( const ii& topLeft, const ii& bottomRight ) {
        int whitePixelCount = pixelCount( topLeft, bottomRight );

        if( whitePixelCount == 0 )
            return { true, 0 };
        else if( whitePixelCount == querySize( topLeft, bottomRight ) )
            return { true, 1 };
        else
            return { false, -1 };
    }
    int pixelCount( const ii& topLeft, const ii& bottomRight, bool whitePixel=true ) {
        bitmap.boundsCheckAndAssert( topLeft );
        bitmap.boundsCheckAndAssert( bottomRight );

        int totalElements = querySize( topLeft, bottomRight );

        int sum = partialSum[ bottomRight.first ][ bottomRight.second ];
        if( bitmap.isIndexWithinBounds( topLeft.first - 1, bottomRight.second ) )
            sum -= partialSum[ topLeft.first -1 ][ bottomRight.second ];
        if( bitmap.isIndexWithinBounds( bottomRight.first, topLeft.second - 1 ) )
            sum -= partialSum[ bottomRight.first ][ topLeft.second - 1 ];
        if( bitmap.isIndexWithinBounds( topLeft.first - 1, topLeft.second - 1 ) )
            sum += partialSum[ topLeft.first - 1 ][ topLeft.second - 1 ];

        return whitePixel ? sum : totalElements - sum;
    }
private:
    int querySize( const ii& topLeft, const ii& bottomRight ) {
        int queryRows = bottomRight.first - topLeft.first + 1;
        int queryCols = bottomRight.second - topLeft.second + 1;
        assert( queryRows > 0 && queryCols > 0 );
        return queryRows * queryCols;
    }
    void preprocessBitmap() {
        partialSum.resize( bitmap.rows() );
        for( int i = 0; i < bitmap.rows(); ++i )
            partialSum[ i ].resize( bitmap.cols(), 0 );
        //Fill the element at (0,0).
        partialSum[ 0 ][ 0 ] = bitmap.valueAt( 0, 0 );
        //Fill first row and first column.
        for( int col = 1; col < bitmap.cols(); ++col )
            partialSum[ 0 ][ col ] = partialSum[ 0 ][ col - 1 ] + bitmap.valueAt( 0, col );
        for( int row = 1; row < bitmap.rows(); ++row )
            partialSum[ row ][ 0 ] = partialSum[ row - 1 ][ 0 ] + bitmap.valueAt( row, 0 );
        //Fill the remaining cells.
        for( int row = 1; row < bitmap.rows(); ++row ) {
            for( int col = 1; col < bitmap.cols(); ++ col ) {
                int A = partialSum[ row - 1 ][ col ];
                int B = partialSum[ row ][ col - 1 ];
                int C = partialSum[ row - 1 ][ col - 1 ];
                partialSum[ row ][ col ] = A + B - C + bitmap.valueAt( row, col );
            }
        }
    }
};

class BitmapQuadrantDivisionTree {
public:
    struct BitmapQuadrantDivisionTreeNode {
        ii cutPos;
        bool isWhite;
        BitmapQuadrantDivisionTreeNode* quad[4];

        BitmapQuadrantDivisionTreeNode( const ii& cutPos ) : cutPos( cutPos ) {
            quad[0] = quad[1] = quad[2] = quad[3] = nullptr;
        }
        BitmapQuadrantDivisionTreeNode( bool isWhite ) : isWhite( isWhite ) {
            quad[0] = quad[1] = quad[2] = quad[3] = nullptr;
        }
        bool isLeaf() const {
            //For a leaf node, all quadrant pointers must be NULL.
            if( quad[0] == nullptr ) {
                assert( quad[1] == nullptr && quad[2] == nullptr &&
                        quad[3] == nullptr );
                return true;
            }
            //For an internal node, there are three possibilities:
            //1. All four pointers are non NULL.
            //2. quad[0], quad[1] are non NULL and quad[2] = quad[3] = NULL.
            //3. quad[0], quad[2] are non NULL and quad[1] = quad[3] = NULL.
            if( quad[1] == nullptr ) {
                assert( quad[2] != nullptr && quad[3] == nullptr );
            } else {
                if( quad[2] == nullptr ) assert( quad[3] == nullptr );
                else assert( quad[3] != nullptr );
            }
            return false;
        }
        bool isInternalNode() const {
            return !isLeaf();
        }
    };
public:
    BitmapQuadrantDivisionTreeNode* root;
    int M, N, depth_;

    BitmapQuadrantDivisionTree( BitmapQuadrantDivisionTreeNode* root, int M, int N, int depth ) :
                                root( root ), M( M ), N( N ), depth_( depth ) {}
    bool depth() { return depth_; }

    bool pixelValue( int i, int j ) {
        assert( i >= 0 && j >= 0 && i < M && j < N );
        return pixelValue_( root, i, j );
    }
    bool pixelValue( const ii& index ) {
        return pixelValue( index.first, index.second );
    }
    ii totalPixelDepth() {
        ii pixelDepth( 0, 0 );
        totalPixelDepth_( root, { 0, 0 }, { M - 1, N - 1 }, 0, pixelDepth );
        return pixelDepth;
    }
    void print() {
        printf( "BitmapQuadrantDivisionTreeNode: M=%d N=%d depth=%d\n", M, N, depth_ );
        print_( root, { 0, 0 }, { M - 1, N - 1 } );
    }
private:
    void totalPixelDepth_( const BitmapQuadrantDivisionTreeNode* n,
                           const ii& topLeft, const ii& bottomRight, int depth,
                           ii& pixelDepth) {
        if( n == nullptr ) return;
        int cellCount = ( bottomRight.first - topLeft.first + 1 ) *
                        ( bottomRight.second - topLeft.second + 1 );
        if( n->isLeaf() ) {
            n->isWhite ? pixelDepth.first += cellCount * depth :
                         pixelDepth.second += cellCount * depth ;
            return;
        }
        const auto & [ u, v ] = n->cutPos;
        totalPixelDepth_( n->quad[0], topLeft, { u, v }, depth + 1, pixelDepth );
        totalPixelDepth_( n->quad[1], { topLeft.first, v + 1 }, { u, bottomRight.second },
                          depth + 1, pixelDepth );
        totalPixelDepth_( n->quad[2], { u + 1, topLeft.second }, { bottomRight.first, v },
                          depth + 1, pixelDepth );
        totalPixelDepth_( n->quad[3], { u + 1, v + 1}, bottomRight, depth + 1, pixelDepth );
    }
    bool pixelValue_( const BitmapQuadrantDivisionTreeNode* n, int i, int j ) {
        assert( n != nullptr );
        if( n->isLeaf() )
            return n->isWhite;
        const auto& [ u, v ] = n->cutPos;
        if( i <= u ) {
            if( j <= v ) return pixelValue_( n->quad[0], i, j );
            else pixelValue_( n->quad[1], i, j );
        } else {
            if( j <= v ) return pixelValue_( n->quad[2], i, j );
            else return pixelValue_( n->quad[3], i, j );
        }
    }
    void print_( const BitmapQuadrantDivisionTreeNode* n,
                 const ii& topLeft, const ii& bottomRight ) {
        if( n == nullptr ) return;

        int rows = bottomRight.first - topLeft.first + 1;
        int cols = bottomRight.second - topLeft.second + 1;
        if( n->isLeaf() ) {
            printf( "Leaf: [%d X %d] color = %s\n", rows, cols, n->isWhite ? "White" : "Black" );
            return;
        }
        assert( n->isInternalNode() );
        const auto& [ u, v ] = n->cutPos;
        printf( "Internal Node: [%d X %d] Split = (%d, %d)\n", rows, cols, u - topLeft.first + 1,
                v - topLeft.second + 1 );
        print_( n->quad[0], topLeft, n->cutPos );
        print_( n->quad[1], { topLeft.first, v + 1 }, { u, bottomRight.second } );
        print_( n->quad[2], { u + 1, topLeft.second }, { bottomRight.first, v } );
        print_( n->quad[3], { u + 1, v + 1 }, bottomRight );
    }
};

class BitmapQuadrantDivision {
public:
    enum { BitmapQuadrantDivition_MIN_DEPTH = 0,
           BitmapQuadrantDivition_AVG_PIXEL_DEPTH = 1,
           BitmapQuadrantDivition_AVG_BLACK_PIXEL_DEPTH = 2,
           BitmapQuadrantDivition_AVG_WHITE_PIXEL_DEPTH = 3
         };
    BitmapQuadrantDivision( const Bitmap& bitmap ) : bitmap( bitmap ),
                            compute( bitmap ) {}

    BitmapQuadrantDivisionTree
    createBitmapQuadrantDivision( int bitmapQuadrantDivisionType = BitmapQuadrantDivition_MIN_DEPTH ) {
        int M = bitmap.rows(), N = bitmap.cols();
        ii topLeft = { 0, 0 }, bottomRight = { M - 1, N - 1 };
        map< RectangularArea, SplitLocAndValue > optimalSplit;
        int depth = createBitmapQuadrantDivision_( topLeft, bottomRight, optimalSplit,
                                                   bitmapQuadrantDivisionType );
        auto root = createBitmapQuadrantTree_( topLeft, bottomRight, optimalSplit );
        return BitmapQuadrantDivisionTree( root, M, N, depth );
    }
private:
    const Bitmap& bitmap;
    SolidBlockComputation compute;
public:
    enum { SOLID_BLOCK_CUT_DEPTH = 0 };
    typedef pair< ii, ii > RectangularArea;
    typedef pair< ii, pair< int, bool > > SplitLocAndValue;
private:
    BitmapQuadrantDivisionTree::BitmapQuadrantDivisionTreeNode*
    createBitmapQuadrantTree_( const ii& topLeft, const ii& bottomRight,
                               map< RectangularArea, SplitLocAndValue > &optimalSplit ) {
        int rows = bottomRight.first - topLeft.first + 1;
        int cols = bottomRight.second - topLeft.second + 1;

        //For a block with a single row or a single column, two of the four
        //quadrant pointers will be NULL.
        if( rows <= 0 || cols <= 0 )
            return nullptr;

        RectangularArea key = { topLeft, bottomRight };
        const auto& [ splitLoc, depthAndFlag ] = optimalSplit[ key ];
        const auto& [ depth, isWhite ] = depthAndFlag;

        if( depth == SOLID_BLOCK_CUT_DEPTH ) {
            return new BitmapQuadrantDivisionTree::BitmapQuadrantDivisionTreeNode( isWhite );
        }
        BitmapQuadrantDivisionTree::BitmapQuadrantDivisionTreeNode* newNode =
        new BitmapQuadrantDivisionTree::BitmapQuadrantDivisionTreeNode( splitLoc );
        newNode->quad[0] = createBitmapQuadrantTree_( topLeft, splitLoc, optimalSplit );
        newNode->quad[1] = createBitmapQuadrantTree_( { topLeft.first, splitLoc.second + 1 },
                                                      { splitLoc.first, bottomRight.second }, optimalSplit );
        newNode->quad[2] = createBitmapQuadrantTree_( { splitLoc.first + 1, topLeft.second },
                                                      { bottomRight.first, splitLoc.second }, optimalSplit );
        newNode->quad[3] = createBitmapQuadrantTree_( { splitLoc.first + 1, splitLoc.second + 1},
                                                      bottomRight, optimalSplit );
        return newNode;
    }
    int createBitmapQuadrantDivision_( const ii& topLeft, const ii& bottomRight,
                                       map< RectangularArea, SplitLocAndValue > &optimalSplit,
                                       int divisionType ) {
        RectangularArea key = { topLeft, bottomRight };
        if( optimalSplit.count( key ) > 0 ) {
            const auto& [ splitLoc, depthAndFlag ] = optimalSplit[ key ];
            const auto& [ depth, flag ] = depthAndFlag;
            return depth;
        }
        const auto & [ isSolidBlock, color ] = compute.isSolidBlock( topLeft, bottomRight );
        if( isSolidBlock ) {
            ii splitLoc { -1, -1 };
            bool isWhite = ( color == 1 );
            optimalSplit[ key ] = { splitLoc, { SOLID_BLOCK_CUT_DEPTH, isWhite } };
            return SOLID_BLOCK_CUT_DEPTH;
        }
        int minDepth = INT_MAX;
        ii bestSplitPos;

        if( topLeft.second == bottomRight.second ) {
            //Special case for single column grid.
            for( int row = topLeft.first; row < bottomRight.first; ++row ) {
                int left = createBitmapQuadrantDivision_( topLeft, { row, bottomRight.second },
                                                          optimalSplit, divisionType );
                int right = createBitmapQuadrantDivision_( { row + 1, bottomRight.second }, bottomRight,
                                                           optimalSplit, divisionType );
                int curDepth = 1 + max( left, right );
                if( curDepth < minDepth ) {
                    minDepth = curDepth;
                    bestSplitPos = { row, bottomRight.second };
                }
            }
        } else if( topLeft.first == bottomRight.first ) {
            //Special case for single row grid.
            for( int col = topLeft.second; col < bottomRight.second; ++col ) {
                int left = createBitmapQuadrantDivision_( topLeft, { topLeft.first, col },
                                                          optimalSplit, divisionType );
                int right = createBitmapQuadrantDivision_( { topLeft.first, col + 1 }, bottomRight,
                                                           optimalSplit, divisionType );
                int curDepth = 1 + max( left, right );
                if( curDepth < minDepth ) {
                    minDepth = curDepth;
                    bestSplitPos = { topLeft.first, col };
                }
            }
        }
        for( int i = topLeft.first; i < bottomRight.first; ++i ) {
            for( int j = topLeft.second; j < bottomRight.second; ++j ) {
                ii splitPos = { i, j }; //The top left quadrant includes the cell at splitPos.
                int q1, q2, q3, q4;
                q1 = createBitmapQuadrantDivision_( topLeft, splitPos, optimalSplit, divisionType );
                q2 = createBitmapQuadrantDivision_( { topLeft.first, splitPos.second + 1 },
                                                    { splitPos.first, bottomRight.second }, optimalSplit,
                                                    divisionType );
                q3 = createBitmapQuadrantDivision_( { splitPos.first + 1, topLeft.second },
                                                    { bottomRight.first, splitPos.second }, optimalSplit,
                                                    divisionType );
                q4 = createBitmapQuadrantDivision_( { splitPos.first + 1, splitPos.second + 1 }, bottomRight,
                                                    optimalSplit, divisionType );
                int curDepth = 1 + max( max( q1, q2 ), max( q3, q4 ) );
                if( curDepth < minDepth ) {
                    minDepth = curDepth;
                    bestSplitPos = splitPos;
                }
            }
        }
        optimalSplit[ key ] = { bestSplitPos, { minDepth, false } };
        return minDepth;
    }
};

class GuillotineDivisionTree {
public:
    enum { CUT_HORIZONTAL = 0, CUT_VERTICAL = 1 };
    struct GuillotineDivisionTreeNode {
        bool isWhite;
        int cutPos, cutType;
        GuillotineDivisionTreeNode *left, *right;

        GuillotineDivisionTreeNode( int cutPos, int cutType ) :
                                    cutPos( cutPos), cutType( cutType ),
                                    left( nullptr ), right( nullptr ) {}
        GuillotineDivisionTreeNode( bool isWhite ) : isWhite( isWhite ),
                                    left( nullptr ), right( nullptr ) {}
        bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }
        bool isInternalNode() const {
            return left != nullptr && right != nullptr;
        }
    };
public:
    GuillotineDivisionTreeNode* root;
    int M, N, size_, depth_;
public:
    GuillotineDivisionTree( GuillotineDivisionTreeNode* root, int M, int N,
                            int size, int depth ) :
                            root( root ), M( M ), N( N ),
                            size_( size ), depth_( depth ) {}
    int size() const { return size_; }
    int depth() const { return depth_; }
    void print() {
        printf( "GuillotineDivisionTree: Size=%d Depth=%d\n", size_, depth_ );
        auto printFunc = []( const GuillotineDivisionTreeNode* n,
                             const ii& topLeft, const ii& bottomRight,
                             int cellCount, int depth, void* data ) -> int {
            int r = bottomRight.first - topLeft.first + 1;
            int c = bottomRight.second - topLeft.second + 1;
            if( n->isLeaf() ) {
                printf( "Leaf Node [ %2d X %2d ] Color=%s\n",
                        r, c, n->isWhite ? "White" : "Black" );
            } else {
                int cutPos;
                string cutTag;
                if( n->cutType == CUT_HORIZONTAL ) {
                    cutTag = "HORIZONTAL CUT";
                    cutPos = n->cutPos - topLeft.first + 1;
                } else {
                    assert( n->cutType == CUT_VERTICAL );
                    cutTag = "VERTICAL CUT";
                    cutPos = n->cutPos - topLeft.second + 1;
                }
                printf( "Internal Node [ %2d X %2d ] CutType=%s CutPosition=%d\n",
                        r, c, cutTag.c_str(), cutPos );
            }
            return TRAVERSE_BOTH_LEFT_RIGHT_TREE;
        };
        traverseGuillotineDivisionTree( root, { 0, 0 }, { M - 1, N - 1 }, M * N, 0, printFunc,
                                        nullptr );
    }
    ii totalPixelDepth() {
        struct UserData {
            int whitePixelDepth;
            int blackPixelDepth;
            UserData() : whitePixelDepth( 0 ), blackPixelDepth( 0 ) {}
        } userdata;
        auto depthCalc = []( const GuillotineDivisionTreeNode* n,
                             const ii& topLeft, const ii& bottomRight,
                             int cellCount, int depth, void* data ) -> int {
            UserData * u = ( UserData* ) data;
            if( n->isLeaf() ) {
                if( n->isWhite )
                    u->whitePixelDepth += ( cellCount * depth );
                else
                    u->blackPixelDepth += ( cellCount * depth );
            }
            return TRAVERSE_BOTH_LEFT_RIGHT_TREE;
        };
        traverseGuillotineDivisionTree( root, { 0, 0 }, { M - 1, N - 1 }, M * N, 0, depthCalc,
                                        &userdata );
        return { userdata.whitePixelDepth, userdata.blackPixelDepth };
    }
    bool pixelValue( int i, int j ) { return pixelValue( { i, j } ); }
    bool pixelValue( const ii& index ) {
        auto & [ u, v ] = index;
        assert( u >= 0 && v >= 0 && u < M && v < N );

        struct UserData {
            const int &u, &v;
            bool isWhite;
            UserData( const int& u, const int& v ) : u( u ), v( v ),
                                                     isWhite( false ) {}
        } userdata( u, v );

        auto pixelValueCalc = []( const GuillotineDivisionTreeNode* n,
                                  const ii& topLeft, const ii& bottomRight,
                                  int cellCount, int depth, void* data ) -> int {
            UserData* p = (UserData*) data;
            if( n->isLeaf() ) {
                p->isWhite = n->isWhite;
                return END_TRAVERSAL;
            }
            if( n->cutType == CUT_HORIZONTAL ) {
                return ( p->u <= n->cutPos ) ? TRAVERSE_LEFT_TREE_ONLY :
                                               TRAVERSE_RIGHT_TREE_ONLY;
            } else {
                assert( n->cutType == CUT_VERTICAL );
                return ( p->v <= n->cutPos ) ? TRAVERSE_LEFT_TREE_ONLY :
                                               TRAVERSE_RIGHT_TREE_ONLY;
            }
        };
        traverseGuillotineDivisionTree( root, { 0, 0 }, { M - 1, N - 1 }, M * N, 0,
                                        pixelValueCalc, &userdata );
        return userdata.isWhite;
    }
private:
    typedef int (*traversalFunc) ( const GuillotineDivisionTreeNode*,
                                   const ii& topLeft, const ii& bottomRight,
                                   int cellCount, int depth, void* userData );
    enum { TRAVERSE_BOTH_LEFT_RIGHT_TREE = 0,
           TRAVERSE_LEFT_TREE_ONLY = 1,
           TRAVERSE_RIGHT_TREE_ONLY = 2,
           END_TRAVERSAL = -1 };
    int gridCellCount( const ii& topLeft, const ii& bottomRight ) {
        int rows = bottomRight.first - topLeft.first + 1;
        int cols = bottomRight.second - topLeft.second + 1;
        assert( rows > 0 && cols > 0 );
        return rows * cols;
    }
    void traverseGuillotineDivisionTree( const GuillotineDivisionTreeNode* n,
                                         const ii& topLeft, const ii& bottomRight,
                                         int cellCount, int depth, traversalFunc func,
                                         void* userData ) {
        int ret = func( n, topLeft, bottomRight, cellCount, depth, userData );
        if( ret == END_TRAVERSAL ) return;
        if( n->isLeaf() ) {
            return;
        }
        assert( n->isInternalNode() );
        ii A, B;
        if( n->cutType == CUT_HORIZONTAL ) {
            A = { n->cutPos, bottomRight.second };
            B = { n->cutPos + 1, topLeft.second };
        } else {
            assert( n->cutType == CUT_VERTICAL );
            A = { bottomRight.first, n->cutPos };
            B = { topLeft.first, n->cutPos + 1 };
        }
        if( ret == TRAVERSE_BOTH_LEFT_RIGHT_TREE ||
            ret == TRAVERSE_LEFT_TREE_ONLY )
            traverseGuillotineDivisionTree( n->left, topLeft, A, gridCellCount( topLeft, A ),
                                            depth + 1, func, userData );
        if( ret == TRAVERSE_BOTH_LEFT_RIGHT_TREE ||
            ret == TRAVERSE_RIGHT_TREE_ONLY )
            traverseGuillotineDivisionTree( n->right, B, bottomRight, gridCellCount( B, bottomRight ),
                                            depth + 1, func, userData );
    }
};

class GuillotineDivision {
public:
    enum { GuillotineDivision_MIN_SIZE = 0,
           GuillotineDivision_MIN_DEPTH = 1,
           GuillotineDivision_AVG_PIXEL_DEPTH = 2,
           GuillotineDivision_AVG_BLACK_PIXEL_DEPTH = 3,
           GuillotineDivision_AVG_WHITE_PIXEL_DEPTH = 4,
    };
private:
    const Bitmap& bitmap;
    SolidBlockComputation compute;

    enum { SOLID_BLOCK_CUT_SIZE = 1,
           SOLID_BLOCK_CUT_DEPTH = 0,
           SOLID_BLOCK_CUT_POS  = -1 }; //For a solid block, the cutPos is set to -1.
public:
    typedef pair< ii, ii > RectangularArea;
    // { cutSize, cutDepth }
    typedef ii CutValue;
    // { cutPos, cutFlag }
    // cutPos = -1, for solid blocks. cutFlag = true for a horizontal cut, false for a
    // vertical cut.
    typedef pair< int, bool > CutType;
    typedef pair< CutValue, CutType > CutValueAndType;
    typedef pair< int, double > AverageDepthType;
    GuillotineDivision( const Bitmap& bitmap ) : bitmap( bitmap ),
                                                 compute( bitmap ) {}

    GuillotineDivisionTree
    createGuillotineDivision( int guillotineDivisionType = GuillotineDivision_MIN_SIZE ) {

        ii topLeft = { 0, 0 };
        ii bottomRight = { bitmap.rows() - 1, bitmap.cols() - 1 };
        map< RectangularArea, CutValueAndType > optimalCuts;
        map< RectangularArea, AverageDepthType > depthStat;
        ii cutValue = createGuillotineDivision_( topLeft, bottomRight, optimalCuts,
                                                 depthStat, guillotineDivisionType );

        GuillotineDivisionTree::GuillotineDivisionTreeNode*
        root = createGuillotineTree_( topLeft, bottomRight, optimalCuts );
        auto & [ treeSize, treeDepth ] = cutValue;
        return GuillotineDivisionTree( root, bitmap.rows(), bitmap.cols(), treeSize, treeDepth );
    }
private:
    GuillotineDivisionTree::GuillotineDivisionTreeNode*
    createGuillotineTree_( const ii& topLeft, const ii& bottomRight,
                           map< RectangularArea, CutValueAndType > &optimalCuts ) {
        RectangularArea key = { topLeft, bottomRight };

        auto & [ cutValue, cutPosAndFlag ] = optimalCuts[ key ];
        auto & [ cutPos, cutFlag ] = cutPosAndFlag;

        GuillotineDivisionTree::GuillotineDivisionTreeNode* newNode;
        if( cutPos == SOLID_BLOCK_CUT_POS ) {
            return new GuillotineDivisionTree::GuillotineDivisionTreeNode( cutFlag );
        }
        int cutType = cutFlag ? GuillotineDivisionTree::CUT_HORIZONTAL :
                                GuillotineDivisionTree::CUT_VERTICAL;
        newNode = new GuillotineDivisionTree::GuillotineDivisionTreeNode( cutPos, cutType );
        if( cutType == GuillotineDivisionTree::CUT_HORIZONTAL ) {
            newNode->left = createGuillotineTree_( topLeft, { cutPos, bottomRight.second },
                                                   optimalCuts );
            newNode->right = createGuillotineTree_( { cutPos + 1, topLeft.second }, bottomRight,
                                                    optimalCuts );
        } else {
            newNode->left = createGuillotineTree_( topLeft, { bottomRight.first, cutPos },
                                                   optimalCuts );
            newNode->right = createGuillotineTree_( { topLeft.first, cutPos + 1 }, bottomRight,
                                                    optimalCuts );
        }
        return newNode;
    }

    ii createGuillotineDivision_( const ii& topLeft, const ii& bottomRight,
                                  map< RectangularArea, CutValueAndType > & optimalCuts,
                                  map< RectangularArea, AverageDepthType > & depthStat,
                                  int guillotineDivisionType ) {

        RectangularArea key = { topLeft, bottomRight };
        if( optimalCuts.count( key ) > 0 ) {
            auto & [ cutValue, cutPosAndFlag ] = optimalCuts[ key ];
            return cutValue;
        }

        //Best cut value found so far. The parameter "guillotineDivisionType" determines
        //the way in which the best cut is determined.
        //{ bestSize, bestDepth }
        ii bestCutValueSoFar = { INT_MAX, INT_MAX } ;
        int cellCount; double bestAverageDepth = DBL_MAX;
        int totalRows, totalCols, totalCells;
        totalRows = bottomRight.first - topLeft.first + 1;
        totalCols = bottomRight.second - topLeft.second + 1;
        totalCells = totalRows * totalCols;

        const auto & [ isSolidBlock, color ] = compute.isSolidBlock( topLeft, bottomRight );
        if( isSolidBlock ) {
            bool isWhite = ( color == 1 );
            bestCutValueSoFar = { SOLID_BLOCK_CUT_SIZE, SOLID_BLOCK_CUT_DEPTH } ;
            optimalCuts[ key ] = { bestCutValueSoFar, { SOLID_BLOCK_CUT_POS, isWhite } };

            bool updateDepthStat = ( guillotineDivisionType ==
                                     GuillotineDivision::GuillotineDivision_AVG_PIXEL_DEPTH );
            updateDepthStat |= ( isWhite && guillotineDivisionType ==
                                            GuillotineDivision::GuillotineDivision_AVG_WHITE_PIXEL_DEPTH );
            updateDepthStat |= ( !isWhite && guillotineDivisionType ==
                                             GuillotineDivision::GuillotineDivision_AVG_BLACK_PIXEL_DEPTH );

            //Store the pixel count and average depth ( 0 for a solid block ) in depthStat.
            if( updateDepthStat )
                depthStat[ key ] = { totalCells, 0.0 };
            return bestCutValueSoFar;
        }

        int cutPos = -1;
        bool horizontalCut = true;
        //First, try horizontal cuts.
        for( int rowNo = topLeft.first; rowNo < bottomRight.first; ++rowNo ) {
            ii topHalf = { rowNo, bottomRight.second };
            ii bottomHalf = { rowNo + 1, topLeft.second };
            ii A = createGuillotineDivision_( topLeft, topHalf, optimalCuts, depthStat,
                                              guillotineDivisionType );
            ii B = createGuillotineDivision_( bottomHalf, bottomRight, optimalCuts, depthStat,
                                              guillotineDivisionType );

            if( guillotineDivisionType == GuillotineDivision::GuillotineDivision_MIN_SIZE ) {
                int curSize = A.first + B.first;
                if( bestCutValueSoFar.first > curSize ) {
                    bestCutValueSoFar.first = curSize;
                    bestCutValueSoFar.second = max( A.second, B.second );
                    cutPos = rowNo;
                }
            } else if( guillotineDivisionType == GuillotineDivision::GuillotineDivision_MIN_DEPTH ) {
                int curDepth = max( A.second, B.second );
                if( bestCutValueSoFar.second > curDepth ) {
                    bestCutValueSoFar.second = curDepth;
                    bestCutValueSoFar.first = A.first + B.first;
                    cutPos = rowNo;
                }
            } else {
                RectangularArea key = { topLeft, topHalf };
                auto & [ cellCount1, avg1 ] = depthStat[ key ];
                key = { bottomHalf, bottomRight };
                auto & [ cellCount2, avg2 ] = depthStat[ key ];
                double curValue = ( cellCount1 * avg1 ) + ( cellCount2 * avg2 );
                if( cellCount1 + cellCount2 > 0 )
                    curValue /= ( cellCount1 + cellCount2 );
                if( bestAverageDepth > curValue ) {
                    bestAverageDepth = curValue;
                    cellCount = cellCount1 + cellCount2;
                    bestCutValueSoFar = { A.first + B.first, max( A.second, B.second ) };
                    cutPos = rowNo;
                }
            }
        }
        //Now, try vertical cuts.
        for( int colNo = topLeft.second; colNo < bottomRight.second; ++colNo ) {
            ii leftHalf = { bottomRight.first, colNo };
            ii rightHalf = { topLeft.first, colNo + 1 };
            ii A = createGuillotineDivision_( topLeft, leftHalf, optimalCuts, depthStat,
                                              guillotineDivisionType );
            ii B = createGuillotineDivision_( rightHalf, bottomRight, optimalCuts, depthStat,
                                              guillotineDivisionType );

            if( guillotineDivisionType == GuillotineDivision::GuillotineDivision_MIN_SIZE ) {
                int curSize = A.first + B.first;
                if( bestCutValueSoFar.first > curSize ) {
                    bestCutValueSoFar.first = curSize;
                    bestCutValueSoFar.second = max( A.second, B.second );
                    cutPos = colNo;
                    horizontalCut = false;
                }
            } else if( guillotineDivisionType == GuillotineDivision::GuillotineDivision_MIN_DEPTH ) {
                int curDepth = max( A.second, B.second );
                if( bestCutValueSoFar.second > curDepth ) {
                    bestCutValueSoFar.second = curDepth;
                    bestCutValueSoFar.first = A.first + B.first;
                    cutPos = colNo;
                    horizontalCut = false;
                }
            } else {
                RectangularArea key = { topLeft, leftHalf };
                auto & [ cellCount1, avg1 ] = depthStat[ key ];
                key = { rightHalf, bottomRight };
                auto & [ cellCount2, avg2 ] = depthStat[ key ];
                double curValue = ( cellCount1 * avg1 ) + ( cellCount2 * avg2 );
                if( cellCount1 + cellCount2 > 0 )
                    curValue /= ( cellCount1 + cellCount2 );
                if( bestAverageDepth > curValue ) {
                    bestAverageDepth = curValue;
                    cellCount = cellCount1 + cellCount2;
                    bestCutValueSoFar = { A.first + B.first, max( A.second, B.second ) };
                    cutPos = colNo;
                    horizontalCut = false;
                }
            }
        }
        //Cache the computed result and return. Increment the "depth" of bestCutValueSoFar.
        ++bestCutValueSoFar.second;
        optimalCuts[ key ] = { bestCutValueSoFar, { cutPos, horizontalCut } };
        depthStat[ key ] = { cellCount, bestAverageDepth + 1.0 };
        return bestCutValueSoFar;
    }
};

class BitmapTest {
public:
    static void runTest() {
        //basicBitmapClassTest();
        //blackenConnectedComponentTest();
        //solidBlockComputationTest();
        //guillotineDivisionTest();
        bitmapQuadrantDivisionTest();
    }
private:
    static void bitmapQuadrantDivisionTest() {
        vector< string > grid = {
            "00000000",
            "00000000",
            "11111100",
            "11100000",
            "11100000",
            "11111111",
            "11100111",
            "11100111",
        };
        Bitmap bitmap( grid );
        BitmapQuadrantDivision b( bitmap );

        auto u = b.createBitmapQuadrantDivision( BitmapQuadrantDivision::BitmapQuadrantDivition_MIN_DEPTH );

        //Verify the pixelValue() method of BitmapQuadrantDivision.
        for( int i = 0; i < bitmap.rows(); ++i ) {
            for( int j = 0; j < bitmap.cols(); ++j ) {
                assert( u.pixelValue( i, j ) == bitmap.valueAt( i, j ) );
            }
        }
    }
    static void guillotineDivisionTest() {
        vector< string > grid = {
            "00000000",
            "00000000",
            "11111100",
            "11100000",
            "11100000",
            "11111111",
            "11100111",
            "11100111",
        };
        Bitmap bitmap( grid );
        GuillotineDivision g( bitmap );

        //Create GuillotineDivision trees.
        auto a = g.createGuillotineDivision( GuillotineDivision::GuillotineDivision_MIN_SIZE );
        auto b = g.createGuillotineDivision( GuillotineDivision::GuillotineDivision_MIN_DEPTH );
        auto c = g.createGuillotineDivision( GuillotineDivision::GuillotineDivision_AVG_PIXEL_DEPTH );
        auto d = g.createGuillotineDivision( GuillotineDivision::GuillotineDivision_AVG_WHITE_PIXEL_DEPTH );
        auto e = g.createGuillotineDivision( GuillotineDivision::GuillotineDivision_AVG_BLACK_PIXEL_DEPTH );

        vector< GuillotineDivisionTree* > gtrees = { &a, &b, &c, &d, &e };

        //Verify the pixelValue() method of GuillotineDivision.
        for( int i = 0; i < bitmap.rows(); ++i ) {
            for( int j = 0; j < bitmap.cols(); ++j ) {
                for( auto ptr : gtrees )
                    assert( bitmap.isSet( i, j ) == ptr->pixelValue( i, j ) );
            }
        }
        //Verify the size(), depth(), totalPixelDepth() properties.
        int minSize = a.size();
        int minDepth = b.depth();

        ii bestTotalPixelDepth = c.totalPixelDepth();
        ii bestWhitePixelDepth = d.totalPixelDepth();
        ii bestBlackPixelDepth = e.totalPixelDepth();

        int minTotalPixelDepth = bestTotalPixelDepth.first + bestTotalPixelDepth.second;
        int minWhitePixelDepth = bestWhitePixelDepth.first;
        int minBlackPixelDepth = bestBlackPixelDepth.second;

        printf( "Best Size=%3d, Depth=%3d, TotalPixelDepth=%3d, WhitePixelDepth=%3d BlackPixelDepth=%3d\n",
                minSize, minDepth, minTotalPixelDepth, minWhitePixelDepth, minBlackPixelDepth );
        for( auto ptr : gtrees ) {
            int A = ptr->size();
            int B = ptr->depth();
            ii result = ptr->totalPixelDepth();
            int C = result.first + result.second;

            printf( "Tree: Size=%3d, Depth=%3d, TotalPixelDepth=%3d, WhitePixelDepth=%3d BlackPixelDepth=%3d\n",
                    A, B, C, result.first, result.second );
            assert( A >= minSize );
            assert( B >= minDepth );
            assert( C >= minTotalPixelDepth );
            assert( result.first >= minWhitePixelDepth );
            assert( result.second >= minBlackPixelDepth );
        }
    }
    static void solidBlockComputationTest() {
        vector< string > grid = {
            "00000000",
            "00000000",
            "11111100",
            "11100000",
            "11100000",
            "11111111",
            "11100111",
            "11100111",
        };
        Bitmap bitmap( grid );

        vector < pair< pair< ii, ii >, pair< bool, int > > > testcases = {
            { { {0,0},{7,7} }, { false, -1 } },
            { { {0,0},{1,7} }, { true ,  0 } },
            { { {2,0},{7,2} }, { true,   1 } },
            { { {3,3},{4,7} }, { true,   0 } },
            { { {5,0},{5,7} }, { true,   1 } },
            { { {1,1},{6,6} }, { false, -1 } },
            { { {6,3},{7,4} }, { true,   0 } },
            { { {3,5},{5,7} }, { false, -1 } },
        };
        SolidBlockComputation comp( bitmap );
        for( auto & [ query, result ] : testcases ) {
            assert( comp.isSolidBlock( query.first, query.second ) == result );
        }
    }
    static void basicBitmapClassTest() {
    }
    static void blackenConnectedComponentTest() {
        printf( "Start: %s\n", __FUNCTION__ );

        BlackenConnectedComponent b( 8, 8 );
        vector< ii > pos = { {0,0},{0,4},{0,5},{0,7},{1,0},{1,2},{1,7},
        {2,0},{2,2},{2,3},{2,4},{2,7},{3,0},{3,4},{3,6},{4,0},{4,1},{4,2},{4,6},
        {5,0},{5,1},{5,3},{5,6},{6,3},{6,6},{7,0},{7,1},{7,3} };

        int i;
        for( const ii& index : pos )
            i = b.blacken( index );
        assert( i == 9 );

        vector< pair< ii, int > > testcases = {
            { {2,5}, 9 }, { {2,6}, 14 }, { {5,2}, 14 }, { {6,0}, 16 },
            { {7,2}, 17 },{ {3,2}, 32 },
        };
        for( auto & [ index, expectedResult ] : testcases )
            assert( b.blacken( index ) == expectedResult );

        Bitmap bitmap( 8, 8, true );
        for( const ii& index : pos )
            bitmap.reset( index );
        assert( BlackenConnectedComponent::LargestConnectedBlackComponent( bitmap )
                == 9 );

        printf( "Success: %s\n", __FUNCTION__ );
    }
};

#endif // __BITMAP_H__
