#include "graph.h"

Graph::Graph( int nvertices, bool directed ) :
       nvertices( nvertices ), directed( directed ) {
           assert( nvertices > 0 );
           adjList.resize( nvertices );
}

bool Graph::addEdge( int u, int v, int weight, int label ) {
    if( !isVertexWithinRange( u ) || !isVertexWithinRange( v ) )
        return false;
    Edge edge( v, weight, label );
    adjList[ u ].push_back( edge );
    ++nedges;
    if( !isDirected() ) {
        Edge revEdge( u, weight, label );
        adjList[ v ].push_back( revEdge );
    }
    return true;
}

bool Graph::addEdgesWithUnitWeights( const vector<ii> &edges ) {
    for( const ii& edge : edges )
        addEdge( edge.first, edge.second );
}

bool Graph::addEdgesWithWeights( const vector< EdgeData > & edges ) {
    for( const EdgeData & edge : edges )
        addEdge( edge.u, edge.v, edge.weight );
}

void Graph::printGraph() {
    printf( "Graph@%p Vertices=%3d Edges=%3d Directed=%s\n",
            this, nvertices, nedges, directed ? "True" : "False" );
    for( int i = 0; i < nvertices; ++i ) {
        printf( "Vertex:%3d --- ", i );
        for( Edge& e : adjList[ i ] ) {
            printf( "( %3d ,%3d, %3d ) ", e.v, e.weight, e.label );
        }
        printf( "\n" );
    }
}

int GraphFunctions::bidirectionalBFS( const Graph & g, int startVertex, int endVertex,
                                      vector< int > & path ) {
    const int notFound = -1;
    if( !g.isVertexWithinRange( startVertex ) || !g.isVertexWithinRange( endVertex ) )
        return notFound;

    unordered_map< int, int > forwardParentVertex, backwardParentVertex;
    forwardParentVertex[ startVertex ] = startVertex;
    backwardParentVertex[ endVertex ] = endVertex;

    bool pathFound = false;
    int curForwardVertex, curBackwardVertex;
    bool metInTheMiddle = false;
    int metInTheMiddleVertex;

    queue< int > forwardq, backwardq;
    forwardq.push( startVertex );
    backwardq.push( endVertex );

    while( !forwardq.empty() && !backwardq.empty() ) {
        int N;

        N = forwardq.size();
        while( N-- ) {
            curForwardVertex = forwardq.front(); forwardq.pop();
            if( curForwardVertex == endVertex ) {
                pathFound = true;
                goto endSearch;
            }
            for( const Graph::Edge & edge : g.adjList[ curForwardVertex ] ) {
                int v = edge.v;
                if( forwardParentVertex.find( v ) != forwardParentVertex.end() )
                    continue;
                if( backwardParentVertex.find( v ) != backwardParentVertex.end() ) {
                    pathFound = metInTheMiddle = true;
                    metInTheMiddleVertex = v;
                    forwardParentVertex[ v ] = curForwardVertex;
                    goto endSearch;
                }
                forwardParentVertex[ v ] = curForwardVertex;
                forwardq.push( v );
            }
        }
        N = backwardq.size();
        while( N-- ) {
            curBackwardVertex = backwardq.front(); backwardq.pop();
            if( curBackwardVertex == startVertex ) {
                pathFound = true;
                goto endSearch;
            }
            for( const Graph::Edge & edge : g.adjList[ curBackwardVertex ] ) {
                int v = edge.v;
                if( backwardParentVertex.find( v ) != backwardParentVertex.end() )
                    continue;
                if( forwardParentVertex.find( v ) != forwardParentVertex.end() ) {
                    pathFound = metInTheMiddle = true;
                    metInTheMiddleVertex = v;
                    backwardParentVertex[ v ] = curBackwardVertex;
                    goto endSearch;
                }
                backwardParentVertex[ v ] = curBackwardVertex;
                backwardq.push( v );
            }
        }
    }
endSearch:
    if( pathFound ) {
        if( curForwardVertex == endVertex ) {
            while( true ) {
                path.push_back( curForwardVertex );
                if( curForwardVertex == startVertex )
                    break;
                curForwardVertex = forwardParentVertex[ curForwardVertex ];
            }
            reverse( path.begin(), path.end() );
        } else if( curBackwardVertex == startVertex ) {
            while( true ) {
                path.push_back( curBackwardVertex );
                if( curBackwardVertex == endVertex )
                    break;
                curBackwardVertex = backwardParentVertex[ curBackwardVertex ];
            }
        } else {
            assert( metInTheMiddle );
            curForwardVertex = metInTheMiddleVertex;
            while( true ) {
                path.push_back( curForwardVertex );
                if( curForwardVertex == startVertex )
                    break;
                curForwardVertex = forwardParentVertex[ curForwardVertex ];
            }
            reverse( path.begin(), path.end() );
            curBackwardVertex = backwardParentVertex[ metInTheMiddleVertex ];
            while( true ) {
                path.push_back( curBackwardVertex );
                if( curBackwardVertex == endVertex )
                    break;
                curBackwardVertex = backwardParentVertex[ curBackwardVertex ];
            }
        }
    }
    return pathFound ? path.size() : notFound;
}

int GraphFunctions::BFS( const Graph & g, int startVertex, int endVertex,
                         vector< int > & path ) {
    const int notFound = -1;
    if( !g.isVertexWithinRange( startVertex ) || !g.isVertexWithinRange( endVertex ) )
        return notFound;

    if( startVertex == endVertex )
        return 0;

    unordered_map< int, int > parentVertex;
    parentVertex[ startVertex ] = startVertex;

    bool pathFound = false;
    int curVertex;
    queue< int > q;
    q.push( startVertex );

    while( !q.empty() ) {
        curVertex = q.front(); q.pop();

        if( curVertex == endVertex ) {
            pathFound = true;
            break;
        }
        for( const Graph::Edge & edge : g.adjList[ curVertex ] ) {
            int v = edge.v;
            if( parentVertex.find( v ) == parentVertex.end() ) {
                parentVertex[ v ] = curVertex;
                q.push( v );
            }
        }
    }

    if( pathFound ) {
        while( true ) {
            path.push_back( curVertex );
            if( curVertex == startVertex )
                break;
            curVertex = parentVertex[ curVertex ];
        }
        reverse( path.begin(), path.end() );
    }
    return pathFound ? path.size() : notFound;
}

int GraphFunctions:: minimumSpanningTreeUsingUF( const Graph & g, vector< ii > & treeEdges ) {
    if( g.isEmpty() )
        return 0;
    assert( !g.isDirected() );

    UnionFind< int > uf;
    vector< Graph::EdgeData > allEdges;

    struct Comparator {
        bool operator () ( const Graph::EdgeData& e1, const Graph::EdgeData& e2 ) {
            return e1.weight < e2.weight;
        }
    };

    for( int i = 0; i < g.nvertices; ++i ) {
        uf.addSet( i );
        for( const Graph::Edge & edge : g.adjList[ i ] ) {
            allEdges.emplace_back( Graph::EdgeData( i, edge.v, edge.weight ) );
        }
    }
    sort( allEdges.begin(), allEdges.end(), Comparator() );

    int totalWeight = 0;
    for( const Graph::EdgeData & e : allEdges ) {
        if( uf.find( e.u ) != uf.find( e.v ) ) {
            totalWeight += e.weight;
            treeEdges.push_back( { e.u, e.v } );
            uf.merge( e.u, e.v );
        }
    }
    return totalWeight;
}

int GraphFunctions::minimumSpanningTree( const Graph & g, vector< ii > & treeEdges ) {
    if( g.isEmpty() )
        return 0;
    assert( !g.isDirected() );
    vector< bool > vertexIncludedInMST( g.nvertices, false );

    priority_queue< Graph::EdgeData, vector< Graph::EdgeData >, Comparator > pq;
    int startVertex = 0;
    pq.push( Graph::EdgeData( startVertex, startVertex, 0 ) );

    int totalWeight = 0;
    while( !pq.empty() ) {
        Graph::EdgeData e = pq.top(); pq.pop();

        if( vertexIncludedInMST[ e.v ] )
            continue;
        if( e.u != e.v )
            treeEdges.push_back( { e.u, e.v } );
        totalWeight += e.weight;
        vertexIncludedInMST[ e.v ] = true;

        for( const Graph::Edge & edge : g.adjList[ e.v ] ) {
            if( vertexIncludedInMST[ edge.v ] == false )
                pq.push( Graph::EdgeData( e.v, edge.v, edge.weight ) );
        }
    }
    return totalWeight;
}

void GraphFunctionsTest::minimumSpanningTreeTest() {
    Graph g( 5, false );
    vector< Graph::EdgeData > edges = {
        { 0, 1, 4 }, { 0, 2, 4 }, { 0, 3, 6 }, { 0, 4, 6 }, { 1, 2, 2 }, { 2, 3, 8 },
        { 3, 4, 9 }
    };
    g.addEdgesWithWeights( edges );
    vector< ii > treeEdges;
    int mstWeight = GraphFunctions::minimumSpanningTree( g, treeEdges );
    printf( "Weight of Minimum Spanning Tree = %d\n", mstWeight );
    treeEdges.clear();
    mstWeight = GraphFunctions::minimumSpanningTreeUsingUF( g, treeEdges );
    printf( "Weight of Minimum Spanning Tree = %d\n", mstWeight );

    Graph g2( 5, false );
    vector< Graph::EdgeData > edges2 = {
        { 0, 1, 9 }, { 0, 2, 75 }, { 2, 3, 51 }, { 2, 1, 95 }, { 1, 3, 19 }, { 1, 4, 42 },
        { 3, 4, 31 }, { 2, 4, 66 }
    };
    g2.addEdgesWithWeights( edges2 );
    treeEdges.clear();
    int mstWeight2 = GraphFunctions::minimumSpanningTree( g2, treeEdges );
    printf( "Weight of Minimum Spanning Tree = %d\n", mstWeight2 );
    treeEdges.clear();
    mstWeight2 = GraphFunctions::minimumSpanningTreeUsingUF( g2, treeEdges );
    printf( "Weight of Minimum Spanning Tree = %d\n", mstWeight2 );
}

int GraphFunctions::findPath( const Graph & g, int startVertex, int endVertex, int policy,
                              vector< ii > & path ) {
    assert( policy >= 0 && policy < INVALID_POLICY );
    if( g.isEmpty() || !g.isVertexWithinRange( startVertex ) || !g.isVertexWithinRange( endVertex )
        || startVertex == endVertex )
        return PATH_WEIGHT_ZERO;

    priority_queue< ii, vector< ii >, greater< ii > > pq;
    vector< ii > costAndParent( g.nvertices, { INT_MAX, 0 } );
    bool searchSuccessful = false;

    pq.push( { 0, startVertex } );

    while( !pq.empty() ) {
        ii curState = pq.top(); pq.pop();
        auto & [ curCost, curVertex ] = curState;
        //Process the current vertex only if its associated cost is "better"
        //than what we have recorded so far.
        if( curCost > costAndParent[ curVertex ].first )
            continue;
        if( curVertex == endVertex ) {
            searchSuccessful = true;
            break;
        }
        for( const Graph::Edge & e : g.adjList[ curVertex ] ) {
            int optCost;
            switch( policy ) {
            case MIN_MAX_WEIGHT_PATH:
                optCost = max( curCost, e.weight ); break;
            case SHORTEST_WEIGHT_PATH:
                optCost = curCost + e.weight; break;
            default:
                //Control should not reach here since we have already
                //checked the value of policy.
                assert( false );
            }
            if( optCost < costAndParent[ e.v ].first ) {
                costAndParent[ e.v ] = { optCost, curVertex };
                pq.push( { optCost, e.v } );
            }
        }
    }
    if( searchSuccessful ) {
        int curVertex = endVertex;
        while( true ) {
            auto & [ cost, parent ] = costAndParent[ curVertex ];
            path.push_back( { parent, curVertex } );
            curVertex = parent;
            if( curVertex == startVertex )
                break;
        }
        reverse( path.begin(), path.end() );
        return costAndParent[ endVertex ].first;
    }
    return PATH_WEIGHT_NO_PATH;
}

void GraphFunctionsTest::findPathTest() {
    Graph g( 7, false );
    vector< Graph::EdgeData > edges = {
        { 0, 1, 50 }, { 0, 2, 60 }, { 1, 4, 90 }, { 4, 6, 40 }, { 2, 5, 50 }, { 5, 6, 140 },
        { 1, 3, 120 }, { 3, 6, 70 }, { 3, 5, 80 }
    };
    g.addEdgesWithWeights( edges );

    vector< ii > path;
    int pathLen = GraphFunctions::findPath( g, 0, 6, GraphFunctions::MIN_MAX_WEIGHT_PATH, path );
    printf( "Best path weight = %d\n", pathLen );

    path.clear();
    pathLen = GraphFunctions::findPath( g, 0, 6, GraphFunctions::SHORTEST_WEIGHT_PATH, path );
    printf( "Best path weight = %d\n", pathLen );
}
