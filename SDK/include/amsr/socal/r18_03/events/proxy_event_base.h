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
/**        \file  socal/include/amsr/socal/r18_03/events/proxy_event_base.h
 *         \brief Proxy Event Base class providing polling based APIs.
 *
 *      \details  ProxyEventBase templated class represents an event of a proxy. For proxy instance,
 *                ProxyEventBase shall be instantiated for all events defined in this Proxy. Each Proxy/Event type
 *                combination represents a different ProxyEventBase class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_BASE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/string_view.h"
#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/proxy_logger_util.h"
#include "amsr/socal/internal/sample_container_type.h"
#include "amsr/socal/internal/utility.h"
#include "ara/com/e2e_types.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace r18_03 {
namespace events {

/*!
 * \brief Generic template class for event handling (Subscribe, Unsubscribe, Update, ...) by a proxy.
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimeStampConfiguration  Configures if the time stamp is enabled or disabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r18_03_Sync
 */
template <typename EventSampleType, typename TimestampConfiguration>
class ProxyEventBase : public ::amsr::socal::internal::events::EventSubscriberInterface {
 public:
  /*!
   * \brief Type-alias for the data type of the event.
   * \vpublic
   */
  using SampleType = EventSampleType;

  /*!
   * \brief SampleContainer type.
   * \vpublic
   */
  using SampleContainer = typename ::amsr::socal::internal::SampleContainerType<SampleType>::Type;

  /*!
   * \brief Type alias for proxy event backend.
   */
  using ProxyEventBackend = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;

  /*!
   * \brief Alias for return type of ReadSamples API.
   */
  using ReadSamplesResult = typename ProxyEventBackend::ReadSamplesResult;

  /*!
   * \brief Type alias for proxy event backend pointer.
   */
  using ProxyEventBackendPtr = std::add_pointer_t<ProxyEventBackend>;

  /*!
   * \brief Sample data containing the memory pointer, e2e check status and time stamp.
   */
  using SampleData = typename ProxyEventBackend::SampleData;

  /*!
   * \brief Callable type to be invoked for every deserialized sample.
   */
  using CallableReadSamplesResult = typename ProxyEventBackend::CallableReadSamplesResult;

  ProxyEventBase() = delete;
  ProxyEventBase(ProxyEventBase const&) = delete;
  ProxyEventBase(ProxyEventBase&&) = delete;
  ProxyEventBase& operator=(ProxyEventBase const&) & = delete;
  ProxyEventBase& operator=(ProxyEventBase&&) & = delete;

  /*!
   * \brief Define destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
  ~ProxyEventBase() noexcept override = default;

  /*!
   * \brief Subscribes to the skeleton counterpart of this proxy-side event.
   * \param[in] policy      Update policy of the underlying event cache.
   * \param[in] cache_size  Size limit of this cache.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If given cache size exceeds PTRDIFF_MAX
   *   - Log fatal and abort.
   * - Update the event cache update policy.
   * - If not already subscribed
   *   - Set subscription state to true.
   *   - If update policy is kLastN
   *     - Reserve the capacity of visible sample cache to twice the given cache size.
   *   - Otherwise reserve the capacity to the given cache size.
   *   - Update cache capacity.
   *   - Set subscription state.
   *   - Call Subscribe() on the event backend.
   * \endinternal
   *
   * \trace SPEC-4980372
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  void Subscribe(::ara::com::EventCacheUpdatePolicy policy, std::size_t const cache_size) noexcept {
    if (cache_size > PTRDIFF_MAX) {
      logger_.LogFatalAndAbort(
          [cache_size](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            s << "The given cache size is too large: " << cache_size;  // VCA_SOCAL_CALLING_STL_APIS
            s << ", must not exceed PTRDIFF_MAX: " << PTRDIFF_MAX;     // VCA_SOCAL_CALLING_STL_APIS
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    event_cache_update_policy_ = policy;

    if (!is_subscribed_.load()) {
      std::size_t visible_sample_cache_size{cache_size};
      if (policy == ::ara::com::EventCacheUpdatePolicy::kLastN) {
        // For LastN Policy, allocate double the size to avoid memory allocations or unnecessary copies in Update call.
        // Here we end up consuming more memory than necessary, but this is compromised because of performance gain.
        visible_sample_cache_size += cache_size;
      }

      visible_sample_cache_.reserve(visible_sample_cache_size);  // VCA_SOCAL_CALLING_STL_APIS
      cache_capacity_ = cache_size;
      is_subscribed_ = true;

      // All the relevant member variables must be updated prior to calling Subscribe() on the backend,
      // because after this call an event reception handler can be triggered anytime.
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      proxy_event_backend_.Subscribe(this, visible_sample_cache_size);

    } else {
      logger_.LogWarn(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "Event is already subscribed. Subscription ignored.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Updates the event cache container visible to the user via GetCachedSamples().
   * \details Calling Update() will invalidate any reference to a SampleContainer that has been acquired via
   *          GetCachedSamples() API before, after that GetCachedSamples has to be called again to get a new valid
   *          reference to the SampleContainer.
   * \param[in] filter  A function that indicates whether an event should be copied to the event cache.
   * \return true if at least one new event was transferred to the event cache.
   * \pre         Subscribe() has been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against GetCachedSamples and Cleanup APIs for same class instance.
   *              This API can be called from event receive handler when subscription / unsubscription is requested
   *              in parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If subscribed
   *   - Define a callable which
   *     - Apply a filter function on a received sample (if it is valid).
   *     - If filter is passed (or no filter is provided)
   *       - Emplace the received sample and e2e status into the visible sample cache.
   *   - If event cache update policy is 'kNewestN'
   *     - Call UpdateNewest.
   *   - If event cache update policy is 'kLastN'
   *     - Call UpdateLast.
   *
   * Calls "ara::core::Abort()" if:
   * - The event has not been subscribed to.
   * \endinternal
   *
   * \trace SPEC-8053568
   */
  bool Update(::ara::com::FilterFunction<SampleType> const& filter = {}) noexcept {
    // PTP-B-Socal-ProxyEvent_Update
    bool result{false};
    if (is_subscribed_.load()) {
      auto callable_samples_result = [&result, &filter, this](SampleData&& sample_data) {
        bool relevant{true};
        if (filter) {
          relevant = filter(sample_data.memory_wrapper_if_ptr->operator*());
        }
        if (relevant) {
          // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_MEMBER_VARIABLE_METHOD_INVOCATION
          visible_sample_cache_.emplace_back(std::move(sample_data.memory_wrapper_if_ptr), sample_data.cache_ptr,
                                             sample_data.e2e_check_status);
          result = true;
        }
      };

      if (event_cache_update_policy_ == ::ara::com::EventCacheUpdatePolicy::kNewestN) {
        UpdateNewest(callable_samples_result);
      } else {
        UpdateLast(callable_samples_result);
      }
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before subscription or after unsubscription of the event.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-Socal-ProxyEvent_Update
    return result;
  }

  /*!
   * \brief Returns the event cache container visible to the user.
   * \return A reference to the event cache container. The returned reference to SampleContainer is only valid until the
   *         next calls to Update/Cleanup/Unsubscribe APIs, after that GetCachedSamples has to be called again to get a
   *         new valid reference to the SampleContainer.
   * \pre         Subscribe() has been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against Update and Cleanup APIs for same class instance.
   *              This API can be called from event receive handler when subscription / unsubscription is requested in
   *              parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Return event cache container.
   * \endinternal
   *
   * \trace SPEC-4980059, SPEC-8053568
   */
  SampleContainer const& GetCachedSamples() const noexcept { return visible_sample_cache_; }

  /*!
   * \brief   Access the E2EState of the recent E2ECheck.
   * \details The E2E state machine is updated only upon deserialization of an event. This means it doesn't even 'see'
   *          events which were discarded when Update() is not called fast enough to keep up with new notifications as
   *          they continue to arrive.
   * \remark  To get the latest E2E state this API should be invoked after every Update() call.
   * \return The E2E status of the latest E2E check.
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
   * - Get E2E result from the proxy event backend, and extract the E2E state.
   *
   * Calls "std::terminate()" if:
   * - Construction of a new shared pointer fails.
   * \endinternal
   *
   * \trace SPEC-4980061
   */
  ::ara::com::E2E_state_machine::E2EState GetE2EState() const noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    return proxy_event_backend_.GetE2EResult().GetState();
  }

  /*!
   * \brief   Clears the cached event samples visible to the user if the selected caching policy is
   *          ara::com::EventCacheUpdatePolicy::kNewestN. Otherwise has no effect.
   * \details The returned reference to SampleContainer from GetCachedSamples API is only valid until call to Cleanup.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against Update and GetCachedSamples APIs for same class instance.
   *              This API can be called from event receive  handler when subscription / unsubscription is requested
   *              in parallel from the application thread without the need of additional synchronization measures.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the event's update policy is kNewestN.
   *   - Clear the event cache visible to the user.
   * \endinternal
   */
  void Cleanup() noexcept {
    if (event_cache_update_policy_ == ::ara::com::EventCacheUpdatePolicy::kNewestN) {
      visible_sample_cache_.clear();  // VCA_SOCAL_CALLING_STL_APIS
    }
  }

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         -
   * \context     ANY
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
   * \param[in] event_backend  Pointer to the event backend. The given pointer must not be null and must be valid until
   *                           the current object's lifetime.
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
        proxy_event_backend_{::amsr::socal::internal::Utility::GetRef(event_backend)} {
    static_assert(std::is_same<TimestampConfiguration, internal::events::TimeStampDisabled>::value,
                  "Time stamp feature can only be enabled when using R20-11");
  }

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
   * \brief   Unsubscribes from the skeleton counterpart of this proxy-side event.
   * \details Calling Unsubscribe() will invalidate any reference to a SampleContainer that has been acquired via
   *          GetCachedSamples() API before.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If subscribed
   *   - Set subscription state to false.
   *   - Clean visible sample cache.
   *   - Call Unsubscribe() on the event backend.
   * \endinternal
   */
  void UnsubscribeInternal() noexcept {
    if (is_subscribed_.load()) {
      is_subscribed_ = false;  // First reset the state before informing bindings.

      // Clean the cache before unsubscribing to save memory, to make sure that cache size is correct by next
      // subscription and to make sure that acquired memory is returned.
      visible_sample_cache_.clear();  // VCA_SOCAL_CALLING_STL_APIS

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
   * \brief Handles visible cache update for kLastN policy.
   * \param[in] callable  A function that is invoked for every newly deserialized sample.
   * \pre         Subscribe has been called, but Unsubscribe has not been called yet.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against GetCachedSamples and Cleanup APIs for same class instance.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Read the new samples from the event backed and log an error message in case of failure.
   * - Remove excess samples (the oldest ones) from the visible sample cache, if any. Calls "amsr::core::Abort()" if:
   * - ReadSamples returns an error.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  void UpdateLast(CallableReadSamplesResult const& callable) noexcept {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    ReadSamplesResult const result{proxy_event_backend_.ReadSamples(cache_capacity_, callable)};

    if (!result.HasValue()) {
      ::ara::core::ErrorCode const error_code{result.Error()};
      logger_.LogFatalAndAbort(
          [&error_code](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            ::ara::core::StringView const error_message{error_code.Message()};
            // VCA_SOCAL_CALLING_STL_APIS
            s << "ReadSamples returned with error code: " << error_message;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    if (visible_sample_cache_.size() > cache_capacity_) {
      // If the overall cache size exceeds the cache capacity, then remove the excess elements from the beginning
      // of visible sample cache.
      std::size_t const nr_excess_elements{visible_sample_cache_.size() - cache_capacity_};

      // It is safe to type cast the nr_excess_elements to ptrdiff_t, as it is already checked in the call to
      // Subscribe() that the user may not allocate a cache size more than the sizeof(PTRDIFF_MAX).
      static_cast<void>(visible_sample_cache_.erase(  // VCA_SOCAL_CALLING_STL_APIS
          visible_sample_cache_.cbegin(),
          std::next(visible_sample_cache_.cbegin(), static_cast<std::ptrdiff_t>(nr_excess_elements))));
    }
  }

  /*!
   * \brief Handles visible cache update for kNewestN policy.
   * \param[in] callable  A function that is invoked for every newly deserialized sample.
   * \pre         Subscribe has been called, but Unsubscribe has not been called yet.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against GetCachedSamples and Cleanup APIs for same class instance.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Clear the visible cache.
   * - Read the new samples from the event backed and log an error message in case of failure.
   *
   * Calls "amsr::core::Abort()" if:
   * - ReadSamples returns an error.
   * \endinternal
   */
  void UpdateNewest(CallableReadSamplesResult const& callable) noexcept {
    visible_sample_cache_.clear();  // VCA_SOCAL_CALLING_STL_APIS
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    ReadSamplesResult const result{proxy_event_backend_.ReadSamples(cache_capacity_, callable)};

    if (!result.HasValue()) {
      ::ara::core::ErrorCode const error_code{result.Error()};
      logger_.LogFatalAndAbort(
          [&error_code](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            ::ara::core::StringView const error_message{error_code.Message()};
            // VCA_SOCAL_CALLING_STL_APIS
            s << "ReadSamples returned with error code: " << error_message;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Boolean indicating whether the proxy event is subscribed.
   */
  std::atomic<bool> is_subscribed_{false};

  /*!
   * \brief Event cache visible to the user.
   */
  SampleContainer visible_sample_cache_{};

  /*!
   * \brief Maximum number of stored events.
   */
  std::size_t cache_capacity_{0U};

  /*!
   * \brief The proxy event backend.
   */
  ProxyEventBackend& proxy_event_backend_;

  /*!
   * \brief The event's update policy.
   */
  ::ara::com::EventCacheUpdatePolicy event_cache_update_policy_{::ara::com::EventCacheUpdatePolicy::kNewestN};
};

}  // namespace events
}  // namespace r18_03
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_BASE_H_
