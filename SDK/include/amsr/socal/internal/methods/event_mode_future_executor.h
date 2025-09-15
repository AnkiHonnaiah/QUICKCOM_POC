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
/**        \file  event_mode_future_executor.h
 *        \brief  Class used for execution of callbacks from ara::core::Future::then in event-driven mode.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_FUTURE_EXECUTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_FUTURE_EXECUTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/thread_pools_manager_interface.h"
#include "ara/core/future.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using namespace ::vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Class implementing the ExecutorInterface for event-driven mode, to execute the callbacks registered using
 *        ara::core::Future::then() API. The callbacks are executed by worker thread(s) of the continuation thread-pool.
 *
 * \unit Socal::Proxy::ProxyMethod::FutureExecutor::EventModeFutureExecutor
 */
class EventModeFutureExecutor final : public ::ara::core::internal::ExecutorInterface {
 public:
  /*!
   * \brief Type alias for the work item submitted to this executor.
   */
  using WorkItemType = ::vac::language::UniqueFunction<void()>;
  /*!
   * \brief Type alias for singleton instance of ThreadPoolsManager.
   */
  using ThreadPoolsManagerInterfaceAccessType =
      ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface>;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOCAL_AutosarC++17_10-M7.1.2_pass_by_rvalue_instead_of_lvalue_ref
  /*!
   * \brief Construct a new EventModeFutureExecutor object.
   * \param[in] thread_pools_manager  Singleton Access of the ThreadPoolsManagerInterface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit EventModeFutureExecutor(ThreadPoolsManagerInterfaceAccessType&& thread_pools_manager) noexcept;

  /*!
   * \brief Delete default constructor.
   */
  EventModeFutureExecutor() noexcept = delete;

  /*!
   * \brief Delete copy constructor.
   */
  EventModeFutureExecutor(EventModeFutureExecutor const&) noexcept = delete;

  /*!
   * \brief Delete move constructor.
   */
  EventModeFutureExecutor(EventModeFutureExecutor&&) noexcept = delete;

  /*!
   * \brief Delete copy assignment.
   */
  EventModeFutureExecutor& operator=(EventModeFutureExecutor const&) & noexcept = delete;

  /*!
   * \brief Delete move assignment.
   */
  EventModeFutureExecutor& operator=(EventModeFutureExecutor&&) & noexcept = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  ~EventModeFutureExecutor() noexcept final = default;

  /*!
   * \brief Submit a work item which will be executed in the continuation thread context.
   * \param work_item  The callback function which shall be executed.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  void SubmitWork(WorkItemType&& work_item) noexcept final;

 private:
  /*!
   * \brief Singleton access of ThreadPoolsManagerInterface.
   */
  ThreadPoolsManagerInterfaceAccessType thread_pools_manager_;

  /*!
   * \brief Logger for this component.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "EventModeFutureExecutor"_sv};
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_EVENT_MODE_FUTURE_EXECUTOR_H_
