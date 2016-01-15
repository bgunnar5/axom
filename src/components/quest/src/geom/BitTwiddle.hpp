
#ifndef BIT_TWIDDLE_HXX_
#define BIT_TWIDDLE_HXX_

#include "quest/Point.hpp"
#include "quest/Vector.hpp"

#ifdef USE_CXX11
    #include <type_traits>
#else
    #include <boost/type_traits.hpp>
    # ifndef BOOST_NO_SFINAE
    #  include <boost/utility/enable_if.hpp>
    # endif
#endif

#include <limits>           // for numeric_limits

/**
 * \file
 * \brief Some helper functions for efficient bitwise operations.
 *
 * Currently has some function to convert Points to/from Morton indices,
 * and a hash functor that uses these on Point types.
 *
 */

namespace {

}

namespace quest
{
    /**
     * \brief A type for the morton index
     * \note For now, we will assume that we always want a size_t since that is what std::hash expects.
     *       If we need to change this later, we can add an additional template parameter to the
     *       Mortonizer class.
     */
    typedef std::size_t MortonIndex;

    /**
     * \class
     * \brief Base class for Dimension independent Morton indexing
     * \note Uses CRTP to access dimension-depended data from the derived class
     * \note This class only works for integral CoordTypes
     */
    template<typename CoordType, typename Derived>
    struct MortonBase
    {
        // static assert to ensure that this class is only instantiated on integral types
#ifdef USE_CXX11
        static_assert( std::is_integral<CoordType>::value, "Coordtype must be integral for Morton indexing" );
#else
        BOOST_STATIC_ASSERT(boost::is_integral<CoordType>::value);
#endif

    private:
        // Magic numbers for efficient base-2 log-like function -- maxSetBit()
        static const CoordType MaxBit_B[];
        static const int MaxBit_S[];

    protected:

        /**
         * \brief Expands bits in bitwise representation of an integral type and fills holes with zero
         * \param [in] x The integral type that we are expanding
         * \return An expanded MortonIndex
         * In dimension D, it adds (D-1) zeros between each bit,
         * so, e.g. in 2D, 6 == 0b0110 becomes 0b*0*1*1*0 == 0b00010100 == 20
         * \pre Number of bits must fit into a MortonIndex's representation
         * \todo We might be able to reduce the number of iterations MAX_ITER
         *       based on the number of bits in CoordType.
         */
        static MortonIndex expandBits(MortonIndex x)
        {
            SLIC_ASSERT_MSG( maxSetBit(x) <= Derived::MAX_BITS
                             , "Mortonizer: Morton indexing in " << Derived::NDIM << "D"
                             <<" currently only supports "<< Derived::MAX_BITS << " bits per coordinate. "
                             << "Attempted to index an integer (" << x <<") with " <<  maxSetBit(x) << " bits.");

            for(int i=Derived::MAX_ITER; i >= 0; --i)
            {
                x = (x | (x << Derived::S[i])) & Derived::B[i];
            }

            return x;
        }

        /**
         * \brief Contracts bits in bitwise representation of x
         * \param [in] x The Morton index that we are contracting
         * \return A contracted MortonIndex
         * In dimension D, it retains every (D-1)\f$^th\f$ bit,
         * so, e.g. in 2D, 20 = 0b00010100 == 0b*0*1*1*0 becomes  0b0110 = 6
         * \todo We might be able to reduce the number of iterations MAX_ITER
         *       based on the number of bits in CoordType.
         */
        static MortonIndex contractBits(MortonIndex x)
        {
            for(int i=0; i < Derived::MAX_ITER; ++i)
            {
                x = (x | (x >> Derived::S[i])) & Derived::B[i+1];
            }

            return x;
        }

    public:
        /**
         * \brief Finds the index of the maximum set bit (MSB) in an integral type
         * \todo We might be able to reduce the number of iterations MAX_ITER
         *       based on the number of bits in CoordType.
         */
        static int maxSetBit(CoordType x)
        {
            CoordType res = 0;
            for(int i= Derived::MAX_ITER; i >= 0; --i)
            {
                if(x & MaxBit_B[i])
                {
                    x >>= MaxBit_S[i];
                    res |= MaxBit_S[i];
                }
            }

            return static_cast<int>(res);
        }

    };


    /**
     * \class
     * \brief Helper class for MortonIndexing of a point's coordinate
     * The Morton index of a point interleaves the bits of its coordinates
     * (with the least significant bit coming from the x-coordinate
     * E.g. if we have a point in 2D (6,3) == (0b0110, 0b0011) in its binary representation.
     * If we label the individual set bits we get (0b0ab0, 0b00yz).
     * When we expand the bits by inserting a 0 (denoted with *), we get (0b*0*a*b*0, 0b*0*0*y*z).
     * Finally, by shifting the y-coordinate and interleaving, we get
     * the Morton index of this point: 0b000yazb0 == 0b00011110 ==  30
     */
    template<typename CoordType, int DIM>
    struct Mortonizer;

    /**
     * \class
     * \brief A 2D instance of Mortonizer to convert between Grid points and MortonIndexes
     * where a grid point is a point whose coordinates are integral types
     * Expand bits will add a 0 between every bit,
     * and contract bits will remove every other bit
     * \see Mortonizer
     */
    template<typename CoordType>
    struct Mortonizer<CoordType,2> : public MortonBase<CoordType, Mortonizer<CoordType, 2> >
    {
        typedef MortonBase<CoordType, Mortonizer<CoordType, 2> > Base;


        // Magic numbers in 3D
        static const MortonIndex B[];
        static const int S[];

        enum { NDIM = 2
            ,  COORD_BITS = std::numeric_limits<CoordType>::digits
            ,  MORTON_BITS = std::numeric_limits<MortonIndex>::digits
            ,  MB_PER_DIM = MORTON_BITS / NDIM
            ,  MAX_BITS = (MB_PER_DIM < COORD_BITS) ? MB_PER_DIM : COORD_BITS
            ,  MAX_ITER = 5
        };

        /**
         * \brief A function to convert a 2D point to a Morton index
         *
         * Morton indexing interleaves the bits of the point's coordinates
         * \param [in] x,y The coordinates of the point
         * \pre CoordType must be an integral type
         * \pre x and y must be positive with value less than \f$ 2^{16} \f$
         * \note These preconditions can easily be relaxed, if desired
         * (e.g. to 32 bits for 64 bit Morton indices)
         * \return The MortonIndex of the 2D point
         */
        static inline MortonIndex mortonize(CoordType x, CoordType y)
        {
            return (  Base::expandBits(x)
                   | (Base::expandBits(y)<< 1) );
        }

        /**
         * \brief A function to convert a 2D point to a Morton index
         * \see mortonize(CoordType, CoordType)
         */
        static inline MortonIndex mortonize(const Point<CoordType,NDIM> & pt)
        {
            return (  Base::expandBits(pt[0])
                   | (Base::expandBits(pt[1])<< 1) );
        }


        /**
         * \brief A function to convert a Morton index back to a 2D point
         *
         * \param [in] morton The MortonIndex of the desired point
         * \param [out] x,y The coordinates of the point
         *  Morton indexing interleaves the bits of the point's coordinates
         * \return The point's coordinates are in the x and y parameters
         */
        static inline void demortonize(MortonIndex morton, CoordType &x, CoordType & y)
        {
            const MortonIndex& b0 = B[0];

            x = static_cast<CoordType>( Base::contractBits( morton      & b0));
            y = static_cast<CoordType>( Base::contractBits((morton >>1) & b0));
        }

        /**
         * \brief A function to convert a Morton index back to a 2D point
         * \see demortonize(MortonIndex,CoordType,CoordType)
         */
        static inline Point<CoordType,NDIM> demortonize(MortonIndex morton)
        {
            Point<CoordType,NDIM> pt;
            demortonize(morton, pt[0],pt[1]);
            return pt;
        }
    };

    /**
     * \class
     * \brief A 3D instance of Mortonizer to convert between Grid points and MortonIndexes
     * where a grid point is a point whose coordinates are integral types
     * Expand bits will add two zeros between every bit,
     * and contract bits will remove every other bit
     * \see Mortonizer
     */
    template<typename CoordType>
    struct Mortonizer<CoordType,3>: public MortonBase<CoordType, Mortonizer<CoordType, 3> >
    {
        typedef MortonBase<CoordType, Mortonizer<CoordType, 3> > Base;

        static const MortonIndex B[];
        static const int S[];

        enum { NDIM = 3
            ,  COORD_BITS = std::numeric_limits<CoordType>::digits
            ,  MORTON_BITS = std::numeric_limits<MortonIndex>::digits
            ,  MB_PER_DIM = MORTON_BITS / NDIM
            ,  MAX_BITS = (MB_PER_DIM < COORD_BITS) ? MB_PER_DIM : COORD_BITS
            ,  MAX_ITER = 5
        };

        /**
         * \brief A function to convert a 3D point to a Morton index
         *
         *  Morton indexing interleaves the bits of the point's coordinates
         * \param [in] x,y,z The coordinates of the point
         * \pre CoordType must be an integral type
         * \pre x and y must be positive with value less than \f$ 2^{10} \f$
         * \note These preconditions can easily be relaxed, if desired
         * (e.g. to 21 bits for 64 bit Morton indices)
         * \return The MortonIndex of the 2D point
         */
        static inline MortonIndex mortonize(CoordType x, CoordType y, CoordType z)
        {
            return (  Base::expandBits(x)
                   | (Base::expandBits(y) << 1)
                   | (Base::expandBits(z) << 2) );
        }

        /**
         * \brief A function to convert a 3D point to a Morton index
         * \see mortonize(CoordType, CoordType, CoordType)
         */
        static inline MortonIndex mortonize(const Point<CoordType,NDIM> & pt)
        {
            return (  Base::expandBits(pt[0])
                   | (Base::expandBits(pt[1]) << 1)
                   | (Base::expandBits(pt[2]) << 2) );
        }

        /**
         * \brief A function to convert a Morton index back to a 3D point
         *
         * \param [in] morton The MortonIndex of the desired point
         * \param [out] x,y,z The coordinates of the point
         *  Morton indexing interleaves the bits of the point's coordinates
         * \return The point's coordinates are in the x, y and z parameters
         */
        static inline void demortonize(MortonIndex morton, CoordType &x, CoordType & y, CoordType &z)
        {
            const MortonIndex& b0 = B[0];

            x = static_cast<CoordType>( Base::contractBits( morton      & b0));
            y = static_cast<CoordType>( Base::contractBits((morton >>1) & b0));
            z = static_cast<CoordType>( Base::contractBits((morton >>2) & b0));
        }

        /**
         * \brief A function to convert a Morton index back to a 3D point
         * \see demortonize(MortonIndex,CoordType,CoordType,CoordType)
         */
        static inline Point<CoordType,NDIM> demortonize(MortonIndex morton)
        {
            Point<CoordType,NDIM> pt;
            demortonize(morton, pt[0],pt[1],pt[2]);
            return pt;
        }

    };


    template<typename CoordType, typename Derived>
    const CoordType MortonBase<CoordType,Derived>::MaxBit_B[] = {
                        static_cast<CoordType>(0x2),
                        static_cast<CoordType>(0xC),
                        static_cast<CoordType>(0xF0),
                        static_cast<CoordType>(0xFF00),
                        static_cast<CoordType>(0xFFFF0000),
                        static_cast<CoordType>(0xFFFFFFFF00000000)};

    template<typename CoordType, typename Derived>
    const int MortonBase<CoordType,Derived>::MaxBit_S[] = {1, 2, 4, 8, 16, 32};


    template<typename CoordType>
    const MortonIndex Mortonizer<CoordType,2>::B[] = {
                        0x5555555555555555,     // 0101'0101
                        0x3333333333333333,     // 0011'0011
                        0x0F0F0F0F0F0F0F0F,     // 0000'1111
                        0x00FF00FF00FF00FF,     // 0x8  1x8
                        0x0000FFFF0000FFFF,     // 0x16 1x16
                        0x00000000FFFFFFFF };  //  0x32 1x32

    template<typename CoordType>
    const int Mortonizer<CoordType,2>::S[] = { 1, 2, 4, 8, 16, 32};


    // Magic numbers in 3D from C. Ericson's Real Time Collision Detection book
    template<typename CoordType>
    const MortonIndex Mortonizer<CoordType,3>::B[] = {
                        0x9249249249249249,     // 0010'0100'1001'0010'0100'1001
                        0x30C30C30C30C30C3,     // 0000'1100'0011'0000'1100'0011
                        0xF00F00F00F00F00F,     // 0000'0000'1111'0000'0000'1111
                        0x00FF0000FF0000FF,     // 0000'0000'0000'0000'1111'1111
                        0xFFFF00000000FFFF,     // x16
                        0x00000000FFFFFFFF };     // x32

    template<typename CoordType>
    const int Mortonizer<CoordType,3>::S[] = { 2, 4, 8, 16, 32, 64};

    /**
     * \brief A helper function to convert a 2D point directly to a MortonIndex
     * \return The Morton index of the 2D point
     */
    template<typename CoordType>
    inline MortonIndex convertPointToMorton2D(const Point<CoordType,2>& pt)
    {
        return Mortonizer<CoordType,2>::mortonize(pt);
    }

    /**
     * \brief A helper function to convert a 3D point directly to a MortonIndex
     * \return The Morton index of the 3D point
     */
    template<typename CoordType>
    inline MortonIndex convertPointToMorton3D(const Point<CoordType,3>& pt)
    {
        return Mortonizer<CoordType,3>::mortonize(pt);
    }

    /**
     * \brief A helper function to convert MortonIndex back to a 2D point
     * \return The demortonized 2D Point
     */
    template<typename CoordType>
    inline Point<CoordType,2> convertMortonToPoint2D(MortonIndex idx)
    {
        return Mortonizer<CoordType,2>::demortonize(idx);
    }

    /**
     * \brief A helper function to convert MortonIndex back to a 3D point
     * \return The demortonized 3D Point
     */
    template<typename CoordType>
    inline Point<CoordType,3> convertMortonToPoint3D(MortonIndex idx)
    {
        return Mortonizer<CoordType,3>::demortonize(idx);
    }

    /**
     * \class
     * \brief A functor class for Mortonizing points.
     *
     * This can be used as a hashing function for Points in dimensions 1..4
     */
    template<typename CoordType>
    struct PointHash
    {
        /**
         * \brief Mortonizes a coordinate (viewed as a 1D point)
         * \note This is a no-op and is provided for genericity in point dimension
         * \param [in] coord The coordinate of the
         * \returns The morton index of the 1D point
         */
        std::size_t operator()(CoordType & coord) const
        {
            return static_cast<std::size_t>(coord);
        }

        /**
         * \brief Mortonizes a 1D point
         * \note This is a no-op and is provided for genericity in point dimension
         * \param [in] pt The 1D point
         * \returns The morton index of the point
         */
        std::size_t operator()(Point<CoordType,1> const& pt) const
        {
            return static_cast<std::size_t>(pt[0]);
        }

        /**
         * \brief Mortonizes a 2D point
         * \param [in] pt The 2D point
         * \returns The morton index of the point
         */
        std::size_t operator()(Point<CoordType,2> const& pt) const
        {
            return Mortonizer<CoordType,2>::mortonize(pt);
        }

        /**
         * \brief Mortonizes a 3D point
         * \param [in] pt The 3D point
         * \returns The morton index of the point
         */
        std::size_t operator()(Point<CoordType,3> const& pt) const
        {
            return Mortonizer<CoordType,3>::mortonize(pt);
        }

        /**
         * \brief Mortonizes a 4D point
         * \param [in] pt The 4D point
         * \returns A morton index of the point
         */
        std::size_t operator()(Point<CoordType,4> const& pt) const
        {
            // Mortonize two morton indices for a 4D morton index

            typedef Point<MortonIndex,2> Pt2M;

            Pt2M pMorton = Pt2M::make_point(
                      Mortonizer<CoordType,2>::mortonize(pt[0], pt[2])
                    , Mortonizer<CoordType,2>::mortonize(pt[1], pt[3]));

            return Mortonizer<MortonIndex,2>::mortonize(pMorton);
        }
    };

} // end namespace quest

#endif  // BIT_TWIDDLE_HXX_
