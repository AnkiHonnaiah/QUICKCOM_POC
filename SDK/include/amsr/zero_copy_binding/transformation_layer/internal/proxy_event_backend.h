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
 *         \brief  Proxy event backend for the ZeroCopyBinding.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ProxyEventBackend
 *
 *    \complexity  The high complexity of the class ProxyEventBackend from the equally named unit results from:
 *                 - High number of inter-unit and inter-module calls required for correct use of the ZeroCopyCommon
 *                   client and connection.
 *                 - The necessary error handling in addition contributes to the inter-unit and inter-module calls.
 *                 - The sequence that is triggered by Subscribe() and lasts until OnStateTransition(), which is
 *                   necessary to establish a connection to the skeleton event manager counterpart. Here the inter-unit
 *                   and inter-module calls from the individual contributing functions are accumulated when a path
 *                   covering big parts of the connection sequence is traversed.
 *                 The first two points cannot be avoided and are necessary for this class to work in a correct and safe
 *                 manner. The error handling can be tested as it is carried out by individual, self-contained
 *                 functions. The functions that are necessary to establish a connection to the skeleton event manager
 *                 have to be properly synchronized in order to handle exceptional cases like connection breakdowns or
 *                 premature unsubscription by the user along the way. Further they are an integral part of the
 *                 functionality of the class and cannot be reasonably outsourced as a whole without compromising
 *                 maintainability. During testing, it is possible to reach all states and error cases without major
 *                 effort, as the individual steps can be executed by helper functions performing required standard
 *                 actions.
 *                 In summary, the complexity is increased by many calls between other units and modules that are
 *                 necessary for the correct functionality of this unit and cannot be reasonably refactored. Careful
 *                 design of the class ensures that testing is not impeded and thus the risk is kept as low as possible.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cassert>
#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <type_traits>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/e2e/e2e_result.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/memory_wrapper_proxy_event.h"
#include "ara/com/types_common.h"
#include "ara/core/memory_resource.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/core/internal/slot_analyzer.h"
#include "amsr/zero_copy_binding/core/internal/slot_content.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_service_update_listener.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/zero_copy_common_client_manager.h"

#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
#include "amsr/zero_copy_binding/core/internal/trace/life_cycle_manager_trace.h"
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Template for the ZeroCopyBinding proxy event backend.
 * \tparam  SampleType   Type of a single event sample.
 */
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYBINDING_Metric-OO.WMC.One_weightedMethodsPerClassTooHigh
template <typename SampleType>
class ProxyEventBackend final : public ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>,
                                public ProxyEventServiceUpdateListener {
  /*!
   * \brief Check that the SampleType is not a reference or has cv qualifiers.
   */
  static_assert((!std::is_reference<SampleType>::value) && (!std::is_const<SampleType>::value) &&
                    (!std::is_volatile<SampleType>::value),
                "SampleType must not be a reference or have cv qualifiers.");

  /*!
   * \brief The types of the ZeroCopyCommon client and client builder interfaces.
   */
  using ClientBuilderInterface = ::amsr::zero_copy_common::internal::ClientBuilderInterface;
  using ClientInterface = ::amsr::zero_copy_common::internal::ClientInterface;
  using ClientState = ::amsr::zero_copy_common::internal::ClientInterface::ClientState;
  using ZeroCopyCommonErrc = ::amsr::zero_copy_common::common::ZeroCopyCommonErrc;
  using SideChannelInterface = ::amsr::zero_copy_common::internal::SideChannelInterface;

  /*!
   * \brief SlotContent type of the sample slot.
   */
  using SlotContent = core::internal::SlotContent<SampleType>;

  /*!
   * \brief Callback type of the reactor software event.
   */
  using ReactorSoftwareEvent = ::amsr::ipc_service_discovery::common::internal::ReactorSoftwareEvent;

 public:
  /*!
   * \brief Fetching type names from event backend interface class.
   */
  using ProxyEventBackendInterface = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;
  using typename ProxyEventBackendInterface::CallableReadSamplesResult;
  using typename ProxyEventBackendInterface::EventSubscriberInterface;
  using typename ProxyEventBackendInterface::ReadSamplesResult;
  using typename ProxyEventBackendInterface::SampleData;
  using typename ProxyEventBackendInterface::TimeStamp;
  using CallableEventNotification = typename ProxyEventBackendInterface::CallableEventNotification;
  using CallableSubscriptionStateUpdate = typename ProxyEventBackendInterface::CallableSubscriptionStateUpdate;

  /*!
   * \brief       Constructor of ProxyEventBackend.
   * \param[in]   zero_copy_binding_config_view  A view to the binding's config struct.
   *                                             It must remain valid for the object's entire lifetime.
   *                                             lifetime to ensure its validity.
   * \param[in]   provided_instance_id           The provided instance identifier of the skeleton counterpart.
   * \param[in]   life_cycle_manager_core        A pointer to the LifeCycleManagerCore.
   *                                             The LifeCycleManagerCore has to exist for the object's entire lifetime.
   * \param[in]   proxy_instance_id              The instance id of the corresponding proxy xf.
   * \internal steady FALSE \endinternal
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  ProxyEventBackend(common::internal::config::ConfigView zero_copy_binding_config_view,
                    ::amsr::socal::internal::ProvidedInstanceIdentifier provided_instance_id,
                    std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
                    std::size_t proxy_instance_id) noexcept;

  /*!
   * \brief       Destructor of ProxyEventBackend.
   * \details     Blocks until all callbacks of the used connection have finished.
   * \context     App
   * \pre         All retrieved samples must have been returned.
   * \internal steady FALSE \endinternal
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  ~ProxyEventBackend() noexcept final;

  ProxyEventBackend() = delete;
  ProxyEventBackend(ProxyEventBackend const&) = delete;
  ProxyEventBackend(ProxyEventBackend&&) = delete;
  auto operator=(ProxyEventBackend const&) -> ProxyEventBackend& = delete;
  auto operator=(ProxyEventBackend&&) -> ProxyEventBackend& = delete;

  /*!
   * \brief         Subscribes to the event.
   * \param[in,out] event       A pointer to the corresponding proxy event. Used for notification handling.
   *                            Given pointer must not be a nullptr. The ownership of the object is not transferred.
   *                            The caller of the API must ensure passing a valid pointer. The passed pointer must be
   *                            valid until Unsubscribe() is called.
   * \param[in]     cache_size  The maximum number of cached events. This parameter is currently not supported by the
   *                            ZeroCopyBinding.
   * \pre           Event must not have already been subscribed to.
   * \internal steady FALSE \endinternal
   * \context       App
   * \threadsafe    TRUE
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   FALSE Subscription state is reported asynchronously.
   */
  void Subscribe(EventSubscriberInterface* event, std::size_t cache_size) noexcept final;

  /*!
   * \brief         Unsubscribes from the event.
   * \details       After the call to Unsubscribe() the user will no longer be notified about incoming events.
   *                The samples obtained from this event have to be returned until the object is destroyed.
   * \param[in,out] event   A pointer to the corresponding proxy event. Used for notification handling.
   *                        Given event pointer must not be a nullptr.
   *                        Given event pointer must equal the event pointer from the Subscribe() call.
   * \pre           -
   * \internal steady FALSE \endinternal
   * \context       App
   * \threadsafe    TRUE
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   TRUE
   */
  // VECTOR NL AutosarC++17_10-M7.1.2: MD_ZEROCOPYBINDING_M7.1.2_nonConstPointerArgumentOverriddenFunction
  void Unsubscribe(EventSubscriberInterface* event) noexcept final;

  /*!
   * \brief       Accesses the samples on the shared memory and passes them to the provided callable.
   * \details     Reads all current available samples until max_samples has been reached.
   * \param[in]   max_samples            Maximum number of samples that shall be processed within this call.
   * \param[in]   callable_sample_result Callable to be invoked per received sample.
   * \return      Result containing the number of events that have been passed to the provided callable.
   * \error       The error code ::ara::com::ComErrc::kMaxSamplesReached from the interface is currently not supported.
   * \pre         Event must be subscribed to.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   *              TRUE against GetE2EResult and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE (However, asynchronous garbage collection is triggered.)
   */
  auto ReadSamples(std::size_t max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final;

  /*!
   * \brief       This function is currently not supported by the ZeroCopyBinding.
   * \return      Always the value 0 to signal that this functionality is not supported.
   * \pre         Event must be subscribed to.
   * \context     App | Callback
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final;

  /*!
   * \brief       This function is currently not supported by the ZeroCopyBinding.
   * \return      Always E2EState::NoData and E2ECheckStatus::NotAvailable to signal that this functionality is not
   *              supported.
   * \pre         -
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              TRUE against ReadSamples and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetE2EResult() const noexcept -> ::ara::com::e2e::Result const final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener::OnServiceInstanceUp
   */
  void OnServiceInstanceUp() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener::OnServiceInstanceDown
   * \details   This function only prints logging messages.
   *            The actual handling of the disconnect will be done in HandleConnectionError().
   */
  void OnServiceInstanceDown() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener::IsConnectedToSkeletonEvent
   */
  auto IsConnectedToSkeletonEvent() const noexcept -> bool final;

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   *
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples. The given
   *                      callable shall not be null and shall be valid at least until it is de-registered. The given
   *                      callable shall not be invoked prior subscription and after unsubscription.
   *
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  auto RegisterReceiveHandler(CallableEventNotification callable) noexcept -> void final;

  /*!
   * \brief Deregisters the event receive handler.
   *
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto DeregisterReceiveHandler() noexcept -> void final;

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   *
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes. The
   *                      given callable shall not be null and shall be valid at least until it is de-registered. The
   *                      given callable shall not be invoked prior subscription and after unsubscription. The given
   *                      callback shall not be invoked with "kNotSubscribed" state. The given callback shall not be
   *                      invoked if the subscription state has not changed.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  auto RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept -> void final;

  /*!
   * \brief Deregisters the subscription state change handler.
   *
   * \pre         The subscription state change handler is registered before.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto DeregisterSubscriptionStateChangeHandler() noexcept -> void final;

  /*!
   * \brief Query current subscription state.
   *
   * \return Current state of the subscription.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final;

 private:
  /*!
   * \brief       This function is called when the connection has been established after StartConnectingToSkeleton().
   * \param[in]   result      The result received from the ipc connection.
   * \param[in]   generation  The generation counter of the currently active connection.
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void OnConnectionCompleted(::ara::core::Result<void> result, std::size_t generation) noexcept;

  /*!
   * \brief       This function is called when the ZeroCopyCommon client undergoes a state transition.
   * \param[in]   client_state  The result received from the ZeroCopyCommon client.
   * \param[in]   generation    The generation counter of the currently active ZeroCopyCommon client.
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void OnStateTransition(ClientState client_state, ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code,
                         std::size_t generation) noexcept;

  /*!
   * \brief       This function is called on notifications of the ZeroCopyCommon client.
   * \param[in]   generation  The generation counter of the currently active ZeroCopyCommon client.
   * \pre         The user must only call GetNewSamples() on the Socal proxy in their notification callback.
   *              This cannot be checked by the implementation.
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void OnZeroCopyCommonNotification(std::size_t generation) noexcept;

  /*!
   * \brief       Start connecting to the skeleton.
   * \return      A result containing an error if the connection was not successful. Note that a positive result does
   *              not mean that a connection could be established, but only that there was no immediate error.
   * \error       GenErrc::kRuntimeResourceNotAvailable   If the service is no longer offered.
   *              Furthermore all errors of ::amsr::ipc::Connection::ConnectAsync() are forwarded.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  auto StartConnectingToSkeleton() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief       Handle errors that occurred during connection establishment.
   * \param[in]   error_code  The error code that shall be inspected. Its domain has to be OsabErrorDomain.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleConnectionEstablishmentErrors(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief       Handle connection errors.
   * \details     The method does not try to reconnect.
   * \param[in]   error_code  The error code that has been received from the ZeroCopyCommon client.
   * \context     ANY
   * \pre         An active ZeroCopyCommon client has to exist and it has to be in state kDisconnected or kCorrupted.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleConnectionError(::amsr::core::ErrorCode const& error_code) const noexcept;

  /*!
   * \brief       Handles a connection error based on the integrity levels.
   * \param[in]   own_integrity_level_result    A result containing the own integrity level.
   * \param[in]   peer_integrity_level_result   A result containing the peer integrity level.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleConnectionErrorBasedOnIntegrityLevel(
      ::ara::core::Result<::amsr::ipc::IntegrityLevel> own_integrity_level_result,
      ::ara::core::Result<::amsr::ipc::IntegrityLevel> peer_integrity_level_result) const noexcept;

  /*!
   * \brief       Handle connection errors obtained from the ZeroCopyCommon client methods by return value.
   * \details     The method does not try to reconnect.
   * \param[in]   error_code  The error code that has been received from the ZeroCopyCommon client.
   * \context     ANY
   * \pre         An active ZeroCopyCommon client has to exist.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleSynchronousZeroCopyCommonError(::amsr::core::ErrorCode const& error_code) const noexcept;

  /*!
   * \brief       Handle errors that were returned by ReleaseSlot() (see ~MemoryWrapperProxyEvent()).
   * \details     The method tries to reconnect in a reactor software event, since the callback might be called in any
   *              context and subscriber_guard_ might already be locked in ReadSamples().
   * \param[in]   error_code                    The error code that has been received from the ZeroCopyCommon client.
   * \param[in]   generation                    The generation counter of the currently active ZeroCopyCommon client.
   * \param[in]   own_integrity_level_result    A result containing the own integrity level.
   * \param[in]   peer_integrity_level_result   A result containing the peer integrity level.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void HandleReleaseSlotErrors(
      ::amsr::core::ErrorCode const& error_code, std::size_t generation,
      ::ara::core::Result<::amsr::ipc::IntegrityLevel> const& own_integrity_level_result,
      ::ara::core::Result<::amsr::ipc::IntegrityLevel> const& peer_integrity_level_result) noexcept;

  /*!
   * \brief       Cleans up the current connection and ZeroCopyCommon client if necessary and notifies Socal on demand.
   * \param[in]   notify  A boolean indicating whether the user shall be notified before the cleanup.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE (Garbage collection is done asynchronously.)
   * \vprivate    Component private
   */
  void CleanUpAndTransitionToPending(bool notify) noexcept;

  /*!
   * \brief       Try to reconnect to the skeleton.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  void TryReconnect() noexcept;

  /*!
   * \brief       Determine whether the ZeroCopyCommon client is in a proper state to receive samples.
   * \return      A boolean which is true if the ZeroCopyCommon client is not ready to receive samples.
   * \pre         The ZeroCopyCommonClientManager has to store an active ZeroCopyCommon client.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsActiveClientSubscriptionPending() const noexcept -> bool;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief A view to the binding's configuration data.
   */
  common::internal::config::ConfigView const zero_copy_binding_config_view_;

  /*!
   * \brief The factory used to obtain the connection wrapper and the ZeroCopyCommon client builder when needed.
   */
  std::shared_ptr<common::internal::FactoryInterface const> const factory_;

  /*!
   * \brief The event subscriber.
   */
  EventSubscriberInterface* subscriber_{nullptr};

  /*!
   * \brief    A mutex for protection against parallel (un-)subscriptions.
   * \details  Must be recursive as ReadSamples() can be called in any context. In particular, when notified about new
   *           messages, the user can call ReadSamples() while the mutex is already locked.
   */
  std::recursive_mutex mutable subscriber_lock_{};

  /*!
   * \brief An atomic variable that indicates whether the event is currently connected to a skeleton event.
   */
  std::atomic_bool connected_to_skeleton_event_{false};

  /*!
   * \brief An atomic variable that indicates whether StartListening() was already called on the ZeroCopyCommon client,
   *        to prevent ReadSamples() from getting samples to early.
   */
  std::atomic_bool start_listening_flag_{false};

  /*!
   * \brief An atomic variable that indicates the event subscription state.
   */
  std::atomic<::ara::com::SubscriptionState> subscription_state_{::ara::com::SubscriptionState::kNotSubscribed};

  /*!
   * \brief The provided instance identifier of the skeleton to which the proxy shall be connected.
   */
  ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id_;

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  /*!
   * \brief The lifecycle manager for tracing.
   */
  ::amsr::core::Optional<std::reference_wrapper<core::internal::trace::LifeCycleManagerTrace>>
      life_cycle_manager_trace_{};
  /*!
   * \brief The trace source id of this trace source.
   */
  ::amsr::core::Optional<core::internal::trace::Dispatcher::TraceSourceId> trace_source_id_{};
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

  /*!
   * \brief A reference to the reactor.
   */
  ::osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief A pointer to the service discovery. Needed to determine the up-to-date unicast address upon connection.
   */
  std::shared_ptr<common::internal::service_discovery::ServiceDiscoveryInterface const> const service_discovery_;

  /*!
   * \brief The ZeroCopyCommon client manager.
   */
  ZeroCopyCommonClientManager zero_copy_common_client_manager_{};

  /*!
   * \brief The SlotAnalyzer for analysis of meta data.
   */
  core::internal::SlotAnalyzer slot_analyzer_{};

  /*!
   * \brief The corresponding proxy xf's instance id.
   */
  std::size_t const proxy_instance_id_;

  /*!
   * \brief The currently ongoing reconnect events needed for the error handling in ReadSamples().
   */
  using ReconnectEventsList = std::list<::amsr::core::Optional<ReactorSoftwareEvent>>;
  ReconnectEventsList reconnect_events_{};

  /*!
   * \brief A mutex for protection against parallel access of reconnect_events_.
   */
  std::mutex mutable reconnect_events_lock_{};

  /*!
   * \brief The polymorphic allocator used to obtain memory for the memory wrapper in ReadSamples().
   */
  ::ara::core::PolymorphicAllocator<MemoryWrapperProxyEvent<SampleType>> const polymorphic_allocator_{};

  /*!
   * \brief The time period that is waited between each check of IsInUse() upon cleanup.
   */
  static std::chrono::milliseconds constexpr kCleanupDelay{10};

  /*! Friend test declaration. */
  FRIEND_TEST(UnicastAddresses, PollForUnicastAddressExpectedResult);
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

// clang-format off
/*!
 * \exclusivearea ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::subscriber_lock_
 *                Serializes all ProxyEventBackend API calls that are executed during the subscription/unsubscription or
 *                depend on the event backend to be in a specific subscription state.
 *                The mutex must be recursive as the user can optionally destruct their samples during the ReadSamples()
 *                method, which can trigger an error handling.
 *
 * \protects ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::subscriber_
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::zero_copy_common_client_manager_
 *
 * \usedin   ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::Subscribe
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::Unsubscribe
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::ReadSamples
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::OnZeroCopyCommonNotification (calls a user callback)
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::GetFreeSampleCount
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::OnServiceInstanceUp
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::OnServiceInstanceDown
 *
 * \exclude  ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::~ProxyEventBackend
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::GetE2EResult
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::IsConnectedToSkeletonEvent
 *
 * \length   LONG The lock is held during the whole execution of the methods. Most methods are not expected to be run
 *           concurrently on normal circumstances, the mutex is mostly there to avoid race conditions during error and
 *           subscription state change handling. In particular during a call to ReadSamples() the connection and
 *           subscription state must not be altered. If there are many samples available and the user does expensive
 *           computations on them, the lock might be held for a longer time. When connection errors are detected, the
 *           method will take care of them itself and return, so there is no interference with the other error handling
 *           routines.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::reconnect_events_lock_
 *                Used to protect the modification of the reconnect_events_ list. This also includes waiting that an
 *                empty optional has been filled in HandleReleaseSlotErrors().
 *
 * \protects ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::reconnect_events_
 *
 * \usedin   ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::HandleReleaseSlotErrors (can be triggered by
 *                        ::amsr::zero_copy_binding::transformation_layer::internal::MemoryWrapperProxyEvent::~MemoryWrapperProxyEvent)
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::ReadSamples
 *
 * \exclude  ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::~ProxyEventBackend
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::Subscribe
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::Unsubscribe
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::GetFreeSampleCount
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::GetE2EResult
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::OnServiceInstanceUp
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::OnServiceInstanceDown
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventBackend::IsConnectedToSkeletonEvent
 *
 * \length   SHORT In ReadSamples() the lock is held a very short time while reconnect_events_ is modified.
 *                 In HandleReleaseSlotErrors() most parts of the function call are protected, but the callback is only
 *                 expected to be triggered if ReleaseSlot() of the ZeroCopyCommon client failed. Since the binding is
 *                 only advertised once per ZeroCopyCommon client on protocol errors, there can only be concurrent calls
 *                 from different generations of ZeroCopyCommon clients and the expected interference is very small.
 * \endexclusivearea
 */
// clang-format on

// VECTOR NL AutosarC++17_10-M16.0.1: MD_ZEROCOPYBINDING_M16.0.1_temporaryPostponedIncludeQuickFix
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_backend_impl.h"

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_BACKEND_H_
