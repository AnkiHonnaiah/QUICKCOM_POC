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
/*!        \file  someip_binding_xf/internal/events/signal_based_event_handler.h
 *        \brief  Event handler for PDU events.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SignalBasedEventHandler
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_HANDLER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/someip_binding_event_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/client_subscriber_interface.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Handler class for SignalBased events.
 *
 * \tparam SampleType  Type of a single event sample.
 */
template <typename SampleType>
class SignalBasedEventHandler final
    : public ::amsr::someip_binding_xf::internal::events::ClientSubscriberInterface<SampleType>,
      public ::amsr::someip_binding_core::internal::SomeipBindingEventHandlerInterface {
 public:
  /*!
   * \brief Type Alias for proxy event backend interface.
   */
  using ProxyEventBackendInterface = ::amsr::someip_binding_xf::internal::events::ProxyEventXfInterface<SampleType>;

  /*!
   * \brief Type alias for shortening.
   */
  using EventSubscriberInterface = typename ProxyEventBackendInterface::EventSubscriberInterface;

  /*!
   * \brief Alias for InvisibleSampleCache.
   */
  using InvisibleSampleCache = ::amsr::someip_binding_core::internal::events::InvisibleSampleCache;

  /*!
   * \brief Type alias for ClientSubscriberInterface.
   */
  using ClientSubscriberInterface = ::amsr::someip_binding_xf::internal::events::ClientSubscriberInterface<SampleType>;

  /*!
   * \brief Constructor.
   *
   * \param[in]   someip_event_identity  SOME/IP event identity.
   * \param[in]   client                 Reference to the related binding client.
   * \param[in]   invisible_sample_cache Event cache for SomeIp events.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  SignalBasedEventHandler(::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
                          ::amsr::someip_binding_core::internal::LocalClient& client,
                          InvisibleSampleCache& invisible_sample_cache) noexcept
      : ClientSubscriberInterface{},
        SomeipBindingEventHandlerInterface{},
        someip_event_identity_{someip_event_identity},
        client_{client},
        invisible_sample_cache_{invisible_sample_cache} {}

  /*!
   * \brief       Destructor.
   * \pre         The event must be in an unsubscribed state.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SignalBasedEventHandler() noexcept final {
    if (subscriber_ != nullptr) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Precondition violation: Event is destroyed while being in subscribed state.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT

  SignalBasedEventHandler(SignalBasedEventHandler const&) = delete;
  SignalBasedEventHandler& operator=(SignalBasedEventHandler const&) = delete;
  SignalBasedEventHandler(SignalBasedEventHandler&&) = delete;
  SignalBasedEventHandler& operator=(SignalBasedEventHandler&&) = delete;

 private:
  /*!
   * \brief       Sends SubscribeEvent to the ClientManager if this is the first subscriber.
   *
   * \param[in]   event A pointer to the corresponding proxy event. Used for notification handling.
   *                    The ownership of the object is not transferred. The caller of the API must ensure
   *                    passing a valid pointer. The passed pointer must be valid until Unsubscribe() is called.
   * \return      Result with the subscription result.
   *
   * \pre         The event must be in an unsubscribed state.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \internal
   * - Check Subscription state.
   * - If Subscribed already
   *   - Report violation and abort.
   * - Else
   *   - Save the subscriber pointer (used to forward back notifications).
   *   - Trigger the client to subscribe to the event.
   * - Return the result to ProxyEventXf.
   * \endinternal
   */
  ::amsr::core::Result<void> Subscribe(EventSubscriberInterface* event) noexcept final {
    if (!IsSubscribed()) {
      subscriber_ = event;
      // The result from SomeIpBindingCore is not used because it is always a positive result.
      // In the future this behavior may change, when validation functionality is moved into
      // the SomeIpBindingCore.
      // After or within the next call, events might start to be received.
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
      static_cast<void>(client_.SubscribeEvent(someip_event_identity_.GetEventId(), this));
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Precondition violation: Same event has been subscribed twice.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Always return a positive result as described in SubscribeEvent call above
    return amsr::core::Result<void>{};
  }

  /*!
   * \brief       Sends unsubscribeEvent to the ClientManager if already subscribed.
   *
   * \details     Called from Event Manager. The SubscriptionState::kNotSubscribed state must be notified in the
   *              current call context as the event subscriber pointer is not valid after this call.
   *
   * \param[in]   event A pointer to the corresponding proxy event. Used for notification handling.
   *
   * \pre         -
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous FALSE
   * \steady      FALSE
   * \spec
   *   requires event != nullptr;
   * \endspec
   *
   * \internal
   * - Check Subscription state.
   * - If Subscribed already
   *   - Trigger the client to unsubscribe to the event.
   *   - Notify the related event subscriber about subscription state update.
   *   - Reset subscriber
   * - Else ignore it.
   * \endinternal
   */
  void Unsubscribe(EventSubscriberInterface* event) noexcept final {
    if (IsSubscribed()) {
      // After this line, it is not allowed to receive any new events.
      client_.UnsubscribeEvent(someip_event_identity_.GetEventId());
      subscriber_ = nullptr;
      // Notify the related event subscriber about subscription state update.
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      event->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kNotSubscribed);
    }
  }

  /*!
   * \brief Must never be called, someip event that cannot be handled by this class.
   *
   * \param[in]   event_message A complete event message.
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnEvent(::amsr::someip_binding_core::internal::events::EventMessage const& event_message) noexcept final {
    static_cast<void>(event_message);
    logger_.LogFatalAndAbort(
        [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
          s << "SOMEIP event on PDU event handler.";
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief        Handle a received event sample of Pdu message
   * \details      Called upon reception of a pdu event.
   *               Enqueues the received event in the invisible cache.
   *               Notifies the subscribed EventManager. Called from SomeipBinding.
   * \param[in]    pdu_message A complete Pdu message.
   * \pre          -
   * \context      Reactor
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \vprivate
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If no subscriber registered
   *   - Log fatal and call Abort()
   * - Enqueue event by passing event buffer and timestamp to invisible cache
   * - Update last received entry
   * \endinternal
   */
  void OnPduEvent(::amsr::someip_binding_core::internal::events::PduMessage const& pdu_message) final {
    // PTP-B-SomeIpDaemon-ProxyEventXF_OnPduEvent
    if (IsSubscribed()) {
      ::amsr::someip_binding_core::internal::DataBufferSharedPtr const pdu_message_buffer{pdu_message.GetBuffer()};

      amsr::core::Optional<::amsr::someip_binding_core::internal::events::PduMessage::TimeStamp> const
          pdu_message_ts_optional{pdu_message.GetTimeStamp()};

      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      bool const sample_dropped{invisible_sample_cache_.Enqueue(pdu_message_buffer, pdu_message_ts_optional)};
      if (sample_dropped) {
        logger_.LogDebug([](::ara::log::LogStream& s) { s << "Sample dropped during OnPduEvent()."; },
                         static_cast<char const*>(__func__), __LINE__);
      }

      // Call HandleEventNotification for all event managers in the list
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
      // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
      subscriber_->HandleEventNotification();
      // PTP-E-SomeIpDaemon-ProxyEventXF_OnPduEvent
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "OnPduEvent() called while not being subscribed!";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief       Calls HandleEventSubscriptionStateUpdate for subscriber.
   * \details     Called from SomeIpBindingCore.
   * \param[in]   state the updated event subscription state the payload.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If no subscriber registered
   *   - Log fatal and call Abort().
   * - Notify the subscriber of the state change.
   * \endinternal
   */
  void OnSubscriptionStateChange(::amsr::someip_protocol::internal::SubscriptionState const state) final {
    logger_.LogDebug(
        [&state](::ara::log::LogStream& s) {
          s << "State: ";
          someip_binding_core::internal::logging::LogBuilder::LogSubscriptionStateAsString(s, state);
          s << ".";
        },
        static_cast<char const*>(__func__), __LINE__);
    // PTP-B-SomeIpbinding-ProxyEventXF_OnSubscriptionStateChange
    {
      if (IsSubscribed()) {
        // Notify subscriber with the new state
        switch (state) {
          case ::amsr::someip_protocol::internal::SubscriptionState::kSubscribed: {
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
            // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
            subscriber_->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kSubscribed);
            break;
          }
          case ::amsr::someip_protocol::internal::SubscriptionState::kNotSubscribed: {
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
            // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
            subscriber_->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kNotSubscribed);
            break;
          }
          case ::amsr::someip_protocol::internal::SubscriptionState::kSubscriptionPending: {
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
            // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
            subscriber_->HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState::kSubscriptionPending);
            break;
          }
          default: {
            logger_.LogError([](::ara::log::LogStream& s) { s << "Unknown / Unsupported Event Subscription "; },
                             static_cast<char const*>(__func__), __LINE__);
            break;
          }
        }
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "OnSubscriptionStateChange() called while not being subscribed!";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }
    // PTP-E-SomeIpbinding-ProxyEventXF_OnSubscriptionStateChange
  }

  /*!
   * \brief Indicates the status of the subscription.
   *
   * \return True if already subscribed, false otherwise.
   *
   * \pre         -
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsSubscribed() const noexcept { return subscriber_ != nullptr; }

  /*!
   * \brief SOME/IP ID of this event.
   */
  ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity_;

  /*!
   * \brief The corresponding LocalClient.
   */
  someip_binding_core::internal::LocalClient& client_;

  /*!
   * \brief Event storage.
   */
  InvisibleSampleCache& invisible_sample_cache_;

  /*!
   * \brief The event subscriber.
   */
  EventSubscriberInterface* subscriber_{nullptr};

  /*!
   * \brief Logger Prefix.
   */
  ::amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SignalBasedEventHandler"_sv, someip_event_identity_.GetServiceId(), someip_event_identity_.GetInstanceId(),
          someip_event_identity_.GetMajorVersion(), someip_event_identity_.GetEventId())};

  /*!
   * \brief Logger for tracing and debugging
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_HANDLER_H_
