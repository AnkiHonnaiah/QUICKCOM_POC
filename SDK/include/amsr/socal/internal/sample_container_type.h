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
/**        \file  sample_container_type.h
 *        \brief  SampleContainerType used by the Proxy event.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SAMPLE_CONTAINER_TYPE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SAMPLE_CONTAINER_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/socal/r18_03/events/sample_ptr.h"
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Template struct to encapsulate the container datatype.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 */
template <typename SampleType>
struct SampleContainerType {
  /*!
   * \brief Alias for SamplePtr type.
   */
  using SamplePtr = ::amsr::socal::r18_03::events::SamplePtr<SampleType const>;
  /*!
   * \brief SampleContainer type.
   */
  using Type = ::ara::com::SampleContainer<SamplePtr>;
  // Methods in ProxyEventBackend require a width of the size type to be at least as wide as std::size_t
  static_assert(std::numeric_limits<typename Type::size_type>::max() >= std::numeric_limits<std::size_t>::max(),
                "size_type must be at least as big as std::size_t");
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SAMPLE_CONTAINER_TYPE_H_
