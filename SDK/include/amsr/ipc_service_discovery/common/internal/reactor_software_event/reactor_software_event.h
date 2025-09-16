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
/*! \file
 *  \brief Abstraction of a single reactor software event.
 *  \unit IpcServiceDiscovery::Common::ReactorSoftwareEvent
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_REACTOR_SOFTWARE_EVENT_REACTOR_SOFTWARE_EVENT_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_REACTOR_SOFTWARE_EVENT_REACTOR_SOFTWARE_EVENT_H_

#include <mutex>
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/types/constants.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief A helper class for a single reactor software event.
 * \details The ReactorSoftwareEvent is used to register a single software event by the given reactor.
 *          The provided callback will be called as soon as the triggered software event is executed
 *          by the reactor. Furthermore, reasonable error handling is implemented.
 *
 * \vprivate Component internal.
 */
class ReactorSoftwareEvent {
 public:
  /*!
   * \brief Interface of the callback function that handles the software event.
   * \context REACTOR
   * \pre
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  using SoftwareEventCallback = vac::language::UniqueFunction<void()>;

  ReactorSoftwareEvent(external::internal::osabstraction::Reactor1Interface& reactor,
                       SoftwareEventCallback software_event_callback);

  /*!
   * \brief Register the software event by the reactor.
   * \details An old handle will be unregistered if set.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Register() noexcept;

  /*!
   * \brief Unregister the software event by the reactor.
   * \details The software event will only be unregistered if a handle is set.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Unregister() noexcept;

  /*!
   * \brief Trigger the software event by the reactor.
   * \context ANY
   * \pre The software event must be registered by the reactor.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Trigger() const noexcept;

  /*!
   * \brief Returns whether resources of a callback are still in use.
   * \return true if resources are still in use, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInUse() const noexcept -> bool;

 private:
  /*!
   * \brief Register the software event by the reactor.
   * \details An old handle will be unregistered if set.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RegisterInternal() noexcept;

  /*!
   * \brief Unregister the software event by the reactor.
   * \details The software event will only be unregistered if a handle is set.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void UnregisterInternal() noexcept;

  /*!
   * \brief Callback to reactor software event.
   * \param[in] callback_handle The callback handle. Must comply with the registered software event handle.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnSoftwareEvent(osabstraction::io::reactor1::CallbackHandle callback_handle) noexcept;

  /*! Reference to the reactor. */
  external::internal::osabstraction::Reactor1Interface& reactor_;

  /*! Logger. */
  Logger logger_{Constants::kLoggerPrefixReactorSoftwareEvent};

  /*! Callback for software event. */
  SoftwareEventCallback software_event_callback_;

  /*! Reactor software event callback handle. */
  osabstraction::io::reactor1::CallbackHandle software_event_reactor_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*! Mutex to guard the software event reactor handle against concurrent access. */
  std::mutex mutable reactor_handle_lock_{};
};

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_REACTOR_SOFTWARE_EVENT_REACTOR_SOFTWARE_EVENT_H_
