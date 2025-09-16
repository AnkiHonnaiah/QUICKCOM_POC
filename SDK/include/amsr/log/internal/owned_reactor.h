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
 *      \brief      Contains the declarations for the owned reactor.
 *      \details
 *
 *      \unit 	    LogAndTrace::Common::ReactorHandling::OwnedReactor
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OWNED_REACTOR_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OWNED_REACTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <cstdint>
#include "amsr/core/string.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/borrowed_reactor.h"
#include "amsr/log/internal/log_error_domain.h"
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
 * \brief Represents a reactor that runs on a separate thread.
 */
class OwnedReactor final {
  /*! \brief The reactor that runs the complete ipc & ip communication. */
  osabstraction::io::reactor1::Reactor1 reactor_;
  /*! \brief Can be set to stop the reactor from running. */
  std::atomic<bool> thread_done_{false};
  /*! \brief The thread on which the reactor is running. */
  Result<thread::Thread> reactor_thread_{
      Err(LogErrc::kNotInitialized, "Reactor thread is not initialized")};  // IWYU pragma: no_forward_declare
  /*! \brief Marks if the thread has been started. */
  std::atomic<bool> has_started_{false};
  /*! \brief The associated TimerManager. */
  ::amsr::timer::TimerManager timer_manager_;

 public:
  /*!
   * \brief       Creates an OwnedReactor construction token.
   *
   * \details     If Preconstruct() returns an error, the user message describes which system call failed and the
   *              support data is set to the POSIX error code of the failed system call.
   *
   * \param[in]   num_callbacks Number of callbacks that the reactor should be able to handle. Valid ange: 0 to 8191.
   *
   * \return      Created ConstructionToken.
   *
   * \error       osabstraction::OsabErrc::kResource    Required resources could not be allocated.
   *
   * \context     ANY
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   */
  static auto Preconstruct(std::uint16_t max_num_file_descriptors) noexcept
      -> Result<osabstraction::io::reactor1::Reactor1::ConstructionToken>;

  /*!
   * \brief       Constructs an OwnedReactor.
   * \param[in]   token    Construction token used to create the Reactor.
   *
   * \context     ANY
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \vprivate    Vector product internal API
   */
  explicit OwnedReactor(osabstraction::io::reactor1::Reactor1::ConstructionToken&& token) noexcept;

  /*!
   * \brief       Constructs an OwnedReactor.
   * \param[in]   num_callbacks     Number of callbacks that the reactor should be able to handle. Valid range: 0 to
   *                                8191.
   * \details Will terminate in case the required system ressources cannot be allocated.
   * \context     ANY
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   */
  explicit OwnedReactor(std::uint16_t max_num_file_descriptors) noexcept;

  /*! \brief Deleted move constructor. */
  OwnedReactor(OwnedReactor&&) noexcept = delete;
  /*! \brief Deleted copy constructor. */
  OwnedReactor(OwnedReactor const&) noexcept = delete;
  /*! \brief Deleted move assignment. */
  auto operator=(OwnedReactor&&) & noexcept -> OwnedReactor& = delete;
  /*! \brief Deleted copy assignment. */
  auto operator=(OwnedReactor const&) & noexcept -> OwnedReactor& = delete;

  /*!
   * \brief Destructor that stops the reactor from running.
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
  ~OwnedReactor() noexcept;

  /*!
   * \brief Returns a mutable reference to the underlying reactor.
   *
   * \return a mutable reference to the underlying reactor.
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
  auto Get() noexcept -> osabstraction::io::reactor1::Reactor1Interface&;

  /*!
   * \brief Start the reactor.
   *
   * \param thread_name The name of the thread to use.
   *
   * \pre         The reactor has not started yet.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \context     ANY
   */
  void RunAsync(core::StringView thread_name) noexcept;

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
   * \synchronous TRUE
   * \context     ANY
   */
  void RunOnce(std::chrono::nanoseconds timeout = std::chrono::nanoseconds{0}) noexcept;

  /*!
   * \brief Stop the reactor.
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
   * \brief Get the TimerManager for this reactor.
   *
   * \return A reference to the TimerManager.
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
  auto GetTimerManager() & noexcept -> ::amsr::timer::TimerManager&;

  /*!
   * \brief Get the internally handled reactor as borrowed reactor.
   *
   * \return the borrowed reactor.
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

 private:
  /*!
   * \brief Checks if the thread is still running.
   *
   * \return true if still running, else false.
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
  auto IsRunning() const noexcept -> bool { return this->reactor_thread_.HasValue(); }

  /*!
   * \brief Run the reactor once.
   *
   * \param reactor The reactor to run.
   * \param timeout The optional timeout for the reactor run.
   *
   * \pre         The reactor has not started yet.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  static void RunOnceInternal(BorrowedReactor reactor, std::chrono::nanoseconds timeout) noexcept;
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_OWNED_REACTOR_H_
