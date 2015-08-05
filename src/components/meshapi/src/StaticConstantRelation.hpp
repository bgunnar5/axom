/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */


/**
 * \file StaticConstantRelation.hpp
 *
 * \brief API for a topological relation between two sets in which entities from the first set
 *        can be related to a constant number of entities from the second set
 *
 */

#ifndef MESHAPI_STATIC_CONSTANT_RELATION_HPP_
#define MESHAPI_STATIC_CONSTANT_RELATION_HPP_

#ifndef MESHAPI_STATIC_CONSTANT_RELATION_ITERATOR_USE_PROXY
//  #define MESHAPI_STATIC_CONSTANT_RELATION_ITERATOR_USE_PROXY
#endif


#include <vector>

//#include <iostream>

#include "slic/slic.hpp"
#include "meshapi/OrderedSet.hpp"
#include "meshapi/Relation.hpp"
#include "meshapi/PolicyTraits.hpp"


namespace asctoolkit {
namespace meshapi    {

  template< typename StridePolicy = policies::RuntimeStrideHolder<Set::PositionType>
          >
  class StaticConstantRelation : public Relation
                               , StridePolicy
  {
#ifdef MESHAPI_STATIC_CONSTANT_RELATION_ITERATOR_USE_PROXY
  private:
    /**
     * A small helper class to allow double subscripting on the relation
     */
    class SubscriptProxy {
    public:
      SubscriptProxy(RelationVecConstIterator it, SetPosition stride) : m_iter(it), m_stride(stride) {}
      SetPosition const& operator[](SetPosition index) const
      {
        SLIC_ASSERT_MSG( index < m_stride, "Inner array access out of bounds."
            << "\n\tPresented value: " << index
            << "\n\tMax allowed value: " << static_cast<int>(m_stride - 1));
        return m_iter[index];
      }
    private:
      RelationVecConstIterator m_iter;
      SetPosition m_stride;
    };
#endif

  public:

    typedef StridePolicy        StridePolicyType;

    //-----

    typedef Relation::SetPosition                                         SetPosition;

    typedef std::vector<SetPosition>                                      RelationVec;
    typedef RelationVec::iterator                                         RelationVecIterator;
    typedef std::pair<RelationVecIterator,RelationVecIterator>            RelationVecIteratorPair;

    typedef RelationVec::const_iterator                                   RelationVecConstIterator;
    typedef std::pair<RelationVecConstIterator,RelationVecConstIterator>  RelationVecConstIteratorPair;

    typedef typename policies::StrideToSize<StridePolicyType, SetPosition, StridePolicyType::DEFAULT_VALUE>::SizeType CorrespondingSizeType;
    typedef OrderedSet< CorrespondingSizeType      // The cardinality of each relational operator is determined by the StridePolicy of the relation
                      , policies::RuntimeOffsetHolder<Set::PositionType>
                      , policies::StrideOne<Set::PositionType>
                      , policies::STLVectorIndirection<Set::PositionType, Set::ElementType> > RelationSet;

  public:
    StaticConstantRelation (Set* fromSet = &s_nullSet, Set* toSet = &s_nullSet)
          : StridePolicy(CorrespondingSizeType::DEFAULT_VALUE), m_fromSet(fromSet), m_toSet(toSet) {}

    ~StaticConstantRelation(){}
    /**
     * \note TODO: swap this out for data in the datastore
     */
    void bindRelationData(const RelationVec & toOffsets, const SetPosition stride = StridePolicyType::DEFAULT_VALUE)
    {
      StridePolicyType::setStride(stride);

      m_toSetIndicesVec.clear();
      m_toSetIndicesVec.reserve(toOffsets.size());
      std::copy(toOffsets.begin(), toOffsets.end(), std::back_inserter(m_toSetIndicesVec));
    }

    RelationVecConstIterator  begin(SetPosition fromSetIndex)       const
    {
      verifyPosition(fromSetIndex);
      return m_toSetIndicesVec.begin() + toSetBeginIndex(fromSetIndex);
    }

    RelationVecConstIterator end(SetPosition fromSetIndex)         const
    {
      verifyPosition(fromSetIndex);
      return m_toSetIndicesVec.begin() + toSetEndIndex(fromSetIndex);
    }

    RelationVecConstIteratorPair range(SetPosition fromSetIndex)   const
    {
      return std::make_pair(begin(fromSetIndex), end(fromSetIndex));
    }

#ifdef MESHAPI_STATIC_CONSTANT_RELATION_ITERATOR_USE_PROXY
    const SubscriptProxy operator[](SetPosition fromSetElt) const
    {
      return SubscriptProxy( begin(fromSetElt), size(fromSetElt) );
    }
#else
    /**
     * This function returns the OrderedSet of all elements in the toSet related to 'fromSetElt' in the fromSet.
     */
    const RelationSet operator[](SetPosition fromSetElt) const
    {
        // Note -- we need a better way to initialize an indirection set
        RelationSet rel(size(fromSetElt),toSetBeginIndex(fromSetElt) );
        rel.data() = &m_toSetIndicesVec;

        return rel;
    }
#endif

    SetPosition size(SetPosition fromSetIndex = 0)                  const
    {
      verifyPosition(fromSetIndex);
      return stride();
    }

    bool isValid(bool verboseOutput = false) const;

  public:

    /**
     * \name DirectDataAccess
     * \brief Accessor functions to get the underlying relation data
     * \note We will have to figure out a good way to limit this access to situations where it makes sense.
     */

    /// \{
    /**
     * \brief Helper function to access the underlying relation data
     * \note The relation currently 'owns' the underlying vector.
     *       This will be changing soon, and we will only have a reference/pointer to the data.
     */
    RelationVec &       toSetPositionsData()       { return m_toSetIndicesVec; }

    /**
     * \brief Helper function to access the underlying relation data
     * \note The relation currently 'owns' the underlying vector.
     *       This will be changing soon, and we will only have a reference/pointer to the data.
     */
    const RelationVec & toSetPositionsData() const { return m_toSetIndicesVec; }

    /// \}
  private:
    inline void         verifyPosition(SetPosition fromSetIndex)    const { SLIC_ASSERT( fromSetIndex < m_fromSet->size() ); }
    inline SetPosition  toSetBeginIndex(SetPosition fromSetIndex)   const { return stride() * (fromSetIndex); }
    inline SetPosition  toSetEndIndex(SetPosition fromSetIndex)     const { return stride() * (fromSetIndex + 1); }

    inline SetPosition  stride() const { return StridePolicyType::stride(); }

  private:

    Set* m_fromSet;
    Set* m_toSet;

    RelationVec m_toSetIndicesVec;            // vector of toSet entries
  };


  template<typename StridePolicy>
  bool StaticConstantRelation<StridePolicy>::isValid(bool verboseOutput) const
  {
    bool bValid = true;

    std::stringstream errSstr;

    if( *m_fromSet == s_nullSet || *m_toSet == s_nullSet)
    {
      if(!m_toSetIndicesVec.empty())
      {
        if(verboseOutput)
        {
          errSstr << "\n\t* toSetIndicesVec was not empty "
                  << " -- fromSet was " << (*m_fromSet == s_nullSet ? "" : " not ") << "null"
                  << " , toSet was " << (*m_toSet == s_nullSet ? "" : " not ") << "null";
        }

        bValid = false;
      }
    }
    else
    {
      if(verboseOutput)
        errSstr << "\n\t* Neither set was null";

      // Check that the toSetIndices vector has the right size
      if( static_cast<SetPosition>(m_toSetIndicesVec.size()) != (stride() * m_fromSet->size()) )
      {
        if(verboseOutput)
        {
          errSstr << "\n\t* toSetIndices has the wrong size."
                  << "\n\t-- from set size is: " << m_fromSet->size()
                  << "\n\t-- constant stride is: " << stride()
                  << "\n\t-- expected relation size: " << (stride() * m_fromSet->size())
                  << "\n\t-- actual size: " << m_toSetIndicesVec.size()
          ;
        }
        bValid = false;
      }


      // Check that all elements of the toSetIndices vector point to valid set elements
      for(RelationVecConstIterator it = m_toSetIndicesVec.begin(), itEnd = m_toSetIndicesVec.end(); it != itEnd; ++it)
      {
        if( *it >= m_toSet->size() )
        {
          if(verboseOutput)
          {
            errSstr << "\n\t* toSetIndices had an out-of-range element."
                    << " -- value of element " << std::distance(m_toSetIndicesVec.begin(), it) << " was " << *it
                    << ". Max possible value should be " << m_toSet->size() << ".";
          }

          bValid = false;
        }
      }
    }


    if(verboseOutput)
    {
      std::stringstream sstr;

      if(bValid)
      {
        sstr << "(static,constant) Relation with stride " << stride() << " was valid." << std::endl;
      }
      else
      {
        sstr  << "Relation was NOT valid.\n"
              << errSstr.str()
              << std::endl;
      }

      sstr << "\n*** Detailed results of isValid on the relation.\n";
      if(m_fromSet)
        sstr << "\n** fromSet has size " << m_fromSet->size() << ": ";
      if(m_toSet)
        sstr << "\n** toSet has size " << m_toSet->size() << ": ";

      sstr << "\n** toSetIndices vec w/ size " << m_toSetIndicesVec.size() << ": ";
      std::copy(m_toSetIndicesVec.begin(), m_toSetIndicesVec.end(), std::ostream_iterator<SetPosition>(sstr, " "));

      std::cout << sstr.str() << std::endl;

    }

    return bValid;
  }



} // end namespace meshapi
} // end namespace asctoolkit

#endif // MESHAPI_STATIC_CONSTANT_RELATION_HPP_
