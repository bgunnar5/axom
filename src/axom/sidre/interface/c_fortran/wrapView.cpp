// wrapView.cpp
// This file is generated by Shroud 0.12.2. Do not edit.
//
// Copyright (c) 2017-2023, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
#include "wrapView.h"
#include <cstring>
#include <string>
#include "axom/sidre/core/Buffer.hpp"
#include "axom/sidre/core/Group.hpp"
#include "axom/sidre/core/View.hpp"

// splicer begin class.View.CXX_definitions
// splicer end class.View.CXX_definitions

extern "C" {

// helper ShroudStrCopy
// Copy src into dest, blank fill to ndest characters
// Truncate if dest is too short.
// dest will not be NULL terminated.
static void ShroudStrCopy(char *dest, int ndest, const char *src, int nsrc)
{
  if(src == NULL)
  {
    std::memset(dest, ' ', ndest);  // convert NULL pointer to blank filled string
  }
  else
  {
    if(nsrc < 0)
    {
      nsrc = std::strlen(src);
    }
    int nm = nsrc < ndest ? nsrc : ndest;
    std::memcpy(dest, src, nm);
    if(ndest > nm)
    {
      std::memset(dest + nm, ' ', ndest - nm);  // blank fill
    }
  }
}
// splicer begin class.View.C_definitions
// splicer end class.View.C_definitions

SIDRE_IndexType SIDRE_View_get_index(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_index
  axom::sidre::IndexType SHC_rv = SH_this->getIndex();
  return SHC_rv;
  // splicer end class.View.method.get_index
}

const char *SIDRE_View_get_name(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_name
  const std::string &SHCXX_rv = SH_this->getName();
  const char *SHC_rv = SHCXX_rv.c_str();
  return SHC_rv;
  // splicer end class.View.method.get_name
}

void SIDRE_View_get_name_bufferify(const SIDRE_View *self, char *SHF_rv, int NSHF_rv)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_name_bufferify
  const std::string &SHCXX_rv = SH_this->getName();
  if(SHCXX_rv.empty())
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, nullptr, 0);
  }
  else
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, SHCXX_rv.data(), SHCXX_rv.size());
  }
  // splicer end class.View.method.get_name_bufferify
}

void SIDRE_View_get_path_bufferify(const SIDRE_View *self, char *SHF_rv, int NSHF_rv)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_path_bufferify
  std::string SHCXX_rv = SH_this->getPath();
  if(SHCXX_rv.empty())
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, nullptr, 0);
  }
  else
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, SHCXX_rv.data(), SHCXX_rv.size());
  }
  // splicer end class.View.method.get_path_bufferify
}

void SIDRE_View_get_path_name_bufferify(const SIDRE_View *self,
                                        char *SHF_rv,
                                        int NSHF_rv)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_path_name_bufferify
  std::string SHCXX_rv = SH_this->getPathName();
  if(SHCXX_rv.empty())
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, nullptr, 0);
  }
  else
  {
    ShroudStrCopy(SHF_rv, NSHF_rv, SHCXX_rv.data(), SHCXX_rv.size());
  }
  // splicer end class.View.method.get_path_name_bufferify
}

SIDRE_Group *SIDRE_View_get_owning_group(SIDRE_View *self, SIDRE_Group *SHC_rv)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_owning_group
  axom::sidre::Group *SHCXX_rv = SH_this->getOwningGroup();
  SHC_rv->addr = SHCXX_rv;
  SHC_rv->idtor = 0;
  return SHC_rv;
  // splicer end class.View.method.get_owning_group
}

bool SIDRE_View_has_buffer(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.has_buffer
  bool SHC_rv = SH_this->hasBuffer();
  return SHC_rv;
  // splicer end class.View.method.has_buffer
}

SIDRE_Buffer *SIDRE_View_get_buffer(SIDRE_View *self, SIDRE_Buffer *SHC_rv)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_buffer
  axom::sidre::Buffer *SHCXX_rv = SH_this->getBuffer();
  SHC_rv->addr = SHCXX_rv;
  SHC_rv->idtor = 0;
  return SHC_rv;
  // splicer end class.View.method.get_buffer
}

bool SIDRE_View_is_external(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_external
  bool SHC_rv = SH_this->isExternal();
  return SHC_rv;
  // splicer end class.View.method.is_external
}

bool SIDRE_View_is_allocated(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_allocated
  bool SHC_rv = SH_this->isAllocated();
  return SHC_rv;
  // splicer end class.View.method.is_allocated
}

bool SIDRE_View_is_applied(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_applied
  bool SHC_rv = SH_this->isApplied();
  return SHC_rv;
  // splicer end class.View.method.is_applied
}

bool SIDRE_View_is_described(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_described
  bool SHC_rv = SH_this->isDescribed();
  return SHC_rv;
  // splicer end class.View.method.is_described
}

bool SIDRE_View_is_empty(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_empty
  bool SHC_rv = SH_this->isEmpty();
  return SHC_rv;
  // splicer end class.View.method.is_empty
}

bool SIDRE_View_is_opaque(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_opaque
  bool SHC_rv = SH_this->isOpaque();
  return SHC_rv;
  // splicer end class.View.method.is_opaque
}

bool SIDRE_View_is_scalar(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_scalar
  bool SHC_rv = SH_this->isScalar();
  return SHC_rv;
  // splicer end class.View.method.is_scalar
}

bool SIDRE_View_is_string(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.is_string
  bool SHC_rv = SH_this->isString();
  return SHC_rv;
  // splicer end class.View.method.is_string
}

SIDRE_TypeIDint SIDRE_View_get_type_id(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_type_id
  axom::sidre::TypeID SHCXX_rv = SH_this->getTypeID();
  SIDRE_TypeIDint SHC_rv = static_cast<SIDRE_TypeIDint>(SHCXX_rv);
  return SHC_rv;
  // splicer end class.View.method.get_type_id
}

size_t SIDRE_View_get_total_bytes(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_total_bytes
  size_t SHC_rv = SH_this->getTotalBytes();
  return SHC_rv;
  // splicer end class.View.method.get_total_bytes
}

size_t SIDRE_View_get_num_elements(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_num_elements
  size_t SHC_rv = SH_this->getNumElements();
  return SHC_rv;
  // splicer end class.View.method.get_num_elements
}

size_t SIDRE_View_get_bytes_per_element(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_bytes_per_element
  size_t SHC_rv = SH_this->getBytesPerElement();
  return SHC_rv;
  // splicer end class.View.method.get_bytes_per_element
}

size_t SIDRE_View_get_offset(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_offset
  size_t SHC_rv = SH_this->getOffset();
  return SHC_rv;
  // splicer end class.View.method.get_offset
}

size_t SIDRE_View_get_stride(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_stride
  size_t SHC_rv = SH_this->getStride();
  return SHC_rv;
  // splicer end class.View.method.get_stride
}

int SIDRE_View_get_num_dimensions(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_num_dimensions
  int SHC_rv = SH_this->getNumDimensions();
  return SHC_rv;
  // splicer end class.View.method.get_num_dimensions
}

int SIDRE_View_get_shape(const SIDRE_View *self, int ndims, SIDRE_IndexType *shape)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_shape
  int SHC_rv = SH_this->getShape(ndims, shape);
  return SHC_rv;
  // splicer end class.View.method.get_shape
}

void SIDRE_View_allocate_simple(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.allocate_simple
  SH_this->allocate();
  // splicer end class.View.method.allocate_simple
}

void SIDRE_View_allocate_from_type(SIDRE_View *self,
                                   SIDRE_TypeID type,
                                   SIDRE_IndexType num_elems)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.allocate_from_type
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->allocate(SHCXX_type, num_elems);
  // splicer end class.View.method.allocate_from_type
}

void SIDRE_View_reallocate(SIDRE_View *self, SIDRE_IndexType num_elems)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.reallocate
  SH_this->reallocate(num_elems);
  // splicer end class.View.method.reallocate
}

void SIDRE_View_attach_buffer_only(SIDRE_View *self, SIDRE_Buffer *buff)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.attach_buffer_only
  axom::sidre::Buffer *SHCXX_buff =
    static_cast<axom::sidre::Buffer *>(buff->addr);
  SH_this->attachBuffer(SHCXX_buff);
  // splicer end class.View.method.attach_buffer_only
}

void SIDRE_View_attach_buffer_type(SIDRE_View *self,
                                   SIDRE_TypeID type,
                                   SIDRE_IndexType num_elems,
                                   SIDRE_Buffer *buff)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.attach_buffer_type
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  axom::sidre::Buffer *SHCXX_buff =
    static_cast<axom::sidre::Buffer *>(buff->addr);
  SH_this->attachBuffer(SHCXX_type, num_elems, SHCXX_buff);
  // splicer end class.View.method.attach_buffer_type
}

void SIDRE_View_attach_buffer_shape(SIDRE_View *self,
                                    SIDRE_TypeID type,
                                    int ndims,
                                    const SIDRE_IndexType *shape,
                                    SIDRE_Buffer *buff)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.attach_buffer_shape
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  axom::sidre::Buffer *SHCXX_buff =
    static_cast<axom::sidre::Buffer *>(buff->addr);
  SH_this->attachBuffer(SHCXX_type, ndims, shape, SHCXX_buff);
  // splicer end class.View.method.attach_buffer_shape
}

void SIDRE_View_clear(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.clear
  SH_this->clear();
  // splicer end class.View.method.clear
}

void SIDRE_View_apply_0(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_0
  SH_this->apply();
  // splicer end class.View.method.apply_0
}

void SIDRE_View_apply_nelems(SIDRE_View *self, SIDRE_IndexType num_elems)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_nelems
  SH_this->apply(num_elems);
  // splicer end class.View.method.apply_nelems
}

void SIDRE_View_apply_nelems_offset(SIDRE_View *self,
                                    SIDRE_IndexType num_elems,
                                    SIDRE_IndexType offset)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_nelems_offset
  SH_this->apply(num_elems, offset);
  // splicer end class.View.method.apply_nelems_offset
}

void SIDRE_View_apply_nelems_offset_stride(SIDRE_View *self,
                                           SIDRE_IndexType num_elems,
                                           SIDRE_IndexType offset,
                                           SIDRE_IndexType stride)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_nelems_offset_stride
  SH_this->apply(num_elems, offset, stride);
  // splicer end class.View.method.apply_nelems_offset_stride
}

void SIDRE_View_apply_type_nelems(SIDRE_View *self,
                                  SIDRE_TypeID type,
                                  SIDRE_IndexType num_elems)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_type_nelems
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->apply(SHCXX_type, num_elems);
  // splicer end class.View.method.apply_type_nelems
}

void SIDRE_View_apply_type_nelems_offset(SIDRE_View *self,
                                         SIDRE_TypeID type,
                                         SIDRE_IndexType num_elems,
                                         SIDRE_IndexType offset)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_type_nelems_offset
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->apply(SHCXX_type, num_elems, offset);
  // splicer end class.View.method.apply_type_nelems_offset
}

void SIDRE_View_apply_type_nelems_offset_stride(SIDRE_View *self,
                                                SIDRE_TypeID type,
                                                SIDRE_IndexType num_elems,
                                                SIDRE_IndexType offset,
                                                SIDRE_IndexType stride)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_type_nelems_offset_stride
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->apply(SHCXX_type, num_elems, offset, stride);
  // splicer end class.View.method.apply_type_nelems_offset_stride
}

void SIDRE_View_apply_type_shape(SIDRE_View *self,
                                 SIDRE_TypeID type,
                                 int ndims,
                                 const SIDRE_IndexType *shape)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.apply_type_shape
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->apply(SHCXX_type, ndims, shape);
  // splicer end class.View.method.apply_type_shape
}

void SIDRE_View_set_scalar_int(SIDRE_View *self, int value)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_scalar_int
  SH_this->setScalar<int>(value);
  // splicer end class.View.method.set_scalar_int
}

void SIDRE_View_set_scalar_long(SIDRE_View *self, long value)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_scalar_long
  SH_this->setScalar<long>(value);
  // splicer end class.View.method.set_scalar_long
}

void SIDRE_View_set_scalar_float(SIDRE_View *self, float value)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_scalar_float
  SH_this->setScalar<float>(value);
  // splicer end class.View.method.set_scalar_float
}

void SIDRE_View_set_scalar_double(SIDRE_View *self, double value)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_scalar_double
  SH_this->setScalar<double>(value);
  // splicer end class.View.method.set_scalar_double
}

void SIDRE_View_set_string(SIDRE_View *self, const char *value)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_string
  const std::string SHCXX_value(value);
  SH_this->setString(SHCXX_value);
  // splicer end class.View.method.set_string
}

void SIDRE_View_set_string_bufferify(SIDRE_View *self, const char *value, int Lvalue)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_string_bufferify
  const std::string SHCXX_value(value, Lvalue);
  SH_this->setString(SHCXX_value);
  // splicer end class.View.method.set_string_bufferify
}

void SIDRE_View_set_external_data_ptr_only(SIDRE_View *self, void *external_ptr)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_external_data_ptr_only
  SH_this->setExternalDataPtr(external_ptr);
  // splicer end class.View.method.set_external_data_ptr_only
}

void SIDRE_View_set_external_data_ptr_type(SIDRE_View *self,
                                           SIDRE_TypeID type,
                                           SIDRE_IndexType num_elems,
                                           void *external_ptr)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_external_data_ptr_type
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->setExternalDataPtr(SHCXX_type, num_elems, external_ptr);
  // splicer end class.View.method.set_external_data_ptr_type
}

void SIDRE_View_set_external_data_ptr_shape(SIDRE_View *self,
                                            SIDRE_TypeID type,
                                            int ndims,
                                            const SIDRE_IndexType *shape,
                                            void *external_ptr)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.set_external_data_ptr_shape
  axom::sidre::TypeID SHCXX_type = static_cast<axom::sidre::TypeID>(type);
  SH_this->setExternalDataPtr(SHCXX_type, ndims, shape, external_ptr);
  // splicer end class.View.method.set_external_data_ptr_shape
}

const char *SIDRE_View_get_string(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_string
  const char *SHC_rv = SH_this->getString();
  return SHC_rv;
  // splicer end class.View.method.get_string
}

void SIDRE_View_get_string_bufferify(SIDRE_View *self, char *name, int Nname)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_string_bufferify
  const char *SHC_rv = SH_this->getString();
  ShroudStrCopy(name, Nname, SHC_rv, -1);
  // splicer end class.View.method.get_string_bufferify
}

int SIDRE_View_get_data_int(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_data_int
  int SHC_rv = SH_this->getData<int>();
  return SHC_rv;
  // splicer end class.View.method.get_data_int
}

long SIDRE_View_get_data_long(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_data_long
  long SHC_rv = SH_this->getData<long>();
  return SHC_rv;
  // splicer end class.View.method.get_data_long
}

float SIDRE_View_get_data_float(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_data_float
  float SHC_rv = SH_this->getData<float>();
  return SHC_rv;
  // splicer end class.View.method.get_data_float
}

double SIDRE_View_get_data_double(SIDRE_View *self)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_data_double
  double SHC_rv = SH_this->getData<double>();
  return SHC_rv;
  // splicer end class.View.method.get_data_double
}

void *SIDRE_View_get_void_ptr(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.get_void_ptr
  void *SHC_rv = SH_this->getVoidPtr();
  return SHC_rv;
  // splicer end class.View.method.get_void_ptr
}

void SIDRE_View_print(const SIDRE_View *self)
{
  const axom::sidre::View *SH_this =
    static_cast<const axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.print
  SH_this->print();
  // splicer end class.View.method.print
}

bool SIDRE_View_rename(SIDRE_View *self, const char *new_name)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.rename
  const std::string SHCXX_new_name(new_name);
  bool SHC_rv = SH_this->rename(SHCXX_new_name);
  return SHC_rv;
  // splicer end class.View.method.rename
}

bool SIDRE_View_rename_bufferify(SIDRE_View *self,
                                 const char *new_name,
                                 int Lnew_name)
{
  axom::sidre::View *SH_this = static_cast<axom::sidre::View *>(self->addr);
  // splicer begin class.View.method.rename_bufferify
  const std::string SHCXX_new_name(new_name, Lnew_name);
  bool SHC_rv = SH_this->rename(SHCXX_new_name);
  return SHC_rv;
  // splicer end class.View.method.rename_bufferify
}

}  // extern "C"
