/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**         \file   generic_memory_wrapper_interface.h
 *         \brief   Wrapper class to support ownership transfer for pre-allocated memory.
 *
 **********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_WRAPPER_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_WRAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "ara/core/span.h"

namespace amsr {
namespace socal {
namespace internal {
/*!
 * \brief Wrapper class to support ownership transfer for pre-allocated memory.
 */
class AMSR_LG_BETA GenericMemoryWrapperInterface {
  /*!
   * \brief Type alias to the GenericDataType.
   */
  using GenericDataType = ::amsr::socal::GenericDataType;

 public:
  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  GenericMemoryWrapperInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ~GenericMemoryWrapperInterface() noexcept = default;

  GenericMemoryWrapperInterface(GenericMemoryWrapperInterface const&) = delete;
  GenericMemoryWrapperInterface& operator=(GenericMemoryWrapperInterface const&) = delete;
  GenericMemoryWrapperInterface(GenericMemoryWrapperInterface&&) = delete;
  GenericMemoryWrapperInterface& operator=(GenericMemoryWrapperInterface&&) = delete;

  /*!
   * \brief Get the read-only view over the underlying data buffers.
   * \return Read-only view over the underlying data buffers.
   *         The returned view must be valid until the lifetime of *this object.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual GenericDataType GetData() const noexcept = 0;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_WRAPPER_INTERFACE_H_
