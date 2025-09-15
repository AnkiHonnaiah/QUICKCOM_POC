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
 *         \brief  A wrapper object providing access to the data in the shared memory via ZeroCopyCommon.
 *
 *       \details  Obtains pointer to shared memory from ZeroCopyCommon and casts to the proper underlying type.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_MEMORY_WRAPPER_PROXY_EVENT_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_MEMORY_WRAPPER_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/abort.h"
#include "amsr/core/error_domain.h"
#include "amsr/socal/internal/events/memory_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/slot_content.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/generic_proxy_predicate.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_client.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Wrapper object providing access to the data in the shared memory via ZeroCopyCommon.
 * \tparam  SampleType  Event sample data type.
 *
 * \unit    ZeroCopyBinding::ZeroCopyBindingTransformationLayer::MemoryWrapperProxyEvent
 */
template <typename SampleType>
class MemoryWrapperProxyEvent final : public ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType> {
  /*!
   * \brief Sample data type as required by the base class.
   */
  using typename ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType>::SampleDataType;

  /*!
   * \brief Interface type of the ZeroCopyCommon client.
   */
  using ClientInterface = ::amsr::zero_copy_common::internal::ClientInterface;

 public:
  /*!
   * \brief Type of the error handler that is called when the ZeroCopyCommon client reports an error.
   */
  using ZeroCopyCommonErrorHandler = ::vac::language::UniqueFunction<void(::amsr::core::ErrorCode const&) const>;

  /*!
   * \brief       Constructor of MemoryWrapperProxyEvent.
   * \param[in]   sample_raw_data             A view to the memory where the managed raw data lies.
   *                                          Must be valid for the object's entire life time.
   *                                          The user has to ensure that it contains the right sample data.
   * \param[in]   slot_token                  The slot token.
   * \param[in]   zero_copy_common_client     A reference to the ZeroCopyCommon client the token was obtained from.
   *                                          The referenced object must be valid for MemoryWrapperProxyEvent's entire
   *                                          life time.
   * \param[in]   release_slot_error_handler  An error handler that specifies how to handle errors that occurred while
   *                                          releasing the slot.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Abort if ZeroCopyCommon client is invalid.
   * - Abort if the release slot error handler is invalid.
   * \endinternal
   */
  MemoryWrapperProxyEvent(::ara::core::Span<std::uint8_t const> const sample_raw_data,
                          ClientInterface::SlotToken slot_token,
                          std::shared_ptr<ClientInterface> zero_copy_common_client,
                          ZeroCopyCommonErrorHandler release_slot_error_handler)
      : ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType>{},
        sample_raw_data_{sample_raw_data},
        slot_token_{std::move(slot_token)},
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
        zero_copy_common_client_{std::move(zero_copy_common_client)},
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
        release_slot_error_handler_{std::move(release_slot_error_handler)} {
    if (!zero_copy_common_client_) {
      ::amsr::core::Abort("MemoryWrapperProxyEvent::MemoryWrapperProxyEvent(): Invalid ZeroCopyCommon client pointer.");
    }
    if (!release_slot_error_handler_) {
      ::amsr::core::Abort("MemoryWrapperProxyEvent::MemoryWrapperProxyEvent(): Invalid ZeroCopyCommon error handler.");
    }
  }

  /*!
   * \brief       Destroy the memory wrapper object.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Release the slot in the ZeroCopyCommon client.
   * \endinternal
   */
  ~MemoryWrapperProxyEvent() noexcept final {  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validZeroCopyCommonClientPointer
    // VCA_ZEROCOPYBINDING_VALID_SHARED_POINTER
    zero_copy_common_client_->ReleaseSlot(std::move(slot_token_))
        .ConsumeError([&release_slot_error_handler = release_slot_error_handler_](auto const& error) {
          release_slot_error_handler(error);
        });
  }

  MemoryWrapperProxyEvent(MemoryWrapperProxyEvent const&) = delete;
  MemoryWrapperProxyEvent(MemoryWrapperProxyEvent&&) = delete;
  auto operator=(MemoryWrapperProxyEvent const&) -> MemoryWrapperProxyEvent& = delete;
  auto operator=(MemoryWrapperProxyEvent&&) -> MemoryWrapperProxyEvent& = delete;

  /*!
   * \brief       Returns a view to the raw data.
   * \return      A view to the raw data.
   * \pre         -
   * \context     ANY
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE for same class instance, TRUE for different instances
   * \reentrant   TRUE for same class instance, TRUE for different instances
   * \synchronous TRUE
   * \vprivate    Component private
   */
  template <typename T = SampleType>
  auto GetRawData() const noexcept ->
      typename std::enable_if_t<std::is_same<T, GenericProxyPredicate>::value, ::ara::core::Span<std::uint8_t const>> {
    return sample_raw_data_;
  }

  /*!
   * \brief       Member access operator -> for accessing the values inside the sample.
   * \return      Pointer to the underlying sample. The returned pointer must be valid and not null.
   * \pre         Shall not be used for generic proxy.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE for same class instance, TRUE for different instances
   * \reentrant   TRUE for same class instance, TRUE for different instances
   * \synchronous TRUE
   */
  auto operator->() noexcept -> SampleDataType* final {
    GenericProxyDependentActions<>();
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastOnSubData
    SampleDataType const* const sample{reinterpret_cast<SampleDataType const*>(sample_raw_data_.data())};
    // VECTOR NC AutosarC++17_10-A5.2.3: MD_ZEROCOPYBINDING_A5.2.3_constCastDueToInterfaceRestriction
    return const_cast<SampleDataType*>(sample);
  }

  /*!
   * \brief       Member indirection operator * for accessing the underlying sample.
   * \return      Reference to the underlying sample.
   * \pre         Shall not be used for generic proxy.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE for same class instance, TRUE for different instances
   * \reentrant   TRUE for same class instance, TRUE for different instances
   * \synchronous TRUE
   */
  auto operator*() noexcept -> SampleDataType& final {
    GenericProxyDependentActions<>();
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastOnSubData
    SampleDataType const* const sample{reinterpret_cast<SampleDataType const*>(sample_raw_data_.data())};
    // VECTOR NC AutosarC++17_10-A5.2.3: MD_ZEROCOPYBINDING_A5.2.3_constCastDueToInterfaceRestriction
    // VCA_ZEROCOPYBINDING_LNG_02_VALID_SAMPLE_REFERENCE
    return *const_cast<SampleDataType*>(sample);
  }

 private:
  /*!
   * \brief       A helper function that aborts if the view's size does not match the sample type.
   * \details     Only enabled if SampleType does not equal GenericProxyPredicate.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  template <typename T = SampleType>
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
  auto GenericProxyDependentActions() const noexcept ->
      typename std::enable_if_t<!std::is_same<T, GenericProxyPredicate>::value, void> {
    if (sample_raw_data_.size() != sizeof(SampleType)) {
      ::amsr::core::Abort("MemoryWrapperProxyEvent: The sample raw data has not the right size.");
    }
  }

  /*!
   * \brief       A helper function that always aborts.
   * \details     Only enabled if SampleType equals GenericProxyPredicate.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  template <typename T = SampleType>
  auto GenericProxyDependentActions() const noexcept ->
      typename std::enable_if_t<std::is_same<T, GenericProxyPredicate>::value, void> {
    ::amsr::core::Abort("Function shall not be used for generic proxy.");
  }

  /*!
   * \brief A span pointing to the raw data of the sample.
   */
  ::ara::core::Span<std::uint8_t const> const sample_raw_data_;

  /*!
   * \brief The slot token obtained from the ZeroCopyCommon client.
   */
  ClientInterface::SlotToken slot_token_;

  /*!
   * \brief Pointer to the ZeroCopyCommon client the token was obtained from. Required to be able to return the token.
   */
  std::shared_ptr<ClientInterface> const zero_copy_common_client_;

  /*!
   * \brief Error handler that is called if the ZeroCopyCommon client reports an error when releasing the corresponding
   *        slot.
   */
  ZeroCopyCommonErrorHandler release_slot_error_handler_;
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_MEMORY_WRAPPER_PROXY_EVENT_H_
