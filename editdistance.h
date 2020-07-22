#ifndef __EDIT_DISTANCE_H__
#define __EDIT_DISTANCE_H__

class EditDistance {
public:
    enum { OPERATION_INVALID = -1,
           OPERATION_COPY = 0,
           OPERATION_REPLACE = 1,
           OPERATION_INSERT = 2,
           OPERATION_DELETE = 3,
           OPERATION_TWIDDLE = 4,

           //How many type of operations are supported?
           OPERATION_TOTAL_COUNT = 5,
    };
    static const inline vector< int > defaultEditCosts = { 1, 3, 2, 2, 0 };
    static const inline vector< string > operationTag = {
        "Copy", "Replace", "Insert", "Delete", "Twiddle"
    };
public:
    static int calculateEditDistance( const string& src, const string& dst,
                                      vector< char > & editSequence,
                                      const vector< int > & editCosts = defaultEditCosts ) {
        int M = src.size(), N = dst.size();
        typedef pair< int, char > State;
        vector< vector< State > > v( M+1, vector< State > ( N+1, State( 0, OPERATION_TOTAL_COUNT ) ) );

        for( int r = 1; r <= M; ++r )
            v[r][0] = { r, OPERATION_DELETE } ;
        for( int c = 1; c <= N; ++c )
            v[0][c] = { c, OPERATION_INSERT } ;

        for( int i = 1; i <= M; ++i ) {
            for( int j = 1; j <= N; ++j ) {
                //Optimal cost and operation.
                int optCost;
                char optOper;

                //If the characters are same, then we can copy the character from the
                //source to destination. If not, then we can apply a replace operation.
                //To apply the twiddle operation, if the last two characters in the source
                //is "XY", then the last two characters in the destination should be "YX".
                bool sameChar = ( src[i-1] == dst[j-1] );
                bool twiddlePossible = ( i > 1 && j > 1 && src[i-1] == dst[j-2] &&
                                         src[i-2] == dst[j-1] );

                if( sameChar ) {
                    optCost = v[i-1][j-1].first + editCosts[OPERATION_COPY];
                    optOper = OPERATION_COPY;
                } else {
                    optCost = v[i-1][j-1].first + editCosts[OPERATION_REPLACE];
                    optOper = OPERATION_REPLACE;
                }

                if( !sameChar ) {
                    //When the characters differ, we can apply either a delete operation
                    //or an insert operation.
                    int deleteCost = v[i-1][j].first + editCosts[OPERATION_DELETE];
                    if( deleteCost < optCost ) {
                        optCost = deleteCost;
                        optOper = OPERATION_DELETE;
                    }
                    int insertCost = v[i][j-1].first + editCosts[OPERATION_INSERT];
                    if( insertCost < optCost ) {
                        optCost = insertCost;
                        optOper = OPERATION_INSERT;
                    }
                }
                if( twiddlePossible ) {
                    int twiddleCost = v[i-2][j-2].first + editCosts[OPERATION_TWIDDLE];
                    if( twiddleCost < optCost ) {
                        optCost = twiddleCost;
                        optOper = OPERATION_TWIDDLE;
                    }
                }
                v[i][j] = { optCost, optOper };
            }
        }
        int i = M, j = N;
        while( true ) {
                if( i + j == 0 ) break;
                char optOper = v[i][j].second;
                editSequence.push_back( optOper );
                switch( optOper ) {
                    case OPERATION_TWIDDLE: i-=2; j-=2; break;
                    case OPERATION_INSERT: --j; break;
                    case OPERATION_DELETE: --i; break;
                    case OPERATION_REPLACE:
                    case OPERATION_COPY:
                        --i; --j; break;
                    default:
                        assert( false );
                }
        }
        reverse( editSequence.begin(), editSequence.end() );

        return v[M][N].first;
    }
    static void printEditSequence( const string& src, const string& dst,
                                   vector< char > & editSequence,
                                   const vector< int > & editCosts = defaultEditCosts ) {
        int i = 0, j = 0;
        int cost = 0;
        string target;

        printf( "Source = [%s] Target = [%s]\n", src.c_str(), dst.c_str() );
        for( char optOper : editSequence ) {
            assert( optOper > OPERATION_INVALID && optOper < OPERATION_TOTAL_COUNT );

            switch( optOper ) {
                case OPERATION_COPY: target.append( 1, src[i] ); ++i; ++j; break;
                case OPERATION_REPLACE: target.append( 1, dst[j] ); ++i; ++j; break;
                case OPERATION_INSERT: target.append( 1, dst[j] ); ++j; break;
                case OPERATION_DELETE: ++i; break;
                case OPERATION_TWIDDLE:
                    target.append( 1, src[i+1] );
                    target.append( 1, src[i] );
                    i += 2; j += 2; break;
            }

            printf( "[%-20s] [%-20s] [ cost = %d ]\n", operationTag[ optOper ].c_str(),
                    target.c_str(), editCosts[ optOper ] );
            cost += editCosts[ optOper ] ;
        }
        printf( "Total cost = %d\n", cost );
    }
};

class EditDistanceTest {
public:
    static void runTest() {
        editDistanceTest();
    }
private:
    static void editDistanceTest() {
        vector< char > editSequence;
        string src = "ALGORITHM", target = "ALTRUISTIC";
        int cost = EditDistance::calculateEditDistance( src, target, editSequence );
        printf( "Total edit distance = %d\n", cost );
        EditDistance::printEditSequence( src, target, editSequence );
    }
};

#endif // __EDIT_DISTANCE_H__
