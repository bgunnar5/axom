/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing definition of DataStore class.
 *
 ******************************************************************************
 */

#ifndef DATASTORE_HPP_
#define DATASTORE_HPP_

// Standard C++ headers
#include <vector>
#include <stack>

#include "relay.hpp"

// Other CS Toolkit headers
#include "common/CommonTypes.hpp"
#include "slic/slic.hpp"

// Sidre project headers
#include "SidreTypes.hpp"

namespace asctoolkit
{
namespace sidre
{

// using directives to make Conduit usage easier and less visible
using conduit::Node;

class DataBuffer;
class DataGroup;

/*!
 * \class DataStore
 *
 * \brief DataStore is the main interface for creating and accessing
 *        buffer objects.
 *
 * It maintains a collection of buffer objects and owns the "root"
 * group, called "/". A group hierachy (a tree) is created by
 * creating child groups within other group.
 */
class DataStore
{
public:

  /*!
   * \brief Default ctor initializes datastore object and creates root group.
   *
   * Ctor also initializes SLIC logging environment if it is not already 
   * initialized.
   */
  DataStore();

  /*!
   * \brief Dtor destroys all contents of the datastore, including data held
   *        in buffers if owned by the buffers.
   */
  ~DataStore();

  /*!
   * \brief Return pointer to the root group.
   */
  DataGroup * getRoot()
  {
    return m_RootGroup;
  };

  /*!
   * \brief Return pointer to the root DataGroup.
   */
  const DataGroup * getRoot() const
  {
    return m_RootGroup;
  };

//@{
//!  @name Methods to query, access, create, and destroy buffers.

  /*!
   * \brief Return number of buffers in the datastore.
   */
  size_t getNumBuffers() const
  {
    return m_data_buffers.size() - m_free_buffer_ids.size();
  }

  /*!
   * \brief Return true if DataStore owns a buffer with given index;
   *        else false.
   */
  bool hasBuffer( IndexType idx ) const
  {
    return ( 0 <= idx && static_cast<unsigned>(idx) < m_data_buffers.size() &&
             m_data_buffers[idx] != ATK_NULLPTR );
  }

  /*!
   * \brief Return (non-const) pointer to data buffer object with given index,
   *        or ATK_NULLPTR if none exists.
   */
  DataBuffer * getBuffer( IndexType idx ) const;

  /*!
   * \brief Create an undescribed data buffer object and return a pointer to it.
   *
   *        The buffer must be described before it can be allocated.
   *
   *        The buffer object is assigned a unique index when created and the
   *        buffer object is owned by the data store object.
   */
  DataBuffer * createBuffer();

  /*!
   * \brief Create a data buffer object with specified type and number of
   *        elements and return a pointer to it.
   *
   *        See the DataBuffer::describe() method for valid data description.
   *
   *        The buffer object is assigned a unique index when created and the
   *        buffer object is owned by the data store object.
   */
  DataBuffer * createBuffer( TypeID type, SidreLength num_elems );

  /*!
   * \brief Remove data buffer from the datastore and destroy it and
   *        its data.
   *
   *        Note that buffer destruction detaches it from all views to
   *        which it is attached.
   */
  void destroyBuffer( DataBuffer * buff );

  /*!
   * \brief Remove data buffer with given index from the datastore and
   *        destroy it and its data.
   *
   *        Note that buffer destruction detaches it from all views to
   *        which it is attached.
   */
  void destroyBuffer( IndexType idx );

  /*!
   * \brief Remove all data buffers from the datastore and destroy them
   *        and their data.
   *
   *        Note that buffer destruction detaches it from all views to
   *        which it is attached.
   */
  void destroyAllBuffers();

//@}

//@{
//!  @name Methods useful for iterating over buffers in DataStore

  /*!
   * \brief Return first valid buffer index.
   *
   *        sidre::InvalidIndex is returned if group has no buffers.
   */
  IndexType getFirstValidBufferIndex() const;

  /*!
   * \brief Return next valid buffer index after given index.
   *
   *        sidre::InvalidIndex is returned if there is no valid next index.
   */
  IndexType getNextValidBufferIndex(IndexType idx) const;

//@}

  /*!
   * \brief Copy DataStore Group hierarchy (starting at root) and buffer 
   *        descriptions to given Conduit node.
   */
  void info(Node& n) const;

  /*!
   * \brief Print JSON description of DataStore Group hierarchy (starting at 
   *        root) and buffer descriptions to std::cout.
   */
  void print() const;

  /*!
   * \brief Print JSON description of DataStore Group hierarchy (starting at
   *        root) and buffer descriptions to given output stream.
   */
  void print(std::ostream& os) const;


  /// Developer notes:
  /// We should reduce these functions when SPIO is fully available ( in both serial and parallel ).
  /// We only need one or two simple save functions.  Try to keep this class simple and move the I/O
  /// interfaces to SPIO.

  /*!
   * \brief Save the datastore to a new file.
   * Supported protocols are conduit (binary), conduit_hdf5, and text (for debugging).
   * If a group is not provided, the root group will be saved.
   */
  void save( const std::string& file_path,
             const std::string& protocol,
             const DataGroup* group = ATK_NULLPTR ) const;

  /*!
   * \brief Save the datastore to an existing hdf5 file.
   * If a group is not provided, the root group will be saved.
   */
  void save( const hid_t& h5_file_id,
             const DataGroup* group = ATK_NULLPTR ) const;

  /*!
   * \brief Load the datastore from a file
   * If a group is not provided, it will be loaded into the root group.
   */
  void load(const std::string& file_path,
            const std::string& protocol,
            DataGroup * group = ATK_NULLPTR);

  /*!
   * \brief Load the datastore from an hdf5 file.
   * If a group is not provided, it will be loaded into the root group.
   */
  void load(const hid_t& h5_file_id,
            DataGroup * group = ATK_NULLPTR);

  /*!
   * \brief Add view description and references to it's data to a conduit tree.
   */

  /*!
   * \brief Add the datastore hierarchy and references to it's data to a conduit tree.
   * This includes the group/view hierarchy and buffers.
   */
  void exportTo( const DataGroup * group,
                  conduit::Node& data_holder ) const;
  /*!
   * \brief Restore a datastore hierarchy and data contents (buffers, etc) from a conduit tree.
   */

  void importFrom(DataGroup * group,
                  conduit::Node& data_holder);

private:
  /*!
   *  Unimplemented ctors and copy-assignment operators.
   */
#ifdef USE_CXX11
  DataStore( const DataStore& ) = delete;
  DataStore( DataStore&& ) = delete;

  DataStore& operator=( const DataStore& ) = delete;
  DataStore& operator=( DataStore&& ) = delete;
#else
  DataStore( const DataStore& );
  DataStore& operator=( const DataStore& );
#endif

  /// Root data group, created when DataStore object is created.
  DataGroup * m_RootGroup;

  /// Collection of buffers in DataStore instance.
  std::vector<DataBuffer *> m_data_buffers;

  /// Collection of unused unique buffer indices (they can be recycled).
  std::stack< IndexType > m_free_buffer_ids;

  /// Flag indicating whether SLIC logging environment was initialized in ctor.
  bool m_need_to_finalize_slic;
};


} /* end namespace sidre */
} /* end namespace asctoolkit */

#endif /* DATASTORE_HPP_ */
