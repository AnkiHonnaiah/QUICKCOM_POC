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
/*!       \file
 *        \brief
 *        \unit   SomeIpBinding::SomeIpBindingGenericLayer::Events::GenericMemoryWrapper
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_MEMORY_WRAPPER_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_MEMORY_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"
#include "amsr/someip_binding_core/internal/events/someip_sample_cache_entry.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/unique_ptr.h"
#include "ara/core/array.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace events {
/*!
 * \brief Wrapper object holding memory for a generic sample.
 */
class GenericMemoryWrapper final : public ::amsr::socal::internal::GenericMemoryWrapperInterface {
 public:
  /*!
   * \brief Data buffer unique pointer type.
   */
  using DataBufferUniquePtr = amsr::someip_binding_core::internal::MemoryBufferPtr;

  /*!
   * \brief Type-alias for immutable buffer view.
   */
  using ImmutableBufferView = ::amsr::socal::GenericDataFragment;

  /*!
   * \brief Type-alias for container of immutable buffer view.
   */
  using ImmutableBufferViews = ::amsr::socal::GenericDataType;

  /*!
   * \brief Type-alias for an array containing one ImmutableBufferView.
   */
  using ImmutableBufferViewContainer = ::ara::core::Array<ImmutableBufferView, 1>;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericMemoryWrapper() noexcept = default;

  /*!
   * \brief       Destroys the object.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   * \steady      FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~GenericMemoryWrapper() noexcept final = default;

  GenericMemoryWrapper(GenericMemoryWrapper const&) = delete;
  GenericMemoryWrapper(GenericMemoryWrapper&&) = delete;
  GenericMemoryWrapper& operator=(GenericMemoryWrapper const&) & = delete;
  GenericMemoryWrapper& operator=(GenericMemoryWrapper&&) & = delete;

  /*!
   * \brief Get the read-only view over the underlying data buffers.
   * \return Read-only view over the underlying data buffers.
   *         The returned view must be valid until the lifetime of *this object.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto GetData() const noexcept -> ImmutableBufferViews final;

  /*!
   * \brief Stores the received response buffer.
   *
   * \param[in] event_packet An event to be stored in the memory wrapper.
   *
   * \pre         Event packet must not be empty.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto StoreSample(
      amsr::UniquePtr<someip_binding_core::internal::events::SomeIpSampleCacheEntry>&& event_packet) noexcept -> void;

 private:
  /*!
   * \brief Event packet
   */
  amsr::UniquePtr<someip_binding_core::internal::events::SomeIpSampleCacheEntry> packet_;

  /*!
   * \brief Single buffer view on the packet
   */
  ImmutableBufferViewContainer single_packet_view_{};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_MEMORY_WRAPPER_H_
