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

bool Graph::addEdges( const vector<ii> &edges ) {
    for( const ii& edge : edges )
        addEdge( edge.first, edge.second );
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
