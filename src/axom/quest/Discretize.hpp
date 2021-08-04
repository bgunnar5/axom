// Copyright (c) 2017-2021, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

#ifndef QUEST_DISCRETIZE_HPP_
#define QUEST_DISCRETIZE_HPP_

// Axom includes
#include "axom/core/Macros.hpp"  // for axom macros

// Geometry
#include "axom/primal/geometry/Sphere.hpp"
#include "axom/primal/geometry/Octahedron.hpp"

namespace axom
{
namespace quest
{
/// \name Discretize primitive shapes to linear shapes
/// @{

using SphereType = primal::Sphere<double, 3>;
using OctType = primal::Octahedron<double, 3>;
using Point2D = primal::Point<double, 2>;

/*!
 * \brief Given a primitive sphere and a refinement level, allocate and return
 *   a list of Octahedra approximating the shape.
 * \param [in] s The sphere to approximate
 * \param [in] levels The number of refinements to perform
 * \param [out] out The newly-allocated array of octahedra representing \a s
 * \param [out] octcount The number of elements in \a out
 * \return false for invalid input or error in computation; true otherwise
 *
 * This routine generates O(4^level) octahedra.  That's exponential growth.
 * Use appropriate caution.
 *
 * This routine allocates an array pointed to by \a out.  The caller is responsible
 * to free the array.
 */
bool discretize(const SphereType& s, int levels, OctType*& out, int& octcount);

/*!
 * \brief Given a 2D polyline revolved around the positive X-axis, allocate
 *   and return a list of Octahedra approximating the shape.
 * \param [in] polyline The polyline to revolve around the X-axis
 * \param [in] len The number of points in \a polyline
 * \param [in] levels The number of refinements to perform
 * \param [out] out The newly-allocated array of octahedra representing the
 *   revolved polyline
 * \param [out] octcount The number of elements in \a out
 * \return false for invalid input or error in computation; true otherwise
 *
 * This routine generates n*O(2^level) octahedra, where n is the number of
 * segments in \a polyline (one less than the length).
 * That's exponential growth.  Use appropriate caution.
 *
 * This routine allocates an array pointed to by \a out.  The caller is responsible
 * to free the array.
 */
template <typename ExecSpace>
bool discretize(Point2D*& polyline,
                int len,
                int levels,
                OctType*& out,
                int& octcount);

/// @}

}  // end namespace quest
}  // end namespace axom

#include "Discretize_impl.hpp"

#endif /* QUEST_DISCRETIZE_HPP_ */
