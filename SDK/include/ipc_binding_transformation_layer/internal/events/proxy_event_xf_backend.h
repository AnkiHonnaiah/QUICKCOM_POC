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
/**        \file
 *        \brief  Ipc binding implementation of ProxyEventXfBackend
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_PROXY_EVENT_XF_BACKEND_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_PROXY_EVENT_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "ara/com/com_error_domain.h"
#include "ara/com/e2e_types.h"
#include "ara/com/types_common.h"
#include "someip-protocol/internal/deserialization/reader.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/events/invisible_sample_cache.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/service_discovery/service_instance_listener_interface.h"
#include "ipc_binding_transformation_layer/internal/events/visible_sample_cache.h"
#include "ipc_binding_xf_common/internal/trace/trace_event_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief   Template for IPC Proxy event manager backend.
 * \details Handles event subscriptions and notification deserialization. Is created once per required service instance.
 *
 * \tparam SampleType              Type of a single event sample.
 * \tparam SampleDeserializerType  Deserializer used to deserialize incoming samples.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyEvent
 */
template <typename SampleType, typename SampleDeserializerType>
class ProxyEventXfBackend final : public ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>,
                                  public ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface {
 public:
  /*!
   * \brief Type of Event identifier.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Type of Client identifier.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Interface type for binding-specific proxy event backend.
   */
  using ProxyEventBackendInterface = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;

  /*!
   * \brief Type alias to use strong type.
   */
  using SizeType = std::size_t;

  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = typename ProxyEventBackendInterface::TimeStamp;

  /*!
   * \brief Type alias for VisibleSampleCache.
   */
  using VisibleSampleCache = events::VisibleSampleCache<SampleType>;

  /*!
   * \brief VisibleSampleCache pointer type.
   */
  using VisibleSampleCachePtr = std::shared_ptr<VisibleSampleCache>;  // IGNORE_BLACKLIST external API

  /*!
   * \brief Callable type to be invoked for every deserialized sample.
   */
  using CallableReadSamplesResult = typename ProxyEventBackendInterface::CallableReadSamplesResult;

  /*!
   * \brief Callable type to be invoked upon receiving new event sample from skeleton.
   */
  using CallableEventNotification = typename ProxyEventBackendInterface::CallableEventNotification;

  /*!
   * \brief Callable type to be invoked upon the event subscription state changes.
   */
  using CallableSubscriptionStateUpdate = typename ProxyEventBackendInterface::CallableSubscriptionStateUpdate;

  /*!
   * \brief Return type of ReadSamples.
   */
  using ReadSamplesResult = typename ProxyEventBackendInterface::ReadSamplesResult;

  /*!
   * \brief Return type of Deserialize.
   */
  using DeserializationResult = ::amsr::core::Result<void>;

  /*!
   * \brief Sample data type for read sample callback.
   */
  using SampleData = typename ProxyEventBackendInterface::SampleData;

  /*!
   * \brief Type alias for ServiceInstanceListenerInterface.
   */
  using ServiceInstanceListenerInterface =
      ::amsr::ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for IpcUnicastAddress.
   */
  using IpcUnicastAddress = ServiceInstanceListenerInterface::IpcUnicastAddress;

  /*!
   * \brief Alias for InvisibleSampleCache.
   */
  using InvisibleSampleCache = ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache;

  /*!
   * \brief Type alias for the container which is used by a consumer of InvisibleSampleCache.
   */
  using SampleCacheContainer = InvisibleSampleCache::SampleCacheContainer;

  /*!
   * \brief Type alias for NotificationMessage.
   */
  using NotificationMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage;

  /*!
   * \brief Type of trace event sink.
   */
  using TraceEventSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface;

  /*!
   * \brief Constructor
   *
   * \param[in] provided_service_instance  Provided service instance ID.
   * \param[in] event_id                   Event ID.
   * \param[in] client_id                  Client ID.
   * \param[in] trace_sink                 Sink for tracing of events.
   * \param[in] proxy_router               Reference to the ProxyRouter.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   *
   * \internal
   * - Add EventXf to the ProxyRouter table to enable routing of incoming ACk/NAck and notifications.
   * \endinternal
   */
  ProxyEventXfBackend(ProvidedServiceInstanceId const& provided_service_instance, EventId const event_id,
                      ClientId const client_id,
                      std::unique_ptr<TraceEventSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
                      ipc_binding_core::internal::ProxyRouterInterface& proxy_router) noexcept
      : ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>(),
        ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface{},
        provided_service_instance_{provided_service_instance},
        event_id_{event_id},
        client_id_{client_id},
        trace_sink_{std::move(trace_sink)},  // VCA_IPCB_VALID_TRACE_SINK
        proxy_router_{proxy_router},
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyEventXfBackend"}} {
    if (!trace_sink_) {
      logger_.TerminateOnViolation("Tried to construct a ProxyEventXfBackend with an invalid (empty) trace sink.",
                                   amsr::ipc_binding_core::internal::logging::AraComLogger::LogLocation{
                                       static_cast<char const*>(__func__), __LINE__});
    }

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    proxy_router_.AddEventXf(event_id_, *this);  // VCA_IPCB_VALID_PROXY_ROUTER
  }

  auto operator=(ProxyEventXfBackend&&) noexcept -> ProxyEventXfBackend& = delete;
  ProxyEventXfBackend(ProxyEventXfBackend const&) noexcept = delete;
  auto operator=(ProxyEventXfBackend const&) noexcept -> ProxyEventXfBackend& = delete;
  ProxyEventXfBackend(ProxyEventXfBackend&&) noexcept = delete;

  /*!
   * \brief Destructor
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Release EventXf from the ProxyRouter table to disable routing of incoming ACk/NAck and notifications.
   * \endinternal
   */
  // VCA_IPCB_MOLE_1298
  ~ProxyEventXfBackend() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
    proxy_router_.ReleaseEventXf(event_id_);  // VCA_IPCB_VALID_PROXY_ROUTER
  }                                           // VCA_IPCB_STD_SHARED_POINTER_DESTRUCTOR, VCA_IPCB_STD_DEQUE_DESTRUCTOR

  /*!
   * \brief   Saves the new subscriber and forwards SubscribeEvent.
   * \details Called from Event subscriber.
   *
   * \param[in,out] event       A pointer to the event manager that subscribes to this event backend. The ownership of
   *                            the object is not transferred. The caller of the API must ensure passing a valid
   *                            pointer. The passed pointer must be valid until Unsubscribe() is called.
   * \param[in]     cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                            memory for storing at least the cache_size number of deserialized samples.
   *
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Event subscriber is valid.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vprivate
   *
   * \internal
   * - Create Subscribe message header.
   * - Check if the event manager is not nullptr.
   *   - Lock guard to protect event manager storage (against parallel Subscriptions or Un-subscriptions).
   *   - Check if event backend doesn't store already a pointer to an event manager.
   *     - Set the cache capacity.
   *     - Save the subscriber pointer (used to forward back notifications).
   *   - Otherwise
   *     - Log Fatal message and abort.
   *   - Free guard against parallel Subscriptions or Un-subscriptions.
   *   - Update size of invisible sample cache with requested cache_size.
   *   - Update size of visible sample cache with requested cache_size and an extra element.
   *   - Create memory for event samples in sample cache.
   *   - Update the subscriber with the current subscription state.
   *   - Check if the service is up:
   *     - Try to access the ConnectionProxy and send event subscription.
   *     - Otherwise log a warning and drop the message.
   *   - Otherwise:
   *     - Log a warning and drop the message.
   * - Otherwise
   *   - Log Fatal message and abort.
   * \endinternal
   */
  void Subscribe(::amsr::socal::internal::events::EventSubscriberInterface* event, SizeType cache_size) noexcept final {
    logger_.LogInfo(
        [this](::ara::log::LogStream& s) {
          s << "Subscribe to event (";
          // VCA_IPCB_SERVICE_INSTANCE_IDENTIFIER_REFERENCE_VALID
          LogBuilder::LogServiceInstanceId(s, provided_service_instance_);
          s << ", ";
          LogBuilder::LogEventId(s, event_id_);
          s << ", ";
          LogBuilder::LogClientId(s, client_id_);
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    SubscribeEventMessageHeader const subscribe_header{
        provided_service_instance_.ServiceId(), provided_service_instance_.InstanceId(),
        provided_service_instance_.MajorVersion(), event_id_, client_id_};

    if (event != nullptr) {
      {
        std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};

        // No need to lock against reactor, before subscription it is not allowed to receive any events.
        if (subscriber_ == nullptr) {
          cache_capacity_ = cache_size;
          subscriber_ = event;
          subscription_status_ = ::ara::com::SubscriptionState::kSubscriptionPending;
        } else {
          logger_
              .TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
                  "Event subscription to an already subscribed event.",
                  [&subscribe_header](::ara::log::LogStream& s) {
                    s << "The event (";
                    LogBuilder::LogCompleteServiceInstanceEventId(
                        s, subscribe_header.service_id_, subscribe_header.major_version_, subscribe_header.instance_id_,
                        subscribe_header.event_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
                    s << ") has already been subscribed";
                  },
                  Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        }
      }

      {
        // Allocate visible cache with one additional spare slot (see AUTOSAR_EXP_ARAComAPI for the rationale).
        std::size_t const nr_samples_to_allocate{cache_capacity_ + 1};

        invisible_sample_cache_.Resize(cache_capacity_);
        // VCA_IPCB_STD_FUNCTION_CALLED
        visible_sample_cache_ =
            // VCA_IPCB_STD_FUNCTION_CALLED
            std::make_shared<VisibleSampleCache>(nr_samples_to_allocate);  // IGNORE_BLACKLIST external API
      }

      // Update the subscriber with subscription state.
      event->HandleEventSubscriptionStateUpdate(  // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
          ::ara::com::SubscriptionState::kSubscriptionPending);

      // Obtain current offered state, guarded by locking mutex
      bool const service_up{[this]() {
        // VCA_IPCB_LOCK_GUARD_LIFETIME
        std::lock_guard<std::mutex> const lock{service_up_lock_};
        return service_state_;
      }()};

      if (service_up) {
        // VCA_IPCB_VALID_PROXY_ROUTER
        amsr::SharedPtr<ConnectionProxyInterface> const connection_proxy{proxy_router_.GetConnectionProxy()};
        if (connection_proxy) {
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
          // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_VALID
          TransmitMessageHandlerInterface& transmit_message_handler{connection_proxy->GetTransmitMessageHandler()};
          // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_TRANSMIT_MESSAGE_HANDLER_VALID
          transmit_message_handler.SubscribeEvent(subscribe_header);
        } else {
          logger_.LogDebug(
              [](::ara::log::LogStream& s) {
                s << "Dropping subscribe event message due to disconnected unicast connection.";
              },
              Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        }
      } else {
        logger_.LogDebug(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            [](::ara::log::LogStream& s) { s << "Dropping subscribe event message as service is not offered"; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    } else {
      logger_.TerminateOnViolation(
          "Subscribe: Invalid (nullptr) event subscriber pointer provided for event (",
          [&subscribe_header](Logger::LogStream& s) {
            s << "Event: ";
            LogBuilder::LogCompleteServiceInstanceEventId(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
                s, subscribe_header.service_id_, subscribe_header.major_version_, subscribe_header.instance_id_,
                subscribe_header.event_id_);
            s << ", ";
            LogBuilder::LogClientId(
                s, subscribe_header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ")";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }

  /*!
   * \brief   Resets the subscriber and sends UnsubscribeEvent to the related ConnectionProxy.
   * \details The SubscriptionState::kNotSubscribed state must be notified in the current call context as the event
   *          subscriber pointer is not valid after this call.
   *
   * \param[in,out] event  A pointer to the event manager that subscribes this event backend. This must not be a
   *                       nullptr.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vprivate
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Guard against parallel Subscriptions or Un-subscriptions.
   * - Remove the subscriber.
   * - Create Unsubscribe message header.
   * - Check if event subscriber pointer is not nullptr.
   *   - Check if the service is up:
   *     - Try to access the ConnectionProxy and send event unsubscription.
   *     - Otherwise log a warning and drop the message.
   *   - Otherwise:
   *     - Log a warning and drop the message.
   *   - Clear invisible sample cache.
   *   - Clear the visible sample cache.
   *   - Notify subscription state update.
   * - Otherwise
   *   - Log Fatal message and abort.
   * \endinternal
   */
  void Unsubscribe(::amsr::socal::internal::events::EventSubscriberInterface* event) noexcept final {
    logger_.LogInfo(
        [this](::ara::log::LogStream& s) {
          s << "Unsubscribe from event (";
          // VCA_IPCB_SERVICE_INSTANCE_IDENTIFIER_REFERENCE_VALID
          LogBuilder::LogServiceInstanceId(s, provided_service_instance_);
          s << ", ";
          LogBuilder::LogEventId(s, event_id_);
          s << ", ";
          LogBuilder::LogClientId(s, client_id_);
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // Guard against parallel unsubscription and callbacks from reactor thread
    {
      std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
      subscriber_ = nullptr;
      subscription_status_ = ::ara::com::SubscriptionState::kNotSubscribed;
    }

    // Create Unsubscribe message header
    UnsubscribeEventMessageHeader const unsubscribe_header{
        provided_service_instance_.ServiceId(), provided_service_instance_.InstanceId(),
        provided_service_instance_.MajorVersion(), event_id_, client_id_};

    if (event != nullptr) {
      // Obtain current offered state, guarded by locking mutex
      bool const service_up{[this]() {
        // VCA_IPCB_LOCK_GUARD_LIFETIME
        std::lock_guard<std::mutex> const lock{service_up_lock_};
        return service_state_;
      }()};

      if (service_up) {
        // VCA_IPCB_VALID_PROXY_ROUTER
        amsr::SharedPtr<ConnectionProxyInterface> const connection_proxy{proxy_router_.GetConnectionProxy()};
        if (connection_proxy) {
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
          // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_VALID
          TransmitMessageHandlerInterface& transmit_message_handler{connection_proxy->GetTransmitMessageHandler()};
          // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_TRANSMIT_MESSAGE_HANDLER_VALID
          transmit_message_handler.UnsubscribeEvent(unsubscribe_header);
        } else {
          logger_.LogDebug(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              [](::ara::log::LogStream& s) {
                s << "Dropping unsubscribe event message due to disconnected unicast connection.";
              },
              Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        }
      } else {
        logger_.LogDebug(
            [](::ara::log::LogStream& s) { s << "Dropping unsubscribe event message as service is not offered"; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }

      // Clear invisible and visible cache and reset size
      invisible_sample_cache_.Clear();
      // VCA_IPCB_STD_FUNCTION_CALLED_WITHOUT_PARAMETER
      visible_sample_cache_.reset();
      cache_capacity_ = 0;

      // Notify the related subscriber about subscription state update.
      event->HandleEventSubscriptionStateUpdate(  // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
          ::ara::com::SubscriptionState::kNotSubscribed);
    } else {
      logger_.TerminateOnViolation(
          "Unsubscribe: Invalid (nullptr) event subscriber pointer provided for event (",
          [&unsubscribe_header](Logger::LogStream& s) {
            s << "Event: ";
            LogBuilder::LogCompleteServiceInstanceEventId(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
                s, unsubscribe_header.service_id_, unsubscribe_header.major_version_, unsubscribe_header.instance_id_,
                unsubscribe_header.event_id_);
            s << ", ";
            LogBuilder::LogClientId(
                s, unsubscribe_header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ")";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnNotificationReceived
   *
   * \internal
   * - If a subscriber is registered:
   *   - Enqueue the event notification message int the invisible sample cache.
   *   - Notify the subscriber about the received event notification.
   * - Return the invisible cache update result.
   * \endinternal
   */
  void OnNotificationReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage&& notification) noexcept final {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogVerbose([](::ara::log::LogStream const&) {},
                       Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    bool cache_updated{false};

    // Guard against parallel unsubscription and callbacks from reactor thread
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    if (subscriber_ != nullptr) {
      // Store a shared pointer of event sample in an invisible cache and remove the oldest cache element if full.
      // No need to check validity of invisible_sample_cache ptr
      // because it is always set/reset together with subscriber_ (which is already checked)
      // VCA_IPCB_VALID_SAMPLE_CACHE_POINTER_ACCESS
      cache_updated = invisible_sample_cache_.Enqueue(std::move(notification));

      // Call HandleEventNotification
      subscriber_->HandleEventNotification();  // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
    }

    if (!cache_updated) {
      logger_.LogWarn([](::ara::log::LogStream& s) { s << "Failed to put notification into invisible sample cache."; },
                      Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnSubscribeAckReceived
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If a subscriber is registered:
   *   - Notify the subscriber of the state change to kSubscribed.
   * \endinternal
   */
  void OnSubscribeAckReceived() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogDebug([](::ara::log::LogStream const&) {},
                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // Guard against parallel unsubscription and callbacks from reactor thread
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    if (subscriber_ != nullptr) {
      subscription_status_ = ::ara::com::SubscriptionState::kSubscribed;
      // Notify all subscribers with the new state
      // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
      subscriber_->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kSubscribed);
    }
  }

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnSubscribeNAckReceived
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If a subscriber is registered:
   *   - Notify the subscriber of the state change to kSubscriptionPending/kNotSubscribed.
   *   - Updating the subscription status to kNotSubscribed is important because it is the only way to inform user that
   *     the SubscribeNAck is received.
   *   - The subscription state can be updated to kSubscriptionPending on SetServiceState(), and then to kSubscribed
   *     when the service is available for the event (after a SubscribeAck is received).
   * \endinternal
   */
  void OnSubscribeNAckReceived() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogDebug([](::ara::log::LogStream const&) {},
                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // Guard against parallel unsubscription and callbacks from reactor thread
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    if (subscriber_ != nullptr) {
      subscription_status_ = ara::com::SubscriptionState::kNotSubscribed;
      // Notify all subscribers with the new state
      // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
      subscriber_->HandleEventSubscriptionStateUpdate(ara::com::SubscriptionState::kNotSubscribed);
    }
  }

  /*!
   * \brief   Reads the serialized samples from underlying receive buffers and deserializes them.
   * \details Binding implementation should start reading all the received samples from the top of its receive buffers.
   *          Reading of samples should be continued until either:
   *          - The pre-allocated memory for storing the deserialized samples is exhausted.
   *          - Given maximum samples have been processed within this call.
   *          - There are no further new samples to read.
   *
   * \param[in] max_samples             Maximum number of samples that can be processed within this call.
   * \param[in] callable_sample_result  Callable to be invoked on successful deserialization. The callable is valid only
   *                                    until the scope of this function call, so storing and invoking it at a later
   *                                    point will lead to undefined behavior.
   *
   * \return An amsr::core::Result containing the number of events successfully processed (with no deserialization
   *         errors) within this call or an error.
   *
   * \error ara::com::ComErrc::kMaxSamplesReached  If all slots from visible sample cache are used at the beginning of
   *                                               the call.
   *
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if.
   *              - Subscribe call is returned successfully, or.
   *              - HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances. TRUE against GetE2EResult and
   *              GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   *
   * \internal
   * - If no subscriber is registered.
   *   - Log fatal and call Abort().
   * - Create a result with an error ComErrc::kMaxSamplesReached.
   * - If there are sample pointers available in the visible sample cache.
   *   - If there are events available in the invisible cache.
   *     - Call ReadSamplesInternal.
   *   - Emplace number of processed events in result.
   *   - Return result.
   * \endinternal
   */
  auto ReadSamples(SizeType const max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final {
    if (subscriber_ == nullptr) {
      constexpr char const* error_message{"ReadSamples() called while not being subscribed!"};
      logger_.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
          error_message, [](::ara::log::LogStream& s) { s << error_message; },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }

    ::ara::com::ComErrorDomain::SupportDataType const no_support_data_provided{0};
    ReadSamplesResult result{ReadSamplesResult::FromError(
        ::ara::com::MakeErrorCode(::ara::com::ComErrc::kMaxSamplesReached, no_support_data_provided,
                                  "Application holds more SamplePtrs than committed in Subscribe"))};

    // VCA_IPCB_VALID_SAMPLE_CACHE_POINTER_ACCESS
    if (visible_sample_cache_->GetFreeSampleCount() > 0) {
      SizeType valid_events_processed{0};

      SampleCacheContainer& invisible_app_cache_container{invisible_sample_cache_.GetSamples(max_samples)};

      // Note: the returned cache might have size equal, greater
      // than or less than max_samples.
      // In case more samples than requested exist, do not
      // process the additional samples

      if (!invisible_app_cache_container.empty()) {
        valid_events_processed =
            ReadSamplesInternal(invisible_app_cache_container, max_samples, callable_sample_result);
      } else {  // No samples were available in invisible cache
        logger_.LogVerbose(
            [](::ara::log::LogStream& s) { s << "No new samples available in invisible sample cache to process."; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
      result.EmplaceValue(valid_events_processed);
    }
    return result;
  }

  /*!
   * \brief Gets the E2E result of the recent E2E check.
   *
   * \return Always E2EState::NoData and E2ECheckStatus::NotAvailable.(IpcBinding does not support E2E).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE for same class instance, TRUE for different instances. TRUE against ReadSamples and
   *              GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Always return E2EState::NoData and E2ECheckStatus::NotAvailable.
   * \endinternal
   */
  auto GetE2EResult() const noexcept -> ::ara::com::e2e::Result const final {
    return ::amsr::e2e::Result{::ara::com::E2E_state_machine::E2EState::NoData,
                               ::ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable};
  }  // COV_IpcBinding_FunctionCoverage_dummy_function_not_supported_feature

  /*!
   * \brief   Query the number of available sample slots in the local sample cache.
   * \details This API must not be called in parallel to service discovery APIs (Subscribe/Unsubscribe).
   *
   * \return The number of free samples available in the local sample cache.
   *
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if.
   *              - Subscribe call is returned successfully, or.
   *              - HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances. TRUE against ReadSamples and GetE2EResult
   *              when invoked on same/different class instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   *
   * \internal
   * - If the event cache is valid.
   *   - If the number of free samples in the cache exceed the number of subscribed samples.
   *     - Return the number of subscribed samples.
   *   - Otherwise
   *     - Return the number of free samples in the cache.
   * - Otherwise
   *   - Return 0.
   * \endinternal
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final {
    std::size_t result{0};
    if (subscriber_ != nullptr) {
      std::size_t const invisible_sample_cache_capacity{invisible_sample_cache_.Capacity()};
      // VCA_IPCB_VALID_SAMPLE_CACHE_POINTER_ACCESS
      std::size_t const visible_sample_cache_free_sample_count{visible_sample_cache_->GetFreeSampleCount()};
      result = std::min(invisible_sample_cache_capacity, visible_sample_cache_free_sample_count);
    }
    return result;
  }

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface::SetConnectionState
   */
  void SetConnectionState(bool connection_state) noexcept final {
    if (connection_state) {
      ResendSubscription();
    }
  }

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface::SetServiceState
   */
  void SetServiceState(bool service_state) noexcept final {
    {
      std::lock_guard<std::mutex> const lock{service_up_lock_};
      service_state_ = service_state;
    }

    if (service_state) {
      ResendSubscription();
    } else {
      HandleEventSubscriptionStateDown();
    }
  }

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   *
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples. The given
   *                      callable shall not be null and shall be valid at least until it is de-registered. The given
   *                      callable shall not be invoked prior subscription and after unsubscription.
   *
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_UnsupportedFunction_has_no_external_effect
  auto RegisterReceiveHandler(CallableEventNotification callable) noexcept -> void final {
    static_cast<void>(callable);
    ::amsr::core::Abort("Unsupported function.");
  }

  /*!
   * \brief Deregisters the event receive handler.
   *
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_UnsupportedFunction_has_no_external_effect
  auto DeregisterReceiveHandler() noexcept -> void final { ::amsr::core::Abort("Unsupported function."); }

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   *
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes. The
   *                      given callable shall not be null and shall be valid at least until it is de-registered. The
   *                      given callable shall not be invoked prior subscription and after unsubscription. The given
   *                      callback shall not be invoked with "kNotSubscribed" state. The given callback shall not be
   *                      invoked if the subscription state has not changed.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_UnsupportedFunction_has_no_external_effect
  auto RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept -> void final {
    static_cast<void>(callable);
    ::amsr::core::Abort("Unsupported function.");
  }

  /*!
   * \brief Deregisters the subscription state change handler.
   *
   * \pre         The subscription state change handler is registered before.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_UnsupportedFunction_has_no_external_effect
  auto DeregisterSubscriptionStateChangeHandler() noexcept -> void final {
    ::amsr::core::Abort("Unsupported function.");
  }

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
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final {
    // The Callback function (for example, CallableSubscriptionStateUpdate) called in a reactor context must not
    // invoke GetSubscriptionState() directly. Else, it may lead to a deadlock.
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    return subscription_status_;
  }

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for SubscribeEventMessageHeader.
   */
  using SubscribeEventMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::SubscribeEventMessageHeader;

  /*!
   * \brief Type alias for UnsubscribeEventMessageHeader.
   */
  using UnsubscribeEventMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::UnsubscribeEventMessageHeader;

  /*!
   * \brief Type alias for ConnectionProxyInterface.
   */
  using ConnectionProxyInterface = ::amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface;

  /*!
   * \brief Type alias for TransmitMessageHandlerInterface.
   */
  using TransmitMessageHandlerInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface;

  /*!
   * \brief   Sends out a subscribe message if the event was already subscribed.
   * \details Called from the ProxyRouter to indicate that monitored service instance is available again.
   *
   * \context     Reactor
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   *
   * \internal
   * - Check the subscription state.
   * - If the subscription is in pending state.
   *   - Send a new subscription request.
   * - Else return and do nothing.
   * \endinternal
   */
  void ResendSubscription() noexcept {
    // Send subscribe only when already subscribed.
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    if (subscription_status_ == ::ara::com::SubscriptionState::kSubscriptionPending) {
      logger_.LogInfo(
          [this](::ara::log::LogStream& s) {
            s << "Resubscribe to event (";
            // VCA_IPCB_SERVICE_INSTANCE_IDENTIFIER_REFERENCE_VALID
            LogBuilder::LogServiceInstanceId(s, provided_service_instance_);
            s << ", ";
            LogBuilder::LogEventId(s, event_id_);
            s << ")";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

      // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
      // VCA_IPCB_VALID_PROXY_ROUTER
      amsr::SharedPtr<ConnectionProxyInterface> const connection_proxy{proxy_router_.GetConnectionProxy()};
      if (connection_proxy) {
        // Create Subscribe message header
        SubscribeEventMessageHeader const subscribe_header{
            provided_service_instance_.ServiceId(), provided_service_instance_.InstanceId(),
            provided_service_instance_.MajorVersion(), event_id_, client_id_};

        // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
        // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_VALID
        TransmitMessageHandlerInterface& transmit_message_handler{connection_proxy->GetTransmitMessageHandler()};
        // VCA_IPCB_PROXYEVENT_CONNECTION_PROXY_TRANSMIT_MESSAGE_HANDLER_VALID
        transmit_message_handler.SubscribeEvent(subscribe_header);
      } else {
        logger_.LogWarn(
            [](::ara::log::LogStream& s) {
              s << "Dropping resend subscribe event message due to disconnected unicast connection.";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }
  }

  /*!
   * \brief   Notification that the monitored service instance is not available anymore.
   * \details Called from the ProxyRouter to communicate pending subscription state to the subscriber.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   *
   * \internal
   * - If a subscriber is registered:
   *   - Notify the subscriber of the state change to kSubscriptionPending.
   * \endinternal
   */
  void HandleEventSubscriptionStateDown() noexcept {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogVerbose([](::ara::log::LogStream const&) {},
                       Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    std::lock_guard<std::mutex> const subscriber_guard{subscriber_lock_};
    if (subscriber_ != nullptr) {
      subscription_status_ = ::ara::com::SubscriptionState::kSubscriptionPending;
      // VCA_IPCB_PROXYEVENT_EVENT_SUBSCRIBER_HANDLER_LIFECYCLE
      subscriber_->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kSubscriptionPending);
    }
  }

  /*!
   * \brief Reads serialized samples from the given sample cache container, deserializes them and calls the provided
   *        callback function.
   *
   * \param[in] invisible_app_cache_container  Container which has the enqueued event samples cache entries.
   * \param[in] max_samples                    Maximum number of samples to process.
   * \param[in] callable_sample_result         Callable to be invoked on successful deserialization.
   *
   * \return The number of event samples which were successfully deserialized and processed.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Calculate how many samples shall be processed by using the min of max_samples or the available serialized
   *   samples.
   * - Do repeatedly for the serialized_samples in invisible_app_cache_container.
   *   - Retrieve one slot from the visible cache.
   *   - When a slot is available.
   *     - Deserialize the sample.
   *     - If deserialization is successful.
   *       - Increase the number of successfully processed events.
   *       - Notify the trace sink of the ReadSample call.
   *       - Invoke callable_sample_result with wrapped, deserialized sample, the e2e check status and time stamp.
   *     - Otherwise
   *       - Return the visible cache slot.
   *   - Otherwise
   *     - Stop further processing of samples.
   * \endinternal
   */
  auto ReadSamplesInternal(SampleCacheContainer& invisible_app_cache_container, std::size_t const max_samples,
                           CallableReadSamplesResult const& callable_sample_result) const noexcept -> std::size_t {
    std::size_t nr_valid_events_processed{0U};
    std::size_t const samples_to_process{std::min(max_samples, invisible_app_cache_container.size())};

    // VECTOR NL AutosarC++17_10-A6.5.1: MD_IPCBINDING_AutosarC++17_10-A6.5.1_loop_counter
    for (std::size_t process_index{0U}; process_index < samples_to_process; ++process_index) {
      // Get free slot for deserialization
      // VCA_IPCB_VALID_SAMPLE_CACHE_POINTER_ACCESS
      typename VisibleSampleCache::CacheEntryType next_visible_cache_slot{visible_sample_cache_->GetNextFreeSample()};

      if (next_visible_cache_slot != nullptr) {
        // Retrieve serialized event
        NotificationMessage& notification_message{invisible_app_cache_container.front()};
        DeserializationResult const deserialization_result{
            Deserialize(notification_message, **next_visible_cache_slot)};  //  VCA_IPCB_VALID_NEXT_VISIBLE_CACHE_SLOT

        if (deserialization_result.HasValue()) {
          ++nr_valid_events_processed;

          // VCA_IPCB_VALID_TRACE_SINK
          trace_sink_->TraceReadSample(provided_service_instance_, client_id_, notification_message);

          ::ara::com::E2E_state_machine::E2ECheckStatus const check_status{
              ::ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable};
          ::amsr::core::Optional<TimeStamp> const timestamp{TimeStamp{}};
          // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
          callable_sample_result(typename ProxyEventBackendInterface::SampleData{
              std::move(next_visible_cache_slot), visible_sample_cache_, check_status, timestamp});
        } else {
          // VCA_IPCB_VALID_SAMPLE_CACHE_POINTER_ACCESS
          visible_sample_cache_->ReturnEntry(std::move(next_visible_cache_slot));
          logger_.LogError(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              [](::ara::log::LogStream& s) { s << "Deserialization error occurred."; },
              Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        }

        invisible_app_cache_container.pop_front();
      } else {
        // This is not an error case, we only process until no more free slot is available.
        logger_.LogDebug(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            [](::ara::log::LogStream& s) { s << "No free slot is available anymore."; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        break;
      }
    }  // VCA_IPCB_STD_SHARED_POINTER_DESTRUCTOR

    return nr_valid_events_processed;
  }

  /*!
   * \brief Deserializes the underlying serialized sample to the given sample_memory.
   *
   * \param[in]     notification_message  The message to deserialize.
   * \param[in,out] sample_memory         A reference to a preallocated sample.
   *
   * \return An amsr::core::Result stating whether deserialization was successful or not.
   *
   * \error ara::com::ComErrc::kErrorNotOk  In case of a deserialization error.
   *
   * \pre         Deserialization must be done only one time.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Emplace 'kErrorNotOk' in result.
   * - Deserialize the serialized byte stream of the sample into the provided sample reference.
   * - If deserialization is successful:
   *   - Emplace empty value in result.
   * - Return result.
   * \endinternal
   */
  static auto Deserialize(NotificationMessage& notification_message, SampleType& sample_memory) noexcept
      -> DeserializationResult {
    // PTP-B-IpcBinding-Xf-Proxy-Deserialize-Notification
    DeserializationResult result{DeserializationResult::FromError(::ara::com::ComErrc::kErrorNotOk)};

    // Prepare byte stream reader for the packet payload
    static ::amsr::ipc_binding_core::internal::ipc_protocol::MessageLength constexpr kHeaderLength{
        ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
        ::amsr::ipc_binding_core::internal::ipc_protocol::kNotificationMessageHeaderLength};

    ::amsr::ipc_binding_core::internal::ipc_protocol::PacketBuffer& packet{*notification_message.GetPacket()};
    ::amsr::someip_protocol::internal::deserialization::BufferView const payload_buffer_view{
        &(packet[kHeaderLength]), packet.size() - kHeaderLength};

    ::amsr::someip_protocol::internal::deserialization::Reader reader{payload_buffer_view};
    // VCA_IPCB_DESERIALIZE_SAFE_FUNCTION
    bool const deserialization_ok{SampleDeserializerType::Deserialize(reader, sample_memory)};

    if (deserialization_ok) {
      result.EmplaceValue();
    }

    // PTP-E-IpcBinding-Xf-Proxy-Deserialize-Notification
    return result;
  }

  /*!
   * \brief The ProvidedServiceInstanceId.
   */
  ProvidedServiceInstanceId const provided_service_instance_;

  /*!
   * \brief Event id.
   */
  EventId const event_id_;

  /*!
   * \brief Client id.
   */
  ClientId const client_id_;

  /*!
   * \brief Maximum number of stored events in the invisible cache.
   */
  SizeType cache_capacity_{0U};

  /*!
   * \brief Event storage.
   */
  InvisibleSampleCache invisible_sample_cache_{};

  /*!
   * \brief The event subscriber.
   */
  ::amsr::socal::internal::events::EventSubscriberInterface* subscriber_{nullptr};

  /*!
   * \brief Mutex for protection against parallel subscriptions/un-subscriptions.
   */
  mutable std::mutex subscriber_lock_{};

  /*!
   * \brief Sink for tracing of events.
   */
  std::unique_ptr<TraceEventSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief Pointer to the ProxyRouter.
   */
  ipc_binding_core::internal::ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Logger for tracing and debugging.
   */
  Logger logger_;

  /*!
   * \brief Event cache visible to the user.
   */
  VisibleSampleCachePtr visible_sample_cache_{nullptr};

  /*!
   * \brief Mutex for protection against parallel requests.
   */
  std::mutex service_up_lock_{};

  /*!
   * \brief Holds the currently known service instance state.
   */
  bool service_state_{true};

  /*!
   * \brief Event subscription status.
   */
  ::ara::com::SubscriptionState subscription_status_{::ara::com::SubscriptionState::kNotSubscribed};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_PROXY_EVENT_XF_BACKEND_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::subscriber_lock_
 *                Used for protecting parallel subscription/unsubscription against callbacks from reactor.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::subscriber_
 * \protects amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::subscription_status_
 *
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::Subscribe
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::Unsubscribe
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::ResendSubscribeMessage
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::OnSubscribeAckReceived
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::OnSubscribeNAckReceived
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length MEDIUM Notification of Socal to update event subscription state.
 * \endexclusivearea
 */
/*!
 * \exclusivearea amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::service_up_lock_
 *                Used for protecting against parallel requests and service instance state callbacks from reactor.
 *
 * \protects amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::service_state_,
 *
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::SetServiceState
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::Subscribe
 * \usedin amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend::Unsubscribe
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length MEDIUM ?.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_FunctionCoverage_dummy_function_not_supported_feature
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            E2E protection is not supported for the IpcBinding. But to fulfill the binding interface requirements
//            a dummy implementation, always returning a negative E2E result, is necessary.
//            The dummy implementation behaviour is verified by an unit test.
//
// \ID COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
//   \ACCEPT  XX
//   \REASON  CallCoverage gap reported as logger is a separate unit.
//            As the error scenario is handled as violation no further unit interactions are possible.
//            Therefore no separate test of CT or MUT level necessary.
//
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
