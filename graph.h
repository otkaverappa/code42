#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "common.h"

class Graph {
private:
    struct EdgeData {
        int u, v, weight;
        EdgeData( int u, int v, int weight ) : u( u ), v( v ), weight( weight ) {}
    };
    struct Edge {
        int v;
        int weight;
        int label;
        int revIndex;

        Edge( int v, int weight = 1, int label = 0 ) :
              v( v ), weight( weight ), label( label ) {}
    };

    int nvertices;
    int nedges;
    vector< vector < Edge > > adjList;
    bool directed;

public:
    Graph( int nvertices, bool directed );

    bool addEdge( int u, int v, int weight=1, int label=0 );
    bool addEdgesWithUnitWeights( const vector< ii > &edges );
    bool addEdgesWithWeights( const vector< EdgeData > & edges );

    bool isDirected() const {
        return directed;
    }

    bool isEmpty() const {
        return nvertices == 0;
    }

    bool isVertexWithinRange( int u ) const {
        return ( u >= 0 && u < nvertices );
    }

    ~Graph() {}

public:
    void printGraph();

    friend class GraphTest;
    friend class GraphFunctions;
    friend class GraphFunctionsTest;
};

class GraphFunctions {
public:
    static int BFS( const Graph & g, int startVertex, int endVertex ,
                    vector< int > & path );
    static int bidirectionalBFS( const Graph & g, int startVertex, int endVertex,
                                 vector< int > & path );
    static int minimumSpanningTree( const Graph & g, vector< ii > & treeEdges );
};

class GraphFunctionsTest {
public:
    static void runTest() {
        minimumSpanningTreeTest();
    }
private:
    static void minimumSpanningTreeTest();
};

class GraphTest {
public:
    static void runTest() {
        snakeAndLadderTest();
        graphColorWalkTest();
        graphDivisiblePathTest();
    }
private:
    static int graphDivisiblePath( Graph& g, int startVertex,
                                   int endVertex, int pathLenDiv,
                                   vector<int> &path ) {
        assert( pathLenDiv > 0 );
        assert( path.size() == 0 );

        bool searchSuccessful = false;
        ii startState( startVertex, 0 ),
           endState( endVertex, 0 );
        ii curState;
        map< ii, ii > visited;
        queue< ii > q;
        q.push( startState );
        visited[ startState ] = startState;

        while( !q.empty() ) {
            curState = q.front(); q.pop();

            //Successful end of search.
            int curVertex = curState.first;
            int curPathLen = curState.second;
            if( curState == endState ) {
                    searchSuccessful = true;
                    break;
            }

            curPathLen = ( curPathLen + 1 ) % pathLenDiv;
            for( auto& adj : g.adjList[ curVertex ] ) {
                ii nextState( adj.v, curPathLen );
                if( visited.count( nextState ) ) continue;
                q.push( nextState );
                visited[ nextState ] = curState;
            }
        }
        if( searchSuccessful ) {
            ii prevState = curState;
            while( true ) {
                path.push_back( prevState.first );
                if( prevState == startState ) break;
                prevState = visited[ prevState ];
            }
        }
        return path.size() - 1;
    }
    static int graphColorWalk( Graph &g, int startVertex, int endVertex,
                               vector< ii > &path ) {
        assert( path.size() == 0 );
        pair< ii, int > startState( ii( -1, -1 ), startVertex ),
                        curState;
        map< pair< ii, int >, pair< ii, int > > visited;
        queue< pair< ii, int > > q;
        q.push( startState );
        visited[ startState ] = startState;
        bool searchSuccessful = false;

        while( !q.empty() ) {
            curState = q.front(); q.pop();
            int color1 = curState.first.first,
                color2 = curState.first.second;
            int curVertex = curState.second;

            if( curVertex == endVertex ) {
                searchSuccessful = true;
                break;
            }

            //Populate the queue with possible state transitions.
            for( auto& adj : g.adjList[ curVertex ] ) {
                if( adj.label == color1 && color1 == color2 )
                    continue;
                pair< ii, int > nextState( { color2, adj.label }, adj.v );
                if( visited.count(nextState) ) continue;
                visited[ nextState ] = curState;
                q.push( nextState );
            }
        }
        if( searchSuccessful ) {
            pair< ii, int > prevState = curState;
            while( true ) {
                path.push_back( { prevState.second, prevState.first.second } );
                if( prevState == startState ) break;
                prevState = visited[ prevState ];
            }
        }
        return path.size() - 1;
    }
    static void graphDivisiblePathTest() {
        Graph g( 6, true );
        vector<ii> edges { {0,1}, {1,2}, {1,4}, {3,2}, {4,3}, {4,5}, {5,0} };
        string vertexLabels = "swtzyx";
        g.addEdgesWithUnitWeights( edges );

        vector< int > path;
        graphDivisiblePath( g, 0, 2, 3, path);
        for( int i = path.size() - 1; i >= 0; --i )
            printf( "%c%s", vertexLabels[ path[i] ],
                    ( i > 0 ) ? " --> " : "" );
        printf( "\n" );
    }
    static void graphColorWalkTest() {
        Graph g( 6, true );
        const int weight = 1, red = 0, blue = 1;
        vector< ii > edges = { {0,1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,0},
                               {2,4}, {3,1}, {4,0}, {5,1} };
        vector< int > colors = { red, red, red, red, red, red,
                                 blue, blue, blue, blue };
        for( int i = 0; i < edges.size(); ++i )
            g.addEdge( edges[i].first, edges[i].second, weight, colors[i] );

        string vertexLabels = "sabtdc";
        vector< ii > path;
        int startVertex = 0, endVertex = 3;
        int len = graphColorWalk( g, startVertex, endVertex, path );
        printf( "Path Length = %d\n", len );
        for( int i = path.size() - 1; i >= 0; --i )
            printf( "%c %s %s", vertexLabels[ path[i].first ],
                    ( i > 0 ) ? ( path[i - 1].second == red ? "(red)" : "(blue)" )
                              : "" ,
                    ( i > 0 ) ? " --> " : "" );
        printf( "\n" );
    }
    static int snakeAndLadder( int target, int k, map< int, int> &ladders,
                               map< int, int > &snakes,
                               vector< int > &moves ) {
        assert( k > 0 && k <=6 );
        assert( target >= 64 && target <= 100 );

        int startCell = 0;
        int curMove;
        bool searchSuccessful = false;

        queue< int > q;
        q.push( startCell );

        map< int, int > visited;
        visited[ startCell ] = startCell;

        while( !q.empty() ) {
            curMove = q.front(); q.pop();
            if( curMove == target ) {
                searchSuccessful = true;
                break;
            }
            int nextMove;
            for( int u = 1; u <= k; ++u ) {
                nextMove = curMove + u;
                if( ladders.count( nextMove ) ) {
                    assert( !snakes.count( nextMove ) );
                    nextMove = ladders[ nextMove ];
                    assert( !ladders.count( nextMove ) );
                    assert( !snakes.count( nextMove ) );
                } else if( snakes.count( nextMove ) ) {
                    nextMove = snakes[ nextMove ];
                    assert( !ladders.count( nextMove ) );
                    assert( !snakes.count( nextMove ) );
                }
                if( visited.count( nextMove ) ) continue;
                q.push( nextMove );
                visited[ nextMove ] = curMove;
            }
        }
        if( searchSuccessful ) {
            int prevMove = curMove;
            while( true ) {
                moves.push_back( prevMove );
                if( prevMove == startCell ) break;
                prevMove = visited[ prevMove ];
            }
        }
        return moves.size() - 1;
    }
    static void snakeAndLadderTest() {
        const int N = 100;
        map< int, int > ladders = { {3,42}, {8,54}, {24,86},
                                    {50,93}, {58,65}, {59,81} };
        map< int, int > snakes = { {27,17}, {67,10}, {79,40}, {95,71}, {98,6} };
        vector< int > moves;

        for( int k = 2; k <=6; ++k ) {
            printf( "With %d steps, the board is solvable in %d moves.\n",
                    k, snakeAndLadder( N, k, ladders, snakes, moves ) );
            for( int m = moves.size() - 1; m >=0 ; --m ) {
                printf( "%s%d%s",
                       m == moves.size() - 1 ? "(START) " : "",
                       moves[ m ],
                       m == 0 ? "(END)" : " --> " );
            }
            printf( "\n" );
            moves.clear();
        }
    }
};

#endif // __GRAPH_H__
