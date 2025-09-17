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
/*!        \file
 *         \brief  A wrapper object providing access to the data in the shared memory.
 *         \unit   ZeroCopyBinding::GenericProxy
 *
 *       \details  Wraps a MemoryWrapperProxyEvent object which manages the slot.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_MEMORY_WRAPPER_PROXY_EVENT_H_
#define LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_MEMORY_WRAPPER_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/generic_proxy_predicate.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/memory_wrapper_proxy_event.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_backend.h"
#include "ara/core/array.h"

namespace amsr {
namespace zero_copy_binding {
namespace generic {
namespace internal {

/*!
 * \brief Wrapper object providing access to the data in the shared memory.
 */
class AMSR_LG_BETA GenericMemoryWrapperProxyEvent final
    : public ::amsr::socal::internal::GenericMemoryWrapperInterface {
  /*!
   * \brief Predicate type to represent a generic proxy.
   */
  using GenericProxyPredicate = transformation_layer::internal::GenericProxyPredicate;

  /*!
   * \brief Type of the embedded MemoryWrapperProxyEvent.
   */
  using MemoryWrapperProxyEventType =
      transformation_layer::internal::MemoryWrapperProxyEvent<transformation_layer::internal::GenericProxyPredicate>;

  /*!
   * \brief Interface type of the embedded MemoryWrapperProxyEvent.
   */
  using MemoryWrapperInterfacePtr =
      ::amsr::socal::internal::events::ProxyEventBackendInterface<GenericProxyPredicate>::MemoryWrapperInterfacePtr;

 public:
  /*!
   * \brief       Constructor of GenericMemoryWrapperProxyEvent.
   * \param[in]   memory_wrapper  A pointer to the MemoryWrapperProxyEvent to be wrapped.
   *                              The referenced ZeroCopyCommon client and raw data must be valid for the object's
   *                              entire lifetime.
   * \context     ANY
   * \pre         The dynamic type of the passed memory wrapper object must be MemoryWrapperProxyEvent.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  explicit GenericMemoryWrapperProxyEvent(MemoryWrapperInterfacePtr&& memory_wrapper) noexcept;

  /*!
   * \brief       Destroy the GenericMemoryWrapperProxyEvent object.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Destroy the member generic_data_ which holds a pointer to the shared memory managed by memory_wrapper_.
   * - Destroy the member memory_wrapper_.
   * \endinternal
   */
  ~GenericMemoryWrapperProxyEvent() noexcept final = default;

  GenericMemoryWrapperProxyEvent(GenericMemoryWrapperProxyEvent const&) = delete;
  GenericMemoryWrapperProxyEvent(GenericMemoryWrapperProxyEvent&&) = delete;
  auto operator=(GenericMemoryWrapperProxyEvent const&) -> GenericMemoryWrapperProxyEvent& = delete;
  auto operator=(GenericMemoryWrapperProxyEvent&&) -> GenericMemoryWrapperProxyEvent& = delete;

  /*!
   * \brief       Returns a view to the underlying shared memory raw data.
   * \return      The shared memory raw data in the form of a GenericDataType object.
   * \pre         -
   * \context     ANY
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE for same class instance, TRUE for different instances
   * \reentrant   TRUE for same class instance, TRUE for different instances
   * \synchronous TRUE
   */
  auto GetData() const noexcept -> ::amsr::socal::GenericDataType final;

 private:
  /*!
   * \brief    The embedded memory wrapper object that manages the shared memory slot.
   * \details  Stored here to guarantee the lifetime of the shared memory. Returns the slot upon destruction.
   */
  MemoryWrapperInterfacePtr const memory_wrapper_;

  /*!
   * \brief    The data the GenericDataType returned by GetData() will point to.
   * \details  The ZeroCopyBinding does not support fragmentation of sample data. Hence the array contains only one
   *           element.
   */
  ::ara::core::Array<::amsr::socal::GenericDataFragment, 1U> const generic_data_;
};

}  // namespace internal
}  // namespace generic
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_MEMORY_WRAPPER_PROXY_EVENT_H_
