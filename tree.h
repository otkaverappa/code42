#include <vector>
#include <map>
#include <stack>

typedef pair< int, int > ii;

class BinaryTreeNode {
public:
    int value;
    int id;
    BinaryTreeNode *left, *right, *parent;

    BinaryTreeNode( int id, BinaryTreeNode * left = nullptr,
                            BinaryTreeNode * right = nullptr,
                            BinaryTreeNode * parent = nullptr )
                            : id( id ),
                              left( left ), right( right ),
                              parent( parent ),
                              value( 0 ) {}

    bool isLeaf() const { return left == nullptr && right == nullptr; }
    bool isEqual( const BinaryTreeNode* other ) const {
        return other != nullptr && id == other->id &&
               value == other->value;
    }
};

class BinaryTree {
public:
    enum { PREORDER=0, INORDER=1, POSTORDER=2, LEVELORDER=3 } ;
private:
    void traverse_recursive( BinaryTreeNode * root, int type, vector <int> &order ) {

        if( root == nullptr ) return;
        if( type == PREORDER ) order.push_back( root->id );
        traverse_recursive( root->left, type, order );
        if( type == INORDER ) order.push_back( root->id );
        traverse_recursive( root->right, type, order );
        if( type == POSTORDER ) order.push_back( root->id );

    }
    bool isEqual_recursive( const BinaryTreeNode* t1, const BinaryTreeNode* t2 ) {
        if( t1 == nullptr && t2 == nullptr ) return true;
        if( t1 == nullptr || t2 == nullptr ) return false;
        return t1->isEqual( t2 ) && isEqual_recursive( t1->left, t2->left )
                                 && isEqual_recursive( t1->right, t2->right );
    }
    int maxDepth_recursive( const BinaryTreeNode* t ) {
        if( t == nullptr ) return -1;
        return 1 + max( maxDepth_recursive( t->left ),
                        maxDepth_recursive( t->right ) );
    }
    void freeAllTreeNodes( BinaryTreeNode* t ) {
        if( t == nullptr ) return;
        freeAllTreeNodes( t->left );
        freeAllTreeNodes( t->right );
        delete t;
    }
public:
    BinaryTreeNode * root;
    int nvertices;

    BinaryTree( BinaryTreeNode * root = nullptr ) : root( root ), nvertices( 0 ) {}
    BinaryTree( BinaryTreeNode * root, int nvertices ) :
                root( root ), nvertices( nvertices ) {
                    assert( nvertices >= 0 );
                    }

    ~BinaryTree() {
        freeAllTreeNodes( root );
    }

    bool isEmpty() { return root == nullptr; }

    void traverse( int type, vector< int > &order ) {
        traverse_recursive( root, type, order );
    }

    void traverse_i( int type, vector< int > &order ) {
        BinaryTreeNode * cur = root;
        stack< BinaryTreeNode* > s;
        s.push( nullptr );

        if( type == PREORDER ) {
            s.push( cur );
            while( ( cur = s.top() ) != nullptr ) {
                s.pop();
                order.push_back( cur->id );
                if( cur->right ) s.push( cur->right );
                if( cur->left ) s.push( cur->left );
            }
        } else if( type == INORDER ) {
            while( cur != nullptr ) {
                while( cur ) {
                    s.push( cur );
                    cur = cur->left;
                }
                BinaryTreeNode * next;
                while( ( cur = s.top() ) != nullptr ) {
                    s.pop();
                    order.push_back( cur->id );
                    if( cur->right ) {
                        cur = cur->right;
                        break;
                    }
                }
            }
        } else if( type == POSTORDER ) {
            set< BinaryTreeNode* > visited;
            while( cur ) {
                if( visited.count( cur ) == 1 ) {
                    order.push_back( cur->id );
                    cur = s.top();
                    s.pop();
                } else {
                    visited.insert( cur );
                    s.push( cur );
                    if( cur->right ) s.push( cur->right );
                    if( cur->left ) {
                        cur = cur->left;
                    } else {
                        cur = s.top(); s.pop();
                    }
                }
            }
        }
    }

    void traverse_p( int type, vector< int > &order ) {
        BinaryTreeNode * cur = root, * prev = nullptr;
        while( cur ) {
            if( prev == cur->parent ) {
                prev = cur;
                BinaryTreeNode *left = cur->left, *right = cur->right;
                if( left == nullptr && right == nullptr ) {
                    order.push_back( cur->id );
                    cur = cur->parent;
                } else if( left == nullptr ) {
                    if( type == PREORDER || type == INORDER )
                        order.push_back( cur->id );
                    cur = cur->right;
                } else {
                    if( type == PREORDER )
                        order.push_back( cur->id );
                    cur = cur->left;
                }
            } else if( prev == cur->left ) {
                if( type == INORDER ||
                    ( type == POSTORDER && cur->right == nullptr ) )
                    order.push_back( cur->id );
                prev = cur;
                cur = ( cur->right ) ? cur->right : cur->parent;
            } else if( prev == cur->right ) {
                if( type == POSTORDER )
                    order.push_back( cur->id );
                prev = cur;
                cur = cur->parent;
            }
        }
    }

    bool isEqual( const BinaryTree &other ) {
        return nvertices == other.nvertices &&
               isEqual_recursive( root, other.root );
    }

    int maxDepth() {
        return maxDepth_recursive( root );
    }

    static BinaryTree construct( const vector< int > &preorder,
                                 const vector< int > &inorder ) {
        int N = preorder.size();
        assert( N == inorder.size() );
        if( N == 0 ) return BinaryTree();

        int startIndex = 0;
        BinaryTree tree( construct_recursive( preorder, startIndex, preorder.size() - 1,
                                              inorder, 0, inorder.size() - 1 ),
                         N );
        return tree;
    }

    static BinaryTree constructCompleteBinaryTree( int depth ) {
        assert( depth >= -1 );
        int nextId = 0;
        BinaryTree tree( construct_recursive_complete_binary_tree(
                         depth, nextId ) );
        tree.nvertices = nextId;
        return tree;
    }

private:
    static BinaryTreeNode* construct_recursive( const vector< int > &preorder, int& i, int j,
                                                const vector< int > &inorder, int u, int v ) {
        if( i > j || u > v ) return nullptr;
        int currentId = preorder[ i++ ];
        int indexInorder = u;
        for( ; indexInorder <= v; ++indexInorder ) {
            if( inorder[ indexInorder ] == currentId ) break;
        }
        assert( indexInorder <= v );

        BinaryTreeNode* curNode = new BinaryTreeNode( currentId );
        curNode->left = construct_recursive( preorder, i, j, inorder, u, indexInorder - 1 );
        curNode->right = construct_recursive( preorder, i, j, inorder, indexInorder + 1, v );
        return curNode;
    }
    static BinaryTreeNode* construct_recursive_complete_binary_tree( int depth, int& nextId ) {
        if( depth == -1 ) return nullptr;
        BinaryTreeNode* curNode = new BinaryTreeNode( nextId++ );
        curNode->left = construct_recursive_complete_binary_tree( depth - 1, nextId );
        curNode->right = construct_recursive_complete_binary_tree( depth - 1, nextId );
        return curNode;
    }
};

BinaryTree createBinaryTree( const vector< pair< int, int > > & links ) {
    int nvertices = 0;
    int nedges = 0;
    map< int, int > parentMap;
    set< int > noLeftChild;
    parentMap[ 0 ] = 0;
    for( const ii& edge : links ) {
        int u = edge.first, v = edge.second;
        if( v == -1 ) { noLeftChild.insert( u ); continue; }
        assert( parentMap.find( v ) == parentMap.end() );
        parentMap[ v ] = u;
        ++nedges;
    }
    nvertices = nedges + 1;
    vector < BinaryTreeNode* > treeNodes;
    for( int i = 0; i < nvertices; ++i )
        treeNodes.push_back( new BinaryTreeNode( i ) );

    for( int v = 0; v < nvertices; ++v ) {
        assert( parentMap.find( v ) != parentMap.end() );
        int u = parentMap[ v ];
        if( v == 0 ) continue;
        if( noLeftChild.count( u ) == 1 ) {
            assert( treeNodes[ u ]->left == nullptr );
            treeNodes[ u ]->right = treeNodes[ v ];
        }
        else {
            if( treeNodes[ u ]->left == nullptr ) {
                assert( treeNodes[ u ]->right == nullptr );
                treeNodes[ u ]->left = treeNodes[ v ];
            }
            else {
                assert( treeNodes[ u ]->left != nullptr );
                treeNodes[ u ]->right = treeNodes[ v ];
            }
        }
        treeNodes[ v ]->parent = treeNodes[ u ];
    }
    return BinaryTree( treeNodes[ 0 ], nvertices );
}

BinaryTree createBinaryTree_N43() {
    vector< pair< int, int > > links = {
    {0,1}, {0,2}, {1,3}, {1,4}, {3,5}, {3,6}, {5,7}, {5,8}, {8,11}, {8,12},
    {6,9}, {9,10}, {2,13}, {2,14}, {14,16}, {14,15}, {15,17}, {17,18}, {18,20}, {18,21},
    {20,22}, {20,23}, {17,19}, {19,24}, {19,25}, {24,26}, {24,27}, {26,28}, {26,29}, {27,30},
    {27,31}, {32,40}, {40,41}, {40,42}, {32,33}, {33,34}, {33,35}, {35,36}, {35,37}, {36,38},
    {36,39}, {15,32}
    };
    return createBinaryTree( links );
}

void testBinaryTreeFunctions() {
    BinaryTree tree = createBinaryTree_N43();
    vector< int > preorder, postorder, inorder;

    tree.traverse( BinaryTree::PREORDER, preorder );
    tree.traverse( BinaryTree::INORDER, inorder );
    tree.traverse( BinaryTree::POSTORDER, postorder );

    //1. Check that the preorder, postorder, and inorder sequences
    //   have the same length. traverse() is a recursive implementation.
    assert( preorder.size() == inorder.size() &&
            preorder.size() == postorder.size() );

    //2. Verify the traversal implementations which use (a) parent pointers
    //   and (b) explicit stack.
    vector< int > preorder_p, preorder_i;
    tree.traverse_p( BinaryTree::PREORDER, preorder_p );
    tree.traverse_i( BinaryTree::PREORDER, preorder_i );
    assert( preorder == preorder_p );
    assert( preorder == preorder_i );
    vector< int > inorder_p, inorder_i;
    tree.traverse_p( BinaryTree::INORDER, inorder_p );
    tree.traverse_i( BinaryTree::INORDER, inorder_i );
    assert( inorder == inorder_p );
    assert( inorder == inorder_i );
    vector< int > postorder_p, postorder_i;
    tree.traverse_p( BinaryTree::POSTORDER, postorder_p );
    tree.traverse_i( BinaryTree::POSTORDER, postorder_i );
    assert( postorder == postorder_p );
    assert( postorder == postorder_i );

    //   Construct a tree from preorder and inorder traversals and
    //   check if it is equal to the initial tree.
    BinaryTree constructedTree = BinaryTree::construct( preorder, inorder );
    assert( tree.isEqual( constructedTree ) );

    //   Construct a complete binary tree, check its size. Reconstruct
    //   it from preorder and inorder traversals.
    const int depth = 3;
    BinaryTree completeTree = BinaryTree::constructCompleteBinaryTree( depth );
    assert( depth == completeTree.maxDepth() );
    preorder.clear(); inorder.clear();
    completeTree.traverse( BinaryTree::PREORDER, preorder );
    completeTree.traverse( BinaryTree::INORDER, inorder );
    BinaryTree constructedTree2 = BinaryTree::construct( preorder, inorder );
    assert( completeTree.isEqual( constructedTree2 ) );
}

class TreeNode {
public:
    int value;
    int id;
    vector< TreeNode* > children;

    TreeNode( int id ) : id( id ) {}

    bool addChild( TreeNode* t ) { children.push_back( t ); return true; }
    bool isLeaf() const { return nChildren() == 0; }
    int nChildren() const { return children.size(); }
};

class Tree {
public:
    TreeNode *root;
    int nvertices;

    Tree( TreeNode *root = nullptr ) : root( root ) {}
    Tree( TreeNode *root, int nvertices ) :
          root( root ), nvertices( nvertices ) {}

    bool isEmpty() const { return root == nullptr; }

    bool traverseLevelOrder( vector< TreeNode* > &result ) const {
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
};

Tree createTree( vector< pair< int, int > > &links,
                 const vector< int > *elements = nullptr ) {
    int nvertices = links.size() + 1; //vertices are numbered from 0 to nvertices - 1,
    //with vertex 0 being the root.
    vector< TreeNode* > treeNodes ( nvertices, nullptr );
    map< int, int > parentMap;

    for( auto& link : links ) {
        int i = link.first, j = link.second;
        //Edge (i, j) so that parentOf(j) is i.
        assert( j > 0 && j < nvertices );
        assert( i >=0 && i < nvertices );
        assert( parentMap.find( j ) == parentMap.end() );
        parentMap[ j ] = i;
    }
    if( elements != nullptr ) assert( elements->size() == nvertices );

    for( int i = 0; i < nvertices; ++i )
        treeNodes[ i ] = new TreeNode( i );
    for( auto& link : links ) {
        int i = link.first, j = link.second;
        treeNodes[ i ]->children.push_back( treeNodes[ j ] );
    }

    return Tree ( treeNodes[ 0 ], nvertices );
}

struct TreeNodeData {
    static const int totalGiftCount = 3;

    int cost[ totalGiftCount ];
    int assignment[ totalGiftCount ];

    TreeNodeData() {
        for( int i = 0; i < totalGiftCount; ++i )
            cost[ i ] = assignment[ i ] = 0;
    }

    pair< int, int> min() {
        assert( totalGiftCount > 0 );
        int minValue = cost[ 0 ], minIndex = 0;
        for( int i = 1; i < totalGiftCount; ++i ) {
            if( cost[i] < minValue ) {
                minValue = cost[i];
                minIndex = i;
            }
        }
        return { minValue, minIndex };
    }
};

void computeMinCostLabel( TreeNode * subtree,
                          vector< TreeNodeData > & subtreeCost ) {
    assert( subtree != nullptr );
    if( subtree->isLeaf() ) return;

    int totalGiftCount = subtreeCost[ subtree->id ].totalGiftCount;

    for( TreeNode * ch : subtree->children ) {
        computeMinCostLabel( ch, subtreeCost );
    }
    for( int giftNo = 0; giftNo < totalGiftCount; ++giftNo ) {
        int minCost = 0;
        for( TreeNode * ch : subtree->children ) {
            int x = INT_MAX;
            for( int i = 0; i < totalGiftCount; ++i ) {
                if( i == giftNo ) continue;
                int y = subtreeCost[ ch->id ].cost[ i ];
                int possibleCost = ( i < giftNo ) ? y + 1 : y;
                if( possibleCost < x ) {
                    x = possibleCost;
                    subtreeCost[ ch->id ].assignment[ giftNo ] = i;
                }
            }
            minCost += x;
        }
        subtreeCost[ subtree->id ].cost[ giftNo ] = minCost;
    }
}

int giggleGiftMinCostLabel( const Tree& tree ) {
    assert( !tree.isEmpty() );

    TreeNode * root = tree.root;
    int V = tree.nvertices;

    vector< TreeNodeData > subtreeCost( V, TreeNodeData() );
    computeMinCostLabel( root, subtreeCost );
    pair< int, int > result = subtreeCost[ root->id ].min();

    int minCost = result.first, minIndex = result.second;
    printf( "Minimum assignment cost possible = %d\n", minCost );

    vector< TreeNode* > levelOrder;
    tree.traverseLevelOrder( levelOrder );

    int chIndex;
    queue< int > q;
    q.push( minIndex );
    for( TreeNode* n : levelOrder ) {
        if( n == nullptr ) { printf( "\n" ); continue; }
        chIndex = q.front(); q.pop();
        if( n == root ) {
            printf( "{%d,%d}\n", n->id, chIndex + 1 );
        }
        for( TreeNode* ch : n->children ) {
            int nextIndex = subtreeCost[ ch->id ].assignment[ chIndex ];
            printf( "{%d,%d} ", ch->id, nextIndex + 1 );
            q.push( nextIndex );
        }
    }
    assert( q.empty() );
}

void testGiggleGift() {
    vector< pair< int, int > > links = {
    {0,1}, {0,2}, {0,3}, {0,4},
    {1,5}, {2,6}, {2,7}, {3,8}, {4,9}, {4,10},
    {5,11}, {5,12}, {8,13}, {8,14}, {8,15}, {10,16},
    {12,17}, {13,18}, {16,19}, {16,20},
    {19,21}
    };
    Tree t = createTree( links, nullptr );
    giggleGiftMinCostLabel( t );
}

int largestCompleteBinarySubtree( TreeNode * subtree, int& maxValue,
                                  TreeNode * &completeTree ) {
    assert( subtree != nullptr );
    int nchildren = subtree->nChildren();
    int leftSubtree, rightSubtree;
    int sol;

    switch( nchildren ) {
        case 0: return 0;
        case 1:
            leftSubtree = largestCompleteBinarySubtree( subtree->children[ 0 ],
                                                        maxValue, completeTree );
            return 0;
        case 2:
            leftSubtree = largestCompleteBinarySubtree( subtree->children[ 0 ],
                                                        maxValue, completeTree );
            rightSubtree = largestCompleteBinarySubtree( subtree->children[ 1 ],
                                                         maxValue, completeTree );

            sol = 1 + min( leftSubtree, rightSubtree );
            if( sol > maxValue ) {
                maxValue = sol;
                completeTree = subtree;
            }
            break;
        default: assert( false );
    }
    return sol;
}

int maxDisjointPathsInSubtree( TreeNode * subtree, int K, int& pathCount ) {
    if( subtree->isLeaf() ) return 0;
    int largestPathLen = INT_MIN;
    for( TreeNode * ch : subtree->children ) {
        int subtreePathLen = maxDisjointPathsInSubtree( ch, K, pathCount );
        largestPathLen = max( largestPathLen, subtreePathLen );
    }
    ++largestPathLen;
    if( largestPathLen == K ) {
        ++pathCount;
        largestPathLen = -1;
    }
    return largestPathLen;
}

int maxDisjointPaths( Tree tree, int K ) {
    int pathCount = 0;
    assert( !tree.isEmpty() );
    int d = maxDisjointPathsInSubtree( tree.root, K, pathCount );
    if( d == K )
        ++pathCount;
    return pathCount;
}

int messageRounds( TreeNode * subtree ) {
    if( subtree->isLeaf() ) return 0;
    int nchildren = subtree->nChildren();
    assert( nchildren > 0 );
    int minRounds = INT_MAX, maxRounds = INT_MIN;
    for( TreeNode * ch : subtree->children ) {
        int n = messageRounds( ch );
        minRounds = min( minRounds, n );
        maxRounds = max( maxRounds, n );
    }
    return max( 1 + maxRounds, nchildren + minRounds );
}

Tree createOrderedTree_N54() {
    vector< pair< int, int > > links = {
    {0,1}, {1,2}, {2,3}, {2,4}, {3,5}, {3,6}, {6,7}, {6,8}, {6,9}, {7,10},
    {7,11}, {9,12}, {11,13}, {13,14}, {13,15}, {13,16}, {13,17}, {13,18}, {17,19}, {1,20},
    {20,21}, {20,22}, {20,23}, {23,24}, {22,25}, {22,26}, {22,39}, {25,27}, {27,28}, {27,29},
    {29,30}, {22,31}, {31,32}, {32,33}, {32,34}, {32,35}, {35,36}, {35,37}, {36,38}, {39,40},
    {39,48}, {40,41}, {40,42}, {40,43}, {42,44}, {44,45}, {44,46}, {45,47}, {48,49}, {48,50},
    {50,51}, {51,52}, {51,53},
    };
    return createTree( links, nullptr );
}

Tree createBinaryUnorderedTree_N43() {
    vector< pair< int, int > > links = {
    {0,1}, {0,2}, {1,3}, {1,4}, {3,5}, {3,6}, {5,7}, {5,8}, {8,11}, {8,12},
    {6,9}, {9,10}, {2,13}, {2,14}, {14,16}, {14,15}, {15,17}, {17,18}, {18,20}, {18,21},
    {20,22}, {20,23}, {17,19}, {19,24}, {19,25}, {24,26}, {24,27}, {26,28}, {26,29}, {27,30},
    {27,31}, {32,40}, {40,41}, {40,42}, {32,33}, {33,34}, {33,35}, {35,36}, {35,37}, {36,38},
    {36,39}, {15,32}
    };
    return createTree( links, nullptr );
}

Tree createBinaryUnorderedTree_N19() {
    vector< pair< int, int > > links = {
    {0,1}, {0,2}, {1,3}, {1,4}, {2,5}, {3,6}, {3,7}, {4,8}, {4,9}, {5,10},
    {5,11}, {7,12}, {7,13}, {8,14}, {8,15}, {11,16}, {11,17}, {12,18},
    };
    return createTree( links, nullptr );
}

Tree createUnorderedTree_N12() {
    vector< pair< int, int > > links = {
    {0,1}, {0,2}, {1,3}, {1,4}, {1,5}, {2,6},
    {3,7}, {3,8}, {6,9}, {6,10}, {10, 11},
    };
    return createTree( links, nullptr );
}

void testLargestCompleteBinarySubtree() {
    Tree t = createBinaryUnorderedTree_N43();
    int maxValue = 0;
    TreeNode * completeTree = nullptr;
    largestCompleteBinarySubtree( t.root, maxValue, completeTree );
    printf( "Largest complete binary subtree: depth = %d, id = %d.\n",
            maxValue, completeTree->id );

    t = createBinaryUnorderedTree_N19();
    maxValue = 0;
    completeTree = nullptr;
    largestCompleteBinarySubtree( t.root, maxValue, completeTree );
    printf( "Largest complete binary subtree: depth = %d, id = %d.\n",
            maxValue, completeTree->id );
}

void testMaxDisjointPaths() {
    Tree t = createOrderedTree_N54();
    const int K = 3;
    int pathCount = maxDisjointPaths( t.root, K );
    printf( "Max disjoint paths of length %d = %d\n", K, pathCount );
}

void testMessageRounds() {
    Tree t = createUnorderedTree_N12();
    int count = messageRounds( t.root );
    printf( "Minimum number of message rounds = %d\n", count );
}

