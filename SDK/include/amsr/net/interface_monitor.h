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
/*!       \file   interface_monitor.h
 *        \brief  Class to represent the interfaces monitor.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_MONITOR_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_MONITOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <utility>
#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/net/interface.h"
#include "amsr/net/internal/specific_types.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/native_types.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace net {

/*!
 * \brief           InterfaceMonitor event enum
 * \vprivate        Vector product internal API
 */
enum class InterfaceMonitorEvent : std::uint8_t {
  /*!
   * \brief    The administrative and operational state of the interface is set.
   */
  kUp = 0x00,

  /*!
   * \brief    The administrative or operational state of the interface is NOT set.
   */
  kDown = 0x01,

  /*!
   * \brief    New IP address was added to the interface.
   */
  kNewAddr = 0x02,

  /*!
   * \brief    An existing IP address was removed from the interface.
   */
  kDelAddr = 0x03,

  /*!
   * \brief   The network interface status change cannot be mapped to any known event.
   */
  kUnknown = 0x99,
};

/*!
 * \brief             Information about the error that occurred in InterfaceMonitor.
 */
struct InterfaceMonitorError {
  /*!
   * \brief           InterfaceMonitor event that identifies in which context the error has occurred.
   */
  InterfaceMonitorEvent interface_event;

  /*!
   * \brief           Error code that identifies the type of the occurred error.
   */
  ::amsr::core::ErrorCode error_code;
};

/*!
 * \brief             Represents a configuration of user callbacks.
 */
class UserCallbackConfig final {
 public:
  /*!
   * \brief           Informs the user that an InterfaceMonitorEvent::kUp event has occurred.
   *
   * \details         InterfaceMonitorEvent::kUp is defined to be true if both the administrative and the
   *                  operational states of the network interface are up.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \context         InterfaceMonitor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  using StatusUpCallback = vac::language::UniqueFunction<void()>;

  /*!
   * \brief           Informs the user that an InterfaceMonitorEvent::kDown event has occurred.
   *
   * \details         All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \context         InterfaceMonitor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  using StatusDownCallback = vac::language::UniqueFunction<void()>;

  /*!
   * \brief           Informs the user that an InterfaceMonitorEvent::kNewAddr event has occurred.
   *
   * \details         All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       ip::Address IP address that was added to the network interface.
   *
   * \context         InterfaceMonitor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  using NewAddressCallback = vac::language::UniqueFunction<void(ip::Address&&)>;

  /*!
   * \brief           Informs the user that an InterfaceMonitorEvent::kDelAddr event has occurred.
   *
   * \details         All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       ip::Address IP address that was removed from the network interface.
   *
   * \context         InterfaceMonitor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  using DeleteAddressCallback = vac::language::UniqueFunction<void(ip::Address&&)>;

  /*!
   * \brief           Informs the user that an error has occurred while monitoring the network interface.
   *
   * \details         All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object. All the errors listed below are only returned by the callback if the
   *                  ErrorCallback is set by the user. The default reaction to all these errors is to abort.
   *
   * \param[in]       InterfaceMonitorError   information about the occurred error.
   *
   * \note            For InterfaceMonitorEvent::kUp events:
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges to
   *                                                                    extract the interface status.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources to extract the
   *                                                                    interface status.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The monitored interface is no longer available
   *
   * \note            For InterfaceMonitorEvent::kNewAddr/kDelAddr events:
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected internal state was detected.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Unsupported address family output by OS.
   *
   * \note            For InterfaceMonitorEvent::kUnknown events:
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \note            No errors for InterfaceMonitorEvent::kDown events.
   *
   * \context         InterfaceMonitor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  using ErrorCallback = vac::language::UniqueFunction<void(InterfaceMonitorError const&)>;

  /*!
   * \brief           Default constructor for UserCallbackConfig.
   *
   * \details         Initialize the callbacks with default callbacks.
   *                  The user can set individual callbacks using the specific setter functions.
   *                  The default implementation of ErrorCallback aborts with the user-defined error message extracted
   *                  from the InterfaceMonitorError parameter.
   *                  The other default callbacks are empty.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_EmptyVoidFunctionHasNoSideEffect
  UserCallbackConfig()
      : status_up_callback_{[]() {}},
        status_down_callback_{[]() {}},
        new_address_callback_{// VECTOR Next Line AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_ReferenceMayBeModifiedInCallback
                              [](ip::Address&&) {}},
        // VECTOR Next Line AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_ReferenceMayBeModifiedInCallback
        delete_address_callback_{[](ip::Address&&) {}},
        error_callback_{[](InterfaceMonitorError const& error) {
          ::amsr::core::Abort(&(error.error_code.UserMessage().front()));
        }} {}

  /*!
   * \brief           Default destructor
   */
  ~UserCallbackConfig() noexcept = default;

  /*!
   * \brief           Deleted copy constructor. The stored callbacks can't be copied since
   *                  vac::language::UniqueFunction is not copyable.
   */
  UserCallbackConfig(UserCallbackConfig const&) = delete;

  /*!
   * \brief           Default move constructor
   */
  UserCallbackConfig(UserCallbackConfig&&) noexcept = default;

  /*!
   * \brief           Deleted copy assignment operator. The stored callbacks can't be copied since
   *                  vac::language::UniqueFunction is not copyable.
   */
  auto operator=(UserCallbackConfig const&) -> UserCallbackConfig& = delete;

  /*!
   * \brief           Default move assignment operator.
   */
  auto operator=(UserCallbackConfig&&) & noexcept -> UserCallbackConfig& = default;

  /*!
   * \brief           Sets the StatusUpCallback
   *
   * \details         InterfaceMonitorEvent::kUp is defined as the administrative and operational states of the
   *                  network interface are up.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       status_up_callback    Callback, the user wants to be called if the status of the monitored
   *                  interface changes from DOWN to UP.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  void SetStatusUpCallback(StatusUpCallback&& status_up_callback) noexcept {
    status_up_callback_ = std::move(status_up_callback);
  }

  /*!
   * \brief           Sets the StatusDownCallback
   *
   * \details         This callback will be called when an InterfaceMonitorEvent::kDown event occurs.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       status_down_callback  Callback, the user wants to be called if the status of the monitored
   *                  interface changes from UP to DOWN.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  void SetStatusDownCallback(StatusDownCallback&& status_down_callback) noexcept {
    status_down_callback_ = std::move(status_down_callback);
  }

  /*!
   * \brief           Sets the NewAddressCallback
   *
   * \details         This callback will be called when an InterfaceMonitorEvent::kNewAddr event occurs.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       new_address_callback   Callback, the user wants to be called if a new IP address has been added to
   *                                         the monitored interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  void SetNewAddressCallback(NewAddressCallback&& new_address_callback) noexcept {
    new_address_callback_ = std::move(new_address_callback);
  }

  /*!
   * \brief           Sets the DeleteAddressCallback
   *
   * \details         This callback will be called when an InterfaceMonitorEvent::kNewAddr event occurs.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       delete_address_callback    Callback, the user wants to be called if an IP address is deleted from
   *                  the monitored interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  void SetDeleteAddressCallback(DeleteAddressCallback&& delete_address_callback) noexcept {
    delete_address_callback_ = std::move(delete_address_callback);
  }

  /*!
   * \brief           Sets the ErrorCallback
   *
   * \details         This callback will be called when an error occurs while monitoring the network interface.
   *                  All captures and references of the user-defined callback must exist throughout the lifetime of
   *                  InterfaceMonitor object.
   *
   * \param[in]       error_callback    Callback, the user wants to be called if an error occurs while monitoring the
   *                  network interface. Only errors that cannot be handled internally are reported to the user.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  void SetErrorCallback(ErrorCallback&& error_callback) noexcept { error_callback_ = std::move(error_callback); }

  /*!
   * \brief           Calls the StatusUpCallback
   *
   * \context         Reactor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_InterfaceMonitorCallbackMayHaveSideEffects
  inline void CallStatusUpCallback() noexcept { status_up_callback_(); }

  /*!
   * \brief           Calls the StatusDownCallback
   *
   * \context         Reactor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_InterfaceMonitorCallbackMayHaveSideEffects
  inline void CallStatusDownCallback() noexcept { status_down_callback_(); }

  /*!
   * \brief           Calls the NewAddressCallback
   *
   * \param[in]       ip_address     Address that was added to the network interface.
   *
   * \context         Reactor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_InterfaceMonitorCallbackMayHaveSideEffects
  inline void CallNewAddressCallback(ip::Address&& ip_address) noexcept {
    new_address_callback_(std::move(ip_address));
  }

  /*!
   * \brief           Calls the DeleteAddressCallback
   *
   * \param[in]       ip_address     Address that was removed from the network interface.
   *
   * \context         Reactor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_InterfaceMonitorCallbackMayHaveSideEffects
  inline void CallDeleteAddressCallback(ip::Address&& ip_address) noexcept {
    delete_address_callback_(std::move(ip_address));
  }

  /*!
   * \brief           Calls the ErrorCallback
   *
   * \param[in]       interface_monitor_error   Information about the occurred error.
   *
   * \context         Reactor
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component product API
   */
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  inline void CallErrorCallback(InterfaceMonitorError const& interface_monitor_error) noexcept {
    error_callback_(interface_monitor_error);
  }

 private:
  /*!
   * \brief           Callback that notifies the user that the status of the monitored interface changed from DOWN to
   *                  UP.
   */
  StatusUpCallback status_up_callback_;

  /*!
   * \brief           Callback that notifies the user that the status of the monitored interface changed from UP to
   *                  DOWN.
   */
  StatusDownCallback status_down_callback_;

  /*!
   * \brief           Callback that notifies the user that a new IP address was added to the monitored interface.
   */
  NewAddressCallback new_address_callback_;

  /*!
   * \brief           Callback that notifies the IP address has been removed from the monitored interface.
   */
  DeleteAddressCallback delete_address_callback_;

  /*!
   * \brief           Callback that notifies that an error has occurred during monitoring of a network interface.
   */
  ErrorCallback error_callback_;
};

/*!
 * \brief             Represents a network interface monitor.
 *
 * \details           User should drive the reactor's HandleEvents() frequently enough, so the user callbacks will
 *                    reflect the contemporary interface state.
 */
class InterfaceMonitor final {
 public:
  /*!
   * \brief           Creates an InterfaceMonitor object
   *
   * \details         The UserCallbackConfig must be already initialized and will be moved to the created object.
   *
   *
   * \param[in]       interface       Interface the user wants to monitor.
   *
   * \param[in]       reactor         Reactor to monitor the changes occurring in the monitored network interface.
   *                                  The reactor has to exist for the whole lifetime of this InterfaceMonitor object.
   *
   * \param[in]       user_callback_config Container that holds the user-defined callbacks. It is moved to this
   *                                      InterfaceMonitor object.
   *
   * \error           osabstraction::OsabErrc::kResource    Not enough system resources to open the socket.
   *                                                        Insufficient resources to register the internal socket to
   *                                                        the reactor. No memory to register the internal callback to
   *                                                        the reactor, or the system limit is reached.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create InterfaceMonitor.
   *
   * \error           osabstraction::OsabErrc::kUnexpected  InterfaceMonitor class has encountered an unrecoverable
   *                                                        error. User should log the error and abort. The internal
   *                                                        callback has been already registered. Waiting for read
   *                                                        events is impossible for the internal file descriptor.
   *                                                        Unexpected error that is mapped to no category.
   *                                                        (QNX) Can't set route msgs filtering sockopt if
   *                                                        filtering route msgs is supported.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError (Linux) can't bind to the socket address.
   *
   * \return          The created and initialized InterfaceMonitor object.
   *
   * \pre             The reactor pointer must point to a valid reactor object.
   *
   *
   *
   * \reentrant       FALSE (this includes but is not limited to that there is no thread safety against a concurrent
   *                         call to HandleEvents() of the passed reactor)
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector product internal API
   */
  static auto MakeInterfaceMonitor(Interface const& interface, osabstraction::io::reactor1::Reactor1Interface& reactor,
                                   UserCallbackConfig&& user_callback_config) noexcept
      -> ::amsr::core::Result<InterfaceMonitor>;

  /*!
   * \brief           Deleted copy constructor. From the user's point of view, it makes no sense to have more than one
   *                  InterfaceMonitor monitoring the same Interface. Both InterfaceMonitors would read from the same
   *                  socket and this is prohibited.
   */
  InterfaceMonitor(InterfaceMonitor const&) = delete;

  /*!
   * \brief           Move constructs the InterfaceMonitor
   *
   * \details         Transfers the ownership of the other reactor_callback_handle_ to this entity.
   *                  Updates the reactor callback target to the internal callback of this entity.
   *                  The moved-from InterfaceMonitor becomes invalid after moving from it. It is prohibited to reuse
   *                  an invalid InterfaceMonitor object.
   *
   * \param[in]       other       Other InterfaceMonitor object to move from.
   *
   * \pre             Moved-from InterfaceMonitor must be valid.
   *                  Reactor of the other object should not execute the internal reactor callback of the other object
   *                  while this API is running. Usually this means that HandleEvents() of the reactor of the other
   *                  object should not be running during the execution of this API.
   *
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector product internal API
   */
  InterfaceMonitor(InterfaceMonitor&& other) noexcept;

  /*!
   * \brief           Deleted copy assignment operator. From the user's point of view, it makes no sense to have more
   *                  than one InterfaceMonitor monitoring the same Interface.
   */
  auto operator=(InterfaceMonitor const&) -> InterfaceMonitor& = delete;

  /*!
   * \brief           Moves other's InterfaceMonitor
   *
   * \details         Transfer the ownership of the other reactor_callback_handle_ to this entity.
   *                  Update the reactor callback target to the internal callback of this entity.
   *                  The moved-from InterfaceMonitor becomes invalid after moving from it. It is prohibited to reuse an
   *                  invalid InterfaceMonitor object.
   *
   * \param[in]       other       Other InterfaceMonitor object to move from.
   *
   * \return          This object
   *
   * \pre             Moved-from and move-to InterfaceMonitor must be valid.
   *                  Reactor of the other object should not execute the internal reactor callback of the other object
   *                  while this API is running. Usually this means that HandleEvents() of the reactor of the other
   *                  object should not be running during the execution of this API.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector product internal API
   */
  auto operator=(InterfaceMonitor&& other) & noexcept -> InterfaceMonitor&;

  /*!
   * \brief           Destroys an InterfaceMonitor.
   *
   * \details         Unsubscribes the internal callback from the reactor. Calls the default destructor for the other
   *                  members.
   *                  User has to avoid to destruct the InterfaceMonitor object while the internal reactor
   *                  callback is executing. This can be done by using a separate software event reactor job, or by
   *                  ensuring per design that InterfaceMonitor is destroyed in the same thread where
   *                  Reactor::HandleEvents() is called. If the InterfaceMonitor object is destroyed while the internal
   *                  reactor callback is executing, the execution will be aborted.
   *
   * \pre             Should not be executed in the context of the user callback.
   *                  Reactor of this object should not execute this internal reactor callback while the destructor is
   *                  running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   * \vprivate        Vector product internal API
   */
  ~InterfaceMonitor() noexcept;

 private:
  /*!
   * \brief           Constructor of InterfaceMonitor object
   *
   * \details         Creates an InterfaceMonitor object from an Interface, Reactor1Interface, and UserCallbackConfig.
   *                  The initialization is done inside MakeInterfaceMonitor() since some operations may fail. This
   *                  ensures that only a fully initialized InterfaceMonitor object has been returned to the user.
   *
   *
   * \param[in]       interface       Interface that the user wants to be monitored.
   *
   * \param[in]       reactor         Reactor to monitor the changes occurring in the monitored network interface.
   *                                  The reactor has to exist the whole lifetime of this InterfaceMonitor object.
   *
   * \param[in]       user_callbacks  Container that holds the user-defined callbacks. Will be moved to this
   *                                  InterfaceMonitor object.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  InterfaceMonitor(Interface interface, osabstraction::io::reactor1::Reactor1Interface& reactor,
                   UserCallbackConfig&& user_callbacks) noexcept
      : target_interface_(std::move(interface)), reactor_(&reactor), user_callbacks_(std::move(user_callbacks)) {}

  /*!
   * \brief           Registers the socket file descriptor and the internal callback to the reactor.
   *
   * \details         In the context of this method, the reactor_callback_handle_ is set.
   *
   * \error           osabstraction::OsabErrc::kResource    No memory to register another callback or system limit
   *                                                        reached.
   * \error           osabstraction::OsabErrc::kUnexpected  InterfaceMonitor class has encountered an unrecoverable
   *                                                        error. User should log the error and abort. The internal
   *                                                        callback has been already registered. Waiting for read
   *                                                        events is impossible for the internal file descriptor.
   *
   * \return          A handle for the registered internal callback.
   *
   * \pre             The internal socket must be already initialized.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  auto RegisterInternalCallback() noexcept -> ::amsr::core::Result<osabstraction::io::reactor1::CallbackHandle>;

  /*!
   * \brief           Implements the internal reactor callback.
   *
   * \details         Read interface status change events from the socket and call the appropriate user callback.
   *                  Messages indicating errors are ignored.
   *                  User should drive the reactor frequently enough so read messages will reflect the contemporary
   *                  interface state.
   *
   * \context         Reactor1Interface::HandleEvent()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  void OnInterfaceStatusChange() noexcept;

  /*!
   * \brief           Maps the route message to an event and calls the specific user callback.
   *
   * \param[in]       single_msg    Message to be processed.
   *
   * \context         OnInterfaceStatusChange()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  void HandleSingleMessage(amsr::net::internal::RouteSocketMessageHdr const* single_msg) noexcept;

  /*!
   * \brief           Handles the InterfaceMonitorEvent::kUp event and then calls the specific user callback.
   *
   * \return          -
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore.
   *
   * \context         OnInterfaceStatusChange()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  auto HandleStatusUpEvent() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Handles the InterfaceMonitorEvent::kDown event and then calls the specific user callback.
   *
   * \context         OnInterfaceStatusChange()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  void HandleStatusDownEvent() noexcept;

  /*!
   * \brief           Handle the InterfaceMonitorEvent::kNewAddr event and then call the specific user callback.
   *
   * \details         Extracts the added IP address from the route address message using the RouteMessageParser and
   *                  pass the extracted IP address to the user.
   *
   * \param[in]       address_message_hdr Pointer to the address message header. Pointer must be valid.
   *
   * \param[in]       payload_len Length of the payload that follows a route address message header.
   *
   * \context         OnInterfaceStatusChange()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  void HandleNewAddrEvent(amsr::net::internal::RouteAddressMessageHdr const* address_message_hdr,
                          std::size_t payload_len) noexcept;

  /*!
   * \brief           Handle the InterfaceMonitorEvent::kDelAddr event and then call the specific user callback.
   *
   * \details         Extracts the removed IP address from the route address message using the RouteMessageParser and
   *                  pass the extracted IP address to the user.
   *
   * \param[in]       address_message_hdr Pointer to the address message header. Pointer must be valid.
   *
   * \param[in]       payload_len Length of the payload that follows the route address message header.
   *
   * \context         OnInterfaceStatusChange()
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceMonitor
   *
   * \vprivate        Vector component internal API
   */
  void HandleDelAddrEvent(amsr::net::internal::RouteAddressMessageHdr const* address_message_hdr,
                          std::size_t payload_len) noexcept;

  /*!
   * \brief           Copy of the Interface object that the monitor class should observe.
   */
  Interface target_interface_;

  /*!
   * \brief           Pointer to the Reactor1Interface object for asynchronous operations.
   */
  osabstraction::io::reactor1::Reactor1Interface* reactor_;

  /*!
   * \brief           Stores the user callbacks, that are called by the internal reactor callback
   *                  (OnInterfaceStatusChange()) upon the occurrence of corresponding events.
   */
  UserCallbackConfig user_callbacks_;

  /*!
   * \brief           File descriptor of the socket that monitors changes in the state of target_interface_.
   */
  osabstraction::io::FileDescriptor socket_fd_{osabstraction::io::kInvalidNativeHandle};

  /*!
   * \brief          Stores the handle to the internal reactor callback (OnInterfaceStatusChange()) to be
   *                 called by reactor_ upon changes in socket_fd_. Should be explicitly destroyed in the
   *                 InterfaceMonitor destructor.
   */
  osabstraction::io::reactor1::CallbackHandle reactor_callback_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};
};

}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_MONITOR_H_
