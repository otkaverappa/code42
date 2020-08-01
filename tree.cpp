#include "tree.h"

Tree Tree::createTree( const vector< ii > & edges ) {
    int rootId;
    int nvertices = edges.size() + 1;

    unordered_map< int, int > parentMap;

    for( auto& link : edges ) {
        int i = link.first, j = link.second;
        //Edge (i, j) so that parentOf(j) is i. We use parentMap to
        //detect an invalid edge.
        assert( parentMap.find( j ) == parentMap.end() );
        parentMap[ j ] = i;
        if( parentMap.find( i ) == parentMap.end() )
            rootId = i;
    }

    unordered_map< int, TreeNode* > treeNodeMap;

    for( auto& link : edges ) {
        int i = link.first, j = link.second;

        TreeNode * parentNode, * childNode;
        parentNode = treeNodeMap[i];
        childNode = treeNodeMap[j];
        if( parentNode == nullptr ) {
            parentNode = new TreeNode( i );
            treeNodeMap[i] = parentNode;
        }
        if( childNode == nullptr ) {
            childNode = new TreeNode( j );
            treeNodeMap[j] = childNode;
        }

        parentNode->children.push_back( childNode );
    }
    return Tree ( treeNodeMap[ rootId ], nvertices );
}

int TreeFunctions::diameter( const Tree & tree, vector< int > & path ) {
}

/*
bool Tree::traverseLevelOrder( vector< TreeNode* > &result ) const {
    queue< TreeNode* > q;
    if( root )
        q.push( root );
    while( !q.empty() ) {
        int count = q.size();
        while( count-- ) {
            TreeNode* n = q.front(); q.pop();
            result.push_back( n );
            for( TreeNode* ch : n->children )
                q.push( ch );
        }
        result.push_back( nullptr );
    }
    return true;
}
*/
