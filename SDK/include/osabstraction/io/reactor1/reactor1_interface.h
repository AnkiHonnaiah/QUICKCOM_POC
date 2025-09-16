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
/**        \file  reactor1_interface.h
 *        \brief  Abstract interface to the reactor.
 *         \unit  osabstraction::Reactor1_PikeOS
 *         \unit  osabstraction::Reactor1_QNX
 *         \unit  osabstraction::Reactor1_Linux
 *   \complexity  The Reactor1_QNX unit slightly exceeds following metrics:
 *                - SUM, mTCYC, mwCYC: Callback registration and HandleEvents() functions are slightly more complex
 *                  than allowed limits: this is considered not an issue for the safety analysis because
 *                  both functionalities can be fully tested and they cannot be split because they implement
 *                  indivisible functionalities.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/timer/timer_reactor_interface.h"
#include "osabstraction/io/reactor1/types.h"
#include "osabstraction/osab_error_domain.h"

namespace osabstraction {
namespace io {
namespace reactor1 {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Timeout value to represent infinite wait time.
 */
constexpr std::chrono::nanoseconds const kInfinity{std::chrono::nanoseconds::max()};

/*!
 * \brief    Abstract interface to the reactor.
 *
 * \details  A threadsafe implementation of the reactor design pattern. The reactor pattern is an event handling pattern
 *           to manage handling of multiple IO sources (e.g. IPC or IP connections) and software events.
 *           Applications can register callbacks for IO and software events in the Reactor.
 *           The Reactor executes triggered callbacks for software events and then
 *           demultiplexes the incoming IO events of the IO sources and dispatches them synchronously to the
 *           registered callbacks. If no IO events are available the reactor will block sleeping.
 *
 * \vprivate pes internal API
 */
class Reactor1Interface : public amsr::timer::TimerReactorInterface {
 public:
  /*!
   * \brief         Registers a callback for an IO source.
   *
   * \details       Only one callback can be registered for each IO source. Previously registered callbacks for
   *                NativeHandle have been unregistered before.
   *
   * \param[in]     io_source   NativeHandle for which the callback should be registered. Cannot be a handle for a file
   *                            or a directory. The handle has to stay valid until it has been unregistered. The user
   *                            has to make sure that NativeHandle is not accessed anymore after it was closed.
   * \param[in]     events      Types of events for which this callback should be active.
   * \param[in,out] callback    Callback to be registered. The CallbackHandle of the callback and the occurred
   *                            EventTypes are passed to the callback if it is called. The callback shall not throw
   *                            exceptions when it is called and when it is assigned. The callback is only called from
   *                            the context of HandleEvents(). callback must not be empty.
   *
   * \return        A handle for the registered callback.
   *
   * \error         osabstraction::OsabErrc::kApiError      A callback is already registered for NativeHandle or waiting
   *                                                        for the specified events is not possible for NativeHandle.
   * \error         osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                        reached.
   *
   * \context       ANY
   * \note          Can be called from a callback.
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-ReactorIOEventRegistration
   *
   * \vprivate      pes internal API
   */
  virtual ::amsr::core::Result<CallbackHandle> Register(NativeHandle io_source, EventTypes events,
                                                        CallbackType&& callback) noexcept = 0;

  /*!
   * \brief         Registers a callback for a software event.
   *
   * \param[in,out] callback    Callback to be registered. The CallbackHandle of the callback and SoftwareEvent
   *                            EventTypes are passed to the callback if it is called. The callback shall not throw
   *                            exceptions when it is called and when it is assigned. The callback is only called from
   *                            the context of HandleEvents(). callback must not be empty.
   *
   * \return        A handle for the registered callback.
   *
   * \error         osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                        reached.
   *
   * \context       ANY
   * \note          Can be called from a callback.
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-ReactorSoftwareEventRegistration
   *
   * \vprivate      pes internal API
   */
  virtual ::amsr::core::Result<CallbackHandle> RegisterSoftwareEvent(CallbackType&& callback) noexcept = 0;

  /*!
   * \brief       Sets active event types for a previously registered callback for IO events.
   *
   * \details     This can be used to e.g. temporarily disable a callback by setting all event types to false or to e.g.
   *              temporarily enable/disable a callback for writable events.
   *
   * \param[in]   callback_handle    CallbackHandle of the callback that should be modified.
   * \param[in]   events             New event types for which this callback should be active.
   *
   * \error       osabstraction::OsabErrc::kApiError      callback_handle is invalid, or not registered in the Reactor,
   *                                                      or waiting for the specified events is not possible for the
   *                                                      NativeHandle this callback was registered for,
   *                                                      or callback_handle is registered for software event,
   *                                                      or passed events contain software event.
   * \error      osabstraction::OsabErrc::kResource       Not enough system resources to perform operation.
   *
   * \context     ANY
   * \note        Can be called from a callback.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorIOChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> SetMonitoredEvents(CallbackHandle callback_handle, EventTypes events) noexcept = 0;

  /*!
   * \brief       Adds active event types for a previously registered callback for IO events.
   *
   * \details     Adding an already active event has no effect and is not considered as an error.
   *
   * \param[in]   callback_handle    CallbackHandle of the callback that should be modified.
   * \param[in]   events             Added event types for which this callback should be active.
   *
   * \error       osabstraction::OsabErrc::kApiError      callback_handle is invalid, or not registered in the Reactor,
   *                                                      or waiting for the specified events is not possible for the
   *                                                      NativeHandle this callback was registered for, or
   *                                                      callback_handle is registered for software event or
   *                                                      passed events contain software event.
   * \error      osabstraction::OsabErrc::kResource       Not enough system resources to perform operation.
   *
   * \context     ANY
   * \note        Can be called from a callback.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorIOChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> AddMonitoredEvents(CallbackHandle callback_handle, EventTypes events) noexcept = 0;

  /*!
   * \brief       Removes active event types for a previously registered callback for IO events.
   *
   * \details     Removing an event which is already inactive has no effect and is not considered as an error.
   *
   * \param[in]   callback_handle    CallbackHandle of the callback that should be modified.
   * \param[in]   events             Events types for which the callback should not be active any more.
   *
   * \error       osabstraction::OsabErrc::kApiError      callback_handle is invalid or not registered in the Reactor or
   *                                                      registered for software event or passed events contain
   *                                                      software event or waiting for the specified events is not
   *                                                      possible.
   *
   * \context     ANY
   * \note        Can be called from a callback.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorIOChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> RemoveMonitoredEvents(CallbackHandle callback_handle,
                                                           EventTypes events) noexcept = 0;

  /*!
   * \brief         Changes the target for a registered callback.
   *
   * \param[in]     callback_handle Callback handle of the callback that should be modified.
   * \param[in]     callback New target of the callback.
   *
   * \return        An empty result if successful.
   *
   * \error         osabstraction::OsabErrc::kApiError  callback_handle is invalid or not registered in the Reactor or
   *                                                    the callback for callback_handle is currently executing.
   *
   * \context       ANY
   *
   * \pre           The callback to be modified must not be executing. The new target of the callback must not be empty.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   * \steady        TRUE
   *
   * \trace         DSGN-Osab-ReactorChangeRegistrationTarget
   *
   * \vprivate      pes internal API
   */
  virtual ::amsr::core::Result<void> SetCallbackTarget(CallbackHandle callback_handle,
                                                       CallbackType callback) noexcept = 0;

  /*!
   * \brief         Trigger a registered software event callback.
   * \details       Triggering an already triggered, not yet executed callback has no effect.
   *                The triggered callback will be executed latest in the next HandleEvents() iteration.
   *                Reactor may be unblocked if necessary.
   *
   * \param[in]     callback_handle Callback handle of the callback that should be scheduled.
   *
   * \return        An empty result if successful (also if the callback was already triggered)
   *
   * \error         osabstraction::OsabErrc::kApiError  callback_handle is invalid or not registered for software events
   *                                                    in the Reactor.
   *
   * \context       ANY
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   * \steady      TRUE
   *
   * \trace         DSGN-Osab-ReactorTriggerSoftwareEvent
   *
   * \vprivate      pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> TriggerSoftwareEvent(CallbackHandle callback_handle) noexcept = 0;

  /*!
   * \brief       Unregisters a previously registered callback.
   *
   * \details     The callback is not executed anymore after this call.
   *              This call is returned immediately after unregistering the callback. If there is any callback running
   *              currently in the reactor HandleEvents() context, it will continue and complete execution.
   *
   *              It can be specified if the file descriptor of callbacks registered for IO events should be closed by
   *              the reactor. If this option is set and the callback is not currently running while this function is
   *              called the file descriptor is closed immediately. If the callback is currently running, the closing of
   *              the file descriptor is delayed until the callback has returned.
   *
   *              The memory owned by the callback is released either immediately, if the callback is not currently
   *              executing or after the execution finishes. That means captured objects destructor may be called.
   *
   * \param[in]   callback_handle      CallbackHandle of the callback that should be unregistered.
   * \param[in]   close_file_handle    Signals if io_source of the callback shall be closed when it is safe to do so.
   *
   * \return      An empty result if successful
   *
   * \error       osabstraction::OsabErrc::kApiError  callback_handle is invalid or registered for software event
   *                                                  in the Reactor.
   *
   * \context     ANY
   * \note        Can be called from a callback.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      FALSE
   *
   * \trace       DSGN-Osab-ReactorIOEventDeregistration
   *
   * \vprivate    pes internal API
   *
   */
  virtual ::amsr::core::Result<void> Unregister(CallbackHandle callback_handle,
                                                CloseOnUnregister close_file_handle) noexcept = 0;

  /*!
   * \brief       Unregisters a previously registered callback for a software event.
   *
   * \details     The callback is not executed anymore after this call.
   *              This call is returned immediately after unregistering the callback. If there is any callback running
   *              currently in the reactor HandleEvents() context, it will continue and complete execution.
   *
   *              The memory owned by the callback is released either immediately, if the callback is not currently
   *              executing or after the execution finishes. That means captured objects destructor may be called.
   *
   * \param[in]   callback_handle      CallbackHandle of the callback that should be unregistered.
   *
   * \return      An empty result if successful
   *
   * \error       osabstraction::OsabErrc::kApiError  callback_handle is invalid or not registered for software event
   *                                                  in the Reactor.
   * \context     ANY
   * \note        Can be called from a callback.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      FALSE
   *
   * \trace       DSGN-Osab-ReactorSoftwareEventDeregistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<void> UnregisterSoftwareEvent(CallbackHandle callback_handle) noexcept = 0;

  /*!
   * \brief       Returns whether resources of a callback are still in use.
   *
   * \details     Returns whether the callback is still in use. This method does not indicate whether a callback is
   *              currently executed or not, it only reports if a callback is fully unregistered which means that all
   *              associated resources have been released and the callback is no longer running.
   *              In case of the I/O events, related NativeHandle can be safely closed if this function returns false
   *              and NativeHandle was not registered again for another callback after the callback for callback_handle
   *              was unregistered from the reactor.

   *
   * \param[in]   callback_handle    CallbackHandle of the callback that is checked.
   *
   * \return      true if resources are still in use, false otherwise.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE for different objects.
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorHandlerInUse
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual bool IsInUse(CallbackHandle callback_handle) const noexcept = 0;

  /*!
   * \brief       Checks whether any software events are triggered and executes the corresponding callbacks.
   *              Afterwards checks whether some IO events are pending on any registered NativeHandle
   *              and dispatches the corresponding callbacks.
   * \details     A maximum amount of software event callbacks equal to the total number of callbacks of
   *              current Reactor object will be executed.
   *
   * \attention   It is possible to call this method with a more rough granular duration than nanoseconds,
   *              like i.e. microseconds but this is discouraged because the user has to take care about possible
   *              overflows during implicit conversion.
   *
   * \details     The function does not block if
   *                - Any software event has been handled.
   *                - The passed timeout is zero.
   *
   *              The function blocks until either
   *                - a NativeHandle becomes ready. (Might be a spurious wakeup in which case the e.g. readable state of
   *                  a file descriptor changes again shortly after the reactor is unblocked to not readable and a read
   *                  to the file would block. Same for all other event types.)
   *                - or the call is interrupted by a signal handler
   *                - or the Reactor is unblocked (either by \ref Unblock() or by another API function, like \ref
   *                  TriggerSoftwareEvent().)
   *                - or the timeout expires
   *
   * \param[in]  timeout Time to wait for events in nanoseconds.
   *                     - If the timeout value is zero or negative, this function will not block.
   *                     - The maximum timeout value is OS dependent, any value above this limit will be converted to
   *                       the maximum allowed value. Refer to the OS specific reactor1.h for more details.
   *                     - Positive timeout values may be increased depending on the exploited OS API granularity.
   *                       Refer to the OS specific reactor1.h for more details.
   *
   * \return      Reason why the Reactor did not block or was unblocked.
   *
   * \error       osabstraction::OsabErrc::kFatal The reactor encountered an unrecoverable error.
   *
   * \context     ANY Only one thread at a time, not from a callback of this Reactor1 object, not while the
   *              HandleEventsLoop() method of this Reactor1 object is currently running.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE for all Reactor1 API functions except HandleEvents() and HandleEventsLoop()
   * \steady      TRUE if callbacks do not allocate
   *
   * \trace       DSGN-Osab-ReactorHandleEvents
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::core::Result<UnblockReason> HandleEvents(std::chrono::nanoseconds timeout) noexcept = 0;

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_interface
  /*!
   * \brief       Blocks until any software events or IO events are available, then calls the callbacks of the occurred
   *              events and blocks again waiting for the next events to occur.
   * \details     This is done in a loop and the function only returns once Unblock() is called on this Reactor1 object.
   *
   * \context     ANY Only one thread at a time, not from a callback of this Reactor1 object, not while the
   *              HandleEvents() method of this Reactor1 object is currently running.
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE for all Reactor1 API functions except HandleEvents() and HandleEventsLoop()
   * \steady      TRUE if callbacks do not allocate
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleEventsLoop() noexcept {
    amsr::core::Abort("Default implementation of Reactor1Interface::HandleEventsLoop() should never be called.");
  }

  /*!
   * \brief       Causes the reactor to return from the blocking call of the function \ref HandleEvents().
   *
   * \details     The reactor will return immediately for the next \ref HandleEvents() call if it is not currently
   *              blocking.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorUnblock
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void Unblock() noexcept override = 0;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: constness_delayed
  /*!
   * \brief       Returns OS specific resource information for Reactor1 usage.
   *
   * \return      OS specific resource information.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorGetOsParameters
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual OsParameters GetOsParameters() noexcept = 0;

  /*!
   * \brief       Returns if currently executing thread is the one which executed HandleEvents() or HandleEventsLoop()
   *              last time.
   *
   * \details     Usually execution context should be clear during API design/implementation and this runtime
   *              check should not be required.
   *              It is assumed that only one thread executes HandleEvents() or HandleEventsLoop()
   *
   * \return      true  if currently executing thread is the one which executed HandleEvents() or HandleEventsLoop()
   *                    last time.
   *              false if HandleEvents() or HandleEvents() loop were not yet executed at all
   *                    or
   *                    if currently executing thread is not the one which executed HandleEvents() or
   *                    HandleEventsLoop() last time.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorIsThisThreadHandlingEvents
   *
   * \vprivate    Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual bool IsThisThreadHandlingEvents() noexcept {
    amsr::core::Abort(
        "Default implementation of Reactor1Interface::IsThisThreadHandlingEvents() should never be called.");
  }
};

}  // namespace reactor1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_INTERFACE_H_
