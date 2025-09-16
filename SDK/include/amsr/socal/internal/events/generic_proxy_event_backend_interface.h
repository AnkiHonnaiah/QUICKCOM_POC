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
/**        \file  generic_proxy_event_backend_interface.h
 *        \brief  Interface definition for the generic proxy event backend.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/generic_memory_cache_interface.h"
#include "ara/com/types_common.h"
#include "ara/core/memory_resource.h"
#include "vac/language/function_ref.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface definition for the generic proxy event backend.
 */
class AMSR_LG_BETA GenericProxyEventBackendInterface {
 public:
  /*!
   * \brief Sample data containing the memory pointer.
   */
  struct SampleData {  // VCA_SOCAL_CALLING_STL_APIS
    /*!
     * \brief Memory pointer to access the raw sample data.
     *        The memory pointer shall never be nullptr.
     */
    GenericMemoryCacheInterface::GenericMemoryCacheEntryPtr memory_wrapper_ptr{};

    /*!
     * \brief Sample cache the data pointer belongs to.
     */
    std::weak_ptr<GenericMemoryCacheInterface> cache_ptr{};
  };

  /*!
   * \brief Alias for return type of ReadSamples API.
   */
  using ReadSamplesResult = ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Callable type to be invoked for every new sample read from the Binding internal memory buffers.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
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
  using CallableEventNotification =
      ::vac::language::UniqueFunction<void(void), ::ara::core::PolymorphicAllocator<char>>;

  /*!
   * \brief   Callable type to be invoked upon the event subscription state changes.
   * \details This callable shall not be invoked prior subscription and after unsubscription.
   *          This callable shall not be invoked with "kNotSubscribed" state.
   *          Schedules a task in R-Port default threadpool to invoke the event subscription state change handler.
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  using CallableSubscriptionStateUpdate = ::vac::language::UniqueFunction<void(::ara::com::SubscriptionState const),
                                                                          ::ara::core::PolymorphicAllocator<char>>;

  /*!
   * \brief Constructs the backend for the generic proxy event.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  GenericProxyEventBackendInterface() noexcept = default;

  GenericProxyEventBackendInterface(GenericProxyEventBackendInterface const&) = delete;
  GenericProxyEventBackendInterface(GenericProxyEventBackendInterface&&) = delete;
  GenericProxyEventBackendInterface& operator=(GenericProxyEventBackendInterface const&) & = delete;
  GenericProxyEventBackendInterface& operator=(GenericProxyEventBackendInterface&&) & = delete;

  /*!
   * \brief Subscribes to the event.
   * \param[in] cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                        memory for storing at least the cache_size number of samples.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual void Subscribe(std::size_t const cache_size) noexcept = 0;

  /*!
   * \brief Unsubscribes from the event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API for this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual void Unsubscribe() noexcept = 0;

  /*!
   * \brief   Reads the serialized samples from underlying receive buffers and passes them to the provided callable.
   * \details Binding implementation should start reading all the received samples from the top of its receive buffers.
   *          Reading of samples should be continued until either:
   *          - The pre-allocated memory for storing the samples is exhausted.
   *          - Given maximum samples have been processed within this call.
   *          - There are no further new samples to read.
   * \param[in] max_samples             Maximum number of samples that can be processed within this call.
   * \param[in] callable_sample_result  Callable to be invoked with the serialized sample.
   *                                    The underlying memory pointer shall never be nullptr.
   *                                    The callable is valid only until the scope of this function call,
   *                                    so storing and invoking it at a later point will lead to undefined behavior.
   * \return Result containing the number of events that have been passed to the provided callable.
   * \error ara::com::ComErrc::kMaxSamplesReached  if all slots from the binding are used at the beginning of the call.
   * \pre         Event must be subscribed. The event is considered to be subscribed, if
   *              - Subscribe call is returned successfully, or
   *              - Registered EventNotification callable is triggered in the middle of subscription.
   * \context     App
   * \threadsafe  FALSE
   *              TRUE against GetFreeSampleCount and
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ReadSamplesResult ReadSamples(std::size_t const max_samples,
                                        CallableReadSamplesResult const& callable_sample_result) noexcept = 0;

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
   * \brief Query the number of available sample slots in the local sample cache.
   * \return The number of free samples available in the local sample cache.
   * \pre         Event must be subscribed. The event is considered to be subscribed, if
   *              - Subscribe call is returned successfully, or.
   * \context     App
   * \threadsafe  TRUE
   *              TRUE against ReadSamples and
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual std::size_t GetFreeSampleCount() const noexcept = 0;

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The APIs RegisterReceiveHandler/DeregisterReceiveHandler shall not be called from within the
   *                      given callback.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
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
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
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
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The subscription state change handler is registered before.
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
   * \brief Destroys the generic proxy event backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~GenericProxyEventBackendInterface() noexcept = default;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_INTERFACE_H_
