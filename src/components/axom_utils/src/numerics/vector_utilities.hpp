/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

/*!
 *
 * \file vector_utilities.hpp
 *
 * \brief Header file containing utility functions to be used for vector
 *  calculations where "vectors" are arrays.
 *
 */

#ifndef AXOM_NUMERICS_VECTOR_UTILITIES_HPP_
#define AXOM_NUMERICS_VECTOR_UTILITIES_HPP_

#include "axom/Types.hpp" // for AXOM_NULLPTR
#include "axom_utils/Utilities.hpp"

// C/C++ includes
#include <cassert> // for assert()
#include <cmath>

namespace axom {
namespace numerics {

/*!
 * \brief Computes the dot product of the arrays u and v.
 *
 * \tparam T data type
 * \param [in] u pointer to an array of size dim
 * \param [in] v pointer to an array of size dim
 * \param [in] dim the dimension of the arrays at u and v
 * \return the dot product of the arrays u and v
 *
 * \pre dim >= 1
 * \pre u != AXOM_NULLPTR
 * \pre v != AXOM_NULLPTR
 * \pre u has at least dim entries
 * \pre v has at least dim entries
 */
template < typename T >
T dot_product(T* u, T* v, int dim);

/*!
 * \brief Makes vec orthogonal to other.
 *
 * \tparam T data type
 * \param [in, out] vec vector to be made orthogonal to other; saves in-place
 * \param [in] other vector that vec is made orthogonal to
 * \param [in] dim dimension of vectors
 *
 * \pre dim >= 1
 * \pre vec != AXOM_NULLPTR
 * \pre other != AXOM_NULLPTR
 */
template < typename T >
void make_orthogonal(T* vec, T* other, int dim);

/*!
 * \brief Performs Gram-Schmidt orthonormalization in-place on a 2D array
 * of shape size,size where it treats the rows as the individual vectors.
 *
 * \tparam T data type
 * \param [in, out] basis vectors to be made orthonormal; saves them in-place
 * \param [in] size number of vectors
 * \param [in] dim dimension of vectors
 * \param [in] eps If one of the vectors, after being made orthogonal to the
 *  others, has norm less than eps, then the orthonormalization is declared a
 *  failure. Note that this may well destory the data pointed to by basis.
 * \return return value, nonzero if the orthonormalization is successful
 *
 * \pre dim >= 1
 * \pre 1 <= size <= dim
 * \pre basis != AXOM_NULLPTR
 */
template < typename T >
bool orthonormalize(T* basis, int size, int dim, double eps = 1e-16);

/*!
 * \brief Normalizes the passed in array.
 *
 * \tparam T data type
 * \param [in] v pointer the array
 * \param [in] dim the dimension of v
 * \param [in] eps fuzz parameter
 * \note If squared norm of v is less than eps, the normalization
 *  fails and we do nothing to v
 *
 * \return true if normalization is successful, false otherwise.
 *
 * \pre dim >= 1
 * \pre v != AXOM_NULLPTR
 */
template < typename T >
bool normalize(T* v, int dim, double eps = 1e-16);

} /* end namespace numerics */
} /* end namespace axom */


//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
namespace axom {
namespace numerics {

template < typename T >
T dot_product(T* u, T* v, int dim)
{
  assert("pre: u pointer is null" && (u != AXOM_NULLPTR));
  assert("pre: v pointer is null" && (v != AXOM_NULLPTR));
  assert("pre: dim >= 1" && (dim >= 1));

  T res = u[0]*v[0];
  for (int i = 1; i < dim; ++i) res += u[i]*v[i];

  return res;
}

template < typename T >
void make_orthogonal(T* vec, T* other, int dim)
{
  assert("pre: vec pointer is null" && (vec != AXOM_NULLPTR));
  assert("pre: other pointer is null" && (other != AXOM_NULLPTR));
  assert("pre: dim >= 1" && (dim >= 1));

  double norm = static_cast< double >(dot_product(other, other, dim));

  if (norm < 1e-16) return;

  T tnorm = static_cast< T >(norm);

  T dot = dot_product(vec, other, dim);

  for (int l = 0; l < dim; ++l) vec[l] -= ((dot*other[l])/tnorm);
}

template < typename T >
bool orthonormalize(T* basis, int size, int dim, double eps)
{
  assert("pre: basis pointer is null" && (basis != AXOM_NULLPTR));
  assert("pre: dim >= 1" && (dim >= 1));
  assert("pre: size >= 1" && (size >= 1));
  assert("pre: size <= dim" && (size <= dim));

  for (int i = 0; i < size; ++i) {
    T* curr = &basis[i*dim];

    // make curr orthogonal to previous ones
    for (int j = 0; j < i; ++j) {
      T* other = &basis[j*dim];

      make_orthogonal(curr, other, dim);
    }

    bool res = normalize(curr, dim, eps);

    if (!res) {
      return false;
    }
  }

  // success
  return true;
}

template < typename T >
bool normalize(T* v, int dim, double eps)
{
  assert("pre: v pointer is null" && (v != AXOM_NULLPTR));
  assert("pre: dim >= 1" && (dim >= 1));

  double norm = static_cast< double >(dot_product< T >(v, v, dim));

  if (utilities::isNearlyEqual< double >(norm, 0., eps)) {
    return false;
  }


  T tnorm = static_cast< T >(std::sqrt(norm));
  for (int l = 0; l < dim; ++l) {
    v[l] /= tnorm;
  }

  // success
  return true;
}

} /* end namespace numerics */
} /* end namespace axom */

#endif /* AXOM_NUMERICS_VECTOR_UTILITIES_HPP_ */
