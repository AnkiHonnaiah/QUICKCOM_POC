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
 *         \brief  Allocated sample unique ptr for Zero-Copy.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_ZERO_COPY_SAMPLE_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_ZERO_COPY_SAMPLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_server.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief       A single Zero-Copy sample, containing a pointer to the underlying data and a slot token to identify it
                towards the communication server.
 * \tparam      SampleType The type of the payload data.
 * \unit        ZeroCopyBinding::ZeroCopyBindingTransformationLayer::AllocatedZeroCopySample
 */
template <typename SampleType>
class AllocatedZeroCopySample final : public ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType> {
  /*!
   * \brief Type for the ServerInterface of ZeroCopyCommon.
   */
  using ServerInterface = ::amsr::zero_copy_common::internal::ServerInterface;

 public:
  /*!
   * \brief Type of the AllocatedSampleInterface.
   */
  using AllocatedSampleInterface = ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType>;

  /*!
   * \brief Type for the EventId.
   */
  using EventIdType = common::internal::config::EventId::Type;

  /*!
   * \brief Type for the InstanceId.
   */
  using InstanceIdType = common::internal::config::InstanceId::Type;

  /*!
   * \brief       Constructor of an AllocatedZeroCopySample.
   * \param[in,out] sample_data
   * \param[in]   slot_token
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  AllocatedZeroCopySample(ServerInterface& zero_copy_common_server, SampleType* sample_data,
                          ServerInterface::SlotToken&& slot_token,
                          common::internal::config::ConfigView const zero_copy_binding_config_view) noexcept
      : AllocatedSampleInterface{},
        sample_data_{sample_data},
        slot_token_{std::move(slot_token)},
        zero_copy_common_server_{zero_copy_common_server},
        zero_copy_binding_config_view_{zero_copy_binding_config_view} {}

  AllocatedZeroCopySample(AllocatedZeroCopySample const&) = delete;
  auto operator=(AllocatedZeroCopySample const&) -> AllocatedZeroCopySample& = delete;
  AllocatedZeroCopySample(AllocatedZeroCopySample&&) = delete;
  auto operator=(AllocatedZeroCopySample&&) -> AllocatedZeroCopySample& = delete;

  /*!
   * \brief       Destroy the AllocatedZeroCopySample.
   * \details     On destruction, it is checked whether the underlying slot token has already been used to send the
   *              slot. If this is not the case, it must be explicitly returned to the ZeroCopyCommon server it belongs
   *              to.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  ~AllocatedZeroCopySample() noexcept final {  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
    if (slot_token_.has_value()) {
      zero_copy_common_server_
          // VCA_ZEROCOPYBINDING_ALLOCATED_ZERO_COPY_SAMPLE_ZERO_COPY_COMMON_SERVER_POINTER_VALID
          .UnacquireSlot(std::move(slot_token_.value()))
          // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
          .ConsumeError([](::amsr::core::ErrorCode const&) {
            ::amsr::core::Abort("AllocatedZeroCopySample::~AllocatedZeroCopySample(): Unacquire slot failed.");
          });

      slot_token_.reset();
    }
  }

  /*!
   * \brief       Member indirection operator * for accessing the underlying sample data.
   * \return      Reference to the underlying sample data.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  // VCA_ZEROCOPYBINDING_LNG_02_RETURNED_POINTER_VALID
  auto operator*() noexcept -> SampleType& final { return *sample_data_; }

  /*!
   * \brief       Member access operator -> for accessing the underlying sample data.
   * \return      Pointer to the underlying sample data.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto operator->() noexcept -> SampleType* final { return &operator*(); }

  /*!
   * \brief       Getter for the slot token.
   * \return      Slot token.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto GetSlotToken() && noexcept -> ServerInterface::SlotToken {
    if (!slot_token_.has_value()) {
      ::amsr::core::Abort("AllocatedZeroCopySample::GetSlotToken(): Slot token was already collected.");
    }
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_ZEROCOPYBINDING_A7.1.1_CanBeConst_FalsePositive
    ServerInterface::SlotToken slot_token{std::move(slot_token_.value())};
    slot_token_.reset();
    return slot_token;
  }

  /*!
   * \brief       Check sample for validity.
   *
   * \param[in]   config_view Config view of a SkeletonEventManager to check validity against.
   * \return      True if the given config view contains the same event id and instance id as stored in the sample,
   *              false otherwise.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto IsSampleValid(common::internal::config::ConfigView const config_view) const noexcept -> bool {
    return (
        std::tie(
            // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            config_view.AsProvidedEventConfig().event_id.value,
            // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            config_view.AsProvidedConfig().instance_id.value) ==
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        std::tie(zero_copy_binding_config_view_.AsProvidedEventConfig().event_id.value,
                 // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                 zero_copy_binding_config_view_.AsProvidedConfig().instance_id.value));
  }

 private:
  /*!
   * \brief       Pointer to the sample data.
   */
  SampleType* sample_data_;

  /*!
   * \brief       Slot token identifying the AllocatedZeroCopySample towards the respective server.
   */
  ::amsr::core::Optional<ServerInterface::SlotToken> slot_token_;

  /*!
   * \brief The ZeroCopyCommon server this AllocatedZeroCopySample belongs to.
   *        Hint for VCA findings: The reference to the server will always be valid in this context. Background: Slots
   *        must be returned before the server is destructed. If the server shall be destructed before, an abort is
   *        triggered. The reference to the server is only used upon destruction of the AllocatedZeroCopySample when
   *        the slot is still valid, i.e., not previously sent (and therefore returned) to the ZeroCopyCommon server.
   */
  ServerInterface& zero_copy_common_server_;

  /*!
   * \brief A view to the binding's configuration data.
   */
  common::internal::config::ConfigView const zero_copy_binding_config_view_;
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_ZERO_COPY_SAMPLE_H_
