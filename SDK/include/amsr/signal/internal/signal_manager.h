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
/**        \file
 *        \brief  SignalManager Linux declaration.
 *        \unit   osabstraction::Signal_Linux
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_MANAGER_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <unordered_set>
#include "amsr/core/result.h"
#include "amsr/signal/internal/signal_definitions.h"
#include "amsr/signal/signal_interface.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace signal {
namespace internal {

/*!
 * \brief    Implementation of the SignalManager for Linux.
 *
 * \details
 * Allows handling of POSIX signals synchronously with other IO events in a Reactor1.
 * The SignalManager triggers an event in a Reactor1 and calls a user specified callback every time a handled signal
 * is received by the process.
 *
 * \vprivate Vector product internal API
 */
class SignalManager final {
 public:
  /*!
   * \brief         Creates and initializes the SignalManager.
   *
   * \details       Creates the unique SignalManger instance and allocates the needed resources to start
                    signal handling.
   * \param[in]     reactor           Reactor that the user wants to use to wait for signals.The reactor has
   *                                  to exist for the whole lifetime of this SignalManager object.
   * \param[in]     signal_notifications  Container that associates the signals that shall be handled with
   *                                  their signal callbacks and the configured notification type.
   *                                  The SignalManager takes ownership of the callbacks which may be called
   *                                  during Reactor1::HandleEvents() as long as the SignalManager is not destroyed.
   * \error         osabstraction::OsabErrc::kResource  The initialization failed because required
   *                                                     resources could not be allocated.
   * \context       InitPhase of the process
   * \pre           There is no other existing SignalManager instance.
   * \pre           All signals to be handled are blocked in ALL threads.
   * \pre           No signal handler was installed.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   * \trace         DSGN-Osab-SignalManagerInitialization
   * \trace         DSGN-Osab-SignalManagerLinux
   * \vprivate      Vector product internal API
   */
  static ::amsr::core::Result<std::unique_ptr<SignalManager>> Initialize(
      osabstraction::io::reactor1::Reactor1Interface& reactor,
      SignalNotificationRegistrations&& signal_notifications) noexcept;

  /*!
   * \brief         Deallocates the acquired resources and stops signal handling.
   * \details       No signals will be reported anymore and signals send to the process after this will stay
   *                pending because they are still blocked in all threads.
   *                The user has to avoid destructing the SignalManager object while the internal reactor
   *                callback is executing. This can be done by using a separate software event reactor job, or by
   *                ensuring per design that SignalManager is destroyed in the same thread where Reactor::HandleEvents()
   *                is called.
   * \context       TerminationPhase of the process
   * \pre           Should not be executed in the context of the user callback.
   *                Reactor of this object should not execute this internal reactor callback while the destructor is
   *                running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                execution of the destructor.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   * \trace         DSGN-Osab-SignalManagerDeallocation
   * \vprivate      Vector product internal API
   */
  ~SignalManager() noexcept;

  SignalManager(SignalManager const&) = delete;
  SignalManager& operator=(SignalManager const&) = delete;
  SignalManager(SignalManager&& other) = delete;
  SignalManager& operator=(SignalManager&& other) = delete;

 private:
  /*!
   * \brief         Private constructor.
   * \details       Called in context of Initialize() to restrict SignalManager creation attempts.
   * \context       InitPhase of the process
   * \pre           -
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   * \trace         DSGN-Osab-SignalManagerCreation
   * \vprivate      Vector component internal API
   */
  // VECTOR Next Line AutosarC++17_10-A15.4.2: MD_OSA_A15.4.2_SignalManagerUnorderedSetAllocation
  SignalManager() noexcept { already_notified_signals_.reserve(kNumberOfSupportedSignals); }

  /*!
   * \brief         Installs a dummy signal handler for a signal.
   * \details
   * The dummy signal handler is used to change the signal disposition from the default disposition because POSIX
   * allows signals with the default disposition SIG_IGN to be dropped even if they are blocked. It shall never be
   * called and aborts when it is called.
   * \param[in]     signal  Signal identifier for which the handler shall be installed.
   * \context       Initialize()
   * \pre           -
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   */
  static void InstallDummySignalHandler(internal::SignalIdentifier signal);

  /*!
   * \brief         Uninstalls a dummy signal handler for a signal.
   * \details
   * Resets the signal disposition to SIG_DFL.
   * \context       ANY
   * \pre           -
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   */
  void UninstallDummySignalHandlers();

  /*!
   * \brief         Closes the signalfd file descriptor if it is open.
   * \context       ANY
   * \pre           -
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   */
  void CloseSignalFd();

  /*!
   * \brief         Registers a callback for the signalfd file descriptor to Reactor1.
   * \return        true if the registration was successful, false otherwise.
   * \context       Initialize()
   * \pre           -
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   * \trace         DSGN-Osab-SignalManagerGeneralDesign
   */
  bool RegisterReactorCallback();

  /*!
   * \brief    Pointer to the Reactor used by the signal manager.
   */
  osabstraction::io::reactor1::Reactor1Interface* reactor_{nullptr};

  /*!
   * \brief    CallbackHandle of the registered Reactor1 callback.
   */
  osabstraction::io::reactor1::CallbackHandle reactor_callback_{osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*!
   * \brief    signalfd file descriptor.
   */
  osabstraction::io::FileDescriptor signal_fd_{};

  /*!
   * \brief    Container for the registered signal notifications.
   */
  SignalNotificationRegistrations signal_notifications_{};

  /*!
   * \brief    Container for the already notified signals.
   */
  std::unordered_set<internal::SignalIdentifier> already_notified_signals_{};

  /*!
   * \brief    Creation attempts counter.
   */
  static std::uint8_t creation_attempts_counter_;
};

}  // namespace internal
}  // namespace signal
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_MANAGER_H_
