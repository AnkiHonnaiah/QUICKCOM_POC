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
/**        \file  polling_mode_future_executor.h
 *        \brief  Class used for execution of callbacks from ara::core::Future::then in polling mode.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_POLLING_MODE_FUTURE_EXECUTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_POLLING_MODE_FUTURE_EXECUTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/socal/internal/logging/ara_com_logger.h"
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
 * \brief Class implementing the ExecutorInterface for polling mode, to execute the callbacks registered using
 *        ara::core::Future::then() API. The callbacks are executed immediately.
 *
 * \unit Socal::Proxy::ProxyMethod::FutureExecutor::PollingModeFutureExecutor
 */
class PollingModeFutureExecutor final : public ::ara::core::internal::ExecutorInterface {
 public:
  /*!
   * \brief Type alias for the work item submitted to this executor.
   */
  using WorkItemType = ::vac::language::UniqueFunction<void()>;

  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  PollingModeFutureExecutor() noexcept = default;

  /*!
   * \brief Delete copy constructor.
   */
  PollingModeFutureExecutor(PollingModeFutureExecutor const&) noexcept = delete;

  /*!
   * \brief Delete move constructor.
   */
  PollingModeFutureExecutor(PollingModeFutureExecutor&&) noexcept = delete;

  /*!
   * \brief Delete copy assignment.
   */
  PollingModeFutureExecutor& operator=(PollingModeFutureExecutor const&) & noexcept = delete;

  /*!
   * \brief Delete move assignment.
   */
  PollingModeFutureExecutor& operator=(PollingModeFutureExecutor&&) & noexcept = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  ~PollingModeFutureExecutor() noexcept final = default;

  /*!
   * \brief   Submit a work item which triggers an abort of the process.
   * \details The usage of ara::core::Future::then() callbacks is prohibited in polling mode, to prevent potential
   *          deadlock scenarios. Future::then() callbacks should be used only in event-driven mode.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  void SubmitWork(WorkItemType&&) noexcept final;

 private:
  /*!
   * \brief Logger for this component.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "PollingModeFutureExecutor"_sv};
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_POLLING_MODE_FUTURE_EXECUTOR_H_
