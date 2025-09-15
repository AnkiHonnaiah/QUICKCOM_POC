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
/**        \file  socal/r20_11/events/proxy_event_base.h
 *        \brief  Proxy Event Base
 *
 *      \details  ProxyEventBase templated class represents an event of a proxy. For proxy instance, ProxyEvent shall be
 *                instantiated for all events defined in this Proxy. Each Proxy/Event type combination represents a
 *                different ProxyEvent class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_BASE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <limits>
#include <mutex>
#include <string>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/string_view.h"
#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/proxy_logger_util.h"
#include "amsr/socal/internal/utility.h"
#include "amsr/socal/r20_11/events/sample_ptr.h"
#include "ara/com/com_error_domain.h"
#include "ara/com/e2e_types.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace r20_11 {
namespace events {

/*!
 * \brief Generic template class for event handling (Subscribe, Unsubscribe, GetNewSamples, ...) by a proxy.
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimeStampConfiguration  Configures if the time stamp is enabled or disabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r20_11_Sync
 */
template <typename EventSampleType,
          typename TimestampConfiguration = ::amsr::socal::internal::events::TimeStampDisabled>
class ProxyEventBase : public ::amsr::socal::internal::events::EventSubscriberInterface {
 public:
  /*!
   * \brief Type alias for the data type of the event.
   * \vpublic
   */
  using SampleType = EventSampleType;

  /*!
   * \brief Type alias for SamplePtr.
   */
  using SamplePtr = ::amsr::socal::r20_11::events::SamplePtr<SampleType const, TimestampConfiguration>;

  /*!
   * \brief Type alias for proxy event backend interface pointer type.
   */
  using ProxyEventBackend = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;

  /*!
   * \brief Type alias for proxy event backend pointer.
   */
  using ProxyEventBackendPtr = std::add_pointer_t<ProxyEventBackend>;

  /*!
   * \brief Type alias for subscription return type.
   */
  using SubscriptionResult = ::ara::core::Result<void>;

  /*!
   * \brief Type alias for get new samples return type.
   */
  using GetNewSamplesResult = typename ProxyEventBackend::ReadSamplesResult;

  /*!
   * \brief Sample data containing the memory pointer, e2e check status and time stamp.
   */
  using SampleData = typename ProxyEventBackend::SampleData;

  ProxyEventBase() = delete;
  ProxyEventBase(ProxyEventBase const&) = delete;
  ProxyEventBase(ProxyEventBase&&) = delete;
  ProxyEventBase& operator=(ProxyEventBase const&) & = delete;
  ProxyEventBase& operator=(ProxyEventBase&&) & = delete;

  /*!
   * \brief  Define destructor.
   * \remark Called from user thread.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~ProxyEventBase() noexcept override = default;

  /*!
   * \brief Subscribes to the skeleton counterpart of this proxy-side event.
   * \param[in] max_sample_count  Size limits of this cache.
   * \return Result with no value or an error.
   * \error ara::com::ComErrc::kMaxSampleCountNotRealizable  If the event is already subscribed and the provided
   *                                                         max_sample_count value is different from the value for the
   *                                                         current subscription.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the event is already subscribed
   *   - If the same max_sample_count is used as with the original subscription call
   *     - Return silently.
   *   - Otherwise
   *     - Emplace kMaxSampleCountNotRealizable in subscription_result and return it.
   * - Otherwise
   *   - Set is_subscribed_ to true.
   *   - Call Subscribe on event backend.
   * - Return the subscription_result.
   * \endinternal
   *
   * \trace SPEC-4980372
   */
  SubscriptionResult Subscribe(std::size_t max_sample_count) noexcept {
    SubscriptionResult subscription_result{
        SubscriptionResult::FromError(::ara::com::ComErrc::kMaxSampleCountNotRealizable)};

    if (max_sample_count < std::numeric_limits<decltype(max_sample_count)>::max()) {
      if (is_subscribed_.load()) {
        if (max_sample_count == subscribed_max_sample_count_) {
          subscription_result.EmplaceValue();
        }
      } else {
        is_subscribed_ = true;  // Set the state before informing bindings.
        subscribed_max_sample_count_ = max_sample_count;
        // All the relevant member variables must be updated prior to calling Subscribe() on the backend,
        // because after this call an event reception handler can be triggered anytime.
        // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
        proxy_event_backend_.Subscribe(this, max_sample_count);
        subscription_result.EmplaceValue();
      }
    } else {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) { s << "Max sample count exceeds the allowed limit."; },
          static_cast<char const*>(__func__), __LINE__);
    }
    return subscription_result;
  }

  /*!
   * \brief Query the number of available sample slots from the binding.
   * \return The number of free samples available.
   * \pre         Subscribe() has been called.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              This API can be called from event receive handler when subscription / unsubscription is requested in
   *              parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the event is subscribed.
   *   - Call GetFreeSampleCount() on event backend.
   *   - Return the retrieved sample count.
   *
   * Calls "ara::core::Abort()" if:
   * - The event has been not subscribed to.
   * \endinternal
   *
   * \trace SPEC-8053573
   */
  std::size_t GetFreeSampleCount() const noexcept {
    std::size_t sample_count{0U};
    if (!is_subscribed_.load()) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before subscription or after unsubscription of the event.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      sample_count = proxy_event_backend_.GetFreeSampleCount();
    }
    return sample_count;
  }

  /*!
   * \brief Reads the serialized samples from underlying receive buffers and deserializes them.
   * \tparam    F            User provided callable function with the signature
   *                         void(ara::com::SamplePtr<SampleType const>).
   * \param[in] f            Callable to be invoked on every deserialized sample.
   * \param[in] max_samples  Maximum number of samples that can be processed within this call.
   * \return Result containing the number of successfully processed events within this call or an error.
   * \error ara::com::ComErrc::kMaxSamplesReached  If all slots from visible sample cache are used.
   * \pre         Subscribe() has been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              This API can be called from event receive handler when subscription / unsubscription is requested in
   *              parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Create a result with the valid event processed set to zero.
   * - If the event is subscribed
   *   - Define a callable which
   *     - Create a SamplePtr from the deserialized data.
   *     - Invoke the user provided callable f.
   *   - Invoke ReadSamples on the proxy_event_backend_ and emplace its return value into result.
   * - Otherwise
   *   - Log a warning that API is called prior to subscription.
   * - Return the result.
   *
   * Calls "ara::core::Abort()" if:
   * - The event has been not subscribed to.
   * \endinternal
   *
   * \trace SPEC-8053568
   */
  template <typename F>
  auto GetNewSamples(F&& f, std::size_t max_samples = std::numeric_limits<size_t>::max()) noexcept
      -> GetNewSamplesResult {
    // PTP-B-Socal-ProxyEvent_GetNewSamples
    GetNewSamplesResult result{GetNewSamplesResult::FromValue(0UL)};
    if (is_subscribed_.load()) {
      auto callable_sample_result = [this, &f](SampleData&& deserialized_data) {
        // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_FROM_CALLBACK_SYNCHRONOUSLY
        f(GetSamplePtr(std::move(deserialized_data)));
      };
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      result = proxy_event_backend_.ReadSamples(max_samples, callable_sample_result);
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before subscription or after unsubscription of the event.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-Socal-ProxyEvent_GetNewSamples
    return result;
  }

  /*!
   * \brief Return the global SMState of this specific proxy event.
   * \return The SMState of the last E2E_Check from GetNewSamples.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              This API can be called from event receive handler when subscription / unsubscription is requested in
   *              parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Return the latest E2E state machine state given by the backend.
   * \endinternal
   *
   * \trace SPEC-9644012
   */
  ::ara::com::e2e::SMState GetSMState() const noexcept { return GetResult().GetState(); }

  /*!
   * \brief Gets the E2E result of the recent E2E check.
   * \return The E2E result of the recent E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              This API can be called from event receive handler when subscription / unsubscription is requested in
   *              parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Return the latest E2E check result given by the backend.
   * \endinternal
   *
   * \trace SPEC-4980375
   */
  ::ara::com::e2e::Result const GetResult() const noexcept {
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_SOCAL_AutosarC++17_10-A7.2.1_GetE2EResult
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    return proxy_event_backend_.GetE2EResult();
  }

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-5951300
   */
  ::ara::com::SubscriptionState GetSubscriptionState() const noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    return proxy_event_backend_.GetSubscriptionState();
  }

 protected:
  /*!
   * \brief Creates an unsubscribed proxy event.
   * \param[in] event_backend  Pointer to the related event backend. The given pointer must not be null and must be
   *                           valid until this object has been destroyed.
   * \param[in] service_name   The service name for this proxy event.
   * \param[in] event_name     The event name of this proxy event.
   * \param[in] instance_id    The instance identifier to be used with this proxy event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit ProxyEventBase(ProxyEventBackendPtr event_backend, std::string const& service_name,
                          std::string const& event_name, ::ara::com::InstanceIdentifier const& instance_id) noexcept
      : ::amsr::socal::internal::events::EventSubscriberInterface{},
        logger_{::amsr::socal::internal::logging::kAraComLoggerContextId,
                ::amsr::socal::internal::logging::kAraComLoggerContextDescription,
                // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
                ::amsr::socal::internal::ProxyLoggerUtil::ConstructPrefix("ProxyEvent", event_name, service_name,
                                                                          instance_id)},
        proxy_event_backend_{::amsr::socal::internal::Utility::GetRef(event_backend)} {}

  /*!
   * \brief Returns true, if user has subscribed for the event.
   * \return True, if the user has put a subscription request, false otherwise.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool IsSubscribed() const { return is_subscribed_.load(); }

  /*!
   * \brief Unsubscribes from the skeleton counterpart of this proxy-side event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the event is previously subscribed
   *   - Set state to unsubscribed.
   *   - Call Unsubscribe() on event backend.
   * \endinternal
   */
  void UnsubscribeInternal() noexcept {
    if (is_subscribed_.load()) {
      is_subscribed_ = false;  // First reset the state before informing bindings.
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      proxy_event_backend_.Unsubscribe(this);
    }
  }

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_;

 private:
  /*!
   * \brief Construct the SamplePtr from the deserialization result when the parameter of time stamp is disabled.
   * \tparam    Config             Configures if the time stamp is enabled or disabled.
   * \param[in] deserialized_data  The deserialized event data.
   * \return SamplePtr containing the sample data and e2e check status.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
  template <typename Config = TimestampConfiguration>
  auto GetSamplePtr(SampleData&& deserialized_data) const noexcept
      -> std::enable_if_t<Config::IsEnabled != true, SamplePtr> {
    // VCA_SOCAL_CALLING_STL_APIS
    return SamplePtr{std::move(deserialized_data.memory_wrapper_if_ptr), std::move(deserialized_data.cache_ptr),
                     deserialized_data.e2e_check_status};
  }

  /*!
   * \brief Construct the SamplePtr from the deserialization result when the parameter of time stamp is enabled.
   * \tparam    Config             Configures if the time stamp is enabled or disabled.
   * \param[in] deserialized_data  The deserialized event data.
   * \return sample_ptr containing the deserialized data, preallocated sample and time stamp.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the deserialization result does not contain a valid time stamp, abort the application.
   * - Construct and return the SamplePtr which includes the time stamp.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  auto GetSamplePtr(SampleData&& deserialized_data) const noexcept -> std::enable_if_t<Config::IsEnabled, SamplePtr> {
    if (!deserialized_data.time_stamp.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid time stamp!";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    typename SamplePtr::TimeStamp const timestamp{deserialized_data.time_stamp.value()};
    // VCA_SOCAL_CALLING_STL_APIS
    return SamplePtr{std::move(deserialized_data.memory_wrapper_if_ptr), std::move(deserialized_data.cache_ptr),
                     deserialized_data.e2e_check_status, timestamp};
  }

  /*!
   * \brief Boolean indicating whether the proxy event is subscribed.
   */
  std::atomic<bool> is_subscribed_{false};

  /*!
   * \brief The proxy event backend.
   */
  ProxyEventBackend& proxy_event_backend_;

  /*!
   * \brief Subscribed cache size.
   */
  std::size_t subscribed_max_sample_count_{0};
};

}  // namespace events
}  // namespace r20_11
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_BASE_H_
