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
/**        \file  event_mode_method_executor_functor.h
 *        \brief  Specialized Threadpool functor to handle method requests asynchronously.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_METHOD_EXECUTOR_FUNCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_METHOD_EXECUTOR_FUNCTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include "amsr/socal/internal/method_executor_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief Functor for handling method requests.
 *
 * \unit Socal::Skeleton::MethodExecutor::EventModeMethodExecutor
 */
class EventModeMethodExecutorFunctor final {
 public:
  /*!
   * \brief Alias for method executer interface.
   */
  using MethodExecutorInterface = ::amsr::socal::internal::MethodExecutorInterface;

  /*!
   * \brief Constructs the executor functor.
   * \param[in] executor  The executor instance. Must be valid until the ThreadPoolTask is executed/removed from the
   *                      threadpool.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit EventModeMethodExecutorFunctor(MethodExecutorInterface& executor) noexcept;

  /*!
   * \brief Delete copy constructor.
   */
  EventModeMethodExecutorFunctor(EventModeMethodExecutorFunctor const&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(EventModeMethodExecutorFunctor const&) & noexcept -> EventModeMethodExecutorFunctor& = delete;

  /*!
   * \brief Move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  EventModeMethodExecutorFunctor(EventModeMethodExecutorFunctor&&) noexcept = default;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(EventModeMethodExecutorFunctor&&) & noexcept -> EventModeMethodExecutorFunctor& = delete;

  /*!
   * \brief Destroys the EventModeMethodExecutorFunctor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~EventModeMethodExecutorFunctor() noexcept = default;

  /*!
   * \brief   Execute the next method request.
   * \details Called from the the Thread Pool Worker Thread.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void operator()() const noexcept;

 private:
  /*!
   * \brief The executor instance.
   */
  std::reference_wrapper<MethodExecutorInterface> executor_;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_METHOD_EXECUTOR_FUNCTOR_H_
