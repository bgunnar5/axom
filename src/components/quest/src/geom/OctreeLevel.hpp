
#ifndef OCTREE_LEVEL__HXX_
#define OCTREE_LEVEL__HXX_


#include "common/config.hpp"

#include "fmt/fmt.hpp"
#include "slic/slic.hpp"

#include "quest/MortonIndex.hpp"
#include "quest/NumericArray.hpp"
#include "quest/Point.hpp"
#include "quest/Vector.hpp"


#ifdef USE_CXX11
#include <unordered_map>
#else
#include "boost/unordered_map.hpp"
#endif

#include <boost/iterator/iterator_facade.hpp>

/**
 * \file
 * \brief Defines templated OctreeLevel class
 * An OctreeLevel associates data with the integer points on a sparse grid.
 */

namespace quest
{

    /**
     * \brief Helper enumeration for status of a BlockIndex within an OctreeLevel instance
     */
    enum TreeBlockStatus {
          BlockNotInTree   // Status of blocks that are not in the tree
        , LeafBlock        // Status of blocks that are leaves in the tree
        , InternalBlock    // Status of blocks that are internal to the tree
    };

    /**
     * \class
     * \brief A class to represent a sparse level of blocks within an octree.
     * Each block is associated with an integer grid point whose coordinates
     * have values between 0 and 2^L (where L = this->level() is the encoded level).
     * The OctreeLevel associates data of (templated) type BlockDataType with each such block.
     * \note BlockDataType must define a predicate function with the signature: bool isLeaf() const;
     */
    template<int DIM, typename BlockDataType>
    class OctreeLevel
    {
    public:
        typedef int CoordType;
        typedef quest::Point<CoordType,DIM> GridPt;
        typedef quest::Vector<CoordType,DIM> GridVec;

    private:
        /**
         * \class
         * \brief Private inner class to handle subindexing of block data within octree siblings
         * \note A brood is a collection of siblings that are generated simultaneously.
         * \note This class converts a grid point at the given level into a brood index of the point.
         *       The base brood point is that of the grid point's octree parent
         *       and its offset index is obtained by interleaving the least significant bit of its coordinates.
         */
      struct Brood {
          enum { NUM_CHILDREN = 1 << DIM };

          /**
           * \brief Constructor for a brood offset relative to the given grid point pt
           * \param [in] pt The grid point within the octree level
           */
          Brood(const GridPt& pt)
              : m_broodPt( pt.array() /2), m_idx(0)
          {
              for(int i=0; i< DIM; ++i)
              {
                  m_idx |= (pt[i]& 1) << i; // interleave the least significant bits
              }
          }

          /** \brief Accessor for the base point of the entire brood */
          const GridPt& base() const { return m_broodPt; }

          /** \brief Accessor for the index of the point within the brood */
          const int& index() const { return m_idx; }

      private:
          GridPt m_broodPt;  /** Base point of all blocks within the brood */
          int m_idx;         /** Index of the block within the brood. Value is in [0, 2^DIM) */
      };

    public:

        // A brood is a collection of sibling blocks that are generated simultaneously
        typedef quest::NumericArray< BlockDataType, Brood::NUM_CHILDREN> BroodData;

      #if defined(USE_CXX11)
        typedef std::unordered_map<GridPt, BroodData, PointHash<int> > MapType;
      #else
        typedef boost::unordered_map<GridPt, BroodData, PointHash<int> > MapType;
      #endif

        typedef typename MapType::iterator       MapIter;
        typedef typename MapType::const_iterator ConstMapIter;

        template<typename OctreeLevel, typename InnerIterType, typename DataType> class BlockIterator;

        typedef BlockIterator<OctreeLevel, MapIter, BlockDataType> BlockIter;
        typedef BlockIterator<const OctreeLevel, ConstMapIter, const BlockDataType> ConstBlockIter;


    public:

        /**
         * \class
         * \brief An iterator type for the blocks of an octree level
         */
        template<typename OctreeLevel, typename InnerIterType, typename DataType>
        class BlockIterator : public boost::iterator_facade< BlockIterator<OctreeLevel, InnerIterType, DataType>
                                   , DataType
                                   , boost::forward_traversal_tag
                                   , DataType
                                   >
        {
        public:
          typedef BlockIterator<OctreeLevel, InnerIterType, DataType>              iter;

          BlockIterator(OctreeLevel* octLevel, bool begin = false)
              : m_octLevel(octLevel), m_data(ATK_NULLPTR), m_idx(0)
          {
              SLIC_ASSERT(octLevel != ATK_NULLPTR);

              m_endIter = m_octLevel->m_map.end();
              m_levelIter = begin ? m_octLevel->m_map.begin() : m_endIter;

              update();
          }

          /**
           * \brief A const dereference function used for
           * \note Only valid for constant access to data associated with an octree block
           * \note For non-const access on a non-const accessor, use the data() function
           */
          const DataType& dereference() const { return *m_data; }

          /**
           * \brief Const accessor for the iterator's current grid point
           */
          const GridPt& pt() const { return m_pt; }
          
          /**
           * \brief Non-const accessor for data associated with the iterator's current grid point
           */
          DataType& data() { return *m_data; }

          /**
           * \brief Const accessor for data associated with the iterator's current grid point
           */
          const DataType& data() const { return *m_data; }

          /**
           * \brief Equality test against another iterator
           * \param other The other iterator
           * \return true, if the two iterators are equal, false otherwise
           */
          bool equal(const iter& other) const
          {
              return (m_octLevel == other.m_octLevel)       // point to same object
                   && (m_levelIter == other.m_levelIter)   // iterators are the same
                   && (m_idx == other.m_idx);               // brood indices are the same
          }

          /**
           * \brief Increment the iterator to the next point
           */
          void increment()
          {
              ++m_idx;

              if(m_idx == Brood::NUM_CHILDREN || m_octLevel->m_level == 0)
              {
                  ++m_levelIter;
                  m_idx = 0;
              }

              update();
          }

        private:
          /**
           * \brief Utility function to update the iterator's data after an increment
           */
          void update()
          {
              // Test to see if we have finished iterating
              if(m_levelIter == m_endIter)
                  return;

              // Reconstruct the grid point from its brood representation
              const GridPt& itPt = m_levelIter->first;
              for(int i=0; i<DIM; ++i)
                  m_pt[i] = (itPt[i]<<1) + ( m_idx & (1 << i)? 1 : 0);

              m_data = &m_levelIter->second[m_idx];
          }

        private:
          friend class boost::iterator_core_access;
          OctreeLevel*  m_octLevel;             /** Pointer to the iterator's container class */
          InnerIterType m_levelIter, m_endIter; /** Iterator's into the level's data map */

          GridPt        m_pt;   /** Current grid point associated with iterator */
          DataType*     m_data; /** Pointer to data associated with iterator's current grid point */
          int           m_idx;  /** Index of current point w.r.t. brood's base point (pointed to by m_levelIter.first) */

        };

    public:

        /**
         * \brief Default constructor for an octree level
         */
        OctreeLevel(int level = -1): m_level(level){}

        /**
         * \brief Returns the maximum coordinate value in the level
         * \note This is (2^L -1), where L is the current level
         */
        CoordType maxCoord() const
        {
            return (1<< m_level) -1;
        }

        /**
         * \brief Returns a GridPt whose coordinates are set to maxCoord
         * \sa maxCoord()
         */
        GridPt maxGridCell() const
        {
            return GridPt(maxCoord());
        }

        /**
         * \brief Predicate to check whether the block associated with the given GridPt pt is a leaf block
         */
        bool isLeaf(const GridPt& pt) const { return blockStatus(pt) == LeafBlock; }

        /**
         * \brief Predicate to check whether the block associated with the given GridPt pt is an internal block
         */
        bool isInternal(const GridPt& pt) const { return blockStatus(pt) == InternalBlock; }

        /**
         * \brief Predicate to check whether the block associated with the given GridPt pt is in the current level
         */
        bool hasBlock(const GridPt& pt) const
        {
            const Brood brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());
            return blockIt != m_map.end();
        }

        /**
         * \brief Adds all children of the given grid point to the octree level
         * \param [in] pt The gridPoint associated with the parent of the children that are being added
         * \pre pt must be in bounds for the level
         * \sa inBounds()
         */
        void addAllChildren(const GridPt& pt)
        {
            SLIC_ASSERT_MSG(inBounds(pt)
                           , "Problem while inserting children of point " << pt
                           << " into octree level " << m_level
                           << ". Point was out of bounds -- "
                           << "each coordinate must be between 0 and " << maxCoord() << ".");

            m_map[pt];  // Adds children, if not already present, using default BlockDataType() constructor
        }

        /**
         * \brief Predicate to check whether the block associated with the given GridPt pt is an allowed block ih the level
         * \param [in] pt The gridpoint of the block to check
         * \note pt is inBounds if each of its coordinates is a non-negative integer less than maxCoord()
         * \sa maxCoord()
         */
        bool inBounds(const GridPt& pt) const
        {
            const CoordType maxVal = maxCoord();
            for(int i=0; i< DIM; ++i)
                if( pt[i] < 0 || pt[i] > maxVal)
                    return false;
            return true;
        }

        /** \brief Accessor for the data associated with pt */
        BlockDataType& operator[](const GridPt& pt)
        {
            const Brood brood(pt);
            return m_map[brood.base()][brood.index()];
        }

        /** \brief Const accessor for the data associated with pt */
        const BlockDataType& operator[](const GridPt& pt) const
        {
            SLIC_ASSERT_MSG(hasBlock(pt)
                            ,"(" << pt <<", "<< m_level << ") was not a block in the tree at level.");

            // Note: Using find() method on hashmap since operator[] is non-const
            const Brood brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());
            return blockIt->second[brood.index()];
        }


        /** \brief Begin iterator to points and data in tree level */
        BlockIter      begin()       { return BlockIter(this,true); }

        /** \brief Const begin iterator to points and data in tree level */
        ConstBlockIter begin() const { return ConstBlockIter(this,true); }

        /** \brief End iterator to points and data in tree level */
        BlockIter      end()         { return BlockIter(this,false); }

        /** \brief Const end iterator to points and data in tree level */
        ConstBlockIter end()   const { return ConstBlockIter(this,false); }

        /**
         * \brief Predicate to check if there are any blocks in this octree level
         */
        bool empty() const { return m_map.empty(); }

        /**
         * \brief Helper function to determine the status of an octree block within this octree level
         * \param pt The grid point of the block index that we are testing
         * \return The status of the grid point pt (e.g. LeafBlock, InternalBlock, ...)
         */
        TreeBlockStatus blockStatus(const GridPt & pt) const
        {
            const Brood brood(pt);
            ConstMapIter blockIt = m_map.find(brood.base());

            return (blockIt == m_map.end())
                    ? BlockNotInTree
                    : (blockIt->second[brood.index()].isLeaf())
                        ? LeafBlock
                        : InternalBlock;
        }

    //private:
    //  DISABLE_COPY_AND_ASSIGNMENT(OctreeLevel);

    private:
      MapType m_map;
      int m_level;
    };


} // end namespace quest

#endif  // OCTREE_LEVEL__HXX_
