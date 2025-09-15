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
/*!        \file someip_binding_generic_layer/internal/methods/generic_memory_wrapper.h
 *        \brief Contains an implementation for a generic memory wrapper.
 *         \unit SomeIpBinding::SomeIpBindingGenericLayer::Methods::GenericMemoryWrapper
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_MEMORY_WRAPPER_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_MEMORY_WRAPPER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/array.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Wrapper object holding memory for a generic sample.
 *
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
  using ImmutableBufferViewContainer = ::amsr::core::Array<ImmutableBufferView, 1>;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  GenericMemoryWrapper() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~GenericMemoryWrapper() noexcept final = default;

  GenericMemoryWrapper(GenericMemoryWrapper&&) noexcept = delete;
  auto operator=(GenericMemoryWrapper&&) noexcept -> GenericMemoryWrapper& = delete;
  GenericMemoryWrapper(GenericMemoryWrapper const&) noexcept = delete;
  auto operator=(GenericMemoryWrapper const&) noexcept -> GenericMemoryWrapper& = delete;

  /*!
   * \brief Get the read-only view over the underlying data buffers.
   *
   * \return Read-only view over the underlying data buffers. The returned view must be valid until the lifetime of
   *         this object.
   *
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
   * \param[in] response_buffer A response to be stored in the memory wrapper.
   *
   * \pre         Response buffer must not be an empty.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto StoreSample(DataBufferUniquePtr&& response_buffer) noexcept -> void;

 private:
  /*!
   * \brief Method response buffer.
   */
  DataBufferUniquePtr response_buffer_{};
  /*!
   * \brief Single buffer view on the buffer.
   */
  ImmutableBufferViewContainer single_packet_view_{};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_MEMORY_WRAPPER_H_
