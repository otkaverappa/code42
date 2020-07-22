#ifndef __BIT_VECTOR_H__
#define __BIT_VECTOR_H__

class BitVector {
private:
    unsigned int * bitVector;
    unsigned int fractionalBits;
    unsigned int numberOfBits;
    unsigned int size_;
    static const int bitsPerInteger = sizeof( unsigned int ) * 8;
private:
    void boundsCheck( int bitNumber ) {
        assert( bitNumber >= 0 && bitNumber < numberOfBits );
    }
public:
    BitVector( unsigned int numberOfBits ) : numberOfBits( numberOfBits ) {
        fractionalBits = numberOfBits % bitsPerInteger;
        size_ = ( numberOfBits / bitsPerInteger ) + ( fractionalBits ? 1 : 0 );

        bitVector = new unsigned int [ size_ ] ();
    }
    ~BitVector() { delete [] bitVector; }
    bool set( int bitNumber ) {
        boundsCheck( bitNumber );
        bitVector[ bitNumber / bitsPerInteger ] |= ( 0x1 << ( bitNumber % bitsPerInteger ) );
    }
    bool reset( int bitNumber ) {
        boundsCheck( bitNumber );
        bitVector[ bitNumber / bitsPerInteger ] &= ~( 0x1 << ( bitNumber % bitsPerInteger ) );
    }
    bool isSet( int bitNumber ) {
        boundsCheck( bitNumber );
        return bitVector[ bitNumber / bitsPerInteger ] & ( 0x1 << ( bitNumber % bitsPerInteger ) );
    }
    bool any() {
        for( int i = 0; i < size_; ++i ) {
            if( bitVector[i] != 0 )
                return true;
        }
        return false;
    }
    bool all() {
        int i = 0;
        for( ; i < size_ - 1; ++i ) {
            if( bitVector[i] != ~0 )
                return false;
        }
        //i is the last valid index for bitVector.
        if( fractionalBits == 0 )
            return bitVector[i] == ~0;
        return bitVector[i] == ~ ( ~0 << fractionalBits );
    }
    int popcount() {
        int count = 0;
        for( int i = 0; i < size_; ++i ) {
            unsigned int v = bitVector[i];
            while( v ) {
                v &= v - 1;
                ++count;
            }
        }
        return count;
    }
    bool operator < ( const BitVector & other ) const {
        for( int i = 0; i < min( size_, other.size_); ++i ) {
                if( bitVector[i] == other.bitVector[i] )
                    continue;
                return bitVector[i] < other.bitVector[i];
        }
        return false;
    }
};

class BitVectorTest {
public:
    static void runTest() {
        bitVectorTest( sizeof( unsigned int ) * 8 - 2 );
        bitVectorTest( sizeof( unsigned int ) * 8 );
        bitVectorTest( sizeof( unsigned int ) * 42 + 42 );
    }
private:
    static void bitVectorTest( int numberOfBits ) {
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
