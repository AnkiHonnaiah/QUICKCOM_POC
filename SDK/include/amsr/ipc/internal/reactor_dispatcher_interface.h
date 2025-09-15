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
/*!        \file
 *        \brief  Interface to the ReactorDispatcher.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *
 *      \details  The ReactorDispatcher implements a way for multiple different Reactor callbacks to have the same entry
 *                point.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/io/reactor1/types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Identifier for a callback registered to ReactorDispatcher.
 * \details Should be handled as an opaque type by the user of the ReactorDispatcher. Only the ReactorDispatcher
 *          implementation should inspect the members of this struct.
 */
struct ReactorDispatcherId {
  /*!
   * \brief   Index of the callback entry in the ReactorDisptacher internal data structure.
   */
  std::size_t index;

  /*!
   * \brief   Sequence number of the callback entry in the ReactorDisptacher internal data structure.
   */
  std::uint64_t sequence_num;
};

/*!
 * \brief              Type of callback that can be registered to the ReactorDispatcher.
 *
 * \param[in]          events   Occurred Reactor events.
 *
 * \context            ReactorDispatcher::Dispatch()
 *
 * \pre                -
 *
 * \reentrant          FALSE
 * \synchronous        TRUE
 * \threadsafe         FALSE
 */
using ReactorDispatcherCallback = vac::language::UniqueFunction<void(osabstraction::io::reactor1::EventTypes events)>;

/*!
 * \brief   Interface to the ReactorDispatcher.
 * \details The ReactorDispatcher implements a way for multiple different Reactor callbacks to have the same entry
 *          point.
 */
class ReactorDispatcherInterface {
 public:
  /*!
   * \brief           Creates a ReactorDispatcherInterface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ReactorDispatcherInterface() noexcept = default;

  // The class shall not be copied. Move semantics are not implemented.
  ReactorDispatcherInterface(ReactorDispatcherInterface const& other) = delete;
  ReactorDispatcherInterface(ReactorDispatcherInterface&& other) = delete;
  ReactorDispatcherInterface& operator=(ReactorDispatcherInterface const& other) = delete;
  ReactorDispatcherInterface& operator=(ReactorDispatcherInterface&& other) = delete;

  /*!
   * \brief           Destroys the object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual ~ReactorDispatcherInterface() noexcept = default;

  /*!
   * \brief           Registers a callback for a file descriptor to the ReactorDispatcher.
   *
   * \details         Can be called from a ReactorDispatcherCallback.
   *
   * \param[in]       io_source   File descriptor for which the callback shall be registered. No callback is registered
   *                              to the Reactor for this file descriptor yet, the file descriptor is valid (and stays
   *                              valid until the callback is unregistered) and waiting for the specified events in the
   *                              Reactor is possible.
   * \param[in]       events      Types of Reactor events for which this callback should be active.
   * \param[in]       callback    Callback to be registered, must not be empty. This callback is called from Dispatch()
   *                              if the identifier for this callback is passed to Dispatch(). This object takes
   *                              ownership of the passed callback. The callback is destroyed when Unregister() is
   *                              called. If the callback is currently executing during the Unregister() call, the
   *                              destruction of the callback is delayed until after the callback returned, otherwise
   *                              the callback is destroyed during the Unregister() call.
   *
   * \return          Identifier for the registered callback. Can be used in future calls to SetMonitoredEvents() /
   *                  AddMonitoredEvents() / RemoveMonitoredEvents() / Unregister().
   *
   * \error           osabstraction::OsabErrc::kResource       No free ReactorDispatcher slot or Reactor1 slot available
   *                                                           or system resource limit reached.
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual auto Register(osabstraction::io::NativeHandle io_source, osabstraction::io::reactor1::EventTypes events,
                        ReactorDispatcherCallback&& callback) noexcept -> ::amsr::core::Result<ReactorDispatcherId> = 0;

  /*!
   * \brief           Registers a callback for a Reactor software event to the ReactorDispatcher.
   * \details         Can be called from a ReactorDispatcherCallback.
   *
   * \param[in]       callback    Callback to be registered must not be empty. This callback is called from Dispatch()
   *                              if the identifier for this callback is passed to Dispatch(). This object takes
   *                              ownership of the passed callback. The callback is destroyed when
   *                              UnregisterSoftwareEvent() is called. If the callback is currently executing during the
   *                              UnregisterSoftwareEvent() call, the destruction of the callback is delayed until after
   *                              the callback returned, otherwise  the callback is destroyed during the
   *                              UnregisterSoftwareEvent() call.
   *
   * \return          Identifier for the registered callback. Can be used in future calls to SetMonitoredEvents() /
   *                  AddMonitoredEvents() / RemoveMonitoredEvents() / Unregister().
   *
   * \error           osabstraction::OsabErrc::kResource      No free ReactorDispatcher slot or Reactor1 slot available.
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual auto RegisterSoftwareEvent(ReactorDispatcherCallback&& callback) noexcept
      -> ::amsr::core::Result<ReactorDispatcherId> = 0;

  /*!
   * \brief           Sets active event types for a previously registered callback for a file descriptor.
   *
   * \details         Can be called from a ReactorDispatcherCallback.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback whose active event types
   *                              shall be set. The callback shall be registered for a file descriptor.
   * \param[in]       events      New event types for which the callback shall be active, shall not contain software
   *                              event.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void SetMonitoredEvents(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept = 0;

  /*!
   * \brief           Adds active event types for a previously registered callback for a file descriptor.
   *
   * \details         Can be called from a ReactorDispatcherCallback.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback whose active event types
   *                              shall be set. The callback shall be registered for a file descriptor.
   * \param[in]       events      Event types to add to the current event types for which the callback shall be active,
   *                              shall not contain software event.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void AddMonitoredEvents(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept = 0;

  /*!
   * \brief           Removes active event types for a previously registered callback for a file descriptor.
   *
   * \details         Can be called from a ReactorDispatcherCallback.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback whose active event types
   *                              shall be set. The callback shall be registered for a file descriptor.
   * \param[in]       events      Event types to remove from the active event types for which the callback shall be
   *                              active, shall not contain software event.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void RemoveMonitoredEvents(ReactorDispatcherId id,
                                     osabstraction::io::reactor1::EventTypes events) noexcept = 0;

  /*!
   * \brief           Trigger a registered reactor software event.
   * \details         Can be called from a ReactorDispatcherCallback. Triggering an already triggered, not yet executed
   *                  callback has no effect.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback that shall be triggered. The
   *                              callback shall be registered for a software event.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void TriggerSoftwareEvent(ReactorDispatcherId id) noexcept = 0;

  /*!
   * \brief           Unregister a registered callback for a file descriptor.
   *
   * \details         Can be called from a ReactorDispatcherCallback. The callback is not executed anymore after this
   *                  call, currently running callbacks will finish executing. The file descriptor is not closed by this
   *                  call.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback that shall be unregistered.
   *                              The callback shall be registered for a file descriptor.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void Unregister(ReactorDispatcherId id) noexcept = 0;

  /*!
   * \brief           Unregister a registered callback for a software event.
   *
   * \details         Can be called from a ReactorDispatcherCallback. The callback is not executed anymore after this
   *                  call, currently running callbacks will finish executing.
   *
   * \param[in]       id          Valid (not yet unregistered) identifier for the callback that shall be unregistered.
   *                              The callback shall be registered for a software event.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void UnregisterSoftwareEvent(ReactorDispatcherId id) noexcept = 0;

  /*!
   * \brief           Calls the callback associated with the passed identifier with the passed events if the callback
   *                  was not yet unregistered.
   *
   * \details         Other ReactorDispatcher API functions (other than Dispatch()) are allowed to be called from
   *                  the callbacks called by Dispatch().
   *
   * \param[in]       id          Identifier of the callback for which the Reactor reported events. The registered
   *                              callback will not be called if the callback for this identifier was already
   *                              unregistered.
   * \param[in]       events      Events that the Reactor reported and that will be forwarded to the registered
   *                              callback.
   *
   * \context         Common callback entry point for this ReactorDispatcher.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  virtual void Dispatch(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept = 0;
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_INTERFACE_H_
