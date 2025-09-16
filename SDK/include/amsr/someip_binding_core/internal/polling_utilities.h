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
/**        \file  amsr/someip_binding_core/internal/polling_utilities.h
 *        \brief  SomeIpBinding polling utility function
 *        \unit   SomeIpBinding::SomeIpBindingCore::Utilities
 *
 *      \details  This class provides the function to poll the reactor periodically.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_POLLING_UTILITIES_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_POLLING_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <functional>
#include <utility>

#include "amsr/someip_binding_core/internal/constants.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/thread/this_thread.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief  This class provides the function to poll the reactor periodically.
 */
class PollingUtilities final {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  /*!
   * \brief Calls the HandleEvent of the reactor until the response timeout occurs or the given
   *        predicate evaluates to true.
   * \param[in] safe_ipc_polling  Contains reactor polling function to be called while waiting for
   *                              a command response (only in Polling Mode).
   * \param[in] wait_predicate    A predicate which shall return true to signal that waiting shall be stopped.
   * \param[in] response_timeout  Max time to wait for the predicate.
   * \return True if the connection is established,otherwise false.
   *
   * \pre Cannot be used within the callstack of ara::com::runtime::ProcessPolling. Particularly not in a Timer as this
   * will lead to a deadlock as ProcessPolling is not reentrant.
   * \context Init | App
   * \reentrant FALSE
   * \steady    TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a boolean to store the predicate value.
   * - Enter the waiting loop:
   *   - Record the time before entering the reactor.
   *   - Handle events in poll mode and sleep for default time period.
   *   - Record the time when the reactor returned.
   *   - Calculate the total time the reactor was blocked inside the polling function.
   *   - Adjust the total timeout according to the time the reactor blocked inside the polling function.
   *   - Update the predicate value.
   *   - Continue the loop until the timeout expired or predicate becomes true.
   * \endinternal
   */
  static bool WaitForInPollingMode(std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling,
                                   vac::language::UniqueFunction<bool()> wait_predicate,
                                   std::chrono::steady_clock::duration const response_timeout) noexcept {
    std::chrono::steady_clock::duration wait_time{0};
    bool wait_predicate_result{false};
    while ((!wait_predicate_result) && wait_time < response_timeout) {
      // Record the time before we enter the reactor
      std::chrono::steady_clock::time_point const before_time_point{std::chrono::steady_clock::now()};

      // Let the reactor handle some events for a fixed time which does not exceed the response timeout
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      safe_ipc_polling->Poll();
      // Wait between two polls while receiving a response from daemon.
      amsr::thread::SleepFor(kDefaultResponseTriggerPeriod);

      // Record the time when the reactor returned
      std::chrono::steady_clock::time_point const after_time_point{std::chrono::steady_clock::now()};
      wait_time += after_time_point - before_time_point;

      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
      wait_predicate_result = wait_predicate();
    }
    return wait_predicate_result;
  }
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_POLLING_UTILITIES_H_
