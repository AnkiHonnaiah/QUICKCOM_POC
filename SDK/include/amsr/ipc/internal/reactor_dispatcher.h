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
 *        \brief  Implements the ReactorDispatcher with a fixed number of callback slots.
 *
 *      \details  The ReactorDispatcher implements a way for multiple different Reactor callbacks to have the same entry
 *                point.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>
#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/ipc/internal/ipc_errors.h"
#include "amsr/ipc/internal/linux_os_api.h"
#include "amsr/ipc/internal/reactor_dispatcher_interface.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Implements the ReactorDispatcher.
 * \details The ReactorDispatcher implements a way for multiple different Reactor callbacks to have the same entry
 *          point.
 *
 * \tparam  Handler                     Class that handles the common Reactor callback entry point. Has to derive from
 *                                      std::enable_shared_from_this<Handler>. This makes sure that the Handler object
 *                                      is not destroyed as long as the common Reactor callback entry point can still be
 *                                      called. A std::shared_ptr<Handler> has to exist at the time Register() or
 *                                      RegisterSoftwareEvent() are called.
 * \tparam  ReactorCallbackEntryPoint   Method of Handler that will be used as common Reactor callback entry point. This
 *                                      means that the Reactor will first call this method for all
 *                                      ReactorDispatcherCallbacks that are registered to this ReactorDispatcher.
 *                                      From this method Dispatch() should be called which then calls the
 *                                      corresponding registered ReactorDispatcherCallback. Care has to be taken if
 *                                      ReactorDispatcher APIs are called from threads other than the Reactor event
 *                                      handling thread. In this case this function has to make sure that the call to
 *                                      Dispatch() is serialized with all other ReactorDispatcher API calls as the
 *                                      ReactorDispatcher is not thread-safe. Calling ReactorDispatcher APIs from a
 *                                      ReactorDispatcherCallback is allowed though.
 * \tparam  NumDispatchedCallbacks      Number of callback slots the ReactorDispatcher should have. This limits the
 *                                      maximum number of ReactorDispatcherCallbacks that can be registered at the same
 *                                      time.
 *
 * \unit    osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 */
template <class Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId callback_id,
                                                     osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
class ReactorDispatcher final : public ReactorDispatcherInterface {
 public:
  /*!
   * \brief           Creates a ReactorDispatcher.
   *
   * \param[in]       handler   Reference to object that handles the common Reactor callback entry point. The object
   *                            must exist or the whole lifetime of this ReactorDispatcher object.
   * \param[in]       reactor   Reactor to use for all callbacks that will be registered. Must exist for the whole
   *                            lifetime of this ReactorDispatcher object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit ReactorDispatcher(Handler& handler, osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      : ReactorDispatcherInterface{}, reactor_{reactor}, handler_{handler} {}

  ReactorDispatcher(ReactorDispatcher const& other) noexcept = delete;
  ReactorDispatcher(ReactorDispatcher&& other) noexcept = delete;
  ReactorDispatcher& operator=(ReactorDispatcher const& other) noexcept = delete;
  ReactorDispatcher& operator=(ReactorDispatcher&& other) noexcept = delete;

  /*!
   * \brief           Destroys the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ReactorDispatcher() noexcept final = default;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::Register()
   */
  auto Register(osabstraction::io::NativeHandle io_source, osabstraction::io::reactor1::EventTypes events,
                ReactorDispatcherCallback&& callback) noexcept -> ::amsr::core::Result<ReactorDispatcherId> final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::RegisterSoftwareEvent()
   */
  auto RegisterSoftwareEvent(ReactorDispatcherCallback&& callback) noexcept
      -> ::amsr::core::Result<ReactorDispatcherId> final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::SetMonitoredEvents()
   */
  void SetMonitoredEvents(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::AddMonitoredEvents()
   */
  void AddMonitoredEvents(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::RemoveMonitoredEvents()
   */
  void RemoveMonitoredEvents(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::TriggerSoftwareEvent()
   */
  void TriggerSoftwareEvent(ReactorDispatcherId id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::Unregister()
   */
  void Unregister(ReactorDispatcherId id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::UnregisterSoftwareEvent()
   */
  void UnregisterSoftwareEvent(ReactorDispatcherId id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc::internal::ReactorDispatcherInterface::Dispatch()
   */
  void Dispatch(ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept final;

 private:
  /*!
   * \brief Stores data for one registered ReactorDispatcherCallback.
   */
  class ReactorDispatcherCallbackEntry final {
   public:
    // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_DelegateConstructorParameterAreIncompatible
    /*!
     * \brief           Creates an empty, unused ReactorDispatcherCallbackEntry.
     *
     * \details         The sequence number of the entry will be 0.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     */
    ReactorDispatcherCallbackEntry() noexcept
        : handle_{osabstraction::io::reactor1::kInvalidCallbackHandle}, callback_{}, valid_{false}, sequence_num_{0} {};

    /*!
     * \brief           Creates a ReactorDispatcherCallbackEntry that is in use.
     *
     * \param[in]       handle         Handle of the Reactor callback that was registered for the ReactorDispatcher
     *                                 callback of this entry.
     * \param[in]       sequence_num   Sequence number that the created ReactorDispatcherCallbackEntry shall have.
     * \param[in]       callback       ReactorDispatcher callback that shall be stored in this entry. The entry takes
     *                                 ownership of this callback.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     */
    explicit ReactorDispatcherCallbackEntry(osabstraction::io::reactor1::CallbackHandle handle,
                                            std::uint64_t sequence_num, ReactorDispatcherCallback&& callback) noexcept
        : handle_{handle}, callback_{std::move(callback)}, valid_{true}, sequence_num_{sequence_num} {}

    /*!
     * \brief        Move constructs a ReactorDispatcherCallbackEntry.
     *
     * \param[in]    other   ReactorDispatcherCallbackEntry to move construct from.
     *
     * \context      ANY
     *
     * \pre          -
     *
     * \reentrant    FALSE
     * \synchronous  TRUE
     * \threadsafe   FALSE
     */
    ReactorDispatcherCallbackEntry(ReactorDispatcherCallbackEntry&& other) noexcept = default;

    /*!
     * \brief        Move assigns a ReactorDispatcherCallbackEntry.
     *
     * \param[in]    other   ReactorDispatcherCallbackEntry to move assign.
     *
     * \return       Reference to this object.
     *
     * \context      ANY
     *
     * \pre          This ReactorDispatcherCallbackEntry is invalid and not currently executing its callback.
     *
     * \reentrant    FALSE
     * \synchronous  TRUE
     * \threadsafe   FALSE
     */
    /*!
     * \internal
     *  - Check for self-assignment
     *  - If this is not the case
     *    - Assert that this object is invalid and has no callback currently executing.
     *    - Move member variables from other object.
     *    - Set other object to invalid with no callback executing.
     *  - Return reference to this object.
     * \endinternal
     */
    ReactorDispatcherCallbackEntry& operator=(ReactorDispatcherCallbackEntry&& other) & noexcept {
      if (this != &other) {
        if (IsValid() || IsCallbackExecuting()) {
          ::amsr::core::Abort(
              "ReactorDispatcherCallbackEntry object is still in use while other ReactorDispatcherCallbackEntry is "
              "assigned.");
        }

        handle_ = other.handle_;
        callback_ = std::move(other.callback_);
        valid_ = other.valid_;
        executing_ = other.executing_;
        sequence_num_ = other.sequence_num_;

        other.handle_ = osabstraction::io::reactor1::kInvalidCallbackHandle;
        other.valid_ = false;
        other.executing_ = false;
      }
      return *this;
    }

    ReactorDispatcherCallbackEntry(ReactorDispatcherCallbackEntry const& other) noexcept = delete;
    ReactorDispatcherCallbackEntry& operator=(ReactorDispatcherCallbackEntry const& other) noexcept = delete;

    /*!
     * \brief        Destroys the ReactorDispatcherCallbackEntry.
     *
     * \context      ANY
     *
     * \pre          This ReactorDispatcherCallbackEntry is invalid and not currently executing its callback.
     *
     * \reentrant    FALSE
     * \synchronous  TRUE
     * \threadsafe   FALSE
     */
    /*!
     * \internal
     *  - Assert that this object is invalid and has no callback currently executing.
     * \endinternal
     */
    ~ReactorDispatcherCallbackEntry() noexcept {
      if (IsValid() || IsCallbackExecuting()) {
        ::amsr::core::Abort("ReactorDispatcherCallbackEntry object is still in use during destruction.");
      }
    }

    /*!
     * \brief        Get Reactor CallbackHandle.
     */
    auto GetHandle() const noexcept -> osabstraction::io::reactor1::CallbackHandle { return handle_; }

    /*!
     * \brief        Get registered ReactorDisptacherCallback.
     */
    auto GetCallback() noexcept -> ReactorDispatcherCallback& { return callback_; };

    /*!
     * \brief        Check if the entry is valid.
     */
    auto IsValid() const noexcept -> bool { return valid_; }

    /*!
     * \brief        Check if the callback is currently executing.
     */
    auto IsCallbackExecuting() const noexcept -> bool { return executing_; }

    /*!
     * \brief        Get the sequence number.
     */
    auto GetSequenceNumber() const noexcept -> std::uint64_t { return sequence_num_; }

    /*!
     * \brief        Set this entry to invalid.
     */
    void Invalidate() noexcept { valid_ = false; };

    /*!
     * \brief        Set if the ReactorDisptacherCallback is currently executing.
     */
    void SetCallbackExecuting(bool executing) noexcept { executing_ = executing; }

    /*!
     * \brief        Destroy the stored ReactorDisptacherCallback.
     *
     * \context      ANY
     *
     * \pre          Callback is not currently executing.
     *
     * \reentrant    FALSE
     * \synchronous  TRUE
     * \threadsafe   FALSE
     */
    /*!
     * \internal
     *  - Assert that this object has no callback currently executing.
     * \endinternal
     */
    void ResetCallback() noexcept {
      if (IsCallbackExecuting()) {
        ::amsr::core::Abort(
            "Callback stored in ReactorDispatcherCallbackEntry cannot be reset as it is being executed.");
      }
      callback_ = ReactorDispatcherCallback{};
    };

   private:
    /*!
     * \brief   CallbackHandle for the callback registered in the Reactor.
     */
    osabstraction::io::reactor1::CallbackHandle handle_;

    /*!
     * \brief   Registered ReactorDispatcherCallback.
     */
    ReactorDispatcherCallback callback_;

    /*!
     * \brief   Flag that stores if the entry is valid.
     */
    bool valid_;

    /*!
     * \brief   Flag that stores if the ReactorDispatcherCallback is currently executing.
     */
    bool executing_{false};

    /*!
     * \brief   Sequence number of this callback entry.
     */
    std::uint64_t sequence_num_;
  };

  /*!
   * \brief   Storage type for registered ReactorDispatcherCallbacks.
   */
  using CallbackContainer = ::amsr::core::Array<ReactorDispatcherCallbackEntry, NumDispatchedCallbacks>;

  /*!
   * \brief        Finds registered callback associated with the passed callback identifier.
   *
   * \details      It is not checked if the sequence number of the passed callback identifier matches that of the
   *               registered callback entry.
   *
   * \param[in]    id   Identifier of a callback as returned by Register() or RegisterSoftwareEvent().
   *
   * \return       Reference to callback entry of registered callback.
   *
   * \context      ANY
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   */
  auto IdToEntry(ReactorDispatcherId id) noexcept -> ReactorDispatcherCallbackEntry&;

  /*!
   * \brief        Finds registered callback associated with the passed callback identifier and checks that the sequence
   *               number of the passed callback identifier matches that of the registered callback entry.
   *
   * \param[in]    id   Identifier of a callback as returned by Register() or RegisterSoftwareEvent() that is still
   *                    registered to the ReactorDispatcher.
   *
   * \return       Reference to callback entry of registered callback.
   *
   * \context      ANY
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   */
  auto IdToMatchingEntry(ReactorDispatcherId id) noexcept -> ReactorDispatcherCallbackEntry&;

  /*!
   * \brief   Reactor used for all callbacks.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief   Reference to object that handles the common Reactor callback entry point.
   */
  Handler& handler_;

  /*!
   * \brief   Storage for registered ReactorDispatcherCallbacks.
   */
  CallbackContainer dispatched_callbacks_{};

  /*!
   * \brief   Flag that indicates if the last executed ReactorDispatcherCallback shall be reset after the callback
   *          returned.
   * \details Set if the callback is unregistered but cannot be destroyed immediately because it is currently executing.
   */
  bool destroy_last_executed_callback_{false};
};

// VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_OSA_A15.5.3_ExceptionViolatesnoexceptSpecification_shared_from_this
// VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_OSA_A15.4.2_ExceptionViolatesnoexceptSpecification_shared_from_this
/*!
 * \internal
 *  - Find an unused callback slot. An used callback slot is a slot where the entry is invalid and the callback is not
 *    currently executing.
 *  - Return the kResource error if no unused slot was found.
 *  - Register a callback to the Reactor1 for the specified file descriptor and the specified events. A copy of the
 *    Handler shared pointer and the identifier of the registered ReactorDispatcherCallback is captured in this
 *    callback. The only thing that the callback does is to call the common Reactor callback entry point with the
 *    reported Reactor events and the identifier of the ReactorDispatcherCallback. This identifier contains a sequence
 *    number that allows to detect outdated (already unregistered) callbacks in the Dispatch() call from the common
 *    Reactor callback entry point.
 *  - If the registration failed
 *    - Map the error and return it
 *  - Otherwise return the identifier of the ReactorDispatcherCallback.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
auto ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::Register(
    osabstraction::io::NativeHandle io_source, osabstraction::io::reactor1::EventTypes events,
    ReactorDispatcherCallback&& callback) noexcept -> ::amsr::core::Result<ReactorDispatcherId> {
  ::amsr::core::Result<ReactorDispatcherId> result{MakeErrorCode(
      osabstraction::OsabErrc::kResource, "Cannot add another callback to ReactorDispatcher as all slots are used.")};

  typename CallbackContainer::iterator const callback_container_slot{
      std::find_if(std::begin(dispatched_callbacks_), std::end(dispatched_callbacks_),
                   [](ReactorDispatcherCallbackEntry const& callback_entry) -> bool {
                     return (!callback_entry.IsValid()) && (!callback_entry.IsCallbackExecuting());
                   })};

  if (callback_container_slot != std::end(dispatched_callbacks_)) {
    ReactorDispatcherId const new_id{
        static_cast<std::size_t>(std::distance(std::begin(dispatched_callbacks_), callback_container_slot)),
        callback_container_slot->GetSequenceNumber() + 1};
    result = reactor_
                 .Register(io_source, events,
                           [shared_ptr_handler = handler_.shared_from_this(), new_id](
                               osabstraction::io::reactor1::CallbackHandle,
                               osabstraction::io::reactor1::EventTypes reported_events) {
                             (*shared_ptr_handler.*ReactorCallbackEntryPoint)(new_id, reported_events);
                           })
                 .MapError([](::amsr::core::ErrorCode const& error) -> ::amsr::core::ErrorCode {
                   return os_interface::MapReactorRegistrationErrors(error);
                 })
                 .Map([callback_container_slot, &callback,
                       new_id](osabstraction::io::reactor1::CallbackHandle callback_handle) -> ReactorDispatcherId {
                   *callback_container_slot = std::move(
                       ReactorDispatcherCallbackEntry{callback_handle, new_id.sequence_num, std::move(callback)});
                   return ReactorDispatcherId{new_id};
                 });
  }

  return result;
}

// VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_OSA_A15.5.3_ExceptionViolatesnoexceptSpecification_shared_from_this
// VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_OSA_A15.4.2_ExceptionViolatesnoexceptSpecification_shared_from_this
/*!
 * \internal
 *  - Find an unused callback slot. An used callback slot is a slot where the entry is invalid and the callback is not
 *    currently executing.
 *  - Return the kResource error if no unused slot was found.
 *  - Register a software events callback to the Reactor1. A copy of the Handler shared pointer and the identifier of
 *    the registered ReactorDispatcherCallback is captured in this callback. The only thing that the callback does is to
 *    call the common Reactor callback entry point with the reported Reactor events and the identifier of the
 *    ReactorDispatcherCallback. This identifier contains a sequence number that allows to detect outdated (already
 *    unregistered) callbacks in the Dispatch() call from the common Reactor callback entry point.
 *  - If the registration failed
 *    - Map the error and return it
 *  - Otherwise return the identifier of the ReactorDispatcherCallback.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
auto ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::RegisterSoftwareEvent(
    ReactorDispatcherCallback&& callback) noexcept -> ::amsr::core::Result<ReactorDispatcherId> {
  ::amsr::core::Result<ReactorDispatcherId> result{MakeErrorCode(
      osabstraction::OsabErrc::kResource, "Cannot add another callback to ReactorDispatcher as all slots are used.")};

  typename CallbackContainer::iterator const callback_container_slot{
      std::find_if(std::begin(dispatched_callbacks_), std::end(dispatched_callbacks_),
                   [](ReactorDispatcherCallbackEntry const& callback_entry) -> bool {
                     return (!callback_entry.IsValid()) && (!callback_entry.IsCallbackExecuting());
                   })};

  if (callback_container_slot != std::end(dispatched_callbacks_)) {
    ReactorDispatcherId const new_id{
        static_cast<std::size_t>(std::distance(std::begin(dispatched_callbacks_), callback_container_slot)),
        callback_container_slot->GetSequenceNumber() + 1};
    result = reactor_
                 .RegisterSoftwareEvent([shared_ptr_handler = handler_.shared_from_this(), new_id](
                                            osabstraction::io::reactor1::CallbackHandle,
                                            osabstraction::io::reactor1::EventTypes reported_events) {
                   (*shared_ptr_handler.*ReactorCallbackEntryPoint)(new_id, reported_events);
                 })
                 .MapError([](::amsr::core::ErrorCode const& error) -> ::amsr::core::ErrorCode {
                   return os_interface::MapReactorSoftwareEventRegistrationErrors(error);
                 })
                 .Map([callback_container_slot, &callback,
                       new_id](osabstraction::io::reactor1::CallbackHandle callback_handle) -> ReactorDispatcherId {
                   *callback_container_slot = std::move(
                       ReactorDispatcherCallbackEntry{callback_handle, new_id.sequence_num, std::move(callback)});
                   return ReactorDispatcherId{new_id};
                 });
  }

  return result;
}

/*!
 * \internal
 *  - Call Reactor::SetMonitoredEvents() for the Reactor callback associated with the passed ReactorDispatcherCallback
 *    identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::SetMonitoredEvents(
    ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept {
  ::amsr::core::Result<void> const result{reactor_.SetMonitoredEvents(IdToEntry(id).GetHandle(), events)};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorChangeRegistrationErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Call Reactor::AddMonitoredEvents() for the Reactor callback associated with the passed ReactorDispatcherCallback
 *    identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::AddMonitoredEvents(
    ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept {
  ::amsr::core::Result<void> const result{reactor_.AddMonitoredEvents(IdToMatchingEntry(id).GetHandle(), events)};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorChangeRegistrationErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Call Reactor::RemoveMonitoredEvents() for the Reactor callback associated with the passed
 *    ReactorDispatcherCallback identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::RemoveMonitoredEvents(
    ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept {
  ::amsr::core::Result<void> const result{reactor_.RemoveMonitoredEvents(IdToMatchingEntry(id).GetHandle(), events)};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorChangeRegistrationErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Call Reactor::TriggerSoftwareEvent() for the Reactor callback associated with the passed ReactorDispatcherCallback
 *    identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::TriggerSoftwareEvent(
    ReactorDispatcherId id) noexcept {
  ::amsr::core::Result<void> const result{reactor_.TriggerSoftwareEvent(IdToMatchingEntry(id).GetHandle())};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorTriggerSoftwareEventErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Call Reactor::Unregister() for the Reactor callback associated with the passed ReactorDispatcherCallback
 *    identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 *  - Invalidate the callback entry.
 *  - Reset the stored callback if it is not executing right now.
 *  - Otherwise mark the callback to be reset later.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::Unregister(
    ReactorDispatcherId id) noexcept {
  ReactorDispatcherCallbackEntry& callback_entry{IdToMatchingEntry(id)};

  ::amsr::core::Result<void> const result{
      reactor_.Unregister(callback_entry.GetHandle(), osabstraction::io::reactor1::CloseOnUnregister::kDoNotCloseHandle)
          .Inspect([this, &callback_entry]() -> void {
            callback_entry.Invalidate();
            if (!callback_entry.IsCallbackExecuting()) {
              callback_entry.ResetCallback();
            } else {
              destroy_last_executed_callback_ = true;
            }
          })};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorUnregistrationErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Call Reactor::UnregisterSoftwareEvent() for the Reactor callback associated with the passed
 *    ReactorDispatcherCallback identifier.
 *  - If this failed
 *    - Map the error. This aborts as all errors are unexpected.
 *  - Invalidate the callback entry.
 *  - Reset the stored callback if it is not executing right now.
 *  - Otherwise mark the callback to be reset later.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::UnregisterSoftwareEvent(
    ReactorDispatcherId id) noexcept {
  ReactorDispatcherCallbackEntry& callback_entry{IdToMatchingEntry(id)};

  ::amsr::core::Result<void> const result{
      reactor_.UnregisterSoftwareEvent(callback_entry.GetHandle()).Inspect([this, &callback_entry]() -> void {
        callback_entry.Invalidate();
        if (!callback_entry.IsCallbackExecuting()) {
          callback_entry.ResetCallback();
        } else {
          destroy_last_executed_callback_ = true;
        }
      })};

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
  if (!result.HasValue()) {
    os_interface::MapReactorSoftwareEventUnregistrationErrors(result.Error());
  }
}

/*!
 * \internal
 *  - Find the entry associated with the passed ReactorDispatcherCallback identifier.
 *  - If the entry was not unregistered or reused yet
 *    - Set the callback executing flag
 *    - Call the ReactorDispatcherCallback with the occurred events.
 *    - Reset the callback executing flag
 *    - If the callback was marked to be destroyed
 *      - Reset the callback.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
void ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::Dispatch(
    ReactorDispatcherId id, osabstraction::io::reactor1::EventTypes events) noexcept {
  ReactorDispatcherCallbackEntry& registered_callback{IdToEntry(id)};
  if (registered_callback.IsValid() && (registered_callback.GetSequenceNumber() == id.sequence_num)) {
    registered_callback.SetCallbackExecuting(true);
    registered_callback.GetCallback()(std::move(events));
    registered_callback.SetCallbackExecuting(false);
    if (destroy_last_executed_callback_) {
      registered_callback.ResetCallback();
      destroy_last_executed_callback_ = false;
    }
  }
}

/*!
 * \internal
 *  - Assert that the passed identifier is not out-of-range.
 *  - Return the callback entry associated with the identifier.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
auto ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::IdToEntry(
    ReactorDispatcherId id) noexcept -> ReactorDispatcherCallbackEntry& {
  if (id.index >= dispatched_callbacks_.size()) {
    ::amsr::core::Abort("Invalid ReactorDispatcherId passed to ReactorDispatcherId::IdToEntry().");
  }

  return dispatched_callbacks_[id.index];
}

/*!
 * \internal
 *  - Assert that the passed identifier is not out-of-range.
 *  - Return the callback entry associated with the identifier.
 * \endinternal
 */
template <typename Handler,
          void (Handler::*ReactorCallbackEntryPoint)(ReactorDispatcherId, osabstraction::io::reactor1::EventTypes),
          std::size_t NumDispatchedCallbacks>
auto ReactorDispatcher<Handler, ReactorCallbackEntryPoint, NumDispatchedCallbacks>::IdToMatchingEntry(
    ReactorDispatcherId id) noexcept -> ReactorDispatcherCallbackEntry& {
  // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OutOfBoundsCheck_FalsePositive
  if ((id.index >= dispatched_callbacks_.size()) ||
      (dispatched_callbacks_[id.index].GetSequenceNumber() != id.sequence_num)) {
    ::amsr::core::Abort("Invalid ReactorDispatcherId passed to ReactorDispatcherId::IdToMatchingEntry().");
  }

  // VECTOR Next Line AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OutOfBoundsCheck_FalsePositive
  return dispatched_callbacks_[id.index];
}

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_REACTOR_DISPATCHER_H_
