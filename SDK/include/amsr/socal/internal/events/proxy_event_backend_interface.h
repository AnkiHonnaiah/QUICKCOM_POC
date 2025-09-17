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
/**        \file  proxy_event_backend_interface.h
 *        \brief  Interface class for the binding-specific proxy event backend.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_BACKEND_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/socal/internal/events/cache_interface.h"
#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/socal/internal/events/memory_wrapper_interface.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "amsr/socal/internal/types.h"
#include "ara/com/e2e_types.h"
#include "vac/language/function_ref.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface class for binding-specific proxy event backend.
 * \tparam EventSampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 */
template <typename EventSampleType>
class ProxyEventBackendInterface {
 public:
  /*!
   * \brief Constructs the backend for the proxy event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProxyEventBackendInterface() noexcept = default;

  ProxyEventBackendInterface(ProxyEventBackendInterface const&) = delete;

  ProxyEventBackendInterface(ProxyEventBackendInterface&&) = delete;

  ProxyEventBackendInterface& operator=(ProxyEventBackendInterface const&) & = delete;

  ProxyEventBackendInterface& operator=(ProxyEventBackendInterface&&) & = delete;

  /*!
   * \brief Alias for event sample type.
   */
  using SampleType = EventSampleType;

  /*!
   * \brief Alias for EventSubscriberInterface type.
   */
  using EventSubscriberInterface = ::amsr::socal::internal::events::EventSubscriberInterface;

  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = ::amsr::socal::internal::events::TimeStamp;

  /*!
   * \brief Alias for return type of ReadSamples API.
   */
  using ReadSamplesResult = ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Alias for a MemoryWrapperInterface object with SampleType.
   */
  using MemoryWrapperInterface = ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType>;

  /*!
   * \brief Alias for pointer type to a MemoryWrapperInterface object.
   */
  using MemoryWrapperInterfacePtr =
      typename ::amsr::socal::internal::events::CacheInterface<SampleType>::CacheEntryType;

  /*!
   * \brief Sample data containing the memory pointer, e2e check status and time stamp.
   */
  struct SampleData {  // VCA_SOCAL_CALLING_STL_APIS
    /*!
     * \brief Memory wrapper pointer to access the deserialized samples.
     */
    MemoryWrapperInterfacePtr memory_wrapper_if_ptr{};

    /*!
     * \brief Cache pointer memory wrapper was taken from.
     */
    std::weak_ptr<::amsr::socal::internal::events::CacheInterface<SampleType>> cache_ptr;

    /*!
     * \brief E2ECheckStatus for the sample.
     */
    ::ara::com::e2e::ProfileCheckStatus e2e_check_status{::ara::com::e2e::ProfileCheckStatus::Error};

    /*!
     * \brief The time stamp for when receiving a message.
     */
    ::amsr::core::Optional<TimeStamp> time_stamp{};
  };

  /*!
   * \brief Callable type to be invoked for every deserialized sample.
   * \steady TRUE
   */
  using CallableReadSamplesResult = ::vac::language::FunctionRef<void(SampleData&&)>;

  /*!
   * \brief   Callable type to be invoked upon receiving new event sample from skeleton.
   * \details This callable shall not be invoked prior subscription and after unsubscription.
   *          Schedules a task in R-Port default threadpool to invoke the event receive handler.
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  using CallableEventNotification = ::vac::language::UniqueFunction<void(void)>;

  /*!
   * \brief   Callable type to be invoked upon the event subscription state changes.
   * \details This callable shall not be invoked prior subscription and after unsubscription.
   *          This callable shall not be invoked with "kNotSubscribed" state.
   *          This callable shall not be invoked if the subscription state has not been changed.
   *          Schedules a task in R-Port default threadpool to invoke the event subscription state change handler.
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  using CallableSubscriptionStateUpdate = ::vac::language::UniqueFunction<void(void)>;

  /*!
   * \brief Subscribes to the event.
   * \param[in,out] event       A pointer to the corresponding proxy event. Used for notification handling. Given
   *                            pointer must not be a nullptr. The ownership of the object is not transferred. The
   *                            caller of the API must ensure passing a valid pointer. The passed pointer must be valid
   *                            until Unsubscribe() is called.
   * \param[in]     cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                            memory for storing at least the cache_size number of deserialized samples.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void Subscribe(EventSubscriberInterface* event, std::size_t const cache_size) noexcept = 0;

  /*!
   * \brief Unsubscribes from the event.
   * \param[in,out] event  A pointer to the corresponding proxy event. Used for notification handling. Given event
   *                       pointer must not be a nullptr. This parameter is deprecated and shall not be used anymore.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void Unsubscribe(EventSubscriberInterface* event) noexcept = 0;

  /*!
   * \brief   Reads the serialized samples from underlying receive buffers and passes them to the provided callable.
   *          Event samples without E2E protection will be ignored in case of a deserialization error.
   * \details Binding implementation should start reading all the received samples from the top of its receive buffers.
   *          Reading of samples should be continued until either:
   *          - The pre-allocated memory for storing the deserialized samples is exhausted.
   *          - Given maximum samples have been processed within this call.
   *          - There are no further new samples to read.
   * \param[in] max_samples             Maximum number of samples that can be processed within this call.
   * \param[in] callable_sample_result  Callable to be invoked on successful deserialization. The callable is valid only
   *                                    until the scope of this function call, so storing and invoking it at a later
   *                                    point will lead to undefined behavior.
   * \return Result containing the number of events that have been passed to the provided callable.
   * \error ara::com::ComErrc::kMaxSamplesReached  If all slots from the binding are used at the beginning of the call.
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if
   *              - Subscribe call is returned successfully, or.
   *              - HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              TRUE against GetE2EResult and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ReadSamplesResult ReadSamples(std::size_t const max_samples,
                                        CallableReadSamplesResult const& callable_sample_result) noexcept = 0;

  /*!
   * \brief Gets the E2E result of the recent E2E check.
   * \return The E2E result of the recent E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              TRUE against ReadSamples and GetFreeSampleCount when invoked on same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ::ara::com::e2e::Result const GetE2EResult() const noexcept = 0;

  /*!
   * \brief   Query the number of available sample slots in the local sample cache.
   * \details This API must not be called in parallel to service discovery APIs (Subscribe/Unsubscribe).
   * \return The number of free samples available in the local sample cache.
   * \pre         Event must be subscribed. It is assumed that the event is successfully subscribed, if
   *              - Subscribe call is returned successfully, or
   *              - HandleEventNotification is triggered on the EventSubscriberInterface in the middle of subscription.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against ReadSamples and GetE2EResult when invoked on same/different class instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual std::size_t GetFreeSampleCount() const noexcept = 0;

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ::ara::com::SubscriptionState GetSubscriptionState() const noexcept = 0;

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The APIs RegisterReceiveHandler/DeregisterReceiveHandler shall not be called from within the
   *                      given callback.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires callable != nullptr; \endspec
   * \steady FALSE
   */
  virtual void RegisterReceiveHandler(CallableEventNotification callable) noexcept = 0;

  /*!
   * \brief Deregisters the event receive handler.
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual void DeregisterReceiveHandler() noexcept = 0;

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The given callback shall not be invoked with "kNotSubscribed" state.
   *                      The given callback shall not be invoked if the subscription state has not changed.
   *                      The APIs RegisterSubscriptionStateChangeHandler/DeregisterSubscriptionStateChangeHandler shall
   *                      not be called from within the given callback.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires callable != nullptr; \endspec
   * \steady FALSE
   */
  virtual void RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept = 0;

  /*!
   * \brief Deregisters the subscription state change handler.
   * \pre         The subscription state change handler is registered before.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual void DeregisterSubscriptionStateChangeHandler() noexcept = 0;

 protected:
  /*!
   * \brief Destroys the ProxyEventBackendInterface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProxyEventBackendInterface() noexcept = default;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_BACKEND_INTERFACE_H_
