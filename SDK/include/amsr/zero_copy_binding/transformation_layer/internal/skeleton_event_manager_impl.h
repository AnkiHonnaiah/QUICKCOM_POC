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
 *         \brief  Contains the definition of the templated SkeletonEventManager class.
 *
 *       \details This header file has to be included by the source file skeleton_event_manager.cpp, where all used
 *                types will be instantiated explicitly.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_IMPL_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <tuple>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/generic/generic_error_domain.h"
#include "ara/com/com_error_domain.h"
#include "ara/core/vector.h"

#include "amsr/thread/this_thread.h"
#include "amsr/unique_ptr.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/allocated_zero_copy_sample.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_server.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

template <typename SampleType>
struct SkeletonEventManager<SampleType>::CanAddReceiverResult {
  bool can_add_receiver;
  std::size_t class_index;
};

/*!
 * \internal
 * - Collect the provided instance specific event configuration.
 * - Calculate the total number of slots required.
 * - Create and configure a server builder for the underlying communication server.
 * - Build a new server. If this fails:
 *   - Abort.
 * \endinternal
 */
// VECTOR NC VectorC++-V12.1.1: MD_ZEROCOPYBINDING_VectorC++-V12.1.1_BaseClassConstructor
template <typename SampleType>
SkeletonEventManager<SampleType>::SkeletonEventManager(
    common::internal::config::ConfigView const zero_copy_binding_config_view,
    std::shared_ptr<core::internal::LifeCycleManagerCore> life_cycle_manager_core,
    OnConnectionCleanupCallbackRef on_connection_cleanup_cbk) noexcept
    : SkeletonEventManagerInterface(),
      on_connection_cleanup_cbk_{on_connection_cleanup_cbk},
      // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
      logger_{
          "SkeletonEventManager (event id: " +  // VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT, VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
          std::to_string(zero_copy_binding_config_view.AsEventConfig().event_id.value) + ")"},
      zero_copy_binding_config_view_{zero_copy_binding_config_view},
      // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
      life_cycle_manager_core_{std::move(life_cycle_manager_core)} {
  receiver_classes_with_message_drops_.reserve(kNumReceiverClasses);
}

/*!
 * TODO (jmeisinger, 17.01.2024): CDD internal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::Initialize() noexcept {
  assert(!initialized_);
  common::internal::config::ProvidedInstanceSpecificEventConfig const&
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      provided_instance_specific_event_config{zero_copy_binding_config_view_.AsProvidedEventConfig()};

  // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
  class_handles_.resize(kNumReceiverClasses);
  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  std::fill(std::begin(connection_count_), std::end(connection_count_), 0U);

  std::uint32_t const total_number_of_slots{provided_instance_specific_event_config.resource_limits.asil_a.max_slots +
                                            provided_instance_specific_event_config.resource_limits.asil_b.max_slots +
                                            provided_instance_specific_event_config.resource_limits.asil_c.max_slots +
                                            provided_instance_specific_event_config.resource_limits.asil_d.max_slots +
                                            provided_instance_specific_event_config.resource_limits.qm.max_slots +
                                            kTraceMaxSlots};

  std::shared_ptr<ServerBuilderInterface> const zero_copy_common_server_builder{
      // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
      life_cycle_manager_core_->GetFactory()->GetZeroCopyCommonServerBuilder()};

  ::amsr::zero_copy_binding::common::internal::config::MemoryTechnology::Type const allocation_memory_technology{
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      zero_copy_binding_config_view_.AsProvidedEventConfig().allocation_memory_technology.value};
  MemoryTechnology const memory_technology{
      (allocation_memory_technology ==
       ::amsr::zero_copy_binding::common::internal::config::MemoryTechnology::Type::kSharedMemory)
          ? MemoryTechnology::kSharedMemory
          : MemoryTechnology::kPhysContigSharedMemory};

  ServerBuilderInterface::MaxNumberReceivers max_num_receivers{
      provided_instance_specific_event_config.num_connections.value};

  // If tracing is enabled, account for the connection to the local receiver.
  // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPILATION_UNIT
  if (zero_copy_binding_config_view_.AccessConfiguration().trace_enabled) {
    ++max_num_receivers.value;
  }

  // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedServerBuilderSetterCalls
  // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedServerBuilderSetterCalls
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validManagedZeroCopyCommonServerPointer
  std::ignore =
      zero_copy_common_server_builder
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          ->WithNumberSlots(ServerBuilderInterface::NumSlots{total_number_of_slots})
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithSlotContentSize(ServerBuilderInterface::SlotContentSize{sizeof(SlotContent)})
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithSlotContentAlignment(ServerBuilderInterface::SlotContentAlignment{alignof(SlotContent)})
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithMemoryTechnology(memory_technology)
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithMaxNumberReceivers(max_num_receivers)
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithNumberClasses(ServerBuilderInterface::ReceiverClassNumber{kNumReceiverClasses})
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(
              ServerBuilderInterface::ReceiverClassLimit{
                  provided_instance_specific_event_config.resource_limits.qm.max_slots},
              class_handles_.at(0U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(
              ServerBuilderInterface::ReceiverClassLimit{
                  provided_instance_specific_event_config.resource_limits.asil_a.max_slots},
              class_handles_.at(1U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(
              ServerBuilderInterface::ReceiverClassLimit{
                  provided_instance_specific_event_config.resource_limits.asil_b.max_slots},
              class_handles_.at(2U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(
              ServerBuilderInterface::ReceiverClassLimit{
                  provided_instance_specific_event_config.resource_limits.asil_c.max_slots},
              class_handles_.at(3U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(
              ServerBuilderInterface::ReceiverClassLimit{
                  provided_instance_specific_event_config.resource_limits.asil_d.max_slots},
              class_handles_.at(4U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithClass(ServerBuilderInterface::ReceiverClassLimit{kTraceMaxSlots}, class_handles_.at(5U))
          // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
          .WithOnReceiverStateTransitionCallback(
              [this](ServerInterface::ReceiverId receiver_id, ServerInterface::ReceiverState const receiver_state,
                     ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code) {
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
                OnReceiverStateTransition(receiver_id, receiver_state, opt_error_code);
              });

  // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_ZEROCOPYBINDING_V5.0.1_irrelevantFunctionCallOrder
  // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
  LOG_INFO(logger_, ([&zero_copy_binding_config_view = zero_copy_binding_config_view_, &total_number_of_slots,
                      &provided_instance_specific_event_config](auto& stream) {
             stream << "Zero-Copy server for service id "
                    << zero_copy_binding_config_view.AsServiceConfig().service_id.value << " and event id "
                    << provided_instance_specific_event_config.event_id.value << " started with "
                    << total_number_of_slots << " slots of " << sizeof(SampleType)
                    << " Byte size (total payload memory: " << (sizeof(SampleType) * total_number_of_slots) << ")";
           }));

  zero_copy_common_server_builder
      // VCA_ZEROCOPYBINDING_SERVER_BUILDER_POINTER_VALID
      ->Build()  // VECTOR SL AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validManagedZeroCopyCommonServerPointer

      .Consume([&zero_copy_common_server = this->zero_copy_common_server_](auto build_result) {
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_LNG_03_SERVER_VALID_AFTER_BUILD, VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        zero_copy_common_server = std::move(build_result);
      })
      .ConsumeError([&logger = this->logger_](auto /* error */) {
        LOG_FATAL_AND_ABORT(logger, "Failed to build zero_copy_common server, aborting.");
      });

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  // clang-format off
    if (zero_copy_binding_config_view_.AccessConfiguration().trace_enabled) {  // COV_ZEROCOPYBINDING_TRACE_API_TEMPORARY_GAP
    // clang-format on
    trace_client_ =
        std::make_unique<trace::LocalReceiver<SampleType>>(life_cycle_manager_core_, zero_copy_binding_config_view_);
  }
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  initialized_ = true;
}

/*!
 * \internal
 * - Lock the mutex.
 * - Shutdown the underlying communication server.
 * - Wait until all asynchronous operations of the server are completed.
 * - Clear all variables being reused when the SkeletonEventManager is initialized again.
 * \endinternal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::Deinitialize() noexcept {
  assert(initialized_);
  std::lock_guard<std::mutex> const lock_guard{receiver_information_mutex_};
  // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
  static_cast<void>(zero_copy_common_server_->Shutdown());
  // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
  while (zero_copy_common_server_->IsInUse()) {
    ::amsr::thread::SleepFor(kCleanupDelay);
  }
  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  zero_copy_common_server_.reset();
  class_handles_.clear();
  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  receiver_information_.clear();
  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  std::fill(std::begin(connection_count_), std::end(connection_count_), 0U);
  initialized_ = false;
}

/*!
 * \internal
 * - Log and abort when called.
 * \endinternal
 */
template <typename SampleType>
auto SkeletonEventManager<SampleType>::Send(SampleType const& data) noexcept -> ::ara::core::Result<void> {
  assert(initialized_);
  static_cast<void>(data);
  LOG_FATAL_AND_ABORT(logger_, "This method is not supported for a Zero-Copy Binding, aborting.");
  return ::ara::core::Result<void>{};
}

/*!
 * \internal
 * - Check that the sample pointer is not nullptr. If it is:
 *   - Abort.
 * - Cast the base class pointer to the derived Zero-Copy sample pointer.
 * - Check that the allocated zero copy sample is valid for this SkeletonEventManager instance. If it is not:
 *   - Abort.
 * - Collect the slot token belonging to the allocated zero copy sample.
 * - Try to access the slot content. If this fails:
 *   - Abort. This will only occur if shutdown has already been called for the server.
 * - Reclaim slots from the ZeroCopyCommon server before sending. If this fails:
 *   - Call the handler for corrupted receivers when a receiver error is detected. Otherwise, abort.
 * - Cast the slot data to the SlotContent.
 * - Add meta data to the slot content.
 * - Send the slot. If this fails:
 *   - Call the handler for corrupted receivers when a receiver error is detected. Otherwise, abort.
 * - Reset the list of receiver classes with dropped messages since this is not handled.
 * - Create an empty void result and return.
 * \endinternal
 */
template <typename SampleType>
auto SkeletonEventManager<SampleType>::Send(AllocatedSampleUniquePtr sample_ptr) noexcept -> ::ara::core::Result<void> {
  assert(initialized_);
  if (!sample_ptr) {
    LOG_FATAL_AND_ABORT(logger_, "Sample pointer must not be nullptr.");
  } else {
    AllocatedZeroCopySample<SampleType>* const allocated_zero_copy_sample{
        // VECTOR NC AutosarC++17_10-M5.2.3: MD_ZEROCOPYBINDING_M5.2.3_castFromPolymorphicBaseToDerivedClass
        static_cast<AllocatedZeroCopySample<SampleType>*>(sample_ptr.get())};

    // VCA_ZEROCOPYBINDING_LNG_03_VALID_ALLOCATED_ZERO_COPY_SAMPLE_POINTER
    if (!allocated_zero_copy_sample->IsSampleValid(zero_copy_binding_config_view_)) {
      ::amsr::core::Abort("SkeletonEventManager::Send(): Invalid allocated sample unique pointer given.");
    } else {
      ServerInterface::SlotToken sample_slot_token{std::move(*allocated_zero_copy_sample).GetSlotToken()};

      // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
      auto access_result(zero_copy_common_server_->AccessSlotContent(sample_slot_token));

      if (!access_result.HasValue()) {
        LOG_FATAL_AND_ABORT(logger_, "Error occurred during slot access, aborting.");
      }

      // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
      zero_copy_common_server_->ReclaimSlots().ConsumeError(
          [this, &logger = logger_](::amsr::core::ErrorCode const& error_code) {
            if (error_code == ZeroCopyCommonErrc::kReceiverError) {
              // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
              HandleCorruptedReceiver();
            } else {
              LOG_FATAL_AND_ABORT(logger, "Error occurred during sending, aborting.");
            }
          });
      // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastOnSharedMemoryData
      SlotContent* const slot_content{reinterpret_cast<SlotContent*>(access_result.Value().data())};

      static MetaData::SequenceCounter sequence_counter{1U};

      // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validSlotContent
      // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID
      slot_content->header_data.meta_data.time_stamp = std::chrono::steady_clock::now();
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validSlotContent
      // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID
      slot_content->header_data.meta_data.sequence_counter = sequence_counter++;

      zero_copy_common_server_
          // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
          ->SendSlot(std::move(sample_slot_token), receiver_classes_with_message_drops_)
          .ConsumeError([this, &logger = logger_](::amsr::core::ErrorCode const& error_code) {
            if (error_code == ZeroCopyCommonErrc::kReceiverError) {
              // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
              HandleCorruptedReceiver();
            } else {
              LOG_FATAL_AND_ABORT(logger, "Error while sending, aborting.");
            }
          });

      receiver_classes_with_message_drops_.clear();
    }
  }
  return ::ara::core::Result<void>{};
}

/*!
 * \internal
 * - Reclaim slots from the ZeroCopyCommon server before trying to allocate a slot. If this fails:
 *   - Call the handler for corrupted receivers when a receiver error is detected. Otherwise, abort.
 * - Acquire a slot. If this fails, abort.
 * - Check if slot acquisition yielded a new slot. If not:
 *   - Return a sample allocation failure.
 * - Access slot contents. If this fails, abort.
 * - Cast the sample type onto the slot contents.
 * - Initialize the slot contents as given in the provided configuration.
 * - Fill the header with offset and size information.
 * - Create a unique pointer of the acquired slot and return it.
 * \endinternal
 */
template <typename SampleType>
auto SkeletonEventManager<SampleType>::Allocate() noexcept -> ::ara::core::Result<AllocatedSampleUniquePtr> {
  assert(initialized_);
  ::ara::core::Result<AllocatedSampleUniquePtr> result{::ara::com::ComErrc::kSampleAllocationFailure};

  // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
  zero_copy_common_server_->ReclaimSlots().ConsumeError(
      [this, &logger = logger_](::amsr::core::ErrorCode const& error_code) {
        if (error_code == ZeroCopyCommonErrc::kReceiverError) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
          HandleCorruptedReceiver();
        } else {
          LOG_FATAL_AND_ABORT(logger, "Error occurred during reclaiming slots, aborting.");
        }
      });

  // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
  auto slot(zero_copy_common_server_->AcquireSlot());

  if (!slot.HasValue()) {
    LOG_FATAL_AND_ABORT(logger_, "Error occurred during slot acquisition, aborting.");
  } else if (!slot.Value().has_value()) {
    LOG_INFO(logger_, "Failed to acquire slot.");
    result.EmplaceError(::ara::com::ComErrc::kSampleAllocationFailure);
  } else {
    ::amsr::core::Result<::amsr::core::Span<std::uint8_t>> access_result{
        // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
        zero_copy_common_server_->AccessSlotContent(slot.Value().value())};

    if (!access_result.HasValue()) {
      LOG_FATAL_AND_ABORT(logger_, "Error occurred during slot access, aborting.");
    } else {
      // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastOnSharedMemoryData
      SlotContent* const slot_content{reinterpret_cast<SlotContent*>(access_result.Value().data())};

      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      switch (zero_copy_binding_config_view_.AsProvidedEventConfig().initialization.value) {
        case InitializationType::kConstructorInitialization:
          // VECTOR NC AutosarC++17_10-A18.5.2: MD_ZEROCOPYBINDING_A18.5.2_placementNew
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validSlotContent
          // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID, VCA_ZEROCOPYBINDING_LNG_03_VALID_CONSTRUCTOR_IN_TEST
          std::ignore = new (&slot_content->sample) typename AllocatedZeroCopySample<SampleType>::SampleDataType{};
          break;
        case InitializationType::kZeroInitialization:
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validSlotContent
          // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID
          memset(&slot_content->sample, 0, sizeof(typename AllocatedZeroCopySample<SampleType>::SampleDataType));
          break;
        case InitializationType::kNoInitialization:
          // nothing to be done
          break;
      }

      // VCA_ZEROCOPYBINDING_LNG_01_VALID_REFERENCE_ARGUMENT_TO_SET_PAYLOAD_INFO
      core::internal::SetPayloadInfo(*slot_content);

      // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID
      result.EmplaceValue(amsr::AllocateUnique<AllocatedZeroCopySample<SampleType>>(
                              // VCA_ZEROCOPYBINDING_LNG_01_ZERO_COPY_COMMON_SERVER_VALID
                              ara::core::PolymorphicAllocator<AllocatedZeroCopySample<SampleType>>{},
                              *zero_copy_common_server_, &slot_content->sample,
                              std::move(slot.Value().value()),  // VCA_ZEROCOPYBINDING_SLOT_CONTENT_ACCESS_VALID
                              zero_copy_binding_config_view_)
                              .Value());
    }
  }

  return result;
}

// VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_noSideEffectEmptyMethodPreallocate
template <typename SampleType>
void SkeletonEventManager<SampleType>::PreAllocate(std::size_t const count) noexcept {
  static_cast<void>(count);
}

/*!
 * \internal
 * - If the given side channel is nullptr, abort.
 * - Get the integrity level of the peer. Return an error if this fails.
 * - If the own integrity level of the server is not set, query the connection wrapper and set the level. If this
 *   fails, abort.
 * - If access control grants access:
 *   - Check that the connection limit is not exceeded for the receiver class of the receiver to be added. Check that
 *     another receiver can be added to the ZeroCopyCommon server.
 *     - If no further receivers can be added, return an error.
 *   - Add the receiver to the ZeroCopyCommon server.
 *     - If this succeeds, increase the connection counter for the receiver
 *       class and store the receiver information.
 *     - If this fails, abort.
 *   - Return the receiver id.
 * \endinternal
 */
template <typename SampleType>
auto SkeletonEventManager<SampleType>::AddReceiver(
    ConnectionHandle const connection_handle, core::internal::ConnectionWrapperInterface const& connection_wrapper,
    std::unique_ptr<::amsr::zero_copy_common::internal::SideChannelInterface> side_channel,
    bool const is_trace_receiver) noexcept -> ::amsr::core::Result<ReceiverId> {
  assert(initialized_);
  ::amsr::core::Result<ReceiverId> result{::amsr::generic::GenErrc::kRuntimeResourceOutOf};

  if (!side_channel) {
    LOG_FATAL_AND_ABORT(logger_, "Side channel may not be nullptr, aborting.");
  }

  // VCA_ZEROCOPYBINDING_CONNECTION_WRAPPER_VALID
  ::amsr::core::Result<::amsr::ipc::IntegrityLevel> integrity_level{connection_wrapper.GetPeerIntegrityLevel()};

  if (!integrity_level.HasValue()) {
    LOG_WARN(logger_, "Unable to determine peer integrity level.");
    result.EmplaceError(::amsr::generic::GenErrc::kRuntime);
  } else {
    GetOwnIntegrityLevel(connection_wrapper);

    // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    common::internal::config::ServiceId const service_id{zero_copy_binding_config_view_.AsServiceConfig().service_id};
    common::internal::config::InstanceId const instance_id{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        zero_copy_binding_config_view_.AsProvidedConfig().instance_id};
    // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    common::internal::config::EventId const event_id{zero_copy_binding_config_view_.AsEventConfig().event_id};
    // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    ipc::Credentials const peer_id{GetPeerIdentity(connection_wrapper)};

    // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
    if (!life_cycle_manager_core_->GetAccessControl().CheckAccessControlSkeletonSubscribeEventRx(
            service_id, instance_id, event_id, peer_id)) {
      LOG_WARN(logger_, "Cannot add receiver. Access control denied access.");
      result.EmplaceError(::amsr::generic::GenErrc::kSystemPrivilegesNoAccess);
    } else {
      ::amsr::core::Result<CanAddReceiverResult> can_add_receiver_result{
          CanAddReceiver(integrity_level.Value(), is_trace_receiver)};

      if (!can_add_receiver_result.HasValue()) {
        ::amsr::core::Abort("SkeletonEventManager::AddReceiver(): Failed to query CanAddReceiver status.");
      } else if (!can_add_receiver_result.Value().can_add_receiver) {
        LOG_WARN(logger_, "Cannot add receiver. Configured resource limits for this quality class reached.");
        result.EmplaceError(::amsr::generic::GenErrc::kRuntimeResourceOutOf);
      } else {
        std::size_t const class_index{can_add_receiver_result.Value().class_index};

        zero_copy_common_server_
            // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID, VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
            ->AddReceiver(class_handles_.at(class_index).value(), std::move(side_channel))
            // VECTOR NC AutosarC++17_10-A5.1.2: MD_ZEROCOPYBINDING_A5.1.2_defaultReferenceCapture
            .Consume([&](ReceiverId const& receiver_id) {
              // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
              ++connection_count_.at(class_index);
              {
                // VCA_ZEROCOPYBINDING_LNG_01_VALID_LOCK_GUARD_REFERENCE_ARGUMENT
                std::lock_guard<std::mutex> const lock_guard{receiver_information_mutex_};
                // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
                std::ignore = receiver_information_.emplace(
                    receiver_id.GetId(),
                    ReceiverInformation{receiver_id, connection_handle, is_trace_receiver, integrity_level.Value()});
              }
              result.EmplaceValue(receiver_id);
            })
            .ConsumeError([&logger = logger_](auto const&) {
              LOG_FATAL_AND_ABORT(logger, "Adding receiver to ZeroCopyCommon Server failed, aborting.");
            });
      }
    }
  }

  if (!result.HasValue()) {
    // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
    side_channel->Close();
    // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
    while (side_channel->IsInUse()) {
      ::amsr::thread::SleepFor(kCleanupDelay);
    }
  }

  return result;
}

/*!
 * \internal
 * - Forward the call to the ZeroCopyCommon server. If an error is returned:
 *   - Check for error code. If it indicates a corrupted receiver, trigger a state transition for this receiver.
 *     Otherwise, abort.
 * \endinternal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::ConnectReceiver(ReceiverId const receiver_id) noexcept {
  assert(initialized_);
  zero_copy_common_server_
      // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
      ->ConnectReceiver(receiver_id)
      .ConsumeError([this, &logger = logger_, &receiver_id](auto const& error_code) {
        if ((error_code == ZeroCopyCommonErrc::kPeerCrashedError) ||
            (error_code == ZeroCopyCommonErrc::kProtocolError) ||
            (error_code == ZeroCopyCommonErrc::kUnexpectedReceiverState)) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA, VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
          this->OnReceiverStateTransition(receiver_id, ServerInterface::ReceiverState::kCorrupted, error_code);
        } else {
          LOG_FATAL_AND_ABORT(logger, "Error occurred during connecting, aborting.");
        }
      });
}

/*!
 * \internal
 * - Collect the receiver information of the receiver to be removed.
 * - Check that the given receiver is no longer in use.
 *   - If this fails or if the receiver is still in use, abort.
 * - Remove the receiver from the ZeroCopyCommon server.
 * - Reduce the connection count for the given integrity level.
 * \endinternal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::RemoveReceiver(ReceiverId const receiver_id,
                                                      ::amsr::ipc::IntegrityLevel const integrity_level,
                                                      bool const is_trace_receiver) noexcept {
  assert(initialized_);

  zero_copy_common_server_
      // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID, VCA_ZEROCOPYBINDING_ZEROTH_ARGUMENT_POSSIBLY_INVALID
      ->IsReceiverInUse(receiver_id)
      .Consume([&zero_copy_common_server = zero_copy_common_server_, &receiver_id,
                &logger = logger_](bool is_receiver_in_use) {
        if (is_receiver_in_use) {
          LOG_FATAL_AND_ABORT(logger, "Receiver is in use and cannot be removed, aborting.");
        }
        // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
        zero_copy_common_server->RemoveReceiver(receiver_id).ConsumeError([&logger](auto const&) {
          LOG_FATAL_AND_ABORT(logger, "Error during removal of receiver.");
        });
      })
      .ConsumeError([&logger = logger_](auto const&) {
        LOG_FATAL_AND_ABORT(logger, "Unable to determine receiver state, aborting.");
      });

  if (is_trace_receiver) {
    --connection_count_.back();
  } else {
    --connection_count_.at(ConvertIntegrityLevelToUnsignedInt(integrity_level));
  }
}

template <typename SampleType>
void SkeletonEventManager<SampleType>::GetOwnIntegrityLevel(
    core::internal::ConnectionWrapperInterface const& connection_wrapper) noexcept {
  if (!own_integrity_level_.has_value()) {
    // VCA_ZEROCOPYBINDING_CONNECTION_WRAPPER_VALID
    connection_wrapper.GetOwnIntegrityLevel()
        .Consume([this](::amsr::ipc::IntegrityLevel const own_integrity_level) {
          // VCA_ZEROCOPYBINDING_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
          own_integrity_level_.emplace(own_integrity_level);
        })
        .ConsumeError([&logger = logger_](auto const&) {
          LOG_FATAL_AND_ABORT(logger, "Unable to determine integrity level of Skeleton application, aborting.");
        });
  }
}

template <typename SampleType>
auto SkeletonEventManager<SampleType>::GetPeerIdentity(
    core::internal::ConnectionWrapperInterface const& connection_wrapper) noexcept -> ipc::Credentials {
  // VCA_ZEROCOPYBINDING_CONNECTION_WRAPPER_VALID
  return connection_wrapper.GetPeerIdentity()
      .InspectError([&logger = logger_](amsr::core::ErrorCode) {
        LOG_FATAL_AND_ABORT(logger, "Unable to determine peer identity of Skeleton application, aborting.");
      })
      .Value();
}

template <typename SampleType>
auto SkeletonEventManager<SampleType>::CanAddReceiver(::amsr::ipc::IntegrityLevel integrity_level,
                                                      bool is_trace_receiver) noexcept
    -> ::amsr::core::Result<CanAddReceiverResult> {
  ::amsr::core::Result<CanAddReceiverResult> result{::amsr::generic::GenericErrorDomain::Errc::kRuntime};
  std::size_t class_index{0U};
  bool connection_limit_reached{false};

  if (is_trace_receiver) {
    class_index = kTraceClassIndex;
    connection_limit_reached = connection_count_.back() >= kTraceConnectionLimit;
  } else {
    class_index = ConvertIntegrityLevelToUnsignedInt(integrity_level);

    connection_limit_reached =
        connection_count_.at(class_index) >=
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        GetSingleResourceLimit(zero_copy_binding_config_view_.AsProvidedEventConfig(), integrity_level).max_connections;
  }

  // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
  if (::amsr::core::Result<bool> const can_add_receiver{zero_copy_common_server_->CanAddReceiver()}) {
    result = ::amsr::core::Result<CanAddReceiverResult>::FromValue(
        {can_add_receiver.Value() && (!connection_limit_reached), class_index});
  }

  return result;
}

template <typename SampleType>
auto SkeletonEventManager<SampleType>::ConvertIntegrityLevelToUnsignedInt(
    ::amsr::ipc::IntegrityLevel const integrity_level) noexcept -> std::size_t {
  std::size_t result{0U};
  switch (integrity_level) {
    case ::amsr::ipc::IntegrityLevel::kQm:
      result = 0U;
      break;
    case ::amsr::ipc::IntegrityLevel::kAsilA:
      result = 1U;
      break;
    case ::amsr::ipc::IntegrityLevel::kAsilB:
      result = 2U;
      break;
    case ::amsr::ipc::IntegrityLevel::kAsilC:
      result = 3U;
      break;
    case ::amsr::ipc::IntegrityLevel::kAsilD:
      result = 4U;
      break;
  }
  return result;
}

template <typename SampleType>
auto SkeletonEventManager<SampleType>::GetSingleResourceLimit(
    common::internal::config::ProvidedInstanceSpecificEventConfig const& provided_instance_specific_event_config,
    ::amsr::ipc::IntegrityLevel const integrity_level) noexcept -> common::internal::config::SingleResourceLimit {
  common::internal::config::SingleResourceLimit result{0U, 0U};
  switch (integrity_level) {
    case amsr::ipc::IntegrityLevel::kQm:
      result = provided_instance_specific_event_config.resource_limits.qm;
      break;
    case amsr::ipc::IntegrityLevel::kAsilA:
      result = provided_instance_specific_event_config.resource_limits.asil_a;
      break;
    case amsr::ipc::IntegrityLevel::kAsilB:
      result = provided_instance_specific_event_config.resource_limits.asil_b;
      break;
    case amsr::ipc::IntegrityLevel::kAsilC:
      result = provided_instance_specific_event_config.resource_limits.asil_c;
      break;
    case amsr::ipc::IntegrityLevel::kAsilD:
      result = provided_instance_specific_event_config.resource_limits.asil_d;
      break;
  }
  return result;
}

/*!
 * \internal
 * - Collect receiver information and event id.
 * - Check new receiver state.
 *   - When kCorrupted:
 *     - Check integrity level of receiver against own integrity level. Trace receivers are excluded from this check.
 *       Terminate if own integrity level <= receiver integrity level, otherwise terminate the receiver.
 *     - Call registered callback for connection cleanup.
 *   - When kDisconnected:
 *     - Call registered callback for connection cleanup.
 * \endinternal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::OnReceiverStateTransition(
    ReceiverId const receiver_id, ServerInterface::ReceiverState const receiver_state,
    ::amsr::core::Optional<::amsr::core::ErrorCode> const opt_error_code) noexcept {
  assert(initialized_);

  if ((receiver_state == ServerInterface::ReceiverState::kCorrupted) ||
      (receiver_state == ServerInterface::ReceiverState::kDisconnected)) {
    ::amsr::core::Optional<ReceiverInformation const> receiver_information{};

    {
      std::lock_guard<std::mutex> const lock_guard{receiver_information_mutex_};
      if (receiver_information_.find(receiver_id.GetId()) != receiver_information_.end()) {
        receiver_information.emplace(std::move(receiver_information_.at(receiver_id.GetId())));
        // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
        std::ignore = receiver_information_.erase(receiver_id.GetId());
      }
    }
    if (receiver_information.has_value()) {
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPILATION_UNIT
      EventIdType const event_id{zero_copy_binding_config_view_.AsEventConfig().event_id.value};

      switch (receiver_state) {
        case ServerInterface::ReceiverState::kCorrupted:
          // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_ERROR(logger_, ([receiver_id = receiver_id.GetId()](auto& stream) {
                      stream << "Connection corrupted for receiver id " << receiver_id;
                    }));

          if ((opt_error_code.value() ==
               ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError) ||
              (opt_error_code.value() == ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError)) {
            if ((own_integrity_level_.value() <= receiver_information.value().integrity_level) &&
                (!receiver_information.value().is_trace_receiver)) {
              ::amsr::core::Abort(
                  "SkeletonEventManager::OnReceiverStateTransition(): Disconnected or corrupted receiver connection "
                  "and server has less or equal integrity level as the receiver.");
            }
          } else {
            // nothing to be done
          }

          // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID
          zero_copy_common_server_->TerminateReceiver(receiver_id)
              .ConsumeError([&logger = logger_](::amsr::core::ErrorCode const&) {
                LOG_FATAL_AND_ABORT(logger, "Error terminating receiver.");
              });
          on_connection_cleanup_cbk_(receiver_information.value().connection_handle, event_id,
                                     receiver_information.value());
          break;
        case ServerInterface::ReceiverState::kDisconnected:
          on_connection_cleanup_cbk_(receiver_information.value().connection_handle, event_id,
                                     receiver_information.value());
          break;
        default:  // COV_ZEROCOPYBINDING_INVALID_RECEIVER_STATE
          // nothing to be done
          break;
      }
    }
  }
}

/*!
 * \internal
 * - Loop over all receivers and get their receiver state from ZeroCopyCommonServer.
 * - For receivers with kCorrupted or kDisconnected state, call the receiver state transition.
 * \endinternal
 */
template <typename SampleType>
void SkeletonEventManager<SampleType>::HandleCorruptedReceiver() noexcept {
  struct ReceiverStateInformation {
    ReceiverId receiver_id;
    ReceiverState receiver_state{};
    ::amsr::core::Optional<::amsr::core::ErrorCode> error_code_opt;
  };

  assert(initialized_);
  ::ara::core::Vector<ReceiverStateInformation> receiver_states{};
  {
    std::lock_guard<std::mutex> const lock_guard{receiver_information_mutex_};
    for (ReceiverInformationMap::value_type const& receiver : receiver_information_) {
      std::pair<ReceiverState, ::amsr::core::Optional<::amsr::core::ErrorCode>> const receiver_state{
          // VCA_ZEROCOPYBINDING_SLC_10_SLC_22_ZERO_COPY_COMMON_SERVER_VALID, VCA_ZEROCOPYBINDING_ZEROTH_ARGUMENT_POSSIBLY_INVALID
          zero_copy_common_server_->GetReceiverState(receiver.second.receiver_id)};
      if ((receiver_state.first == ServerInterface::ReceiverState::kCorrupted) ||
          (receiver_state.first == ServerInterface::ReceiverState::kDisconnected)) {
        // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
        receiver_states.push_back({receiver.second.receiver_id, receiver_state.first, receiver_state.second});
      }
    }
  }

  // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
  std::ignore = std::for_each(receiver_states.cbegin(), receiver_states.cend(),
                              [this](ReceiverStateInformation const& receiver_state) {
                                // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
                                OnReceiverStateTransition(receiver_state.receiver_id, receiver_state.receiver_state,
                                                          receiver_state.error_code_opt);
                              });
}

/*!
 * \brief Definition of kCleanupDelay.
 */
template <typename SampleType>
std::chrono::milliseconds constexpr SkeletonEventManager<SampleType>::kCleanupDelay;

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//  \ID COV_ZEROCOPYBINDING_INVALID_RECEIVER_STATE
//    \ACCEPT  XX
//    \REASON  The number of possible receiver states here is limited by a previous if condition.
//  \ID COV_ZEROCOPYBINDING_TRACE_API_TEMPORARY_GAP
//    \ACCEPT  XX
//    \ACCEPT  XF
//    \ACCEPT  TX
//    \REASON  Temporary justification until relevant paths are fully tested. The path is encapsulated by compiler
//    switch and it is documented that the component is unsafe once this compiler switch is active. See ara::com
//    technical reference.
// COV_JUSTIFICATION_END

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_IMPL_H_
