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
/**        \file  memory_wrapper_interface.h
 *        \brief  Interface class for accessing the deserialized sample handled by the binding on proxy side.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_CACHE_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_CACHE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <type_traits>
#include "amsr/socal/internal/events/memory_wrapper_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface class for accessing the deserialized sample handled by the binding on proxy side.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 */
template <typename SampleType>
class CacheInterface {
 public:
  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = std::remove_cv_t<std::remove_reference_t<SampleType>>;

  /*!
   * \brief Entry type saved in this cache.
   */
  using CacheEntryType = std::shared_ptr<socal::internal::events::MemoryWrapperInterface<SampleDataType>>;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  CacheInterface() noexcept = default;

  CacheInterface(CacheInterface const&) = delete;
  CacheInterface& operator=(CacheInterface const&) = delete;
  CacheInterface(CacheInterface&&) = delete;
  CacheInterface& operator=(CacheInterface&&) = delete;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~CacheInterface() noexcept = default;

  /*!
   * \brief Returns a CacheEntry to the cache.
   * \param[in] entry  A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE for same class instance, TRUE for different instances.
   * \reentrant  TRUE for same class instance, TRUE for different instances.
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void ReturnEntry(CacheEntryType&& entry) noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_CACHE_INTERFACE_H_
