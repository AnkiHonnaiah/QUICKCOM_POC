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
/*!        \file  someip_binding_xf/internal/events/proxy_event_xf.h
 *        \brief  Proxy event backend for SOME/IP binding.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::ProxyEventXf
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding_core/internal/events/client_event.h"
#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/e2e/client_side_transformer_interface.h"
#include "amsr/someip_binding_xf/internal/events/e2e_sample_interpreter_interface.h"
#include "amsr/someip_binding_xf/internal/events/e2e_sample_reader.h"
#include "amsr/someip_binding_xf/internal/events/event_notification_handler.h"
#include "amsr/someip_binding_xf/internal/events/legacy_signal_based_e2e_sample_interpreter.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"
#include "amsr/someip_binding_xf/internal/events/sample_interpreter_interface.h"
#include "amsr/someip_binding_xf/internal/events/sample_reader.h"
#include "amsr/someip_binding_xf/internal/events/sample_reader_interface.h"
#include "amsr/someip_binding_xf/internal/events/signal_based_e2e_sample_interpreter.h"
#include "amsr/someip_binding_xf/internal/events/signal_based_sample_interpreter.h"
#include "amsr/someip_binding_xf/internal/events/someip_e2e_sample_interpreter.h"
#include "amsr/someip_binding_xf/internal/events/someip_sample_interpreter.h"
#include "amsr/someip_binding_xf/internal/events/thread_safe_e2e_result.h"
#include "amsr/someip_binding_xf/internal/events/visible_sample_cache.h"
#include "amsr/someip_binding_xf/internal/ser_deser/event_deserializer_interface.h"
#include "ara/com/com_error_domain.h"
#include "ara/com/e2e_types.h"
#include "ara/com/types_common.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Template for SOME/IP Proxy event manager backend.
 * \details Handles event subscriptions and notification deserialization.
 *          Is created once per required service interface.
 *
 * \tparam SampleType Type of a single event sample.
 *
 */
template <typename SampleType>
class ProxyEventXf final : public ::amsr::someip_binding_xf::internal::events::ProxyEventXfInterface<SampleType>,
                           public someip_binding_core::internal::events::EventNotificationInterface {
 public:
  /*!
   * \brief Type alias for EventDeserializerInterface.
   */
  using EventDeserializerUniquePtr = std::unique_ptr<internal::ser_deser::EventDeserializerInterface<SampleType>>;

  /*!
   * \brief Type alias for ClientSideTransformerInterface.
   */
  using ClientSideTransformerInterface = ::amsr::someip_binding_xf::internal::e2e::ClientSideTransformerInterface;

  /*!
   * \brief       Constructor.
   * \param[in]   someip_event_identity    SOME/IP event identity.
   * \param[in]   client_event             Client event unique ptr.
   * \param[in]   deserializer             Event samples deserializer.
   * \param[in]   e2e_parametrization_data Event E2E parametrization data. Nullopt if not E2E protected.
   * \param[in]   serialization            Type of serialization. Its value is either SomeIp or S2S.
   * \param[in]   pdu_header_extn          The pdu header extension indicating that the pdu header is
   * extended for signal based messages.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_SOMEIPBINDING_AutosarC++17_10-A12.1.5_UseDelegatingConstructor
  ProxyEventXf(
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
      std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent> client_event,
      EventDeserializerUniquePtr deserializer,
      ::amsr::core::Optional<someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>&&
          e2e_parametrization_data,
      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization,
      amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionRx> const&
          pdu_header_extn)
      : ProxyEventXfInterface<SampleType>{},
        someip_binding_core::internal::events::EventNotificationInterface{},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        notification_handler_{std::make_shared<EventNotificationHandler>(*this)},
        someip_event_identity_{someip_event_identity},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        client_event_{std::move(client_event)},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        deserializer_{std::move(deserializer)},
        serialization_{serialization},
        pdu_header_extension_flag_{pdu_header_extn.has_value() && pdu_header_extn.value()},
        e2e_parametrization_data_{
            std::move(e2e_parametrization_data)} {  // VCA_SOMEIPBINDING_E2E_PROFILE_CONFIG_LIFECYCLE
  }

  auto operator=(ProxyEventXf&&) & -> ProxyEventXf& = delete;
  ProxyEventXf(ProxyEventXf const&) = delete;
  auto operator=(ProxyEventXf const&) & -> ProxyEventXf& = delete;
  ProxyEventXf(ProxyEventXf&&) = delete;

  /*!
   * \brief       Destructor. DeRegisters backend from Client Manager.
   * \pre         The event must be in an unsubscribed state.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ProxyEventXf() noexcept final {
    if (subscribed_proxy_event_xf_.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) noexcept {
            s << "Precondition violation: Event is destroyed while being in subscribed state.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventReceived
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventReceived() noexcept final {
    if (subscribed_proxy_event_xf_.has_value()) {
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      subscribed_proxy_event_xf_.value().subscriber->HandleEventNotification();
    } else {
      logger_.LogFatalAndAbort(
          [](amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "OnEventReceived called while not being subscribed.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventSubscriptionStateUpdate
   */
  void OnEventSubscriptionStateUpdate(::amsr::someip_protocol::internal::SubscriptionState const state) noexcept final {
    logger_.LogDebug(
        [&state](::ara::log::LogStream& s) {
          s << "State: ";
          someip_binding_core::internal::logging::LogBuilder::LogSubscriptionStateAsString(s, state);
          s << ".";
        },
        static_cast<char const*>(__func__), __LINE__);

    if (subscribed_proxy_event_xf_.has_value()) {
      std::lock_guard<std::mutex> const status_update_callable_guard{subscribe_status_update_callable_lock_};
      switch (state) {
        case ::amsr::someip_protocol::internal::SubscriptionState::kSubscribed: {
          state_ = ::ara::com::SubscriptionState::kSubscribed;
          // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
          subscribed_proxy_event_xf_.value().subscriber->HandleEventSubscriptionStateUpdate(state_);
          break;
        }
        case ::amsr::someip_protocol::internal::SubscriptionState::kNotSubscribed: {
          state_ = ::ara::com::SubscriptionState::kNotSubscribed;
          // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
          subscribed_proxy_event_xf_.value().subscriber->HandleEventSubscriptionStateUpdate(state_);
          break;
        }
        case ::amsr::someip_protocol::internal::SubscriptionState::kSubscriptionPending: {
          state_ = ::ara::com::SubscriptionState::kSubscriptionPending;
          // VCA_SOMEIPBINDING_EVENT_SUBSCRIBER_INTERFACE_LIFECYCLE
          subscribed_proxy_event_xf_.value().subscriber->HandleEventSubscriptionStateUpdate(state_);
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
          [](amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "OnSubscriptionStateChange called while not being subscribed.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Type Alias for proxy event backend interface.
   */
  using ProxyEventBackendInterface = ::amsr::someip_binding_xf::internal::events::ProxyEventXfInterface<SampleType>;

  /*!
   * \brief Type alias for shortening.
   */
  using EventSubscriberInterface = typename ProxyEventBackendInterface::EventSubscriberInterface;

  /*!
   * \brief Return type of ReadSamples.
   */
  using ReadSamplesResult = typename ProxyEventBackendInterface::ReadSamplesResult;

  /*!
   * \brief Alias for VisibleSampleCache.
   */
  using VisibleSampleContainer = ::amsr::someip_binding_xf::internal::events::VisibleSampleCache<SampleType>;

  /*!
   * \brief Alias for InvisibleSampleCache.
   */
  using InvisibleSampleCache = ::amsr::someip_binding_core::internal::events::InvisibleSampleCache;

  /*!
   * \brief Callable type to be invoked for every deserialized sample.
   */
  using CallableReadSamplesResult = typename ProxyEventBackendInterface::CallableReadSamplesResult;

  /*!
   * \brief   Callable type to be invoked upon receiving new event sample from skeleton.
   */
  using CallableEventNotification = typename ProxyEventBackendInterface::CallableEventNotification;

  /*!
   * \brief   Callable type to be invoked upon the event subscription state changes.
   */
  using CallableSubscriptionStateUpdate = typename ProxyEventBackendInterface::CallableSubscriptionStateUpdate;

  /*!
   * \brief A SubscribedProxyEventXf holds all objects which are required in subscribed state only
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  struct SubscribedProxyEventXf {
    /*!
     * \brief The event subscriber.
     */
    EventSubscriberInterface* subscriber{};

    /*!
     * \brief Event cache visible to the user.
     */
    std::shared_ptr<VisibleSampleContainer> visible_sample_cache{};

    /*!
     * \brief Reader to read out samples from invisible to visible sample cache.
     */
    std::unique_ptr<SampleReaderInterface<SampleType>> sample_reader{};
  };

  /*!
   * \brief Factory Method which creates all objects needed during subscribed state.
   * \param[in] subscriber                 Pointer to the subscriber of the ProxyEventXf
   * \param[in] cache_capacity             Capacity of the sample caches
   * \return A SubscribedProxyEventXf containing the objects
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Check serialization mode.
   *   - If it is SOME/IP serialization
   *     - Initialize SOME/IP event handler.
   *     - Check e2e
   *       - If it is e2e protected
   *         - Initialize e2e sample reader with SOME/IP e2e sample interpreter.
   *       - Otherwise
   *         - Initialize sample reader with SOME/IP sample interpreter.
   *   - Otherwise
   *     - Initialize signal based event handler.
   *     - Check e2e
   *       - If it is e2e protected
   *          - If it has signal based e2e range
   *            - Initialize e2e sample reader with signal based e2e sample interpreter.
   *          -  Otherwise
   *            - Initialize e2e sample reader with legacy signal based e2e sample interpreter.
   *       - Otherwise
   *         - Initialize sample reader with signal based sample interpreter.
   * - Return a SubscribedProxyEventXf containing the objects.
   * \endinternal
   */
  auto CreateSubscribedProxyEventXf(EventSubscriberInterface* subscriber, std::size_t const cache_capacity) noexcept
      -> SubscribedProxyEventXf {
    bool const is_e2e_protected{e2e_parametrization_data_.has_value()};

    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<SampleReaderInterface<SampleType>> sample_reader{nullptr};

    if (serialization_ == ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::someip) {
      if (is_e2e_protected) {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::unique_ptr<E2eSampleInterpreterInterface> e2e_sample_interpreter{
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            std::make_unique<SomeIpE2eSampleInterpreter>(e2e_parametrization_data_->e2e_profile)};
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        sample_reader =
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            std::make_unique<E2eSampleReader<SampleType>>(*(deserializer_).get(),
                                                          *e2e_parametrization_data_->e2e_transformer_uptr, e2e_result_,
                                                          e2e_parametrization_data_->is_e2e_check_disabled,
                                                          someip_event_identity_, std::move(e2e_sample_interpreter));
      } else {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::unique_ptr<SampleInterpreterInterface> sample_interpreter{// VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
                                                                       std::make_unique<SomeIpSampleInterpreter>()};
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        sample_reader = std::make_unique<SampleReader<SampleType>>(*(deserializer_).get(), someip_event_identity_,
                                                                   std::move(sample_interpreter));
      }
    } else {
      if (is_e2e_protected) {
        std::unique_ptr<E2eSampleInterpreterInterface> e2e_sample_interpreter{nullptr};

        if (e2e_parametrization_data_->has_signal_based_e2e_range) {
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          e2e_sample_interpreter =
              // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
              std::make_unique<SignalBasedE2eSampleInterpreter>(
                  pdu_header_extension_flag_, e2e_parametrization_data_->e2e_update_bit_position,
                  e2e_parametrization_data_->e2e_protected_offset, e2e_parametrization_data_->e2e_protected_length);
        } else {
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          e2e_sample_interpreter =
              // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
              std::make_unique<LegacySignalBasedE2eSampleInterpreter>(pdu_header_extension_flag_);
        }
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        sample_reader =
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            std::make_unique<E2eSampleReader<SampleType>>(*(deserializer_).get(),
                                                          *e2e_parametrization_data_->e2e_transformer_uptr, e2e_result_,
                                                          e2e_parametrization_data_->is_e2e_check_disabled,
                                                          someip_event_identity_, std::move(e2e_sample_interpreter));
      } else {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::unique_ptr<SampleInterpreterInterface> sample_interpreter{
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            std::make_unique<SignalBasedSampleInterpreter>(pdu_header_extension_flag_)};
        //  VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        sample_reader = std::make_unique<SampleReader<SampleType>>(*(deserializer_).get(), someip_event_identity_,
                                                                   std::move(sample_interpreter));
      }
    }

    return SubscribedProxyEventXf{
        subscriber,
        // Allocate one additional spare slot in VisibleSampleCache (see AUTOSAR_EXP_ARAComAPI for the rationale).
        std::make_shared<VisibleSampleContainer>(cache_capacity + 1),  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::move(sample_reader)};
  }

  /*!
   * \brief       Query the number of available sample slots from visible sample cache.
   * \return      The number of free samples available.
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if
   *              > Subscribe call is returned successfully, or
   *              > HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against ReadSamples and GetE2EResult when invoked on same/different class instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate    Product Private
   *
   * \internal
   * - If the event cache is valid
   *   - If the number of free samples in the cache exceed the number of subscribed samples
   *     - Return the number of subscribed samples.
   *   - Otherwise return the number of free samples in the cache.
   * - Otherwise return 0.
   * \endinternal
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final {
    std::size_t result{0};
    if (subscribed_proxy_event_xf_.has_value()) {
      SubscribedProxyEventXf const& subscribed_proxy_event_xf{subscribed_proxy_event_xf_.value()};

      std::size_t const visible_sample_cache_free_sample_count{
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          subscribed_proxy_event_xf.visible_sample_cache->GetFreeSampleCount()};
      result = std::min(cache_size_, visible_sample_cache_free_sample_count);
    }
    return result;
  }

  /*!
   * \brief       Adds the provided event manager to the subscriber list and sends SubscribeEvent
   *              to the ClientManager if this is the first subscriber.
   * \details     Called from Event Manager.
   * \param[in]   event A pointer to the corresponding proxy event. Used for notification handling.
   *                    The ownership of the object is not transferred. The caller of the API must ensure
   *                    passing a valid pointer. The passed pointer must be valid until Unsubscribe() is called.
   * \param[in]   cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                          memory for storing at least the cache_size number of deserialized samples.
   * \pre         Event was not subscribed.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   * \vprivate    Product Private
   *
   * \internal
   * - Note: Before this call, there shall be NO events being received, therefore no protection against callbacks
   *         required.
   *   - Verify and report violation if event has been already subscribed.
   *   - ProxyEventXf is initialized by creating the SubscribedProxyEventXf.
   *   - Trigger the client subscriber to subscribe to the event.
   *   - Handle the error if getting.
   * \endinternal
   */
  void Subscribe(EventSubscriberInterface* event, std::size_t const cache_size) noexcept final {
    // PTP-B-SomeIpbinding-ProxyEventXF_Subscribe
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // No need to lock against reactor, before subscription it is not allowed to receive any events.
    if (!subscribed_proxy_event_xf_.has_value()) {
      subscribed_proxy_event_xf_.template emplace(CreateSubscribedProxyEventXf(event, cache_size));

      // Store the cache size
      cache_size_ = cache_size;

      // The result from SomeIpBindingCore is not used because it is always a positive result.
      // In the future this behavior may change, when validation functionality is moved into
      // the SomeIpBindingCore.
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      static_cast<void>(client_event_->SubscribeEvent(cache_size, notification_handler_));

    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) noexcept {
            s << "Precondition violation: Same event has been subscribed twice.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-SomeIpbinding-ProxyEventXF_Subscribe
  }

  /*!
   * \brief       Removes the provided event manager from the subscriber list.
   * \details     Called from Event Manager. The SubscriptionState::kNotSubscribed state must be notified in the
   *              current call context as the event subscriber pointer is not valid after this call.
   * \param[in]   event A pointer to the corresponding proxy event. Used for notification handling.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check whether subscriber is null or not.
   * - If not null
   *   - Trigger the client subscriber to unsubscribe from the event.
   *   - Reset ProxyEventXf by clearing SubscribedProxyEventXf.
   * - Otherwise, ignore it.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_const_ref
  void Unsubscribe(EventSubscriberInterface* event) noexcept final {
    // PTP-B-SomeIpbinding-ProxyEventXF_Unsubscribe
    // Unscription notification for EventSubscriber will be triggert in ClientEvent
    static_cast<void>(event);
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    if (subscribed_proxy_event_xf_.has_value()) {
      {
        // NOTE: lock has to be release before the unsubscribe call which triggers the callback that would deadlock.
        std::lock_guard<std::mutex> const status_update_callable_guard{subscribe_status_update_callable_lock_};
        state_ = ::ara::com::SubscriptionState::kNotSubscribed;
      }

      // After this line, it is not allowed to receive any new events.
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      client_event_->UnsubscribeEvent();

      subscribed_proxy_event_xf_.reset();
      // PTP-E-SomeIpbinding-ProxyEventXF_Unsubscribe
    }
  }

  /*!
   * \brief   Reads the serialized samples from underlying receive buffers and passes them to the provided callable.
   *          Event samples without E2E protection will be ignored in case of a deserialization error.
   * \details Binding implementation should start reading all the received samples
   *          from the top of its receive buffers. Reading of samples should be continued until either:
   *            > the pre-allocated memory for storing the deserialized samples is exhausted.
   *            > given maximum samples have been processed within this call.
   *            > there are no further new samples to read.
   * \param[in]   max_samples            Maximum number of samples that can be processed within this call.
   * \param[in]   callable_sample_result Callable to be invoked on successful deserialization.
   *                                     The callable is valid only until the scope of this function call,
   *                                     so storing and invoking it at a later point will lead to undefined behavior.
   * \return      An amsr::core::Result containing the number of event samples for which the provided callback is
   * called.
   *
   * \error       ara::com::ComErrc::kMaxSamplesReached if all slots from visible sample cache are used at the
   *              beginning of the call.
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if
   *              > Subscribe call is returned successfully, or
   *              > HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              TRUE against GetE2EResult and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate    Product Private
   * \trace SPEC-10144331, SPEC-10144327
   *
   * \internal
   * - If no subscriber registered
   *   - Log fatal and call Abort().
   * - Create a result with an error ComErrc::kMaxSamplesReached.
   * - If there are sample pointers available in the visible sample cache
   *   - If there are events available in the invisible cache
   *     - Call ReadSamplesInternal
   *     - Store the number of successfully processed events
   *   - Otherwise
   *     - Reset the current E2E result
   *   - Emplace number of processed events in result
   * - Return result
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_STL_exceptions_noexcept
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.5.3_STL_exceptions_noexcept
  auto ReadSamples(std::size_t const max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final {
    if (!subscribed_proxy_event_xf_.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) noexcept {
            s << "Precondition violation: ReadSamples() cannot be called in unsubscribed state.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    ::ara::com::ComErrorDomain::SupportDataType const no_support_data_provided{0};
    ReadSamplesResult result{ReadSamplesResult::FromError(
        ::ara::com::MakeErrorCode(::ara::com::ComErrc::kMaxSamplesReached, no_support_data_provided,
                                  "Application holds more SamplePtrs than committed in Subscribe"))};

    SubscribedProxyEventXf& subscribed_proxy_event_xf{subscribed_proxy_event_xf_.value()};

    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    if (subscribed_proxy_event_xf.visible_sample_cache->GetFreeSampleCount() > 0) {
      std::size_t nr_valid_events_processed{0};

      InvisibleSampleCache::SampleCacheContainer& sample_container{
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          client_event_->ReadSamples(max_samples)};

      logger_.LogDebug(
          [&sample_container, &subscribed_proxy_event_xf, &max_samples](::ara::log::LogStream& s) {
            s << "Trying to read maximum of " << max_samples << " sample(s). ";
            s << "Retrieved from received sample container: ";
            s << sample_container.size() << " sample(s).";
            s << ", Free sample count:";
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            s << subscribed_proxy_event_xf.visible_sample_cache->GetFreeSampleCount();
            s << " sample(s).";
          },
          static_cast<char const*>(__func__), __LINE__);

      // Note: the returned cache might have size equal, greater
      // than or less than max_samples.
      // In case more samples than requested exist, do not
      // process the additional samples.
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      nr_valid_events_processed = subscribed_proxy_event_xf.sample_reader->ReadSamples(
          subscribed_proxy_event_xf.visible_sample_cache, sample_container, max_samples, callable_sample_result);

      result.EmplaceValue(nr_valid_events_processed);
    }  // E2E result is not updated as this is not required by SWS, see ESCAN00112552
    return result;
  }

  /*!
   * \brief       Gets the E2E result of the recent E2E check.
   * \return      The E2E result of the recent E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              TRUE against ReadSamples and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return E2E result from threadsafe e2e result.
   * \endinternal
   */
  ::ara::com::e2e::Result const GetE2EResult() const noexcept final { return e2e_result_(); }

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   *
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples. The given
   *                      callable shall not be null and shall be valid at least until it is de-registered. The given
   *                      callable shall not be invoked prior subscription and after unsubscription.
   *
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
  auto RegisterReceiveHandler(CallableEventNotification callable) noexcept -> void final {
    static_cast<void>(callable);
  }

  /*!
   * \brief Deregisters the event receive handler.
   *
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
  auto DeregisterReceiveHandler() noexcept -> void final {}

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   *
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes. The
   *                      given callable shall not be null and shall be valid at least until it is de-registered. The
   *                      given callable shall not be invoked prior subscription and after unsubscription. The given
   *                      callback shall not be invoked with "kNotSubscribed" state. The given callback shall not be
   *                      invoked if the subscription state has not changed.
   *
   * \pre         Runtime Processing Mode is ThreadDriven
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
  auto RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept -> void final {
    static_cast<void>(callable);
  }

  /*!
   * \brief Deregisters the subscription state change handler.
   *
   * \pre         The subscription state change handler is registered before.
   * \pre         Runtime Processing Mode is ThreadDriven
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
  auto DeregisterSubscriptionStateChangeHandler() noexcept -> void final {}

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
    std::lock_guard<std::mutex> const status_update_callable_guard{subscribe_status_update_callable_lock_};
    return state_;
  }

  /*!
   * \brief Cache size
   */
  std::size_t cache_size_{0U};

  /*!
   * \brief Proxy event xf notification handler to be registered in the SomeIpBindingCore.
   *        SomeIpBindingCore (ClientEvent) will hold a weak pointer to this object,
   *        in order to forward incoming event notifications.
   *        It will be registered into SomeIpBindingCore within subscribe call of this class, and
   *        deregistered during unsubscribe call.
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  std::shared_ptr<EventNotificationHandler> notification_handler_;

  /*!
   * \brief E2e result of last e2e check
   */
  ThreadSafeE2eResult e2e_result_{};

  /*!
   * \brief SOME/IP ID of this event.
   */
  someip_binding_core::internal::SomeIpEventIdentity const someip_event_identity_;

  /*!
   * \brief Client event unique ptr
   */
  std::unique_ptr<::amsr::someip_binding_core::internal::events::ClientEvent> client_event_;

  /*!
   * \brief Deserializer used to deserialize incoming samples.
   */
  EventDeserializerUniquePtr deserializer_;

  /*!
   * \brief Contains information whether someip or signal-based is used.
   */
  ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization_;

  /*!
   * \brief Flag for the usage of the pdu header extension.
   */
  bool pdu_header_extension_flag_;

  /*!
   * \brief Contains e2e configuration and e2e transformer.
   */
  ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
      e2e_parametrization_data_;

  /*!
   * \brief SubscribedProxyEventXf holds all objects needed during subscribed state. During subscribe this optional
   * will be set. It has no value when this ProxyEventXf is not subscribed.
   */
  ::amsr::core::Optional<SubscribedProxyEventXf> subscribed_proxy_event_xf_{};

  /*!
   * \brief Event subscription state
   */
  ::ara::com::SubscriptionState state_{::ara::com::SubscriptionState::kNotSubscribed};

  /*!
   * \brief Mutex for protection against parallel access of state update callable.
   */
  mutable std::mutex subscribe_status_update_callable_lock_{};

  /*!
   * \brief Logger Prefix.
   */
  ::amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "ProxyEventXf", someip_event_identity_.GetServiceId(), someip_event_identity_.GetInstanceId(),
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

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_H_
