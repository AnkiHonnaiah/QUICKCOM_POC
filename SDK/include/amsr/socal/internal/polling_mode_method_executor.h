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
/**        \file  polling_mode_method_executor.h
 *        \brief  Concrete implementation of the method executor interface for polling mode.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_MODE_METHOD_EXECUTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_MODE_METHOD_EXECUTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <deque>
#include <memory>
#include <mutex>
#include <queue>

#include "amsr/core/future.h"
#include "amsr/socal/internal/method_executor_interface.h"
#include "amsr/socal/internal/task.h"
#include "ara/core/memory_resource.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class implementing the MethodExecutorInterface for polling mode.
 *
 * \unit Socal::Skeleton::MethodExecutor::PollingModeMethodExecutor
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class PollingModeMethodExecutor final : public MethodExecutorInterface {
 public:
  /*!
   * \brief Constructs a new PollingModeMethodExecutor object.
   * \param[in] max_queue_size  The max number of method requests that can be queued. Value '0' indicates unlimited
   *                            queue size.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit PollingModeMethodExecutor(std::size_t const max_queue_size) noexcept;

 private:
  /*!
   * \brief Type alias for Task pointer.
   */
  using TaskUniquePtr = amsr::UniquePtr<Task>;

  /*!
   * \brief Enqueues an incoming method request with its parameters.
   * \param[in] request  This object contains all the information necessary to:
   *                     1. Call the concrete method in the frontend; the binding is aware of the method to call and
   *                        stores this information in the functor.
   *                     2. Get the return value with a given type from the future, because the return type is also
   *                        known to the backend.
   *                     3. Call the response path in the binding-related part with the information provided within this
   *                        object.
   * \return true if a request was enqueued, false if not.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against ExecuteNextMethodCall API for same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires request != nullptr; \endspec
   */
  auto HandleMethodRequest(TaskUniquePtr&& request) noexcept -> bool final;

  /*!
   * \brief Execute cleanup of thread-pool tasks which are created by the method executor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_VoidFunctionHasNoExternalSideEffect
  void Cleanup() noexcept final;

  /*!
   * \brief Fetches the next method call from the method queue and executes it.
   * \return The future contains a boolean value of true, if there is a method request pending in the global request
   *         queue, which is being processed, false if there was no method request pending. The promise's value is set
   *         as soon as the request has been processed.
   * \pre         This function must only be called on a skeleton that uses "kPoll" method call processing mode.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against HandleMethodRequest API for same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \spec requires true; \endspec
   */
  auto ExecuteNextMethodCall() noexcept -> ::amsr::core::Future<bool> final;

  /*!
   * \brief Type alias for the buffer that gets filled on incoming method requests.
   */
  using RequestBufferInPollingMode =
      std::queue<TaskUniquePtr, std::deque<TaskUniquePtr, ::ara::core::PolymorphicAllocator<TaskUniquePtr>>>;
  /*!
   * \brief Skeleton request buffer for all incoming method requests if kPoll is active.
   */
  RequestBufferInPollingMode request_buffer_{};  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \brief Mutex for protection of request_buffer_.
   */
  std::mutex request_buffer_lock_{};

  /*!
   * \brief The max number of method requests that can be queued.
   */
  std::size_t const max_queue_size_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_MODE_METHOD_EXECUTOR_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::PollingModeMethodExecutor::request_buffer_lock_
 * Used for protecting the request buffer for incoming method requests
 *
 * \protects ::amsr::socal::internal::PollingModeMethodExecutor::request_buffer_
 * \usedin ::amsr::socal::internal::PollingModeMethodExecutor::HandleMethodRequest
 * \usedin ::amsr::socal::internal::PollingModeMethodExecutor::ExecuteNextMethodCall
 * \exclude All other methods of a PollingModeMethodExecutor object.
 * \length SHORT adds or removes an element from the request queue
 * \endexclusivearea
 */
// clang-format on
