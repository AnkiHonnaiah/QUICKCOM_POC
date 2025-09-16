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
 *        \brief  public signal handling APIs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_SIGNAL_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_SIGNAL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
// VECTOR Next Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
#include "amsr/signal/internal/signal_notification_entry.h"

namespace amsr {
namespace signal {

/*!
 * \brief   SignalManager SupportedSignalInfo type
 * \details Passed as argument to the SignalCallback to allow more detailed logging.
 */
class SupportedSignalInfo final {
 public:
  /*!
   * \brief           Construct a SupportedSignalInfo object.
   * \param[in]       signal    SupportedSignal identifier.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SupportedSignalInfo(SupportedSignal signal) noexcept : supported_signal_{signal} {}

  /*!
   * \brief           Default copy constructor.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SupportedSignalInfo(SupportedSignalInfo const&) noexcept = default;

  /*!
   * \brief           Default move constructor.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SupportedSignalInfo(SupportedSignalInfo&&) noexcept = default;

  /*!
   * \brief           Destructor.
   *
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~SupportedSignalInfo() noexcept = default;

  /*!
   * \brief           Default assignment operator.
   * \return          Reference to this object.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SupportedSignalInfo& operator=(SupportedSignalInfo const&) & noexcept = default;

  /*!
   * \brief           Default move assignment operator.
   * \return          Reference to this object.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SupportedSignalInfo& operator=(SupportedSignalInfo&&) & noexcept = default;

  /*!
   * \brief           Comparison operator.
   * \param[in]       other   The other SupportedSignalInfo object to compare with.
   * \return          True if the supported signals are equal, otherwise false.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(SupportedSignalInfo const& other) const noexcept {
    return this->supported_signal_ == other.supported_signal_;
  }

  /*!
   * \brief           Get signal identifier.
   * \return          SupportedSignal identifier.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-SignalManagerUserCallback
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SupportedSignal GetIdentifier() const noexcept { return (supported_signal_); }

  /*!
   * \brief           Get signal name.
   * \return          amsr::core::String containing the signal name and optional description.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-SignalManagerUserCallback
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::String GetName() const noexcept;

 private:
  /*!
   * \brief  Signal identifier.
   */
  SupportedSignal supported_signal_;
};

/*!
 * \brief   SignalManager signal notification registrations type
 * \details Implements logic to populate and retrieve data from a container of signal notification registrations.
 */
class SignalNotificationRegistrations final {
 public:
  /*!
   * \brief       Type definition for the managed container.
   */
  // VECTOR Next Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
  using SignalNotificationRegistrationsType = std::map<SupportedSignal, internal::SignalNotificationRegistrationEntry>;

  /*!
   * \brief           Construct a SignalNotificationRegistrations object.
   * \context         SignalManager constructor
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SignalNotificationRegistrations() noexcept = default;

  /*!
   * \brief           Deleted copy constructor.
   */
  SignalNotificationRegistrations(SignalNotificationRegistrations const&) = delete;

  /*!
   * \brief           Default move constructor.
   *
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SignalNotificationRegistrations(SignalNotificationRegistrations&&) noexcept = default;

  /*!
   * \brief Destructor.
   *
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~SignalNotificationRegistrations() noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief           Deleted assignment operator.
   */
  SignalNotificationRegistrations& operator=(SignalNotificationRegistrations const&) = delete;

  /*!
   * \brief           Default move assignment operator.
   * \return          This SignalNotificationRegistrations object.
   *
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SignalNotificationRegistrations& operator=(SignalNotificationRegistrations&&) & noexcept =
      default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief           Register a new signal notification
   * \details         Any attempt to register a notification for an already registered signal will be ignored.
   * \param[in]       signal            Signal to be notified.
   * \param[in]       callback          User notification callback.
   * \param[in]       notification_type Signal type of notification.
   * \context         InitPhase
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  void RegisterNotification(SupportedSignal signal, SignalCallback&& callback,
                            SignalNotificationType notification_type) noexcept;

  /*!
   * \brief           Returns the signals that have been registered.
   * \return          array of SupportedSignals.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Array<internal::SignalIdentifier, kNumberOfSupportedSignals> GetRegisteredSignals() noexcept;

  /*!
   * \brief           Checks if the given signal has been registered or not.
   * \param[in]       signal    supported signal
   * \return          true if the signal has been registered, false otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsSignalRegistered(SupportedSignal signal) noexcept;

  /*!
   * \brief           Returns the callback associated to the passed signal.
   * \param[in]       signal    supported signal
   * \return          reference to the user provided callback.
   * \context         ANY
   * \pre             signal has been registered.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SignalCallback& GetCallback(SupportedSignal signal) noexcept;

  /*!
   * \brief           Returns the notification type associated to the passed signal.
   * \param[in]       signal    supported signal
   * \return          const reference to the SignalNotificationType value.
   * \context         ANY
   * \pre             signal has been registered.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  SignalNotificationType const& GetNotificationType(SupportedSignal signal) noexcept;

 private:
  /*!
   * \brief           Container for the notification registration entries.
   */
  SignalNotificationRegistrationsType registration_map_{};
};

/*!
 * \brief    Forward declaration of OS specific SignalManager implementation.
 */
namespace internal {
class SignalManager;
}
/*!
 * \brief    User Interface to manage SignalManager.
 *
 * \details
 * SignalManager allows handling of POSIX signals synchronously with other IO events in a Reactor1.
 * It triggers an event in a Reactor1 and calls a user specified callback every time a registered signal is
 * received by the process.
 *
 * The possible user actions are:
 *  - Creation (start signal handling) using SignalManagerInterface::Initialize().
 *  - Destruction (stop signal handling) using the returned unique_ptr.
 *
 * \vprivate Vector product internal API
 */
class SignalManagerInterface final {
 public:
  /*!
   * \brief         Creates and initializes the SignalManager.
   *
   * \details       Creates the unique SignalManger instance and allocates the needed resources to start
                    signal handling.
   * \param[in]     reactor               Reactor that the user wants to use to wait for signals. The reactor has
   *                                      to exist for the whole lifetime of this SignalManagerInterface object.
   * \param[in]     signal_notifications  Container that associates the signals that shall be handled with
   *                                      their signal callbacks and the configured notification type.
   *                                      The SignalManager takes ownership of the callbacks which may be called
   *                                      during Reactor1::HandleEvents() as long as the SignalManager is not
   *                                      destroyed. Actual signal_notifications parameter shall reside in the valid
   *                                      memory location.
   * \error         osabstraction::OsabErrc::kResource  The initialization failed because required
   *                                                    resources could not be allocated.
   * \context       InitPhase of the process
   * \pre           There is no other existing SignalManager instance.
   * \pre           All signals to be handled are blocked in ALL threads.
   * \pre           No signal handler was installed.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   * \trace         DSGN-Osab-SignalManagerCreation
   * \trace         DSGN-Osab-SignalManagerInitialization
   * \vprivate      Vector product internal API

   */
  static ::amsr::core::Result<std::unique_ptr<internal::SignalManager>> Initialize(
      osabstraction::io::reactor1::Reactor1Interface& reactor, SignalNotificationRegistrations&& signal_notifications);
};

/*!
 * \brief         Mask all possible signals in the currently executing thread.
 * \details       All signals except from: SIGILL, SIGFPE, SIGSEGV, SIGBUS, SIGABRT are masked.
 *                QNX and Linux ignore any attempt to mask SIGKILL and SIGSTOP, because they cannot
 *                be masked from the user.
 *                This function has to be called as soon as possible in the main thread, before
 *                any other thread is spawned.
 * \context       InitPhase, before additional thread creation.
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \threadsafe    FALSE
 * \steady        TRUE
 * \trace         DSGN-Osab-SignalMasking
 * \vprivate      Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
void MaskSignals() noexcept;

}  // namespace signal
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_SIGNAL_INTERFACE_H_
