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
 *         \brief  Contains the definition of the templated ProxyEventBackend class.
 *
 *       \details This header file has to be included by the source file proxy_event_backend.cpp, where all used types
 *                 will be instantiated explicitly.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_IMPL_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <tuple>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/e2e/e2e_types.h"
#include "amsr/e2e/state_machine/state.h"
#include "amsr/ipc/connection.h"
#include "amsr/socal/internal/events/memory_wrapper_interface.h"
#include "ara/core/optional.h"
#include "osabstraction/osab_error_domain.h"

#include "amsr/thread/this_thread.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/ara_com_instance_id_xf.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/connection_validation_info.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_client.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

template <typename SampleType>
ProxyEventBackend<SampleType>::ProxyEventBackend(
    common::internal::config::ConfigView const zero_copy_binding_config_view,
    ::amsr::socal::internal::ProvidedInstanceIdentifier provided_instance_id,
    std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
    std::size_t const proxy_instance_id) noexcept
    : ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>{},
      ProxyEventServiceUpdateListener{},
      // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
      logger_{
          // VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
          "ProxyEventBackend (event id: " +
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
          std::to_string(zero_copy_binding_config_view.AsEventConfig().event_id.value) + ")"},
      zero_copy_binding_config_view_{zero_copy_binding_config_view},
      factory_{life_cycle_manager_core->GetFactory()},
      // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
      provided_instance_id_{std::move(provided_instance_id)},
      reactor_{life_cycle_manager_core->GetReactor()},
      service_discovery_{life_cycle_manager_core->GetServiceDiscovery()},
      proxy_instance_id_{proxy_instance_id} {
  // VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  // clang-format off
  if (zero_copy_binding_config_view_.AccessConfiguration().trace_enabled) { // COV_ZEROCOPYBINDING_TRACE_API_TEMPORARY_GAP
    // clang-format on
    life_cycle_manager_trace_.emplace(life_cycle_manager_core->GetLifeCycleManagerTrace());
    trace_source_id_.emplace(life_cycle_manager_trace_.value().get().GetTraceDispatcher()->RegisterTraceSource());
  }
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
}

/*!
 * \internal
 * - If tracing is enabled: Unregister the TraceSource from the TraceDispatcher.
 * - Abort if the event is still connected to its skeleton event counterpart.
 * - Abort if there is an active connection.
 * - Trigger GarbageCollect() and abort if there are still ZeroCopyCommon clients in use.
 * - Wait until the ZeroCopyCommonClientManager is empty.
 * - Wait until all reconnect events have finished and remove them.
 * \endinternal
 */
template <typename SampleType>
ProxyEventBackend<
    SampleType>::~ProxyEventBackend() noexcept {  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  // clang-format off
  if (zero_copy_binding_config_view_.AccessConfiguration().trace_enabled) { // COV_ZEROCOPYBINDING_TRACE_API_TEMPORARY_GAP
    // clang-format on
    life_cycle_manager_trace_.value().get().GetTraceDispatcher()->UnregisterTraceSource(*trace_source_id_);
  }
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

  if (connected_to_skeleton_event_.load()) {
    LOG_FATAL_AND_ABORT(logger_, "Event is still connected to skeleton event, aborting.");
  }

  if (zero_copy_common_client_manager_.ContainsActiveConnectionWrapper()) {
    LOG_FATAL_AND_ABORT(logger_, "There shall be no active connection upon destruction, aborting.");
  }

  // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_GARBAGE_COLLECT_CHECKED
  std::size_t const zero_copy_common_clients_in_use{zero_copy_common_client_manager_.GarbageCollect()};
  if (zero_copy_common_clients_in_use != 0) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_FATAL_AND_ABORT(logger_, [zero_copy_common_clients_in_use](auto& stream) {
      stream << "There is/are still " << zero_copy_common_clients_in_use
             << " event sample/s in use upon destruction of the proxy event backend, aborting.";
    });
  }

  // Wait until all ZeroCopyCommon clients have been removed.
  // Due to the checks above we know that there is no active ZeroCopyCommon client or connection at this point.
  // So the call to Empty() will eventually return true.
  while (!zero_copy_common_client_manager_.Empty()) {
    // The timeout is longer than the waiting period in the other cleanup loops. However, the thread may resume earlier
    // when notified via the condition variable.
    bool const timeout_reached{zero_copy_common_client_manager_.WaitOnCleanupFor(20 * kCleanupDelay) ==
                               std::cv_status::timeout};
    if (timeout_reached) {
      LOG_DEBUG(logger_, "Timeout of WaitOnCleanupFor() has been reached.");
      std::ignore =
          zero_copy_common_client_manager_
              // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_GARBAGE_COLLECT_CHECKED
              .GarbageCollect();  // COV_ZeroCopyBinding_FunctionCallCoverage_violation_unreachable_garbage_collect
    }
  }

  // HandleReleaseSlotErrors() can no longer be called as all slots have been released after the previous loop
  // has finished.
  // Any ongoing reconnect callback will not try to reconnect, since in Unsubscribe() the last ZeroCopyCommon client
  // has been retired and as such the generation has been increased.
  for (ReconnectEventsList::value_type const& reconnect_event : reconnect_events_) {
    // VCA_ZEROCOPYBINDING_LNG_03_VALID_RECONNECT_EVENT_REFERENCE
    while (reconnect_event->IsInUse())  // COV_ZeroCopyBinding_BranchCoverage_violation_untestable_is_in_use_true
    {
      ::amsr::thread::SleepFor(kCleanupDelay);
    }
  }
  reconnect_events_.clear();  // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
}

/*!
 * \internal
 * - Set the subscription state of the corresponding Socal event to kSubscriptionPending.
 * - Abort if the event has already been subscribed to.
 * - Start to connect to the skeleton.
 * - If the connection returned an error, handle it accordingly.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::Subscribe(EventSubscriberInterface* const event,
                                              std::size_t const cache_size) noexcept {
  LOG_INFO(logger_, "Subscribe to event.");

  if (cache_size != 0U) {
    LOG_WARN(logger_, "The parameter cache_size of ProxyEventBackend::Subscribe() will not be used.");
  }

  if (event == nullptr) {
    LOG_FATAL_AND_ABORT(logger_, "Event subscriber pointer for event is a nullptr, aborting.");
  }

  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  subscription_state_.store(::ara::com::SubscriptionState::kSubscriptionPending);
  // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
  event->HandleEventSubscriptionStateUpdate(subscription_state_);

  if (subscriber_ != nullptr) {
    LOG_FATAL_AND_ABORT(logger_, "The event has already been subscribed to, aborting.");
  } else {
    subscriber_ = event;
  }

  // This call adds a connection wrapper to the ZeroCopyCommon client manager on success.
  StartConnectingToSkeleton().ConsumeError([this](auto const& error_code) {
    if (error_code.Domain() == ::osabstraction::GetOsabDomain()) {
      // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
      this->HandleConnectionEstablishmentErrors(error_code);
    } else {
      // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
      LOG_INFO(this->logger_,
               "Subscription to event could not be completed, since the service is currently not offered. "
               "The subscription will be resumed when the service is offered again.");
    }
  });
}

/*!
 * \internal
 * - If the event is currently not subscribed to:
 *   - Log an info message to the user and return.
 * - Else:
 *   - If parameter preconditions are not met:
 *     - Abort execution with an appropriate error message.
 *   - If there exists an active ZeroCopyCommon client:
 *     - Disconnect and stop listening on the ZeroCopyCommon client.
 *   - If there exists an active connection:
 *     - Remove the current connection and ZeroCopyCommon client.
 *   - Reset the subscriber.
 *   - Set the subscription state of the event backend to kNotSubscribed.
 * \endinternal
 */
template <typename SampleType>
// VECTOR NL AutosarC++17_10-M7.1.2: MD_ZEROCOPYBINDING_M7.1.2_nonConstPointerArgumentOverriddenFunction
void ProxyEventBackend<SampleType>::Unsubscribe(EventSubscriberInterface* const event) noexcept {
  LOG_INFO(logger_, "Unsubscribe from event.");

  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (event == nullptr) {
    LOG_FATAL_AND_ABORT(logger_, "Event unsubscriber pointer for event is a nullptr, aborting.");
  } else if (subscriber_ == nullptr) {
    LOG_INFO(logger_, "Ignoring Unsubscribe() on event, which has not been subscribed to.");
  } else if (event != subscriber_) {
    LOG_FATAL_AND_ABORT(logger_,
                        "Event unsubscriber pointer for event must equal the event subscriber pointer, aborting.");
  } else {
    // TODO (virskl, 15.04.2024): Check for AccessControl (BEFO-2407). Probably not needed.

    if (zero_copy_common_client_manager_.ContainsActiveZeroCopyCommonClient()) {
      zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()->StopListening().ConsumeError(
          [this](auto const& error_code) {
            // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
            this->HandleSynchronousZeroCopyCommonError(error_code);
          });
    }

    if (zero_copy_common_client_manager_.ContainsActiveConnectionWrapper()) {
      zero_copy_common_client_manager_.RetireActiveZeroCopyCommonClient();
    }

    // Non blocking garbage collect. From now on the notification callback set in StartListening() (potentially also
    // from the older ZeroCopyCommon clients) could still be called, however it will no longer forward the call to
    // Socal, because subscriber_ is no longer set. That means the user will no longer get new samples after the call
    // to Unsubscribe(). The user has time to return all samples until the call of the destructor, where also a busy
    // wait is done until no notification callback is on the reactor.
    std::ignore = zero_copy_common_client_manager_.GarbageCollect(&reactor_);
    subscriber_ = nullptr;

    connected_to_skeleton_event_.store(false);
    subscription_state_.store(::ara::com::SubscriptionState::kNotSubscribed);
  }
}

/*!
 * \internal
 * - Initialize the loop counter (i.e., the processed samples) before the loop to be able to return it later.
 * - As long as ClientInterface::ReceiveSlot() returns a sample:
 *   - Access the SlotContent.
 *   - If receiving or accessing the slot failed:
 *     - Handle the returned error based on the error code and the integrity class.
 *     - Return the slot if a slot was received.
 *     - Cleanup the connection wrapper and ZeroCopyCommon client and try to reconnect.
 *     - Leave the function.
 *   - Analyze the SlotContent's header.
 *   - If tracing is enabled: Call the TraceDispatcher.
 *   - Create a view to the sample raw data.
 *   - Initialize an object of type MemoryWrapperProxyEvent using the raw data and the obtained slot.
 *     The MemoryWrapperProxyEvent is passed a shared pointer to the active ZeroCopyCommon client.
 *     It is also passed an error handler that calls HandleReleaseSlotErrors().
 *   - Construct the E2E check status (not available) and a timestamp.
 *   - Construct an object of type SampleData out of the previously constructed object.
 *   - Call the user provided callback handing over the ownership of the sample data.
 *   - Increase the counter of the processed samples.
 * - Return the number of processed samples.
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::ReadSamples(std::size_t const max_samples,
                                                CallableReadSamplesResult const& callable_sample_result) noexcept
    -> ReadSamplesResult {
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (subscriber_ == nullptr) {
    LOG_FATAL_AND_ABORT(logger_, "The event has not been subscribed to, aborting.");
  }

  std::size_t processed_samples{0U};

  // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_GARBAGE_COLLECT_CHECKED
  std::ignore = zero_copy_common_client_manager_.GarbageCollect(&reactor_);

  bool ready_to_read{false};
  if (zero_copy_common_client_manager_.ContainsActiveZeroCopyCommonClient()) {
    ready_to_read = !IsActiveClientSubscriptionPending();
  }

  if (!ready_to_read) {
    LOG_DEBUG(logger_, "The subscription of the event has not yet been completed.");
  } else if (!start_listening_flag_.load()) {
    // We do not allow to read samples before the state transition callback has finished, to simplify the error
    // handling there.
    LOG_DEBUG(logger_, "The proxy event backend has not started to listen to the ZeroCopyCommon client yet.");
  } else {
    LOG_DEBUG(logger_, "The subscription of the event has been completed. Reading available samples.");

    std::shared_ptr<ClientInterface> const active_zero_copy_common_client{
        zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()};

    core::internal::ConnectionWrapperInterface const& active_connection_wrapper{
        zero_copy_common_client_manager_.GetActiveConnectionWrapper()};

    while (processed_samples < max_samples) {
      ::amsr::core::Result<::amsr::core::Optional<ClientInterface::SlotToken>> next_zero_copy_slot{
          // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
          active_zero_copy_common_client->ReceiveSlot()};

      if (!next_zero_copy_slot.HasValue()) {
        HandleSynchronousZeroCopyCommonError(next_zero_copy_slot.Error());
        CleanUpAndTransitionToPending(false);
        TryReconnect();
        break;
      }
      if (!next_zero_copy_slot.Value().has_value()) {
        break;  // No further data is available.
      }

      ClientInterface::SlotToken slot_token{std::move(next_zero_copy_slot.Value().value())};
      auto access_slot_content_result(
          // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
          active_zero_copy_common_client->AccessSlotContent(slot_token));

      if (!access_slot_content_result.HasValue()) {
        HandleSynchronousZeroCopyCommonError(access_slot_content_result.Error());
        // No error handling necessary, since we are already in a failed state.
        // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
        std::ignore = active_zero_copy_common_client->ReleaseSlot(std::move(slot_token));
        CleanUpAndTransitionToPending(false);
        TryReconnect();
        break;
      }
      ::amsr::core::Span<std::uint8_t const> const byte_view{access_slot_content_result.Value()};

      // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastOnSharedMemoryData
      core::internal::SlotContentHeader const* const slot_content_header{
          reinterpret_cast<core::internal::SlotContentHeader const*>(byte_view.data())};

      slot_analyzer_
          .Analyze(*slot_content_header)  // VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
          // VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_ifdefVoidFunctionWithoutSideEffect
          .Consume(
              [this, slot_content_header](
                  core::internal::SlotAnalysis const&
                      slot_analysis) {  // COV_ZeroCopyBinding_FunctionCallCoverage_violation_ignoring_currently_unused_code_path
                std::ignore = slot_content_header;
                std::ignore = slot_analysis;
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
                if (this->zero_copy_binding_config_view_.AccessConfiguration().trace_enabled) {
// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
                  // clang-format off
                  // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
                  if (this->zero_copy_binding_config_view_.AsEventConfig().trace_read_sample_enabled) { // COV_ZEROCOPYBINDING_TRACE_API_TEMPORARY_GAP
                    // clang-format on
                    life_cycle_manager_trace_.value().get().GetTraceDispatcher()->TraceEventReadSample(
                        *trace_source_id_, proxy_instance_id_, *slot_content_header,
                        this->zero_copy_binding_config_view_, slot_analysis);
                  }
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
                } else {
                  std::ignore = slot_content_header;
                  std::ignore = slot_analysis;
                }
              })
          .ConsumeError([this](auto const& error) {
            // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_VALID_USER_MESSAGE_CALL
            LOG_WARN(logger_, [error_message = error.UserMessage()](auto& log_stream) {
              log_stream << "Analysis of slot content failed with error: " << error_message;
            });
          });

      // VECTOR NC AutosarC++17_10-M5.0.15: MD_ZEROCOPYBINDING_M5.0.15_checkedPointerArithmetic
      ::ara::core::Span<std::uint8_t const> const sample_raw_data{
          byte_view.data() + slot_content_header->payload_offset, slot_content_header->payload_size};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_ZEROCOPYBINDING_A18.5.8_movedObjectDetectedAsLocal
      std::shared_ptr<MemoryWrapperProxyEvent<SampleType>> memory_wrapper{
          std::allocate_shared<  // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
              MemoryWrapperProxyEvent<SampleType>>(
              polymorphic_allocator_, sample_raw_data, std::move(slot_token), active_zero_copy_common_client,
              // We required that all samples have been returned before calling the destructor. Hence it is safe
              // to capture the this pointer here.
              // VECTOR NL AutosarC++17_10-A5.0.1, VectorC++-V5.0.1, AutosarC++17_10-A5.1.5: MD_ZEROCOPYBINDING_A5.0.1_unsequencedGetIntegrityLevelCalls, MD_ZEROCOPYBINDING_V5.0.1_unsequencedGetIntegrityLevelCalls, MD_ZEROCOPYBINDING_A5.1.5_lambdaNotUsedLocally
              [this, callback_generation = zero_copy_common_client_manager_.GetCurrentGeneration(),
               // It is important to capture the integrity levels of the current peer at this point, since the
               // active ZeroCopyCommon client might change over time.
               // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
               own_integrity_level_result = active_connection_wrapper.GetOwnIntegrityLevel(),
               peer_integrity_level_result =
                   // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
               active_connection_wrapper.GetPeerIntegrityLevel()](auto const& error) {
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
                this->HandleReleaseSlotErrors(error, callback_generation, own_integrity_level_result,
                                              peer_integrity_level_result);
              })};
      ::ara::com::E2E_state_machine::E2ECheckStatus const check_status{
          ::ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable};
      ::amsr::core::Optional<TimeStamp> const timestamp{TimeStamp{}};

      // Pass the event data to the user callback.
      // We do not use a sample cache (second argument of SampleData).
      // The slot will be released when the memory wrapper is destructed.
      // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
      callable_sample_result(SampleData{std::move(memory_wrapper), {}, check_status, timestamp});

      ++processed_samples;
    }
  }

  {
    std::lock_guard<std::mutex> const reconnect_events_guard{reconnect_events_lock_};
    // VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
    reconnect_events_.remove_if([](auto& reconnect_event_opt) { return !reconnect_event_opt->IsInUse(); });
  }

  return ReadSamplesResult{processed_samples};
}

/*!
 * \internal
 * - Abort if the event is currently not subscribed to.
 * - Log a warn message that this function is currently not supported.
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::GetFreeSampleCount() const noexcept -> std::size_t {
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (subscriber_ == nullptr) {
    LOG_FATAL_AND_ABORT(logger_, "The event is not subscribed to, aborting.");
  }

  LOG_WARN(logger_,
           "The method ProxyEventBackend::GetFreeSampleCount() is currently not supported by ZeroCopyBinding.");
  return 0;
}

/*!
 * \internal
 * - Return a standard result that indicates that this function is not supported.
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::GetE2EResult() const noexcept -> ::ara::com::e2e::Result const {
  return ::amsr::e2e::Result{::ara::com::E2E_state_machine::E2EState::NoData,
                             ::ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable};
}

template <typename SampleType>
// VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
auto ProxyEventBackend<SampleType>::RegisterReceiveHandler(CallableEventNotification callable) noexcept -> void {
  static_cast<void>(callable);
  ::amsr::core::Abort("Unsupported function.");
}

template <typename SampleType>
// VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
auto ProxyEventBackend<SampleType>::DeregisterReceiveHandler() noexcept -> void {
  ::amsr::core::Abort("Unsupported function.");
}

template <typename SampleType>
// VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
auto ProxyEventBackend<SampleType>::RegisterSubscriptionStateChangeHandler(
    CallableSubscriptionStateUpdate callable) noexcept -> void {
  static_cast<void>(callable);
  ::amsr::core::Abort("Unsupported function.");
}

template <typename SampleType>
// VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYBINDING_M0.1.8_VoidFunctionHasSideEffectAbort
auto ProxyEventBackend<SampleType>::DeregisterSubscriptionStateChangeHandler() noexcept -> void {
  ::amsr::core::Abort("Unsupported function.");
}

template <typename SampleType>
auto ProxyEventBackend<SampleType>::GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState {
  return subscription_state_.load();
}

/*!
 * \internal
 * - If there is an active subscriber:
 *   - If the event backend is not connected to a skeleton event:
 *     - Start to connect to the skeleton.
 *     - If the connection returned an error, handle it accordingly.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::OnServiceInstanceUp() noexcept {
  // No subscribe allowed during service update.
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (subscriber_ == nullptr) {
    LOG_DEBUG(logger_, "Event is not subscribed to.");
  } else {
    LOG_INFO(logger_, "Event received ProvidedState::kProvided.");

    // Reconnect to the skeleton.
    if (zero_copy_common_client_manager_.ContainsActiveConnectionWrapper()) {
      LOG_INFO(logger_, "There is already an active connection.");
    } else {
      LOG_INFO(logger_, "Try to reconnect to the skeleton.");

      // This call adds a connection wrapper to the ZeroCopyCommon client manager on success.
      StartConnectingToSkeleton().ConsumeError([this](auto const& error_code) {
        if (error_code.Domain() != ::osabstraction::GetOsabDomain()) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
          LOG_INFO(this->logger_,
                   "Reconnect to skeleton could not be completed, since the service is currently not offered. "
                   "The connection will be retried when the service is offered again.");
        } else {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
          this->HandleConnectionEstablishmentErrors(error_code);
        }
      });
    }
  }
}

/*!
 * \internal
 * - Log debug/info message about the current subscription state of the proxy event backend.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::OnServiceInstanceDown() noexcept {
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (subscriber_ == nullptr) {
    LOG_DEBUG(logger_, "Event is not subscribed to.");
  } else {
    LOG_INFO(logger_, "Event received ProvidedState::kNotProvided. Waiting for Disconnect from ZeroCopyCommon client.");
  }
}

/*!
 * \internal
 * - Return the value of the atomic boolean variable that keeps track of the current connection state.
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::IsConnectedToSkeletonEvent() const noexcept -> bool {
  return connected_to_skeleton_event_.load();
}

/*!
 * \internal
 * - If the current generation does not equal the callback's generation:
 *   - Log a debug message and return.
 * - If the result does not contain a value:
 *   - Call HandleConnectionEstablishmentErrors() and try to reconnect.
 * - Abort if the peer integrity level is smaller than its expected integrity level.
 * - Send the identification info of the event directly via the connection wrapper. If this fails:
 *   - Call HandleConnectionEstablishmentErrors() and try to reconnect.
 * - Obtain the client builder from the factory.
 * - Create the ZeroCopyCommon client by passing among others the created side channel wrapper and the state
 *   transition callback.
 * - Add the created ZeroCopyCommon client to the ZeroCopyCommonClientManager.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::OnConnectionCompleted(::ara::core::Result<void> const result,
                                                          std::size_t const generation) noexcept {
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (generation != zero_copy_common_client_manager_.GetCurrentGeneration()) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_DEBUG(logger_, [generation](auto& stream) {
      stream << "Ignoring outdated connection completed callback from generation " << generation << ".";
    });
  } else {
    if (!result.HasValue()) {
      HandleConnectionEstablishmentErrors(result.Error());
      TryReconnect();
    } else {
      LOG_DEBUG(logger_, "Established connection to the skeleton. Setting up ZeroCopyCommon client.");

      // Check integrity level of peer.
      zero_copy_common_client_manager_.GetActiveConnectionWrapper()
          .GetPeerIntegrityLevel()  // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
          .Consume([&config = zero_copy_binding_config_view_, &logger = logger_](auto const& peer_integrity_level) {
            // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            if (peer_integrity_level < config.AsRequiredConfig().expected_minimum_server_integrity_level.value) {
              LOG_FATAL_AND_ABORT(logger, "Server does not match expected integrity level, aborting.");
            }
          })
          .ConsumeError([&logger = logger_](auto const&) {
            LOG_FATAL_AND_ABORT(logger, "Failed to determine the peer's integrity level, aborting.");
          });

      // TODO (virskl, 15.04.2024): Check for AccessControl (BEFO-2407).

      // Fill and send connection validation info.
      std::array<std::uint8_t, sizeof(ConnectionValidationInfo)> message_buffer{};
      // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastForSendSync
      ConnectionValidationInfo* const validation_info{
          reinterpret_cast<ConnectionValidationInfo*>(message_buffer.data())};
      // VECTOR NC AutosarC++17_10-A18.5.2: MD_ZEROCOPYBINDING_A18.5.2_placementNew
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      new (validation_info) ConnectionValidationInfo{zero_copy_binding_config_view_};

      ::amsr::core::Result<void> const send_result{
          // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
          zero_copy_common_client_manager_.GetActiveConnectionWrapper().SendSync(message_buffer)};
      if (!send_result.HasValue()) {
        HandleConnectionEstablishmentErrors(send_result.Error());
        TryReconnect();
      } else {
        // VCA_ZEROCOPYBINDING_FACTORY_POINTER_VALID
        std::unique_ptr<SideChannelInterface> side_channel_ptr{factory_->GetSafeIPCSideChannel(
            // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
            zero_copy_common_client_manager_.GetActiveConnectionWrapper().GetConnection())};

        // VCA_ZEROCOPYBINDING_FACTORY_POINTER_VALID
        std::shared_ptr<ClientBuilderInterface> const client_builder{factory_->GetZeroCopyCommonClientBuilder()};
        if (!client_builder) {
          LOG_FATAL_AND_ABORT(logger_, "The factory returned an invalid ZeroCopyCommon client builder, aborting.");
        }

        start_listening_flag_.store(false);

        ::amsr::zero_copy_binding::common::internal::config::MemoryTechnology::Type const allocation_memory_technology{
            // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            zero_copy_binding_config_view_.AsRequiredEventConfig().allocation_memory_technology.value};
        ClientBuilderInterface::MemoryTechnology const memory_technology{
            (allocation_memory_technology ==
             ::amsr::zero_copy_binding::common::internal::config::MemoryTechnology::Type::kSharedMemory)
                ? ClientBuilderInterface::MemoryTechnology::kSharedMemory
                : ClientBuilderInterface::MemoryTechnology::kPhysContigSharedMemory};

// TODO (jmeisinger, 08.04.2024):
// When generic proxy will be enabled as non-beta feature, decide between:
// - Remove these lines of code entirely.
// - If C++17 is already activated, make this code compile time conditional based on whether SampleType is
//   GenericProxyPredicate or not.
// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_includeBetaDefinition
#ifndef AMSR_LG_BETA_ENABLED
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedClientBuilderSetterCalls
        // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedClientBuilderSetterCalls
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_previouslyCheckedPointer
        client_builder
            // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
            ->WithSlotContentSize(ClientBuilderInterface::SlotContentSize{sizeof(SlotContent)})
            // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
            .WithSlotContentAlignment(ClientBuilderInterface::SlotContentAlignment{alignof(SlotContent)});
#endif  // AMSR_LG_BETA_ENABLED

        // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedClientBuilderSetterCalls
        // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedClientBuilderSetterCalls
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_previouslyCheckedPointer
        std::unique_ptr<ClientInterface> zero_copy_common_client{
            // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
            client_builder
                // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
                ->WithMemoryTechnology(memory_technology)
                // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
                .WithSideChannel(std::move(side_channel_ptr))
                // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
                .WithOnStateTransitionCallback(
                    [this, generation = zero_copy_common_client_manager_.GetCurrentGeneration()](
                        ClientState client_state, auto opt_error_code) {
                      // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
                      this->OnStateTransition(client_state, opt_error_code, generation);
                    })
                // VCA_ZEROCOPYBINDING_VALID_BUILDER_RETURNED_BY_SETTER_FUNCTION
                .Build()
                .Value()};

        if (zero_copy_common_client == nullptr) {
          ::amsr::core::Abort(
              "ProxyEventBackend::OnConnectionCompleted(): An invalid ZeroCopyCommon client has been obtained.");
        }
        // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID
        ::amsr::core::Result<void> const connect_result{zero_copy_common_client->Connect()};
        if (!connect_result.HasValue()) {
          assert(connect_result.Error() == ZeroCopyCommonErrc::kUnexpectedState);
          ::amsr::core::Abort(
              "ProxyEventBackend::OnConnectionCompleted(): The ZeroCopyCommon client reported an unexpected state.");
        }
        std::ignore = zero_copy_common_client_manager_.AddZeroCopyCommonClient(std::move(zero_copy_common_client));
      }
    }
  }
}

/*!
 * \internal
 * - If the current generation does not equal the callback's generation log a debug message and return.
 * - Depending on the new state do the following:
 *   - If the new state is kCorrupted:
 *     - Trigger error handling, passing the optional error code.
 *   - If the new state is kConnected:
 *     - Set the state of the proxy event backend to connected.
 *     - Start listening to the ZeroCopyCommon client and call OnZeroCopyCommonNotification() on notifications.
 *     - Notify Socal about the subscription state update.
 *   - If the new state is kDisconnected:
 *     - Cleanup the current connection and ZeroCopyCommon client and try to reconnect.
 *   - If the new state is kDisconnectedRemote:
 *     - Cleanup the current connection and ZeroCopyCommon client and try to reconnect.
 *   - Abort if the new state of the ZeroCopyCommon client is unknown.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::OnStateTransition(
    ClientState const client_state, ::amsr::core::Optional<::amsr::core::ErrorCode> const opt_error_code,
    std::size_t const generation) noexcept {
  std::lock_guard<std::recursive_mutex> const subscriber_guard{subscriber_lock_};

  if (generation != zero_copy_common_client_manager_.GetCurrentGeneration()) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_DEBUG(logger_, ([client_state, generation](auto& stream) {
                // VECTOR NC AutosarC++17_10-A4.5.1: MD_ZEROCOPYBINDING_A4.5.1_enumStreamOperatorNotArithmetic
                stream << "Outdated state transition to '" << client_state << "' from generation " << generation
                       << " has been ignored.";
              }));
  } else {
    // During an ongoing connection sequence only the cases kCorrupted, kConnected or kDisconnected can occur.
    // kConnected can only occur during an ongoing connection sequence.
    switch (client_state) {
      case ClientState::kCorrupted:
        assert(opt_error_code.has_value());
        LOG_DEBUG(logger_, "The ZeroCopyCommon client has switched to a corrupted state. Starting error handling.");
        HandleConnectionError(*opt_error_code);
        CleanUpAndTransitionToPending(false);
        TryReconnect();
        break;
      case ClientState::kConnected:
        LOG_DEBUG(logger_, "Performed connection handshake to ZeroCopyCommon client. Starting to listen.");

        connected_to_skeleton_event_.store(true);

        // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedZeroCopyCommonClientManagerMethodCalls
        // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedZeroCopyCommonClientManagerMethodCalls
        zero_copy_common_client_manager_
            .GetActiveZeroCopyCommonClient()
            // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
            ->StartListening([this, generation = zero_copy_common_client_manager_.GetCurrentGeneration()]() {
              // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
              this->OnZeroCopyCommonNotification(generation);
            })
            .ConsumeError([this](auto const& error_code) {
              assert(error_code != ZeroCopyCommonErrc::kUnexpectedState);
              // The ZeroCopyCommon client is now in state kCorrupted.
              // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
              this->CleanUpAndTransitionToPending(false);
              TryReconnect();
            });

        // If the user could have received samples before, we would have to distinguish by quality class in the
        // error handling above.
        start_listening_flag_.store(true);
        subscription_state_.store(::ara::com::SubscriptionState::kSubscribed);
        // VCA_ZEROCOPYBINDING_VALID_SUBSCRIBER_POINTER
        subscriber_->HandleEventSubscriptionStateUpdate(subscription_state_);
        break;
      case ClientState::kDisconnected:
        // The disconnect happened during the handshake (if it was triggered by ourselves we would not get notified in
        // the callback).
        LOG_DEBUG(logger_, "Failed to perform the connection handshake with the skeleton.");
        CleanUpAndTransitionToPending(false);
        TryReconnect();
        break;
      case ClientState::kDisconnectedRemote:
        // The disconnect was triggered from the server.
        // We are expected to call Disconnect() on the ZeroCopyCommon client.
        LOG_DEBUG(logger_, "The ZeroCopyCommon client has been disconnected from the remote server.");
        // Notify the user before cleaning up, to prevent message loss.
        CleanUpAndTransitionToPending(true);
        TryReconnect();
        break;
      default:
        // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
        LOG_FATAL_AND_ABORT(
            logger_,
            // VECTOR NC AutosarC++17_10-A4.5.1: MD_ZEROCOPYBINDING_A4.5.1_enumStreamOperatorNotArithmetic
            [client_state](auto& stream) { stream << "Unexpected client state '" << client_state << "'."; });
        break;
    }
  }
}

/*!
 * \internal
 * - If the callback belongs to the current generation, notify the subscriber.
 * - Else log a debug message.
 * - Notify Socal about the subscription state update.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::OnZeroCopyCommonNotification(std::size_t const generation) noexcept {
  // Ignoring this callback for older generations does not lead to message loss because:
  // * When StopListening() is called in Unsubscribe() the user is no longer interested in new samples.
  // * When StopListening() is called in CleanUpAndTransitionToPending() we in addition notify the user.
  //   So the call to the callback becomes irrelevant for this generation from then on.
  std::lock_guard<std::recursive_mutex> const notification_guard{subscriber_lock_};

  if (generation == zero_copy_common_client_manager_.GetCurrentGeneration()) {
    // TODO (virskl, 15.04.2024): Check for AccessControl (BEFO-2407). Probably not needed.

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_previouslyCheckedPointer
    subscriber_->HandleEventNotification();  // VCA_ZEROCOPYBINDING_VALID_SUBSCRIBER_POINTER
  } else {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_DEBUG(logger_, [generation](auto& stream) {
      stream << "Outdated call to the notification callback from generation " << generation << " has been ignored.";
    });
  }
}

/*!
 * \internal
 * - Construct provided instance identifier of the type the service discovery takes.
 * - Obtain the recent unicast address of the skeleton counterpart or an error when the service is not provided.
 * - If a unicast address could be found:
 *   - Obtain a new connection wrapper from the factory.
 *   - Connect asynchronously to the skeleton, with OnConnectionCompleted() as a callback. Set the maximum shared
 *     memory size to 4KB, which is the default page size.
 *   - Remove the added connection wrapper in case of an immediate error.
 * - Else:
 *   - Notify the user by a debug message that the connection could not be established (the result contains an error).
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::StartConnectingToSkeleton() noexcept -> ::amsr::core::Result<void> {
  LOG_INFO(logger_, "Start trying to connect to the skeleton.");

  auto result(::amsr::core::Result<void>::FromError(::amsr::generic::GenErrc::kRuntimeResourceNotAvailable));

  // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYBINDING_A5.0.1_configViewAccessFunctionsNoSideEffect
  // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_configViewAccessFunctionsNoSideEffect
  common::internal::service_discovery::ProvidedServiceInstanceIdentifier const sd_provided_service_instance_id{
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      AraComInstanceIdXf::BuildServiceInstanceIdentifier(provided_instance_id_.GetAraComInstanceIdentifier()).Value()};

  ::amsr::core::Result<::ara::core::Optional<common::internal::service_discovery::UnicastAddress>> const
      // VCA_ZEROCOPYBINDING_SERVICE_DISCOVERY_POINTER_VALID
      unicast_address_result{service_discovery_->PollService(sd_provided_service_instance_id)};

  if (unicast_address_result.HasValue() && unicast_address_result.Value().has_value()) {
    core::internal::ConnectionWrapperInterface::ConnectCompletionCallback connection_completion_callback{
        [this, generation = zero_copy_common_client_manager_.GetCurrentGeneration()](
            ::ara::core::Result<void> connect_complete_result) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
          this->OnConnectionCompleted(std::move(connect_complete_result), generation);
        }};

    core::internal::ConnectionWrapperInterface& connection_wrapper{
        zero_copy_common_client_manager_.AddConnectionWrapper(
            // VCA_ZEROCOPYBINDING_FACTORY_POINTER_VALID
            factory_->GetConnectionWrapper(
                // VCA_ZEROCOPYBINDING_SPC_15_VALID_PERFECTLY_FORWARDED_CTOR_CALL
                std::make_unique<::amsr::ipc::Connection>(reactor_)))};

    // Choose a number slightly smaller than 4096, as SafeIpc management data needs to be taken into account.
    static std::size_t constexpr kSHMBufferSize{4000UL};

    result =
        // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
        connection_wrapper.ConnectAsync(unicast_address_result.Value().value(),
                                        std::move(connection_completion_callback), kSHMBufferSize);

    if (!result.HasValue()) {
      zero_copy_common_client_manager_.RetireActiveZeroCopyCommonClient();
    }

  } else {
    LOG_DEBUG(logger_,
              "Could not connect to skeleton event since the service the event belongs to is currently not offered.");
    result.EmplaceError(::amsr::generic::GenErrc::kRuntimeResourceNotAvailable,
                        ::amsr::core::StringView{"Service has not been found."});
  }

  return result;
}

/*!
 * \internal
 * - For the error codes that suggest that the skeleton can currently not be reached, log an info message and retire
 *   the current connection.
 * - For the error codes that imply that some more grave errors have occurred, abort the program.
 * - In all other cases abort, reporting an unexpected error.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::HandleConnectionEstablishmentErrors(
    ::amsr::core::ErrorCode const& error_code) noexcept {
  assert(error_code.Domain() == ::osabstraction::GetOsabDomain());

  if ((error_code == ::osabstraction::OsabErrc::kAddressNotAvailable) ||
      (error_code == ::osabstraction::OsabErrc::kDisconnected) ||
      (error_code == ::osabstraction::OsabErrc::kInsufficientPrivileges) ||
      (error_code == ::osabstraction::OsabErrc::kProtocolError)) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_VALID_USER_MESSAGE_CALL
    LOG_INFO(logger_, [error_message = error_code.UserMessage()](auto& stream) {
      stream << "Failed to establish the connection to the skeleton. Retry as soon as the service is announced to be "
                "available again. Further information: "
             << error_message;
    });
    CleanUpAndTransitionToPending(false);
  } else if ((error_code == ::osabstraction::OsabErrc::kAlreadyConnected) ||
             (error_code == ::osabstraction::OsabErrc::kResource) ||
             (error_code == ::osabstraction::OsabErrc::kUnexpected) ||
             (error_code == ::osabstraction::OsabErrc::kSystemEnvironmentError) ||
             // Exclusive errors of ::amsr::ipc::Connection::SendSync():
             (error_code == ::osabstraction::OsabErrc::kBusy) || (error_code == ::osabstraction::OsabErrc::kSize) ||
             (error_code == ::osabstraction::OsabErrc::kUninitialized)) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_VALID_USER_MESSAGE_CALL
    LOG_FATAL_AND_ABORT(logger_, [error_message = error_code.UserMessage()](auto& stream) {
      stream << "Encountered an error upon connecting that cannot be adequately dealt with, aborting. Further "
                "information: "
             << error_message;
    });
  } else {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_VALID_USER_MESSAGE_CALL
    LOG_FATAL_AND_ABORT(logger_, [error_message = error_code.UserMessage()](auto& stream) {
      stream << "Unexpected error, aborting. Further information: " << error_message;
    });
  }
}

/*!
 * \internal
 * - If the server has been disconnected gracefully, continue regularly.
 * - If the server has crashed, continue regularly.
 * - If the server has not been disconnected gracefully or a protocol error occurred, call
 *   HandleConnectionErrorBasedOnIntegrityLevel(), which does:
 *   - If own integrity level is smaller than or equal to the peer integrity level, abort.
 *   - Else, continue regularly.
 * - If the error code is unknown, abort.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::HandleConnectionError(::amsr::core::ErrorCode const& error_code) const noexcept {
  std::shared_ptr<ClientInterface> const active_zero_copy_common_client{
      zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()};

  core::internal::ConnectionWrapperInterface const& active_connection_wrapper{
      zero_copy_common_client_manager_.GetActiveConnectionWrapper()};

  // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
  if (active_zero_copy_common_client->GetState() == ClientState::kDisconnectedRemote) {
    LOG_INFO(logger_, "The server has been disconnected gracefully. Continue and try to reconnect.");
  } else if (error_code == ZeroCopyCommonErrc::kPeerCrashedError) {
    LOG_DEBUG(logger_, "The server has crashed. Continue and try to reconnect.");
  } else if ((error_code == ZeroCopyCommonErrc::kPeerDisconnectedError) ||
             (error_code == ZeroCopyCommonErrc::kProtocolError)) {
    LOG_INFO(logger_, "The server has not been disconnected gracefully.");
    // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_ZEROCOPYBINDING_A5.0.1_unsequencedGetIntegrityLevelCalls
    HandleConnectionErrorBasedOnIntegrityLevel(
        // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
        active_connection_wrapper.GetOwnIntegrityLevel(),
        // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
        active_connection_wrapper.GetPeerIntegrityLevel());
  } else {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_VALID_USER_MESSAGE_CALL
    LOG_FATAL_AND_ABORT(logger_, [error_message = error_code.UserMessage()](auto& stream) {
      stream << "Unexpected connection error, aborting. Further information: " << error_message;
    });
  }

  // No need to disconnect due to the precondition.
}

/*!
 * \internal
 * - Check and retrieve the integrity levels from the passed results.
 * - If own integrity level is smaller than or equal to the peer integrity level, abort.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::HandleConnectionErrorBasedOnIntegrityLevel(
    ::ara::core::Result<::amsr::ipc::IntegrityLevel> const own_integrity_level_result,
    ::ara::core::Result<::amsr::ipc::IntegrityLevel> const peer_integrity_level_result) const noexcept {
  LOG_DEBUG(logger_,
            "Experienced malfunctioning server and/or a protocol error. Deciding based on the integrity level how to "
            "proceed.");

  if (!own_integrity_level_result.HasValue()) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_FATAL_AND_ABORT(logger_, [error_message = own_integrity_level_result.Error().UserMessage()](auto& stream) {
      stream << "Own integrity level could not be determined, aborting. Further information: " << error_message;
    });
  }

  if (!peer_integrity_level_result.HasValue()) {
    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_FATAL_AND_ABORT(logger_, [error_message = peer_integrity_level_result.Error().UserMessage()](auto& stream) {
      stream << "Peer integrity level could not be determined, aborting. Further information: " << error_message;
    });
  }

  if (own_integrity_level_result.Value() <= peer_integrity_level_result.Value()) {
    LOG_FATAL_AND_ABORT(logger_,
                        "Peer has an integrity level greater or equal than the own integrity level, aborting.");
  }
}

/*!
 * \internal
 * - In case of a protocol error or if the server crashed call HandleConnectionError().
 * - Else if the ZeroCopyCommon client reported an unexpected state:
 *   - If the ZeroCopyCommon client is in state corrupted, log a warn message.
 *   - Else log a debug message.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::HandleSynchronousZeroCopyCommonError(
    ::amsr::core::ErrorCode const& error_code) const noexcept {
  if (error_code == ZeroCopyCommonErrc::kProtocolError || error_code == ZeroCopyCommonErrc::kPeerCrashedError) {
    HandleConnectionError(error_code);  // VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
  } else {
    assert(error_code == ZeroCopyCommonErrc::kUnexpectedState);
    // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
    if (zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()->GetState() == ClientState::kCorrupted) {
      LOG_WARN(logger_,
               "The ZeroCopyCommon client is in a corrupted state. No more samples will be received until a reconnect "
               "is tried.");
    } else {
      LOG_DEBUG(logger_, "The ZeroCopyCommon client reported an unexpected state.");
    }
  }
}

/*!
 * \internal
 * - Abort if own integrity level is smaller than or equal to the peer integrity level.
 * - Add and trigger a reactor software event that does the following:
 *   - If the current generation does not equal the callback's generation log a debug message and return.
 *   - Else cleanup and try to reconnect.
 *   - Unregister the reactor software event.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::HandleReleaseSlotErrors(
    ::amsr::core::ErrorCode const& error_code, std::size_t const generation,
    ::ara::core::Result<::amsr::ipc::IntegrityLevel> const& own_integrity_level_result,
    ::ara::core::Result<::amsr::ipc::IntegrityLevel> const& peer_integrity_level_result) noexcept {
  assert(error_code == ZeroCopyCommonErrc::kProtocolError);

  LOG_DEBUG(logger_, "An error occurred while returning the slot token. Starting error handling.");

  HandleConnectionErrorBasedOnIntegrityLevel(own_integrity_level_result, peer_integrity_level_result);

  std::lock_guard<std::mutex> const reconnect_events_guard{reconnect_events_lock_};
  // VCA_ZEROCOPYBINDING_SPC_15_VALID_PERFECTLY_FORWARDED_CTOR_CALL
  reconnect_events_.emplace_back(::amsr::core::nullopt);

  auto const recent_reconnect_event_it(--reconnect_events_.end());

  ReactorSoftwareEvent::SoftwareEventCallback reconnect_callback{
      [this, reconnect_event_it = recent_reconnect_event_it, generation]() {
        // OnStateTransition() locks the same mutex and checks for the generation.
        // So it cannot happen that TryReconnect() is called twice for the same generation and will
        // disconnect the next generation without reason.
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_LOCK_GUARD_REFERENCE_ARGUMENT_PROXY_EVENT_BACKEND
        std::lock_guard<std::recursive_mutex> const subscriber_guard{this->subscriber_lock_};

        // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
        if (generation != this->zero_copy_common_client_manager_.GetCurrentGeneration()) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK, VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_DEBUG(this->logger_, [generation](auto& stream) {
            stream << "Outdated call to the reconnect callback triggered by the MemoryWrapperProxyEvent error handler "
                      "from generation "
                   << generation << " has been ignored.";
          });
        } else {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
          this->CleanUpAndTransitionToPending(false);
          this->TryReconnect();
        }
        // VCA_ZEROCOPYBINDING_LNG_03_VALID_ITERATOR_TO_CLEANUP_EVENT_LIST, VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_REGISTER_UNREGISTER_CHECKED
        reconnect_event_it->value().Unregister();
      }};

  ReactorSoftwareEvent& reconnect_event{
      // VCA_ZEROCOPYBINDING_LNG_03_VALID_PREVIOUSLY_CREATED_ITERATOR_PROXY_EVENT_BACKEND
      recent_reconnect_event_it->emplace(this->reactor_, std::move(reconnect_callback))};
  reconnect_event.Register();  // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_REGISTER_UNREGISTER_CHECKED
  reconnect_event.Trigger();   // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_TRIGGER_CHECKED
}

/*!
 * \internal
 * - If there currently is a used connection:
 *   - If the event is connected to the skeleton:
 *     - Stop listening.
 *     - If demanded, notify the user that there might still be unread samples left.
 *   - Retire the active ZeroCopyCommon client if it was not removed in the meantime.
 *   - Set the Socal event to kSubscriptionPending.
 * - Set the internal connected state to false.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::CleanUpAndTransitionToPending(bool const notify) noexcept {
  if (zero_copy_common_client_manager_.ContainsActiveConnectionWrapper()) {
    if (zero_copy_common_client_manager_.ContainsActiveZeroCopyCommonClient()) {
      zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()
          ->StopListening()  // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
          .Inspect([notify, subscriber = subscriber_]() {
            if (notify) {
              // Notify the subscriber one last time, since otherwise messages could be lost.
              // This is necessary, as there is no guarantee that all notifications are received before the
              // disconnect.
              // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_previouslyCheckedPointer
              subscriber->HandleEventNotification();  // VCA_ZEROCOPYBINDING_VALID_SUBSCRIBER_POINTER
            }
          })
          .ConsumeError([this](auto const& stop_listening_error_code) {
            // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
            this->HandleSynchronousZeroCopyCommonError(stop_listening_error_code);
          });
    }

    // If in HandleEventNotification() an error occurred, there could also a cleanup be triggered. In this case, the
    // connection wrapper and ZeroCopyCommon client could have already been cleaned up.
    if (zero_copy_common_client_manager_.ContainsActiveConnectionWrapper()) {
      zero_copy_common_client_manager_.RetireActiveZeroCopyCommonClient();
      // VCA_ZEROCOPYBINDING_SLC_20_PRECONDITIONS_OF_GARBAGE_COLLECT_CHECKED
      std::ignore = zero_copy_common_client_manager_.GarbageCollect(&reactor_);
    }

    subscription_state_.store(::ara::com::SubscriptionState::kSubscriptionPending);
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_previouslyCheckedPointer
    // VCA_ZEROCOPYBINDING_VALID_SUBSCRIBER_POINTER
    subscriber_->HandleEventSubscriptionStateUpdate(subscription_state_);
  } else {
    LOG_DEBUG(logger_, "No need to cleanup as there is no active connection.");
  }
  connected_to_skeleton_event_.store(false);
}

/*!
 * \internal
 * - Start to connect to the skeleton.
 * - If the connection returned an error, handle it accordingly.
 * \endinternal
 */
template <typename SampleType>
void ProxyEventBackend<SampleType>::TryReconnect() noexcept {
  StartConnectingToSkeleton().ConsumeError([this](auto const& error_code) {
    if (error_code.Domain() == ::osabstraction::GetOsabDomain()) {
      // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
      this->HandleConnectionEstablishmentErrors(error_code);
    } else {
      // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_CALLBACK
      LOG_INFO(this->logger_,
               "Could not reconnect to the server, since the service is currently not offered. "
               "Try again when the service is offered again.");
    }
  });
}

/*!
 * \internal
 * - Obtain the state of the ZeroCopyCommon client and return if it is neither kConnected nor kDisconnectedRemote.
 * \endinternal
 */
template <typename SampleType>
auto ProxyEventBackend<SampleType>::IsActiveClientSubscriptionPending() const noexcept -> bool {
  ClientState const current_zero_copy_common_client_state{
      // VCA_ZEROCOPYBINDING_VALID_OBJECT_MANAGED_BY_ZEROCOPYCOMMONCLIENTMANAGER_VALID
      zero_copy_common_client_manager_.GetActiveZeroCopyCommonClient()->GetState()};

  return (current_zero_copy_common_client_state != ClientState::kConnected) &&
         (current_zero_copy_common_client_state != ClientState::kDisconnectedRemote);
}

/*!
 * \brief Definition of kCleanupDelay.
 */
template <typename SampleType>
std::chrono::milliseconds constexpr ProxyEventBackend<SampleType>::kCleanupDelay;

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_ZeroCopyBinding_BranchCoverage_violation_untestable_is_in_use_true
//   \ACCEPT  XF
//   \REASON  The detected violation is caused by a code path that is unreachable with the current testing setup. There
//            is no way to let ReactorSoftwareEvent::IsInUse() return true at this place without mocking the reactor
//            software event itself. The code in question consists of a waiting loop where returning true would have the
//            effect that the waiting continues until the reactor software event returns false.
//            It can be guaranteed that the waiting will eventually finish here. The reason is that the callback in
//            question is guaranteed to finish when it is run on the reactor. The mutex that it tries to lock is free
//            during the call of the destructor. It is checked by review that it also calls Unregister() on its
//            corresponding reactor software event. Hence, ReactorSoftwareEvent::IsInUse() will return false after the
//            callback has finished.
//
// \ID COV_ZeroCopyBinding_FunctionCallCoverage_violation_ignoring_currently_unused_code_path
//   \ACCEPT  XX
//   \REASON  The detected violation is caused by a code path for a feature that is currently deactivated. If this
//            branch is taken in production code, nothing will be done. If the branch is not taken, the only difference
//            is an additional warning log message. This means that the uncovered code path is functionally equivalent
//            to the default path that is tested in the unit tests.
//
// \ID COV_ZeroCopyBinding_FunctionCallCoverage_violation_unreachable_garbage_collect
//   \ACCEPT  XX
//   \REASON  The detected violation is caused by a code path that depends on a specific timing behavior to be reached.
//            There is currently no reasonable way to set the timing during the component test in such a way that the
//            code path in question is always taken. The corresponding code is well understood and covered in the unit
//            tests, so there is low risk of leaving this part uncovered.
// COV_JUSTIFICATION_END

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_IMPL_H_
