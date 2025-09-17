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
/**        \file  libosabstraction-linux/include/osabstraction/io/reactor1/reactor1.h
 *        \brief  Implementation of the Reactor1 interface for Linux.
 *        \unit   osabstraction::Reactor1_Linux
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/epoll.h>
#include <atomic>
#include <limits>
#include <mutex>
#include <thread>
#include <utility>
#include "amsr/core/result.h"
#include "ara/core/vector.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/container/static_list.h"

namespace osabstraction {
namespace io {
namespace reactor1 {

namespace internal {

/*!
 * \brief   Structure for storing a callback.
 *
 * \details A CallbackEntry is unused if both flags valid and in_callback are false.
 */
struct CallbackEntry {
  /*!
   * \brief File descriptor for which the callback is registered.
   */
  NativeHandle io_source{};

  /*!
   * \brief Registered event types.
   */
  EventTypes registered_events{};

  /*!
   * \brief Callback function object.
   */
  CallbackType callback;

  /*!
   * \brief   Flag that shows if the callback is valid.
   *
   * \details Set to invalid when the callback is unregistered.
   */
  bool valid{false};

  /*!
   * \brief   Flag that shows if the callback has been triggered but not yet executed.
   *
   * \details Valid only for software events.
   */
  bool triggered{false};

  /*!
   * \brief   Flag that shows if the callback is currently executing.
   *
   * \details This changes during the lifetime of the callback registration.
   */
  bool in_callback{false};

  /*!
   * \brief   Sequence number of the callback entry.
   *
   * \details Permanent for this entry. Is increased every time a new callback is registered in this entry.
   */
  std::uint64_t sequence_num{0};

  /*!
   * \brief   Mutex that protects the callback entry.
   *
   * \details For further information, see the description of the exclusive area
   *          osabstraction::io::reactor1::Reactor1::CallbackEntry::mutex_.
   */
  mutable std::mutex mutex;
};

}  // namespace internal

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_Reactor
/*!
 * \brief    Implementation of the Reactor1 interface for Linux.
 *
 * \details  A threadsafe implementation of the reactor design pattern. The reactor pattern is an event handling pattern
 *           to manage handling of IO on multiple file descriptors. Applications can register callbacks for file
 *           descriptors in the Reactor. The Reactor demultiplexes the incoming IO events of the file descriptors and
 *           dispatches them synchronously to the registered callbacks. If no IO events are available the reactor will
 *           block sleeping.
 *
 * \vprivate pes internal API
 */
class Reactor1 final : public Reactor1Interface {
 private:
  /*!
   * \brief File descriptor type.
   */
  using FileDescriptor = osabstraction::io::FileDescriptor;

 public:
  /*!
   * \brief    Construction token for Reactor1.
   *
   * \details  This class is the input for the Reactor1 constructor. By creating a ConstructionToken the resources for
   *           a Reactor1 object are acquired so that the Reactor1 constructor can be exceptionless.
   *
   * \vprivate pes internal API
   */
  class ConstructionToken final {
   public:
    /*!
     * \brief       Builds a ConstructionToken from the preconstructed resources.
     *
     * \param[in]   num_callbacks    Number of callbacks that the reactor should be able to handle. Valid range: 0 to
     *                               8191.
     * \param[in]   epoll_fd         Handle for the epoll instance.
     * \param[in]   unblock_event_fd Handle for the unblock eventfd instance.
     *
     * \context     Reactor1::Preconstruct()
     *
     * \pre         Preconstruction of the resources succeeded.
     *
     * \reentrant   TRUE for different objects
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     */
    ConstructionToken(std::uint16_t num_callbacks, FileDescriptor epoll_fd, FileDescriptor unblock_event_fd) noexcept
        : num_callbacks_{num_callbacks},
          epoll_fd_{std::move(epoll_fd)},
          unblock_event_fd_{std::move(unblock_event_fd)} {};

    ConstructionToken() = delete;

    /*!
     * \brief       Move constructs a ConstructionToken.
     *
     * \param       other   ConstructionToken that should be move construced.
     *
     * \reentrant   TRUE for different objects
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    ConstructionToken(ConstructionToken&& other) noexcept { *this = std::move(other); }

    /*!
     * \brief       Move assigns a ConstructionToken.
     *
     * \param[in]   other   ConstructionToken that should be move assigned.
     *
     * \return      Reference to this object.
     *
     * \context     ANY
     *
     * \pre         -
     *
     * \reentrant   TRUE for different ConstructionTokens
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    ConstructionToken& operator=(ConstructionToken&& other) & noexcept;

    // ConstructionToken shall not be copyable as the required resource allocation might fail.
    ConstructionToken(ConstructionToken const&) = delete;
    ConstructionToken& operator=(ConstructionToken const&) = delete;

    /*!
     * \brief Releases not extracted resources.
     *
     * \reentrant   FALSE
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    ~ConstructionToken() noexcept = default;

    /*!
     * \brief Get number of callbacks.
     *
     * \return      Number of callbacks.
     *
     * \reentrant   TRUE for different objects
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    std::uint16_t GetNumCallbacks() const { return num_callbacks_; }

    /*!
     * \brief       Extract the epoll file descriptor resource.
     *
     * \details     The ConstructionToken does not own the resource anymore after calling this function.
     *
     * \return      epoll file descriptor.
     *
     * \context     ANY
     *
     * \pre         Object still owns the epoll file descriptor resource.
     *
     * \reentrant   TRUE for different objects
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     */
    FileDescriptor ExtractEpollFd() { return std::move(epoll_fd_); }

    /*!
     * \brief       Extract the unblock eventfd file descriptor resource.
     *
     * \details     The ConstructionToken does not own the resource anymore after calling this function.
     *
     * \return      unblock eventfd file descriptor.
     *
     * \context     ANY
     *
     * \pre         Object still owns the eventfd file descriptor resource.
     *
     * \reentrant   TRUE for different objects
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     *
     * \vprivate    pes internal API
     */
    FileDescriptor ExtractUnblockEventFd() { return std::move(unblock_event_fd_); }

   private:
    /*!
     * \brief Number of callbacks the Reactor should be able to handle.
     */
    std::uint16_t num_callbacks_{};

    /*!
     * \brief Handle for the epoll instance.
     */
    FileDescriptor epoll_fd_{};

    /*!
     * \brief Handle for the unblock eventfd.
     */
    FileDescriptor unblock_event_fd_{kInvalidNativeHandle};
  };

  /*!
   * \brief       Creates a Reactor1 construction token.
   *
   * \details     If Preconstruct() returns an error, the user message describes which system call failed and the
   *              support data is set to the POSIX error code of the failed system call.
   *
   * \param[in]   num_callbacks     Number of callbacks that the reactor should be able to handle. Valid range: 0 to
   *                                8191.
   *
   * \return      Created ConstructionToken.
   *
   * \error       osabstraction::OsabErrc::kResource    Required resources could not be allocated.
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
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<ConstructionToken> Preconstruct(std::uint16_t num_callbacks) noexcept;

  /*!
   * \brief       Constructs a Reactor1.
   *
   * \param[in]   token    Construction token used to create the Reactor1.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE for different ConstructionTokens
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Reactor1(ConstructionToken&& token) noexcept;

  /*!
   * \brief Closes open files.
   *
   * \steady   FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate  pes internal API
   */
  ~Reactor1() noexcept final = default;

  Reactor1(Reactor1&&) = delete;
  Reactor1(Reactor1 const&) = delete;
  // VECTOR Next Line AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_Reactor1AssignmentOperator
  Reactor1& operator=(Reactor1&&) = delete;
  // VECTOR Next Line AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_Reactor1AssignmentOperator
  Reactor1& operator=(Reactor1 const&) = delete;

  /*!
   * \brief         Registers a callback for an IO source.
   *
   * \details       Only one callback can be registered for each IO source. Previously registered callbacks for
   *                NativeHandle have been unregistered before.
   *
   *                If Register() returns an error because the epoll_ctl() call failed, the user message describes this
   *                and the support data is set to the POSIX error code of the failed epoll_ctl() call.
   *
   * \param[in]     io_source   NativeHandle for which the callback should be registered. Cannot be a handle for a file
   *                            or a directory. Registering a NativeHandle other than a NativeHandle of a Unix Domain
   *                            Socket, a UDP socket, a TCP socket or a RAW IP socket may lead to unexpected Reactor
   *                            behavior and is not recommended. The handle has to stay valid until it has been
   *                            unregistered. The user has to make sure that NativeHandle is not accessed anymore after
   *                            it was closed.
   * \param[in]     events      Types of events for which this callback should be active.
   * \param[in,out] callback    Callback to be registered. The CallbackHandle of the callback and the occurred
   *                            EventTypes are passed to the callback if it is called. The callback shall not throw
   *                            exceptions when it is called and when it is assigned. The callback is only called from
   *                            the context of HandleEvents(). Callback must not be empty.
   *
   * \return        A handle for the registered callback.
   *
   * \error         osabstraction::OsabErrc::kApiError      A callback is already registered for NativeHandle or waiting
   *                                                        for the specified events is not possible for NativeHandle or
   *                                                        passed events contain software event.
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
   * \trace         DSGN-Osab-ReactorLinuxIOEventRegistration
   *
   * \vprivate      pes internal API
   */
  ::amsr::core::Result<CallbackHandle> Register(NativeHandle io_source, EventTypes events,
                                                CallbackType&& callback) noexcept final;

  /*!
   * \brief         Registers a callback for a software event.
   *
   * \param[in,out] callback    Callback to be registered. The CallbackHandle of the callback and the occurred
   *                            EventTypes are passed to the callback if it is called. The callback shall not throw
   *                            exceptions when it is called and when it is assigned. The callback is only called from
   *                            the context of HandleEvents(). The callback must not be empty.
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
   * \trace         DSGN-Osab-ReactorLinuxSoftwareEventRegistration
   *
   * \vprivate      pes internal API
   */
  ::amsr::core::Result<CallbackHandle> RegisterSoftwareEvent(CallbackType&& callback) noexcept final;

  /*!
   * \brief       Changes the event types of a previously registered callback.
   *
   * \details     This can be used to e.g. temporarily disable a callback by setting all event types to false or to e.g.
   *              temporarily enable/disable a callback for writable events.
   *
   *              If SetMonitoredEvents() returns an error because the epoll_ctl() call failed, the user message
   *              describes this and the support data is set to the POSIX error code of the failed epoll_ctl() call.
   *
   * \param[in]   callback_handle    Callback that should be modified.
   * \param[in]   events             New event types for which this callback should be active.
   *
   * \error       osabstraction::OsabErrc::kApiError     callback_handle is invalid, or not registered in the Reactor,
   *                                                     or waiting for the specified events is not possible for the
   *                                                     NativeHandle this callback was registered for,
   *                                                     or callback_handle is registered for software event,
   *                                                     or passed events contain software event.
   * \error      osabstraction::OsabErrc::kResource      Not enough system resources to perform operation.
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
   * \trace       DSGN-Osab-ReactorLinuxChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> SetMonitoredEvents(CallbackHandle callback_handle, EventTypes events) noexcept final;

  /*!
   * \brief       Adds active event types for a previously registered callback.
   *
   * \details     Adding an already active event has no effect and is not considered as an error.
   *
   *              If AddMonitoredEvents() returns an error because the epoll_ctl() call failed,
   *              the user message describes this and the support data is set to the POSIX error code of the failed
   *              epoll_ctl() call.
   *
   * \param[in]   callback_handle    Callback that should be modified.
   * \param[in]   events             Added event types for which this callback should be active.
   *
   * \error       osabstraction::OsabErrc::kApiError     callback_handle is invalid, or not registered in the Reactor,
   *                                                     or waiting for the specified events is not possible for the
   *                                                     NativeHandle this callback was registered for, or
   *                                                     callback_handle is registered for software event or
   *                                                     passed events contain software event.
   * \error      osabstraction::OsabErrc::kResource      Not enough system resources to perform operation.
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
   * \trace       DSGN-Osab-ReactorLinuxChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> AddMonitoredEvents(CallbackHandle callback_handle, EventTypes events) noexcept final;

  /*!
   * \brief       Removes active event types for a previously registered callback.
   *
   * \details     Removing an event which is already inactive has no effect and is not considered as an error.
   *
   *              If RemoveMonitoredEvents() returns an error because the epoll_ctl() call failed,
   *              the user message describes this and the support data is set to the POSIX error code of the failed
   *              epoll_ctl() call.
   *
   * \param[in]   callback_handle    Callback that should be modified.
   * \param[in]   events             Events types for which the callback should not be active any more.
   *
   * \error       osabstraction::OsabErrc::kApiError     callback_handle is invalid or not registered in the Reactor or
   *                                                     registered for software event or passed events contain
   *                                                     software event or waiting for the specified events is not
   *                                                     possible for the NativeHandle of the callback entry.
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
   * \trace       DSGN-Osab-ReactorLinuxChangeRegistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> RemoveMonitoredEvents(CallbackHandle callback_handle, EventTypes events) noexcept final;

  /*!
   * \copydoc osabstraction::io::reactor1::Reactor1Interface::SetCallbackTarget()

   * \steady      TRUE
   */
  ::amsr::core::Result<void> SetCallbackTarget(CallbackHandle callback_handle, CallbackType callback) noexcept final;

  /*!
   * \copydoc osabstraction::io::reactor1::Reactor1Interface::TriggerSoftwareEvent()
   *
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> TriggerSoftwareEvent(CallbackHandle callback_handle) noexcept final;

  /*!
   * \brief       Unregisters a previously registered callback for IO events.
   *
   * \details     The callback is not executed anymore after this call, currently running callbacks will finish
   *              executing.
   *
   *              It can be specified if the file descriptor should be closed by the reactor. If this option is set and
   *              the callback is not currently running while this function is called the file descriptor is closed
   *              immediately. If the callback is currently running, the closing of the file descriptor is delayed until
   *              the callback has returned.
   *
   *              The memory owned by the callback is released either immediately, if the callback is not currently
   *              executing or after the execution finishes. That means captured objects destructor may be called.
   *
   * \param[in]   callback_handle      Callback that should be unregistered.
   * \param[in]   close_file_handle    Signals if io_source of the callback shall be closed when it is safe to do so.
   *
   * \return      An empty result if successful
   *
   * \error       osabstraction::OsabErrc::kApiError  callback_handle is invalid or is registered for software event
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
   * \trace       DSGN-Osab-ReactorLinuxIOEventDeregistration
   *
   * \vprivate    pes internal API
   */
  ::amsr::core::Result<void> Unregister(CallbackHandle callback_handle,
                                        CloseOnUnregister close_file_handle) noexcept final;

  /*!
   * \brief       Unregisters a previously registered callback for software event.
   *
   * \details     The callback is not executed anymore after this call, currently running callbacks will finish
   *              executing.
   *
   *              The memory owned by the callback is released either immediately, if the callback is not currently
   *              executing or after the execution finishes. That means captured objects destructor may be called.
   *
   * \param[in]   callback_handle      Callback that should be unregistered.
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
   * \trace       DSGN-Osab-ReactorLinuxSoftwareEventDeregistration
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> UnregisterSoftwareEvent(CallbackHandle callback_handle) noexcept final;

  /*!
   * \brief       Returns whether resources of a callback are still in use.
   *
   * \details     The related NativeHandle can be safely closed if this function returns false and NativeHandle was not
   *              registered again for another callback after the callback for callback_handle was unregistered
   *              from the reactor.
   *
   * \param[in]   callback_handle    CallbackHandle of the callback that is checked.
   *
   * \return      true if resources are still in use, false otherwise. Returns false if callback_handle is invalid.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE for different objects
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \trace       DSGN-Osab-ReactorLinuxHandlerInUse
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsInUse(CallbackHandle callback_handle) const noexcept final;

  /*!
   * \brief       Checks whether some software events are triggered and then if any IO events are pending on any
   *              registered NativeHandle and dispatches the corresponding callbacks.
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
   *                - or the Reactor is unblocked i.e. by \ref Unblock() or by \ref TriggerSoftwareEvent().
   *                - or the timeout expires
   *
   *              This function will not return the error code osabstraction::OsabErrc::kFatal as specified in the
   *              interface. Instead the process terminates in case of a fatal error.
   *
   * \param[in]  timeout Time to wait for events in nanoseconds.
   *                     - If the timeout value is zero or negative, this function will not block.
   *                     - The maximum timeout value is INT_MAX milliseconds, any value above this limit will be
   *                       converted to INT_MAX milliseconds.
   *                     - Every positive timeout value, that is not exactly multiple of 1ms will be rounded up to the
   *                       next millisecond. This behavior allows this function to block also in case very small
   *                       positive timeout values (<1ms) are provided.
   *
   * \return      Reason why the Reactor did not block or was unblocked.
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
   * \trace       DSGN-Osab-ReactorLinuxHandleEvents
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<UnblockReason> HandleEvents(std::chrono::nanoseconds timeout) noexcept final;

  /*!
   * \copydoc Reactor1Interface::HandleEventsLoop()
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleEventsLoop() noexcept final;

  /*!
   * \copydoc Reactor1Interface::Unblock()
   *
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void Unblock() noexcept final;

  /*!
   * \brief       Returns OS specific resource information for Reactor1 usage.
   *
   * \details     On Linux exists no OS specific resource information, the returned struct is empty.
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
   * \trace       DSGN-Osab-ReactorLinuxGetOsParameters
   *
   * \vprivate    pes internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OsParameters GetOsParameters() noexcept final {  // COV_OSAB_NO_REAL_IMPLEMENTATION_LINUX
    return OsParameters{};
  };

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
   * \trace       DSGN-Osab-ReactorLinuxIsThisThreadHandlingEvents
   *
   * \vprivate    Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsThisThreadHandlingEvents() noexcept final;

 private:
  /*!
   * \brief       Ensures that a potentially blocking epoll_wait() call on the Reactor epoll file descriptor returns but
   *              does not necessarily cause return from HandleEventsLoop().
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   */
  void UnblockInternal() const noexcept;

  /*!
   * \brief       Checks if unblock is needed.
   * \details     Check if the reactor is currently in a blocking state or is about to enter a blocking state. If yes,
   *              reset reactor_is_blocking_ flag and request an unblock.
   * \return      True if unblock is needed.
   * \context     ANY
   * \pre         -
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  bool IsUnblockNeeded();

  /*!
   * \brief         Registers a callback for any event type (IO and software).
   *
   * \details       It is ensured by design that software events and IO events cannot be registered at the same
   *                time in the same entry.
   *
   * \param[in]     io_source   Valid only in case of IO events:
   *                            NativeHandle for which the callback should be registered. Cannot be a handle for a file
   *                            or a directory. Registering a NativeHandle other than a NativeHandle of a Unix Domain
   *                            Socket, a UDP socket, a TCP socket or a RAW IP socket may lead to unexpected Reactor
   *                            behavior and is not recommended. The handle has to stay valid until it has been
   *                            unregistered. The user has to make sure that NativeHandle is not accessed anymore after
   *                            it was closed.
   * \param[in]     events      Types of events for which this callback should be active. If there are any IO events
   *                            contained, additional epoll registration of the given NativeHandle is performed.
   * \param[in,out] callback    Callback to be registered. The CallbackHandle of the callback and the occurred
   *                            EventTypes are passed to the callback if it is called. The callback shall not throw
   *                            exceptions when it is called and when it is assigned. The callback is only called from
   *                            the context of HandleEvents(). The callback must not be empty.
   *
   * \return        A handle for the registered callback.
   *
   * \error         osabstraction::OsabErrc::kApiError      Only in case of IO events:
   *                                                        - A callback is already registered for NativeHandle
   *                                                        - Waiting for the specified events is not possible for
   *                                                          NativeHandle.
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
   *
   * \trace         DSGN-Osab-ReactorLinuxIOEventRegistration
   * \trace         DSGN-Osab-ReactorLinuxSoftwareEventRegistration
   *
   */
  ::amsr::core::Result<CallbackHandle> RegisterAnyEvent(NativeHandle io_source, EventTypes events,
                                                        CallbackType&& callback) noexcept;

  /*!
   * \brief      Finds a free callback entry for a callback for a new file descriptor that should be registered in the
   *             Reactor.
   *
   * \details    Also checks the currently last used entry in the callback container and updates it. The
   *             registration_mutex_ has to be held while calling this function.
   *
   * \param[in]  io_source          File descriptor that should be registered.
   *                                Software events are mapped to kInvalidNativeHandle file descriptor.
   *
   * \return     Iterator to the free callback entry that was found.
   * \error      osabstraction::OsabErrc::kApiError      A callback is already registered for io_source (only if it is
   *                                                     not kInvalidNativeHandle).
   * \error      osabstraction::OsabErrc::kResource      No more space in the callback container to register another
   *                                                     callback.
   *
   * \context    Reactor1::Register()
   *
   * \pre        -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ::amsr::core::Result<ara::core::Vector<internal::CallbackEntry>::iterator> FindFreeCallbackEntry(
      NativeHandle io_source);

  /*!
   * \brief      Adds a file descriptor to the epoll instance.
   *
   * \details    If this function returns an error because the epoll_ctl() call failed, the user message describes this
   *             and the support data is set to the POSIX error code of the failed epoll_ctl() call.
   *
   * \param[in]  io_source          Valid file descriptor that should be registered.
   * \param[in]  events             Events for which the file descriptor should be registered.
   * \param[in]  callback_handle    Callback for the file descriptor.
   *
   * \error      osabstraction::OsabErrc::kApiError      A callback is already registered for NativeHandle or waiting
   *                                                     for the specified events is not possible for NativeHandle.
   * \error      osabstraction::OsabErrc::kResource      System resources limit reached.
   *
   * \context    ANY
   *
   * \pre        -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ::amsr::core::Result<void> EpollAdd(NativeHandle io_source, EventTypes events, CallbackHandle callback_handle) const;

  /*!
   * \brief       Modifies the type of events a file descriptor is registered for on an epoll instance.
   *
   * \details     If this function returns an error because the epoll_ctl() call failed, the user message describes this
   *              and the support data is set to the POSIX error code of the failed epoll_ctl() call.
   *              If the epoll_ctl() call failed due to insufficient memory the program execution is aborted.
   *
   * \param[in]   io_source          File descriptor that is registered to the epoll instance for which the events
   *                                 should be changed. It has to be valid.
   * \param[in]   events             New events for which it should be registered.
   * \param[in]   callback_handle    Callback for the file descriptor.
   *
   * \error       osabstraction::OsabErrc::kApiError     callback_handle is invalid or not registered in the Reactor or
   *                                                     waiting for the specified events is not possible for
   *                                                     NativeHandle.
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ::amsr::core::Result<void> EpollModify(NativeHandle io_source, EventTypes events,
                                         CallbackHandle callback_handle) const;

  /*!
   * \brief       Removes a file descriptor from the epoll instance.
   *
   * \param[in]   io_source    Valid file descriptor that should be removed.
   *
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ::amsr::core::Result<void> EpollDelete(NativeHandle io_source) const;

  /*!
   * \brief      Updates the file descriptor registration to the epoll instance of a callback entry.
   *
   * \param[in]  entry            Callback entry whose epoll registration shall be updated.
   * \param[in]  new_events       New events that shall registered in the epoll instance.
   * \param[in]  callback_handle  Callback handle for the file descriptor.
   *
   * \error      osabstraction::OsabErrc::kApiError      callback_handle is invalid or not registered in the Reactor or
   *                                                     waiting for the specified events is not possible for the
   *                                                     NativeHandle of the callback entry.
   * \error      osabstraction::OsabErrc::kResource      Not enough system resources to perform operation.
   *
   * \context    ANY
   *
   * \pre        -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ::amsr::core::Result<void> UpdateEpollRegistration(internal::CallbackEntry const& entry, EventTypes new_events,
                                                     CallbackHandle callback_handle) const;

  /*!
   * \brief Handle a single reported event.
   *
   * \param[in] callback_handle    CallbackHandle returned by epoll_wait().
   * \param[in] events             IO events reported by epoll_wait().
   *
   * \context    ANY Only one thread at a time.
   *
   * \pre        -
   *
   * \reentrant  TRUE for different objects
   * \synchronous TRUE
   * \threadsafe TRUE for all Reactor1 methods except HandleSoftwareEvents(), HandleOneEvent()
   */
  void HandleOneEvent(CallbackHandle callback_handle, struct epoll_event events) noexcept;

  /*!
   * \brief      Handle an unblock event.
   *
   * \details    Sets the eventfd to not readable again.
   *
   * \context    ANY
   *
   * \pre        -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE for all Reactor1 API functions except HandleEvents()
   */
  void HandleUnblock() const;

  /*!
   * \param[in]  handle_all_events    true if the function shall ensure that all available software events are handled,
   *                                  false otherwise.
   *
   * \return     Number of software events handled. Note: May be incorrect if handle_all_events == true.
   * \context    ANY
   *
   * \pre        -
   *
   * \reentrant  TRUE for different objects
   * \threadsafe TRUE for all Reactor1 methods except HandleSoftwareEvents(), HandleOneEvent()
   */
  std::uint16_t HandleSoftwareEvents(bool handle_all_events) noexcept;

  /*!
   * \brief   Handle for the epoll instance.
   *
   * \details The epoll instance is used to monitor multiple file descriptors at the same time to see if I/O is possible
   *          on any of them.
   */
  FileDescriptor epoll_fd_;

  /*!
   * \brief Buffer for the events that are reported by epoll_wait().
   */
  ara::core::Vector<struct epoll_event> epoll_events_;

  /*!
   * \brief   Handle for a eventfd instance.
   *
   * \details The eventfd instance is used to unblock the reactor in the \ref Unblock() API function.
   */
  FileDescriptor unblock_event_fd_;

  /*!
   * \brief   Structure that stores all registered callbacks.
   *
   * \details Not all entries are used all the time.
   */
  ara::core::Vector<internal::CallbackEntry> callbacks_;

  /*!
   * \brief   Iterator that points to the callback entry past the last used entry.
   *
   * \details All elements in the callbacks_ vector past and including the element this iterator points to are
   *          guaranteed to be unused.
   */
  ara::core::Vector<internal::CallbackEntry>::iterator callbacks_end_{};

  /*!
   * \brief Queue of triggered software events.
   */
  vac::container::StaticList<CallbackHandle> triggered_software_events_{};

  /*!
   * \brief Flag that signals if the file descriptor of the callback that was just called should be closed by the
   *        reactor.
   */
  bool close_current_callback_fd_{false};

  /*!
   * \brief Flag that signals if the memory owned by the callback that was just called should be released by the
   *        reactor.
   */
  bool release_current_callback_memory_{false};

  /*!
   * \brief   Flag to determine if the reactor is currently in a blocking state or is about to enter a blocking state.
   */
  bool reactor_is_blocking_{true};

  /*!
   * \brief   Mutex that is used to serialize the \ref Register() calls.
   *
   * \details For further information, see the description of the exclusive area
   *          osabstraction::io::reactor1::Reactor1::registration_mutex_.
   */
  std::mutex registration_mutex_{};

  /*!
   * \brief   Mutex that is used to synchronize accesses and modifications of triggered_software_events_ with the
   *          reactor_is_blocking_ flag.
   *
   * \details For further information, see the description of the exclusive area
   *          osabstraction::io::reactor1::Reactor1::triggered_software_events_mutex_.
   */
  std::mutex triggered_software_events_mutex_{};

  /*!
   * \brief   This flags asserts HandleEvents() re-entrance condition.
   *
   * The flag will be set at the beginning of HandleEvents() and cleared before leaving the function. It must never
   * be already in set state on entering HandleEvents().
   */
  std::atomic_flag handle_events_guard_{};

  /*!
   * \brief   This flags indicates if the Reactor shall return from event handling.
   */
  std::atomic<bool> external_unblock_request_{false};

  /*!
   * \brief   ThreadId of the thread which executes HandleEvents() or HandleEventsLoop().
   *
   */
  std::thread::id handle_events_tid_{};
};

/*!
 * \exclusivearea ::osabstraction::io::reactor1::Reactor1::CallbackEntry::mutex_
 * Ensures consistency when reading/modifying a callback entry.
 *
 * \protects ::osabstraction::io::reactor1::Reactor1::CallbackEntry,
 *           ::osabstraction::io::reactor1::Reactor1::close_current_callback_fd_
 *           ::osabstraction::io::reactor1::Reactor1::release_current_callback_memory_,
 *           ::osabstraction::io::reactor1::Reactor1::reactor_is_blocking_,
 * \usedin   ::osabstraction::io::reactor1::Reactor1::Register,
 *           ::osabstraction::io::reactor1::Reactor1::SetMonitoredEvents,
 *           ::osabstraction::io::reactor1::Reactor1::AddMonitoredEvents,
 *           ::osabstraction::io::reactor1::Reactor1::RemoveMonitoredEvents,
 *           ::osabstraction::io::reactor1::Reactor1::Unregister,
 *           ::osabstraction::io::reactor1::Reactor1::SetCallbackTarget,
 *           ::osabstraction::io::reactor1::Reactor1::IsInUse,
 *           ::osabstraction::io::reactor1::Reactor1::HandleEvents,
 *           ::osabstraction::io::reactor1::Reactor1::HandleEventsLoop,
 *           ::osabstraction::io::reactor1::Reactor1::UnregisterSoftwareEvent
 *           ::osabstraction::io::reactor1::Reactor1::TriggerSoftwareEvent
 *           ::osabstraction::io::reactor1::Reactor1::RegisterSoftwareEvent
 * \exclude  ::osabstraction::io::reactor1::Reactor1::Unblock
 *           ::osabstraction::io::reactor1::Reactor1::GetOsParameters,
 * \length   SHORT Read-Modify-Write operations and epoll system calls.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::osabstraction::io::reactor1::Reactor1::triggered_software_events_mutex_
 * Serializes software events trigger, deregistration and handling operations.
 *
 * \protects ::osabstraction::io::reactor1::Reactor1::triggered_software_events_
 *           ::osabstraction::io::reactor1::Reactor1::reactor_is_blocking_
 *           ::osabstraction::io::reactor1::Reactor1::CallbackEntry
 *           ::osabstraction::io::reactor1::Reactor1::handle_events_tid_
 * \usedin   ::osabstraction::io::reactor1::Reactor1::TriggerSoftwareEvent
 *           ::osabstraction::io::reactor1::Reactor1::HandleEvents
 *           ::osabstraction::io::reactor1::Reactor1::HandleEventsLoop
 *           ::osabstraction::io::reactor1::Reactor1::UnregisterSoftwareEvent
 *           ::osabstraction::io::reactor1::Reactor1::IsThisThreadHandlingEvents
 * \exclude  All other methods of Reactor1.
 * \length   SHORT Read-Modify-Write operations of callback entry and queue insertion, removal operations.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::osabstraction::io::reactor1::Reactor1::registration_mutex_
 * Serializes callback registration to the reactor.
 *
 * \protects ::osabstraction::io::reactor1::Reactor1::callbacks_end_
 * \usedin   ::osabstraction::io::reactor1::Reactor1::Register
 *           ::osabstraction::io::reactor1::Reactor1::RegisterSoftwareEvent
 * \exclude  All other methods of Reactor1.
 * \length   MEDIUM Linear search in callbacks_ and modification of a callback entry.
 * \endexclusivearea
 */

}  // namespace reactor1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_REACTOR1_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_OSAB_NO_REAL_IMPLEMENTATION_LINUX
//   \ACCEPT XX
//   \REASON There is no real implementation for this function in Linux. For this reason it should not be covered.
// COV_JUSTIFICATION_END
