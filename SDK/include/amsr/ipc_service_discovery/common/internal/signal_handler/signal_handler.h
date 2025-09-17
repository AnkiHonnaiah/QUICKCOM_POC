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
 *  \brief SignalHandler to handle received termination or interrupt signals.
 *  \unit IpcServiceDiscovery::Common::SignalHandler
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_

#include <atomic>
#include <memory>

#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/signal_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/signal_manager.h"
namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Listens for termination signal
 * \details SignalHandler only handles the termination and interrupt signal. A caller can query if any termination or
 *          interrupt request has been received. SignalHandler changes its internal state to termination request
 *          received on any termination signal. It changes its internal state to interrupt request received on any
 *          interrupt signal. The states cannot be reset later one.
 * \vprivate Component Private
 */

class SignalHandler final {
 public:
  /*!
   * \brief Construct a SignalHandler to listen for termination and interrupt signals.
   * \param[in] reactor Reactor to execute the signal handler callback. Must be valid
   *                    for SignalHandler's entire lifetime.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   */
  explicit SignalHandler(external::internal::osabstraction::Reactor1Interface& reactor) noexcept;

  /*!
   * \brief Registers signal handler for termination signal to reactor.
   * \error osabstraction::OsabErrc::kResource Initialization of SignalManager failed
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   \trace DSGN-IpcServiceDiscovery-Common-SignalHandlerRegister
   */
  auto Register() noexcept -> ara::core::Result<void>;

  /*!
   * \brief Checks whether a termination request has been received or not.
   * \return True if termination request has been received. Otherwise, false.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   */
  auto TerminationRequestReceived() const noexcept -> bool;

  /*!
   * \brief Checks whether an interrupt request has been received or not.
   * \return True if interrupt request has been received. Otherwise, false.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   */
  auto InterruptRequestReceived() const noexcept -> bool;

 private:
  /*! Stores whether a termination request has been received or not.*/
  std::atomic<bool> has_termination_request_received_{false};

  /*! Stores whether an interrupt request has been received or not.*/
  std::atomic<bool> has_interrupt_request_received_{false};

  /*! Reference to reactor that call internal callback once a termination request has been received.*/
  external::internal::osabstraction::Reactor1Interface& reactor_;

  /*! OS dependent signal manager to listen for incoming signals. */
  std::unique_ptr<external::internal::osabstraction::SignalManager> sig_manager_ptr_{nullptr};
};

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_
