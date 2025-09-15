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
/**        \file  socal/r20_11/events/proxy_event_thread_driven.h
 *        \brief  Proxy Event template class specialization for RuntimeProcessingMode kThreadDriven.
 *
 *      \details  Additional to the ProxyEvent used for the polling mode, this class provides the APIs
 *                for receive handlers and subscription state handlers.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_THREAD_DRIVEN_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_THREAD_DRIVEN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/events/event_notification_functor.h"
#include "amsr/socal/internal/events/proxy_event_sync_interface.h"
#include "amsr/socal/internal/events/subscription_state_update_functor.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/thread_pool_task.h"
#include "amsr/socal/internal/thread_pools_manager.h"
#include "amsr/socal/r20_11/events/proxy_event.h"
#include "amsr/socal/r20_11/events/proxy_event_base.h"
#include "ara/com/types_common.h"
#include "ara/core/abort.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace socal {
namespace r20_11 {
namespace events {

/*!
 * \brief   Generic template class for event handling (Subscribe, Unsubscribe, Update, ...) by a proxy.
 * \details The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - Subscribe(),
 *          - Unsubscribe(),
 *          - SetReceiveHandler(),
 *          - UnsetReceiveHandler(),
 *          - SetSubscriptionStateHandler(),
 *          - UnsetSubscriptionStateHandler(),
 *          - GetSubscriptionState()
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimeStampConfiguration  Configures if the time stamp is enabled or disabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r20_11_Async
 */
template <typename EventSampleType, typename TimestampConfiguration>
class ProxyEvent<EventSampleType, TimestampConfiguration,
                 ::amsr::socal::internal::configuration::RuntimeProcessingMode::kThreadDriven>
    : public ::amsr::socal::internal::events::ProxyEventSyncInterface,
      public ::amsr::socal::r20_11::events::ProxyEventBase<EventSampleType, TimestampConfiguration> {
  /*!
   * \brief Type alias for the base class.
   */
  using Base = ::amsr::socal::r20_11::events::ProxyEventBase<EventSampleType, TimestampConfiguration>;

  /*!
   * \brief Wrapper-Type for the ::ara::com::EventReceiveHandler.
   */
  using ProxyEventReceiveHandler = ::vac::language::UniqueFunction<void()>;

  /*!
   * \brief Alias for a pair that temporary holds updated ProxyEventReceiveHandlers.
   */
  using ReceiveHandlerPair = std::pair<bool, ProxyEventReceiveHandler>;

  /*!
   * \brief Wrapper-Type for the ::ara::com::SubscriptionStateChangeHandler.
   */
  using ProxyEventSubscriptionHandler = ::vac::language::UniqueFunction<void(::ara::com::SubscriptionState const)>;

  /*!
   * \brief Alias for a pair that temporary holds updated ProxyEventSubscriptionHandler.
   */
  using SubscriptionHandlerPair = std::pair<bool, ProxyEventSubscriptionHandler>;

 public:
  /*!
   * \brief Type alias for the data type of the event.
   * \vpublic
   */
  using SampleType = EventSampleType;

  /*!
   * \brief Type alias for ThreadPool.
   */
  using ThreadPool = ::amsr::socal::internal::ThreadPool;

  /*!
   * \brief Type alias for proxy event backend interface pointer type.
   */
  using ProxyEventBackend = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;

  /*!
   * \brief Type alias for proxy event backend pointer.
   */
  using ProxyEventBackendPtr = std::add_pointer_t<ProxyEventBackend>;

  ProxyEvent() = delete;
  ProxyEvent(ProxyEvent const&) = delete;
  ProxyEvent(ProxyEvent&&) = delete;
  ProxyEvent& operator=(ProxyEvent const&) & = delete;
  ProxyEvent& operator=(ProxyEvent&&) & = delete;

  /*!
   * \brief Creates an unsubscribed proxy event.
   * \param[in] event_backend  Pointer to the related event backend. The given pointer must not be null and must be
   *                           valid until the current object's lifetime.
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
   * \spec requires event_backend != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.1.6: MD_SOCAL_A12.1.6_delegating_constructor
  explicit ProxyEvent(ProxyEventBackendPtr event_backend, std::string const& service_name,
                      std::string const& event_name, ::ara::com::InstanceIdentifier const& instance_id) noexcept
      : ::amsr::socal::internal::events::ProxyEventSyncInterface{},
        Base{event_backend, service_name, event_name, instance_id} {}

  /*!
   * \brief   Define destructor.
   * \details The destructor will not return until all scheduled tasks are removed or done.
   * \remark  Called from user thread.
   * \pre         This API should not be called if any user-defined mutex is locked inside EventReceiveHandler,
   *              SubscriptionStateChangeHandler and in the thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Remove the task associated with the key that is this object.
   * \endinternal
   */
  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  ~ProxyEvent() noexcept override { RemovePendingTasks(); }

  /*!
   * \brief Unsubscribes from the skeleton counterpart of this proxy-side event.
   * \pre         This API should not be called if any user-defined mutex is locked in both the EventReceiveHandler and
   *              in the thread from where this API is called..
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Lock the receive handler and subscription state handler mutex to protect unsubscription against parallel handler
   *   invocation.
   * - Forward the unsubscription to the base class.
   *
   * Calls "std::terminate()" if:
   * - Mutex locking fails.
   * \endinternal
   *
   * \trace SPEC-4980373
   */
  void Unsubscribe() noexcept {
    // Lock both handler locks with a deadlock avoidance approach and transfer them to individual lock_guards
    // to automatically unlock them on leaving the scope of this function
    std::lock(receive_handler_lock_, subscription_state_handler_lock_);  // VCA_SOCAL_CALLING_STL_APIS
    std::lock_guard<std::recursive_mutex> const receive_handler_lock__guard{receive_handler_lock_, std::adopt_lock};
    std::lock_guard<std::recursive_mutex> const subscription_state_handler_lock__guard{subscription_state_handler_lock_,
                                                                                       std::adopt_lock};
    Base::UnsubscribeInternal();
    latest_notified_subscription_state_ = ::ara::com::SubscriptionState::kNotSubscribed;
  }

  /*!
   * \brief   Set event receive handler, to be notified on reception of new event samples.
   * \details Called from user thread or from event-handler thread.
   * \param[in] handler  User-defined receive handler. Must not be nullptr. Any exception thrown by the callback will
   *                     lead to a termination through "std::terminate()".
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If \p handler is not a nullptr
   *   - Lock receive handler mutex.
   *   - Set the receive handler pair to 'true', that it is changed, and \p handler.
   *   - Unlock the receive handler mutex.
   *   - Set receive_handler_set_ to 'true'.
   * - Otherwise log an error for invalid receive handler.
   *
   * Calls "std::terminate()" if:
   * - Mutex acquisition fails.
   *
   * Calls "ara::core::Abort()" if:
   * - Receive handler is invalid.
   * - Called after Subscribe() or before Unsubscribe().
   * \endinternal
   *
   * \trace SPEC-4980377
   */
  void SetReceiveHandler(::ara::com::EventReceiveHandler handler) noexcept {
    if (Base::IsSubscribed()) {
      this->logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called after Subscribe or before Unsubscribe.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (handler != nullptr) {
      {
        std::lock_guard<std::recursive_mutex> const guard{receive_handler_lock_};
        // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS, VCA_SOCAL_VALID_MOVE_CONSTRUCTION
        new_receive_handler_pair_ = std::make_pair(true, std::move(handler));
      }
      receive_handler_set_.store(true);
      this->logger_.LogInfo(
          [](::amsr::socal::internal::logging::LogStream& s) { s << "Receive Handler has been set."; },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      this->logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid EventReceiveHandler.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Deregister event receive handler.
   * \details Called from user thread or from event-handler thread.
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         This API should not be called if any user-defined mutex is locked in EventReceiveHandler and in the
   *              thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the receive handler is set
   *   - Set receive_handler_set_ to 'false'.
   *   - Lock mutex.
   *   - Set the 'new receive handler' pair to ['true', an empty handler], indicating that the handler was unset.
   *
   * Calls "std::terminate()" if:
   * - Mutex acquisition fails.
   *
   * Calls "ara::core::Abort()" if:
   * - Called after Subscribe() or before Unsubscribe().
   * \endinternal
   *
   * \trace SPEC-4980379
   */
  void UnsetReceiveHandler() noexcept {
    if (Base::IsSubscribed()) {
      this->logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called after Subscribe or before Unsubscribe.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (receive_handler_set_.load()) {
      receive_handler_set_.store(false);
      std::lock_guard<std::recursive_mutex> const guard{receive_handler_lock_};

      new_receive_handler_pair_ = std::make_pair(true, nullptr);
      this->logger_.LogInfo(
          [](::amsr::socal::internal::logging::LogStream& s) { s << "Receive Handler has been unset."; },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Set a subscription state change handler to be called by the Communication Management implementation as
   *          soon as the subscription state of this event changes.
   * \details The Communication Management implementation will serialize calls to the registered handler. If by any
   *          chance the application receives consecutive updates with the same subscription state, the handler will not
   *          be called again until an update with a state different from the last known one arrives.
   * \param[in] handler  User-defined function. Must not be nullptr. Any exception thrown by the callback will lead to a
   *                     termination through "std::terminate()".
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Lock the mutex.
   * - Set the 'new subscription state handler' pair to ['true', \p handler] indicating that the pair was updated with
   *   the provided handler.
   * - Unlock the mutex.
   * - Set subscription_state_handler_set_ to 'true'.
   *
   * Calls "std::terminate()" if:
   * - Mutex acquisition fails.
   *
   * Calls "ara::core::Abort()" if:
   * - provided handler is invalid.
   * \endinternal
   */
  void SetSubscriptionStateHandler(::ara::com::SubscriptionStateChangeHandler handler) noexcept {
    if (handler == nullptr) {
      this->logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid SubscriptionStateChangeHandler.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    {
      std::lock_guard<std::recursive_mutex> const guard{subscription_state_handler_lock_};
      // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS, VCA_SOCAL_VALID_MOVE_CONSTRUCTION
      new_subscription_state_handler_pair_ = std::make_pair(true, std::move(handler));
    }
    subscription_state_handler_set_.store(true);
    this->logger_.LogInfo(
        [](::amsr::socal::internal::logging::LogStream& s) { s << "SubscriptionStateChangeHandler has been set."; },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Remove handler set by \see SetSubscriptionStateHandler.
   * \pre         This API should not be called if any user-defined mutex is locked in the
   *              SubscriptionStateChangeHandler and in the thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the subscription state handler is set
   *   - Set subscription_state_handler_set_ to 'false'.
   *   - Lock the mutex.
   *   - Set the 'new subscription state handler' pair to ['true', an empty handler], indicating that the handler was
   *     unset.
   *
   * Calls "std::terminate()" if:
   * - Mutex acquisition fails.
   * \endinternal
   */
  void UnsetSubscriptionStateHandler() noexcept {
    // Unset handler only if not already done.
    if (subscription_state_handler_set_.load()) {
      subscription_state_handler_set_.store(false);
      std::lock_guard<std::recursive_mutex> const guard{subscription_state_handler_lock_};

      new_subscription_state_handler_pair_ = std::make_pair(true, nullptr);
      this->logger_.LogInfo(
          [](::amsr::socal::internal::logging::LogStream& s) { s << "SubscriptionStateChangeHandler has been unset."; },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Removes all pending tasks related to this event from the thread-pool.
   * \pre      The receive handler and subscription state change handlers are not set.
   * \context  App
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Remove all the pending tasks related to this event from the thread-pool.
   * \endinternal
   */
  void RemovePendingTasks() const noexcept {
    // Stop all scheduled tasks related to this proxy.
    ::amsr::generic::Singleton<::amsr::socal::internal::ThreadPoolsManager>& thread_pools_manager{
        ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance()};
    // VCA_SOCAL_REMOVE_TASKS_UNDEFINED_FUNCTION, VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
    thread_pools_manager.GetAccess()->RemoveTasks(::amsr::socal::internal::kAssignmentNameRPortDefaultThreadPool, this);
  }

 private:
  /*!
   * \brief   Handles a received event notification.
   * \details Called upon every new event reception. Schedules a task in R-Port default thread-pool to invoke
   *          the event receive handler.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \vprivate    Product Private.
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If receive handler is set
   *   - Enqueue a ThreadPoolTask which contains a reference to the caller and an EventNotificationFunctor to the
   *     R-Port default thread-pool.
   * \endinternal
   */
  void HandleEventNotification() noexcept override {
    // Only schedule an event notification task, if a receive handler is set, indicated by
    // receive_handler_set_ equals true. This is done for performance reasons.
    if (receive_handler_set_.load()) {
      // The return value of EnqueueTask() is discarded by design. There is no
      // retry handling or notification to the application when the task queue is full. See also
      // TechnicalReference_AraCom, section "Thread Pools".
      // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_SOCAL_AutosarC++17_10_A5.0.1_falsepositive
      // VCA_SOCAL_DEREFERENCING_SINGLETON_ACCESS
      static_cast<void>(::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess()->EnqueueTask(
          ::amsr::socal::internal::kAssignmentNameRPortDefaultThreadPool,
          // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
          ::amsr::socal::internal::ThreadPoolTask{
              this, ::amsr::socal::internal::ThreadPoolTask::Callable{
                        ::amsr::socal::internal::events::EventNotificationFunctor{*this}}}));
    }
  }

  /*!
   * \brief   Handles an update of the subscription state of the event.
   * \details Called upon subscription state update of this event. Schedules a task in R-Port default thread-Pool
   *          to invoke the subscription state handler.
   * \pre         -
   * \context     Backend | App during Unsubscription.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \vprivate    Product Private.
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the event is subscribed
   *   - If the subscription state handler is set
   *     - Enqueue a ThreadPoolTask which contains a reference to the caller and an SubscriptionStateUpdateFunctor to
   *       the thread pool.
   * \endinternal
   */
  void HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState const) noexcept override {
    if (Base::IsSubscribed()) {
      // Only schedule a subscription state update task if a SubscriptionStateChangeHandler is set, indicated by
      // subscription_state_handler_set_ equals true. This is done for performance reasons.
      if (subscription_state_handler_set_.load()) {
        // Notify the application about updated subscription state
        // The return value of EnqueueTask() is discarded by design. There is no retry handling or notification to the
        // application when the task queue is full. See also TechnicalReference_AraCom, section "Thread Pools".
        // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_SOCAL_AutosarC++17_10_A5.0.1_falsepositive
        // VCA_SOCAL_DEREFERENCING_SINGLETON_ACCESS
        static_cast<void>(::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess()->EnqueueTask(
            ::amsr::socal::internal::kAssignmentNameRPortDefaultThreadPool,
            // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
            ::amsr::socal::internal::ThreadPoolTask{
                this, ::amsr::socal::internal::ThreadPoolTask::Callable{
                          ::amsr::socal::internal::events::SubscriptionStateUpdateFunctor{*this}}}));
      }
    }
  }

  /*!
   * \brief   Notify event receive handler on reception of a new event sample. Internal use only.
   * \details Called in the context of the Default Thread Pool.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Lock mutex.
   * - If the receive handler has changed since last call
   *   - Set receive handler to the new handler.
   * - If receive handler is valid and the event is still subscribed
   *   - Invoke the receive handler.
   *   - If the handler is changed (the handler can be changed within the callback), update the handler with new value.
   *
   * Calls "std::terminate()" if:
   * - Mutex acquisition fails.
   * \endinternal
   *
   * \trace SPEC-4980082, SPEC-4980117, SPEC-4980378
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_callback_function
  void HandleEventNotificationSync() noexcept override {
    // PTP-B-Socal-ProxyEvent_HandleEventNotificationSync
    std::lock_guard<std::recursive_mutex> const guard{receive_handler_lock_};
    // For performance reason we copy handler only if it is changed since last call.
    if (new_receive_handler_pair_.first) {
      receive_handler_ = std::move(new_receive_handler_pair_.second);
      new_receive_handler_pair_.first = false;
    }

    // We call receive handler within the receive_handler_lock_ just to make it deterministic,
    // that once the user call returns from Set/Unset-ReceiveHandler, old receive_handler_ is no more used.
    if (receive_handler_ != nullptr) {
      if (Base::IsSubscribed()) {
        // Do not use receive_handler_ after call of handler. Due to recursive mutex the handler could replace itself.
        // PTP-B-Socal-ProxyEvent_CallReceiveHandler
        this->logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Start: Event receive handler"; },
            static_cast<char const*>(__func__), __LINE__);
        receive_handler_();  // VCA_SOCAL_CALLING_STL_APIS
        this->logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "End: Event receive handler"; },
            static_cast<char const*>(__func__), __LINE__);
        // PTP-E-Socal-ProxyEvent_CallReceiveHandler

        if (new_receive_handler_pair_.first) {
          receive_handler_ = std::move(new_receive_handler_pair_.second);
          new_receive_handler_pair_.first = false;
        }
      }
    }
    // PTP-E-Socal-ProxyEvent_HandleEventNotificationSync
  }

  /*!
   * \brief   Called upon event subscription state update.
   * \details Called from the the Default Thread Pool Worker Thread.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Synchronize against Unsubscribe().
   * - Load subscription state.
   * - If state differs from latest notified subscription state
   *   - Set latest notified subscription state to be the subscription state.
   *   - Lock the subscription state handler mutex.
   *   - If new subscription state handler is changed
   *     - Set the subscription state handler to the new subscription state handler.
   *     - Set the change flag in new_subscription_state_handler_pair_ to false.
   *   - If the subscription state handler is still registered
   *     - Call subscription state handler with the subscription state.
   *     - If the subscription state handler is changed (the handler can be changed within the callback)
   *       - Update the handler with the new value.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_callback_function
  void HandleEventSubscriptionStateUpdateSync() noexcept override {
    // PTP-B-Socal-ProxyEvent_HandleEventSubscriptionStateUpdateSync

    std::lock_guard<std::recursive_mutex> const guard{subscription_state_handler_lock_};
    ::ara::com::SubscriptionState const notified_state{Base::GetSubscriptionState()};

    if (notified_state != latest_notified_subscription_state_) {
      latest_notified_subscription_state_ = notified_state;

      // For performance reason we copy handler only if it is changed since last call
      if (new_subscription_state_handler_pair_.first) {
        subscription_state_handler_ = std::move(new_subscription_state_handler_pair_.second);
        new_subscription_state_handler_pair_.first = false;
      }

      // Notify application only if handler is still registered.
      if (subscription_state_handler_ != nullptr) {
        // Do not use change handler after call to "OnChange()".
        // Due to recursive mutex, the handler could replace itself.
        // PTP-B-Socal-ProxyEvent_CallSubscriptionStateHandler
        this->logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Start: Subscription state change handler"; },
            static_cast<char const*>(__func__), __LINE__);
        subscription_state_handler_(notified_state);  // VCA_SOCAL_CALLING_STL_APIS

        if (new_subscription_state_handler_pair_.first) {
          subscription_state_handler_ = std::move(new_subscription_state_handler_pair_.second);
          new_subscription_state_handler_pair_.first = false;
        }
        this->logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "End: Subscription state change handler"; },
            static_cast<char const*>(__func__), __LINE__);
        // PTP-E-Socal-ProxyEvent_CallSubscriptionStateHandler
      }
    }
    // PTP-E-Socal-ProxyEvent_HandleEventSubscriptionStateUpdateSync
  }

  /*!
   * \brief Event receive handler currently in use (Can be set only from the callback thread (detached threads)).
   */
  ProxyEventReceiveHandler receive_handler_{nullptr};

  /*!
   * \brief Pair containing:
   *        - First: Flag to indicate receive_handler_ changed. It is set once Set/UnsetReceiveHandler is called, and
   *          reset once NotifySync is being called.
   *        - Second: Event receive handler for the next callback (Can be set only from user threads).
   */
  ReceiveHandlerPair new_receive_handler_pair_{false, nullptr};

  /*!
   * \brief Recursive mutex to protect concurrent modification of receive_handler_. A recursive mutex is used to avoid
   *        possible dead-locks caused by calls to other APIs by the receive handler. For example if the receive handler
   *        de-registers the handler by calling UnsetReceiveHandler().
   * \note  Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex receive_handler_lock_{};

  /*!
   * \brief Event subscription state change handler currently in use (Can be set only from the callback thread (reactor
   *        thread)).
   */
  ProxyEventSubscriptionHandler subscription_state_handler_{nullptr};

  /*!
   * \brief Pair containing: first: Flag to indicate subscription_state_handler_ change. It is set once Set/Unset-
   *        SubscriptionStateHandler is called, and reset once NotifySubscriptionStateUpdate is being called. second:
   *        Event subscription state change handler for the next callback (Can be set only from user threads).
   */
  SubscriptionHandlerPair new_subscription_state_handler_pair_{false, nullptr};

  /*!
   * \brief Latest state notified to the handler.
   */
  ::ara::com::SubscriptionState latest_notified_subscription_state_{::ara::com::SubscriptionState::kNotSubscribed};

  /*!
   * \brief Recursive mutex to protect concurrent modification of subscription_state_handler_. A recursive mutex is used
   *        to avoid possible dead-locks caused by calls to other APIs by the subscription state change handler. For
   *        example if the subscription state change handler de-registers the handler by calling
   *        UnsetSubscriptionStateHandler().
   * \note  Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex subscription_state_handler_lock_{};

  /*!
   * \brief An atomic boolean that indicates, if a receive handler is set. If there is no handler set, the value of this
   *        variable is 'false'. The variable is written by application threads or worker threads and read by the
   *        reactor thread.
   */
  std::atomic<bool> receive_handler_set_{false};

  /*!
   * \brief An atomic boolean that indicates, if a subscription state handler is set. If there is no handler set, the
   *        value of this variable is 'false'. The variable is written by application threads or worker threads and read
   *        by the reactor thread.
   */
  std::atomic<bool> subscription_state_handler_set_{false};
};

}  // namespace events
}  // namespace r20_11
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_THREAD_DRIVEN_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::r20_11::events::ProxyEvent::receive_handler_lock_
 * Used for protecting the concurrent modification of the current event receive_handler and for synchronizing the
 * unsubscription against the event receive handler invocation.
 *
 * \protects ::amsr::socal::r20_11::events::ProxyEvent::new_receive_handler_pair_
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::HandleEventNotificationSync
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::SetReceiveHandler
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::UnsetReceiveHandler
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::Unsubscribe
 * \exclude All other methods of a ProxyEvent object.
 * \length MEDIUM Updating callback function for the event receive handler or
 *                wait for the completion of ongoing event receive handler execution, if any.
 * \endexclusivearea
 */
/*!
 * \exclusivearea ::amsr::socal::r20_11::events::ProxyEvent::subscription_state_handler_lock_
 * Used for protecting the concurrent modification of the current event subscription state change handler.
 *
 * \protects ::amsr::socal::r20_11::events::ProxyEvent::new_subscription_state_handler_pair_
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::HandleEventSubscriptionStateUpdateSync
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::SetSubscriptionStateHandler
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::UnsetSubscriptionStateHandler
 * \usedin :amsr::socal::r20_11::events::ProxyEvent::Unsubscribe
 * \exclude All other methods of a ProxyEvent object.
 * \length MEDIUM Updating callback function for the event subscription state change handler.
 * \endexclusivearea
 */
// clang-format on
