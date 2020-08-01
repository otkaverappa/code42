#include "editdistance.h"

int EditDistance::calculateEditDistance( const string& src, const string& dst,
                                         vector< char > & editSequence,
                                         const vector< int > & editCosts ) {
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

void EditDistance::printEditSequence( const string& src, const string& dst,
                                      vector< char > & editSequence,
                                      const vector< int > & editCosts ) {
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

int EditDistance::uniformEditDistance( const string& src, const string& dst ) {
    //Cost of COPY is set to 0, and cost of REPLACE, INSERT, DELETE is set to 1.
    //Cost of TWIDDLE is set to a high value so that it is effectively disabled.
    vector< int > editCosts = { 0, 1, 1, 1, 0xFFFF };
    vector< char > editSequence;
    return EditDistance::calculateEditDistance( src, dst, editSequence, editCosts );
}

bool EditDistance::isUnitEditDistance( const string& src, const string & dst ) {
    int N;
    if( ( N = src.size() ) != dst.size() )
        return false;
    int diffCount = 0;
    for( int i = 0; i < N; ++i ) {
        if( src[i] != dst[i] )
            ++diffCount;
        if( diffCount > 1 )
            return false;
    }
    return diffCount == 1 ;
}

void EditDistanceTest::editDistanceTest() {
    vector< char > editSequence;
    string src = "ALGORITHM", target = "ALTRUISTIC";
    int cost = EditDistance::calculateEditDistance( src, target, editSequence );
    printf( "Total edit distance = %d\n", cost );
    EditDistance::printEditSequence( src, target, editSequence );
    printf( "Uniform edit distance = %d\n", EditDistance::uniformEditDistance( src, target ) );
}
