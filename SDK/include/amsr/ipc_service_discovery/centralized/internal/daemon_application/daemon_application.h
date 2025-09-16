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
 *  \brief Server application for the IpcServiceDiscovery centralized daemon application.
 *  \unit IpcServiceDiscovery::Centralized::DaemonApplication
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_DAEMON_APPLICATION_DAEMON_APPLICATION_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_DAEMON_APPLICATION_DAEMON_APPLICATION_H_

#include "amsr/ipc_service_discovery/centralized/internal/config/config.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_server/sd_server.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/signal_handler/signal_handler.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "ara/core/initialization.h"
#include "ara/core/result.h"

// DISCLAIMER: Only the variant with enabled EM is tested, as disabling EM support was deemed
// low risk by safety coaches and management. Therefore, no variant testing is needed for this
// preprocessor switch when it is disabled (not defined).

#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
#include "amsr/ipc_service_discovery/external/internal/executionmanager/application_client.h"
#endif

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Implementation of the IpcServiceDiscovery daemon application.
 * \details DaemonApplication creates the reactor and the service discovery server and implements the reactor loop. The
 *          internal reactor loop breaks if it receives a termination/interrupt request (e.g. SIGTERM
 *          from Execution Management) or the result of the internal reactor process returns an error.
 * \vprivate Component Private
 */
class DaemonApplication final {
 public:
  /*!
   * \brief Initialize the application members.
   * \details The initialization process contains initialization of the ARA framework, construction of the reactor,
   *          initialization of Logger, SsServer & ApplicationCLient and the registration of termination and interrupt
   *          signals in SignalHandler.
   * \error amsr::application_base::InitializationErrc::kWrongSequence ARA framework  already initialized.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \trace DSGN-IpcServiceDiscovery-Centralized-DaemonApplicationInitialization
   */
  auto Initialize() noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Run the reactor loop.
   * \details This method sets the SdServer in the start state, reports the application state to the execution manager
   *          and then runs the reactor loop. When an registered termination and interrupt signal is received or an
   *          error occurs while handling reactor events the methods exists.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-DaemonApplicationRun
   */
  auto Run() const noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Deinitialize the deamon application by resetting all members and checking for errors.
   * \error amsr::application_base::InitializationErrc::kWrongSequence ARA framework could not correctly be
   *                                                                   deinitialized
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \threadsafe FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-DaemonApplicationDeinitialization
   */
  auto Deinitialize() noexcept -> ::ara::core::Result<void>;

 private:
  /*! Reactor for software events. */
  std::unique_ptr<external::internal::osabstraction::Reactor1Interface> reactor_{};

  /*! Runtimes logger. */
  std::unique_ptr<::amsr::ipc_service_discovery::common::internal::Logger> logger_{};

  /*! IpcServiceDiscovery server for handling of client communication. */
  std::unique_ptr<SdServer> sd_server_{};

// DISCLAIMER: Only the variant with enabled EM is tested, as disabling EM support was deemed
// low risk by safety coaches and management. Therefore, no variant testing is needed for this
// preprocessor switch when it is disabled (not defined).

// VECTOR NL AutosarC++17_10-A16.0.1: MD_IPCSERVICEDISCOVERY_A16.0.1_conditionalCompilation
#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
  /*! Client used for state reporting to EM. */
  std::unique_ptr<external::internal::executionmanager::ApplicationClient> app_client_{};
#endif

  /*! Registers event handler for termination signal in reactor. */
  std::unique_ptr<SignalHandler> signal_handler_{};
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_DAEMON_APPLICATION_DAEMON_APPLICATION_H_
