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
/**        \file  thread_pool_task.h
 *        \brief  Defines a ThreadPoolTask class that represents a work unit which is executed by a ThreadPool.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_TASK_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/events/event_notification_functor.h"
#include "amsr/socal/internal/events/subscription_state_update_functor.h"
#include "amsr/socal/internal/find_service_handler_functor.h"
#include "amsr/socal/internal/methods/continuation_functor.h"
#include "amsr/socal/internal/methods/event_mode_method_executor_functor.h"
#include "ara/core/variant.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Represents a task with a generic callback that is executed by a ThreadPool worker unit.
 *
 * \unit Socal::ThreadPool::ThreadPool
 */
class ThreadPoolTask final {
 public:
  /*!
   * \brief Named alias for a key type to identify the creator of a thread-pool task.
   */
  using CreatorKey = void const*;

  /*!
   * \brief   Named alias for a callable type executed in the context of the thread-pool.
   * \details The callable type is ara::core::Variant which indicates that the callable can be of any of the listed
   *          functor types. The size of the callable is the size of the largest functor type from the Variant type
   *          list.
   */
  using Callable = ::ara::core::Variant<FindServiceHandlerFunctor, events::EventNotificationFunctor,
                                        events::SubscriptionStateUpdateFunctor, methods::ContinuationFunctor,
                                        methods::EventModeMethodExecutorFunctor>;

  /*!
   * \brief Constructor.
   * \param[in] task_creator_key  The key which represents the creator of this task. It is used to remove the tasks
   *                              later when needed. This key must not be null and must remain valid at least
   *                              until the lifetime of this ThreadPoolTask object.
   * \param[in] task_callable     The callable (functor object) associated with this task. It will be invoked in the
   *                              context of the thread-pool. The caller must ensure that a valid functor object is used
   *                              for creation of the task callable.
   * \pre         The given task creator key is not null.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires task_creator_key != nullptr; \endspec
   */
  explicit ThreadPoolTask(CreatorKey task_creator_key, Callable&& task_callable) noexcept;

  /*!
   * \brief Delete copy constructor.
   */
  ThreadPoolTask(ThreadPoolTask const&) noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ThreadPoolTask(ThreadPoolTask&&) noexcept = default;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(ThreadPoolTask const&) & noexcept -> ThreadPoolTask& = delete;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(ThreadPoolTask&&) & noexcept -> ThreadPoolTask& = delete;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~ThreadPoolTask() noexcept = default;

  /*!
   * \brief Gets the key representing the creator of this task.
   * \return The key representing the creator of this task.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto GetCreatorKey() const noexcept -> CreatorKey;

  /*!
   * \brief   Execute the task callable function.
   * \details Shall be called in a thread-pool context, based on the callable type.
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto operator()() noexcept -> void;

 private:
  /*!
   * \brief Key which represents the creator of this task.
   */
  CreatorKey task_creator_key_;

  /*!
   * \brief Executable functor of the task.
   */
  Callable task_callable_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOL_TASK_H_
