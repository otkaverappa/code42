#ifndef __BIT_VECTOR_H__
#define __BIT_VECTOR_H__

class BitVector {
private:
    unsigned int bitVector;
    int numberOfBits;
private:
    void boundsCheck( int bitNumber ) {
        assert( bitNumber >= 0 && bitNumber < numberOfBits );
    }
public:
    BitVector( int numberOfBits ) : numberOfBits( numberOfBits ), bitVector(0) {
        assert( numberOfBits > 0 &&
                numberOfBits <= sizeof( unsigned int ) * 8 );
    }
    bool set( int bitNumber ) {
        boundsCheck( bitNumber );
        bitVector |= ( 0x1 << bitNumber );
    }
    bool reset( int bitNumber ) {
        boundsCheck( bitNumber );
        bitVector &= ~( 0x1 << bitNumber );
    }
    bool isSet( int bitNumber ) {
        boundsCheck( bitNumber );
        return bitVector & ( 0x1 << bitNumber );
    }
    bool any() {
        return bitVector != 0;
    }
    bool all() {
        return bitVector == ( ( 0x1 << numberOfBits ) - 1 );
    }
    int popcount() {
        int count = 0;
        unsigned int v = bitVector;
        while( v ) {
            v &= v - 1;
            ++count;
        }
        return count;
    }
    bool operator < ( const BitVector & other ) const {
        return bitVector < other.bitVector ;
    }
};

class BitVectorTest {
public:
    static void runTest() {
        int numberOfBits = 30;
        BitVector v( numberOfBits );

        for( int i = 0; i < numberOfBits; ++i ) {
            assert( v.popcount() == i );
            i == 0 ? assert( !v.any() ) : assert( v.any() );

            assert( ! v.isSet( i ) );
            v.set( i );
            assert( v.isSet( i ) );

            assert( v.popcount() == i + 1 );
            i == numberOfBits - 1 ? assert( v.all() ) : assert( !v.all() );
        }

        for( int i = 0; i < numberOfBits; ++i ) {
            assert( v.popcount() == numberOfBits - i );
            i == 0 ? assert( v.all() ) : assert( !v.all() );

            assert( v.isSet( i ) );
            v.reset( i );
            assert( !v.isSet( i ) );

            assert( v.popcount() == numberOfBits - i - 1 );
            i == numberOfBits - 1 ? assert( !v.any() ) : assert( v.any() );
        }
    }
};

#endif // __BIT_VECTOR_H__
