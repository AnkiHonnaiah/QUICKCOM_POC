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
 *        \brief  Ipc binding implementation of SkeletonEventXfBackend.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>
#include <mutex>
#include <tuple>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/log/logstream.h"
#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/writer.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_message_builder.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/ipc_protocol/ser_ipc_headers.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/session_handler.h"
#include "ipc_binding_transformation_layer/internal/allocated_sample.h"
#include "ipc_binding_transformation_layer/internal/events/skeleton_event_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/offer_state.h"
#include "ipc_binding_transformation_layer/internal/skeleton_event_subscriber_handler.h"
#include "ipc_binding_xf_common/internal/trace/trace_event_sink_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Template for skeleton event xf backend.
 *
 * \tparam SampleType         Datatype of a single event sample.
 * \tparam PayloadSerializer  Serializer used to serialize notifications. It's static function must be callable during
 *                            SkeletonEventXfBackend's entire lifetime.
 *
 * \unit       IpcBinding::IpcBindingTransformationLayer::SkeletonEvent
 * \complexity The huge number of inter-unit and inter-module calls are mostly calls to libvac's smart pointers and the
 *             CoreUtility, IpcPacket, and IpcProtocol units. These calls are low in complexity, so the overall
 *             complexity of this unit is not too high, as the tool indicates.
 */
template <typename SampleType, typename PayloadSerializer>
class SkeletonEventXfBackend final : public ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>,
                                     public SkeletonEventXfBackendInterface {
 public:
  /*!
   * \brief Type alias to the AllocatedSampleUniquePtr.
   */
  using AllocatedSampleUniquePtr =
      typename ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>::AllocatedSampleUniquePtr;

  /*!
   * \brief Type alias for the AllocatedSample type.
   */
  using AllocatedSampleType = AllocatedSample<SampleType>;

  /*!
   * \brief Type of Event identifier.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Type of ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type of trace event sink interface.
   */
  using TraceEventSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface;

  /*!
   * \brief Constructor of SkeletonEventXfBackend.
   *
   * \param[in] provided_service_instance_id  The provided service instance ID.
   * \param[in] event_id                      Event ID.
   * \param[in] is_field_event                Flag if event is field event.
   * \param[in] offer_state                   Reference to the offer state of the service instance.
   * \param[in] trace_sink                    Trace sink.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  SkeletonEventXfBackend(ProvidedServiceInstanceId const& provided_service_instance_id, EventId event_id,
                         bool is_field_event, OfferState const& offer_state,
                         std::unique_ptr<TraceEventSinkInterface> trace_sink)  // IGNORE_BLACKLIST BSOCIAL-8805
      // () is used to initialize the base class to suppress doxygen issue.
      : ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>(),
        SkeletonEventXfBackendInterface{},
        provided_service_instance_id_{provided_service_instance_id},
        event_id_{event_id},
        is_field_event_{is_field_event},
        offer_state_{offer_state},
        trace_sink_{std::move(trace_sink)}  // VCA_IPCB_VALID_TRACE_SINK
  {
    if (!trace_sink_) {
      logger_.TerminateOnViolation("Tried to construct an SkeletonEventXfBackend with an invalid (empty) trace sink.",
                                   amsr::ipc_binding_core::internal::logging::AraComLogger::LogLocation{
                                       static_cast<char const*>(__func__), __LINE__});
    }
  }

  /*!
   * \brief Use default Destructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~SkeletonEventXfBackend() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  SkeletonEventXfBackend(SkeletonEventXfBackend const&) noexcept = delete;
  auto operator=(SkeletonEventXfBackend const&) noexcept -> SkeletonEventXfBackend& = delete;
  SkeletonEventXfBackend(SkeletonEventXfBackend&&) noexcept = delete;
  auto operator=(SkeletonEventXfBackend&&) noexcept -> SkeletonEventXfBackend& = delete;

  /*!
   * \brief Send an event sample.
   *
   * \param[in] data  Event sample value to be transmitted.
   *
   * \return Empty result.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Store the new sample value.
   * - Increment the current session ID for serialization of the next notification message.
   * - Build the NotificationMessage: Allocate buffer, serialize header and payload.
   * - NotificationMessage construction succeeds or terminates before.
   * - Inform the trace sink of the sent notification.
   * - Transmit the serialized notification message to all subscribers.
   * - Return empty result.
   * \endinternal
   */
  auto Send(SampleType const& data) noexcept -> ::ara::core::Result<void> final {
    logger_.LogVerbose(
        [this](::ara::log::LogStream& s) {
          s << "Send sample of event/field (";
          // VCA_IPCB_SERVICE_INSTANCE_IDENTIFIER_REFERENCE_VALID
          LogBuilder::LogServiceInstanceId(
              s, provided_service_instance_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          LogBuilder::LogEventId(s, event_id_);   // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    std::lock_guard<std::mutex> const guard{sending_lock_};
    StoreSampleValue(data);

    ++current_session_id_;

    NotificationMessage notification_message{BuildNotificationMessage(data, current_session_id_.GetSessionId())};

    // VECTOR NC AutosarC++17_10-A5.0.1: MD_IPCBINDING_AutosarC++17_10_A5.0.1_false_positive
    // VECTOR NC VectorC++-V5.0.1: MD_IPCBINDING_VectorC++_V5.0.1_false_positive
    trace_sink_->TraceSend(provided_service_instance_id_, notification_message);  // VCA_IPCB_VALID_TRACE_SINK
    SendNotification(std::move(notification_message));

    return ::ara::core::Result<void>{};
  }

  /*!
   * \brief Send an event sample using the preallocated pointer.
   *
   * \param[in] sample_ptr  Sample pointer containing the data to be transmitted. The given pointer must not be null.
   *
   * \return Empty result.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Store the new sample value.
   * - Increment the current session ID for serialization of the next notification message.
   * - Build the NotificationMessage: Allocate buffer, serialize header and payload.
   * - NotificationMessage construction succeeds, otherwise it terminates before.
   * - Inform the trace sink of the sent notification.
   * - Transmit the serialized notification message to all subscribers.
   * - Return empty result.
   * \endinternal
   */
  auto Send(AllocatedSampleUniquePtr sample_ptr) noexcept -> ::ara::core::Result<void> final {
    logger_.LogVerbose(
        [this](::ara::log::LogStream& s) {
          s << "Send allocated sample of event (";
          // VCA_IPCB_SERVICE_INSTANCE_IDENTIFIER_REFERENCE_VALID
          LogBuilder::LogServiceInstanceId(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              s, provided_service_instance_id_);
          LogBuilder::LogEventId(s, event_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility;
          s << ")";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // VCA_IPCB_POSSIBLY_CALLING_NULLPTR_WITH_FUNCTION_CONTRACT
    SampleType const& data{**sample_ptr};
    std::lock_guard<std::mutex> const guard{sending_lock_};
    StoreSampleValue(data);

    ++current_session_id_;

    NotificationMessage notification_message{BuildNotificationMessage(data, current_session_id_.GetSessionId())};

    // VECTOR NC AutosarC++17_10-A5.0.1: MD_IPCBINDING_AutosarC++17_10_A5.0.1_false_positive
    // VECTOR NC VectorC++-V5.0.1: MD_IPCBINDING_VectorC++_V5.0.1_false_positive
    // VCA_IPCB_VALID_TRACE_SINK
    trace_sink_->TraceSendAllocate(provided_service_instance_id_, notification_message);

    SendNotification(std::move(notification_message));

    return ::ara::core::Result<void>{};
  }

  /*!
   * \brief   Allocate a AllocatedSampleType pointer to be filled by the user for later event sample transmission.
   * \details The allocated pointer only pre-allocates memory for the deserialized sample value. For dynamic datatypes
   *          (vector, map, string, ...) only the heap memory necessary for default construction is allocated. No memory
   *          pre-allocation for the later packet serialization is done here.
   *
   * \return Result containing a pointer to the allocated memory. The returned pointer must not be null.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3 : MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  auto Allocate() noexcept -> ::ara::core::Result<AllocatedSampleUniquePtr> final {
    // Steady mode lite terminates on bad_alloc, so no check is needed here
    return ::amsr::core::Result<AllocatedSampleUniquePtr>{amsr::tmp::MakeUnique<AllocatedSampleType>().Value()};
  }

  /*!
   * \brief Pre-allocate is deprecated and do nothing, all memory are allocate from the global shared pool.
   *
   * \param[in] count  unused.
   *
   * \pre         API has not been called yet.
   * \context     App
   * \threadsafe  FALSE for same class instances, TRUE for different instances.
   * \reentrant   FALSE for same class instances, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_no_effect
  void PreAllocate(std::size_t const count) noexcept final { std::ignore = count; }

  /*!
   * \copydoc SkeletonEventXfBackendInterface::HandleEventSubscription(ConnectionId connection_id, ConnectionSkeletonPtr& connection_ptr, SubscribeEventMessageHeader const& header)
   *
   * \internal
   * - If the service instance is currently offered:
   *   - If this is field notifier, lock the sending mutex.
   *   - Add the connection to the list of known event subscribers.
   *   - Serialize and send an event subscription Ack message to the subscriber.
   *   - If this event is a field notifier:
   *     - If field value already received:
   *       - Build the initial field value notification message: Allocate buffer, serialize header and payload. Hint 1)
   *         Usage of the current session ID is necessary to allow a sender/receiver mapping of traced event samples.
   *         Hint 2) Construction of the notification message must always succeed because memory pre-allocation is not
   *         supported for field notifiers. Therefore no additional error handling is necessary here.
   *       - Send the initial field value notification message to the subscriber.
   *     - Release the sending mutex.
   * - The connections are closed when stop offering the service instance, hence no Subscription message can arrive when
   *   not offering.
   * \endinternal
   */
  auto HandleEventSubscription(ConnectionId connection_id, ConnectionSkeletonPtr& connection_ptr,
                               SubscribeEventMessageHeader const& header) noexcept -> void final {
    namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;

    logger_.LogVerbose(
        [&header](::ara::log::LogStream& s) {
          s << "Received event subscription for event: ";
          LogBuilder::LogCompleteServiceInstanceEventId(
              s, header.service_id_, header.major_version_, header.instance_id_,
              header.event_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
          s << ", ";
          LogBuilder::LogClientId(s, header.client_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    if (offer_state_) {
      if (is_field_event_) {
        sending_lock_.lock();
      }
      {
        std::lock_guard<std::mutex> const subscribers_guard{subscribers_lock_};
        subscribers_.AddSubscriber(connection_id, connection_ptr);
      }

      ipc_protocol::SubscribeEventAckMessage ack_message{
          ipc_protocol::IpcMessageBuilder::CreateSubscribeEventAckMessage(
              ipc_protocol::IpcMessageBuilder::CreateSubscribeEventAckHeader(header))};
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
      // VCA_IPCB_VALID_CONNECTION_SKELETON
      connection_ptr->SendSubscribeAck(std::move(ack_message));

      if (is_field_event_) {
        // Send initial field notification
        ::amsr::core::Optional<NotificationMessage> initial_field_value_message{};
        // The subscription might occur before the first field update is sent.
        // Therefore, it is a valid case that the initial field value is not yet cached.
        // Notification will be sent with next Send() call.
        if (event_data_.has_value()) {
          initial_field_value_message.emplace(
              BuildNotificationMessage(event_data_.value(), current_session_id_.GetSessionId()));
        }
        if (initial_field_value_message) {
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
          // VCA_IPCB_VALID_CONNECTION_SKELETON
          connection_ptr->SendNotification(std::move(initial_field_value_message.value()));
        }
        sending_lock_.unlock();
      }
    }
  }

  /*!
   * \copydoc SkeletonEventXfBackendInterface::HandleEventUnsubscription()
   */
  void HandleEventUnsubscription(ConnectionId connection_id) noexcept final {
    std::lock_guard<std::mutex> const subscribers_guard{subscribers_lock_};
    subscribers_.RemoveSubscriber(connection_id);
  }

  /*!
   * \copydoc SkeletonEventXfBackendInterface::HandleDisconnect()
   */
  void HandleDisconnect(ConnectionId connection_id) noexcept final {
    std::lock_guard<std::mutex> const subscribers_guard{subscribers_lock_};
    subscribers_.RemoveConnection(connection_id);
  }

  /*!
   * \copydoc SkeletonEventXfBackendInterface::HandleStopOffer()
   */
  void HandleStopOffer() noexcept final {
    std::lock_guard<std::mutex> const subscribers_guard{subscribers_lock_};
    subscribers_.Clear();
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
   * \brief Type alias for NotificationMessage.
   */
  using NotificationMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage;

  /*!
   * \brief Type alias for NotificationMessageHeader.
   */
  using NotificationMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessageHeader;

  /*!
   * \brief Type alias for SessionId.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type alias for SessionHandler.
   */
  using SessionHandler = ::amsr::ipc_binding_core::internal::SessionHandler;

  /*!
   * \brief Type alias for SomeIpProtocol InfSizeT;.
   */
  using InfSizeT = ::amsr::someip_protocol::internal::serialization::InfSizeT;

  /*!
   * \brief Store/Update the current event sample value.
   *
   * \param[in] data  Sample value to be stored.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - If the event is a field event:
   *   - With protection of event data storage against concurrent modification:
   *     - Create a copy event sample (protected by sending_lock_ mutex). The sample copy is necessary for initial field
   *       value to be transmitted on field notifier subscription.
   * \endinternal
   */
  void StoreSampleValue(SampleType const& data) noexcept {
    if (is_field_event_) {
      event_data_.emplace(data);
    }
  }

  /*!
   * \brief Build a NotificationMessage.
   *
   * \param[in] data        Event sample payload.
   * \param[in] session_id  Session ID to be serialized into the notification message.
   *
   * \return The constructed NotificationMessage (header + serialized payload).
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Initialize default result with error code.
   * - Calculate necessary packet size for headers + payload.
   * - Build NotificationMessage which consists of the deserialized header and allocate buffer for serialization.
   * - Serialize the header into the allocated packet buffer.
   * - Serialize the event sample payload into the allocated packet buffer.
   * - Return the constructed NotificationMessage.
   * \endinternal
   */
  auto BuildNotificationMessage(SampleType const& data, SessionId const session_id) const noexcept
      -> NotificationMessage {
    namespace ipc_protocol = ::amsr::ipc_binding_core::internal::ipc_protocol;
    // PTP-B-IpcBinding-Xf-Skeleton-Serialize-Notification

    // VCA_IPCB_GETREQUIRED_BUFFERSIZE_FUNCTION
    std::size_t const payload_size{PayloadSerializer::GetRequiredBufferSize(data)};
    ipc_protocol::PacketBuffer::size_type const packet_size{
        ipc_protocol::kProtocolMessageHeaderLength + ipc_protocol::kNotificationMessageHeaderLength + payload_size};

    NotificationMessageHeader const header{provided_service_instance_id_.ServiceId(),
                                           provided_service_instance_id_.InstanceId(),
                                           provided_service_instance_id_.MajorVersion(), event_id_, session_id};
    NotificationMessage notification_msg{header, ipc_protocol::IpcPacketSharedConstruct(packet_size)};

    ::amsr::someip_protocol::internal::serialization::Writer writer{*notification_msg.GetPacket()};

    ipc_protocol::serialization::SerializeNotificationMessageHeader(writer, header, payload_size);

    // VCA_IPCB_SERIALIZE_SAFE_FUNCTION
    PayloadSerializer::Serialize(writer, data);

    // PTP-E-IpcBinding-Xf-Skeleton-Serialize-Notification
    return notification_msg;
  }

  /*!
   * \brief Send a notification to all subscribers.
   *
   * \param[in] notification_msg  The notification message.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Lock the subscribers map.
   * - For each subscriber:
   *   - Try to access the subscriber-specific ConnectionSkeleton:
   *     - Send a copy (shared-ptr only) via the ConnectionSkeleton.
   *   - Otherwise log an error that the the ConnectionSkeleton could not be accessed.
   * \endinternal
   */
  void SendNotification(NotificationMessage const&& notification_msg) {
    {
      std::lock_guard<std::mutex> const subscribers_guard{subscribers_lock_};

      for (SkeletonEventSubscriberHandler::ConnectionWeakPtr const& subscriber_connection_weak : subscribers_) {
        // VCA_IPCB_SKELETON_EVENT_VALID_SUBSCRIBERS
        ConnectionSkeletonPtr const connection_skeleton{subscriber_connection_weak.Lock()};
        if (connection_skeleton) {
          // VCA_IPCB_SKELETON_EVENT_LOCKED_CONNECTION
          connection_skeleton->SendNotification(NotificationMessage{notification_msg});
        } else {
          logger_.LogError(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              [&notification_msg](Logger::LogStream& s) {
                ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessageHeader const& header{
                    notification_msg.GetMessageHeader()};  // COV_IpcBinding_CoverageComponentOverall_trivial_utility

                s << "Failed to access ConnectionSkeleton for transmission of event notification (";
                LogBuilder::LogCompleteServiceInstanceEventId(
                    s, header.service_id_, header.major_version_, header.instance_id_,
                    header.event_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
                s << ", ";
                LogBuilder::LogSessionId(
                    s, header.session_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
                s << "). The connection might be already closed. Dropping event notification message.";
              },
              Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
        }
      }
    }
  }

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::ara::core::StringView{"SkeletonEventXfBackend"}};

  /*!
   * \brief The ProvidedServiceInstanceId.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief Event ID.
   */
  EventId const event_id_;

  /*!
   * \brief Session ID counter for every event sample.
   */
  SessionHandler current_session_id_{SessionId{0}};  // VCA_IPCB_CALLED_FUNCTION_OUTSIDE_COMPONENT

  /*!
   * \brief Event Data.
   */
  ::amsr::core::Optional<SampleType> event_data_{};

  /*!
   * \brief Mutex to protect concurrent sending, and hence protect also concurrent modification of event_data_.
   */
  std::mutex sending_lock_{};

  /*!
   * \brief Boolean to store if this is a field event or not.
   */
  bool const is_field_event_;

  /*!
   * \brief Reference to the offer state from the service instance.
   */
  OfferState const& offer_state_;

  /*!
   * \brief Sink for tracing of events.
   */
  std::unique_ptr<TraceEventSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief   Necessary buffer size for a serialized notification message (including headers and payload).
   * \details Can be in the state 'infinite' if the necessary buffer size cannot be determined by the Serializer.
   */
  InfSizeT const max_buffer_size_{::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
                                  ::amsr::ipc_binding_core::internal::ipc_protocol::kNotificationMessageHeaderLength +
                                  PayloadSerializer::GetMaximumBufferSize()};  // VCA_IPCB_GET_MAX_BUFFERSIZE_FUNCTION

  /*!
   * \brief Skeleton event subscriber handler.
   */
  SkeletonEventSubscriberHandler subscribers_{};

  /*!
   * \brief Mutex to protect concurrent modification of subscribers_.
   */
  std::mutex subscribers_lock_{};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_H_

// clang-format off
/*!
 * \exclusivearea
 * amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackend::sending_lock_
 * Ensures no overlapping sending and no raise condition between sending and adding a subscriber, including sending of
 * initial notification message.Ensures a consistent modification of the last event sample value.
 * \protects amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackend::event_data_
 *           Separates the send-paths from the adding a field subscription and send initial notification path.
 * \usedin  Send (Write),
 *          Send (Write),
 *          HandleEventSubscription(Read)
 * \exclude  All other methods of a SkeletonEventXfBackend.
 * \length   MEDIUM Limited to the path starting from storing/reading the sample type and session id and ending at
 *           sending.
 * \endexclusivearea
 *
 * \exclusivearea
 * amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackend::subscribers_lock_
 * Ensures a consistent modification of the subscriber map.
 * \protects amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackend::subscribers_
 * \usedin  HandleEventSubscription (Write),
 *          HandleEventUnsubscription (Write),
 *          HandleDisconnect (Write),
 *          HandleStopOffer (Write),
 *          Send (Read)
 * \exclude  All other methods of a SkeletonEventXfBackend.
 * \length   SHORT Only the concrete write access or loop over all when read.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
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
