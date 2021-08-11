// Copyright (c) 2017-2021, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

#ifndef AXOM_PRIMAL_ZIP_RAY_HPP_
#define AXOM_PRIMAL_ZIP_RAY_HPP_

#include "axom/config.hpp"
#include "axom/core/StackArray.hpp"
#include "axom/slic/interface/slic.hpp"

#include "axom/primal/geometry/Ray.hpp"
#include "axom/primal/utils/ZipIndexable.hpp"

namespace axom
{
namespace primal
{
namespace detail
{
template <typename FloatType, int NDIMS>
struct ZipBase<Ray<FloatType, NDIMS>>
{
  using GeomType = Ray<FloatType, NDIMS>;
  using CoordType = FloatType;

  static constexpr int Dims = NDIMS;
  static constexpr bool Exists = true;

  /*!
   * \brief Creates a ZipIndexable from a set of arrays
   * \param [in] orig_arrays the arrays for each dimension storing the origin
   *  of the ray
   * \param [in] dir_arrays the arrays storing for each dimension the direction
   *  of the ray
   */
  ZipBase(const StackArray<const FloatType*, NDIMS>& orig_arrays,
          const StackArray<const FloatType*, NDIMS>& dir_arrays)
  {
    for(int i = 0; i < NDIMS; i++)
    {
      ray_origs[i] = orig_arrays[i];
      ray_dirs[i] = dir_arrays[i];
    }
  }

  /*!
   * \brief Returns the Ray at an index i.
   * \param [in] i the index to access
   */
  AXOM_HOST_DEVICE GeomType operator[](int i) const
  {
    using PointType = typename GeomType::PointType;
    using VectorType = typename GeomType::VectorType;
    StackArray<FloatType, NDIMS> orig_data, dir_data;
    for(int d = 0; d < NDIMS; d++)
    {
      orig_data[d] = ray_origs[d][i];
      dir_data[d] = ray_dirs[d][i];
    }
    return GeomType(PointType(orig_data), VectorType(dir_data));
  }

private:
  const FloatType* ray_origs[NDIMS];
  const FloatType* ray_dirs[NDIMS];
};

}  // namespace detail
}  // namespace primal
}  // namespace axom

#endif  // AXOM_PRIMAL_ZIP_RAY_HPP_
