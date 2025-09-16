/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file
 *      \brief      Contains the declarations for the reactor.
 *      \details
 *
 *      \unit       LogAndTrace::Common::ReactorHandling::Reactor
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <cstdint>
#include "amsr/core/string.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/reactor_fwd.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/utils.h"
#include "amsr/thread/thread.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace log {
namespace internal {

/*!
 * \brief Access to the string literal operator.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;
/*!
 * \brief Thread abstraction
 */
// VCA_LOGAPI_MOLE_1298
class Reactor final {
  /*!
   * \brief The reactor internalization
   */
  Box<OwnedReactor> reactor_;

 public:
  /*!
   * \brief Creates a default reactor using an owned reactor
   *
   * \param max_num_file_descriptors The maximum number of supported file descriptors
   *
   * \context     ANY
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  explicit Reactor(std::uint16_t max_num_file_descriptors) noexcept;

  /*!
   * \brief Start the reactor
   *
   * \param thread_name The name of the thread to use.
   *
   * \pre         The reactor has not started yet.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \context     ANY
   */
  void RunAsync(core::StringView thread_name = "vLogApiReactor"_sv) noexcept;

  /*!
   * \brief       Start the reactor synchronously.
   *
   * \pre         The reactor has not started yet.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  void RunSync() noexcept;

  /*!
   * \brief Run the reactor once.
   *
   * \param timeout The optional timeout for the reactor run.
   *
   * \pre         The reactor has not started yet.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \context     ANY
   */
  void RunOnce(std::chrono::nanoseconds timeout = std::chrono::nanoseconds{0}) noexcept;

  /*!
   * \brief Stop the reactor
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  void Stop() noexcept;

  /*!
   * \brief Get the internally handled reactor as borrowed reactor
   *
   * \return the borrowed reactor
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  auto AsBorrowed() noexcept -> BorrowedReactor;

  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__Reactor, ReactorAsBorrowedTest);
  FRIEND_TEST(UT__Reactor__Async, BorrowedReactorNullTimerTest);
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_H_
