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
/*!        \file  someip_binding_core/internal/someip_binding_core.h
 *        \brief  SomeIpBindingCore serves as an anchor between the SOME/IP daemon and the ara::com related
 *                high-level layers.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpBindingCore
 *      \details  -
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding_core/internal/client_manager.h"
#include "amsr/someip_binding_core/internal/client_receive_from_daemon.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon.h"
#include "amsr/someip_binding_core/internal/constants.h"
#include "amsr/someip_binding_core/internal/local_server_manager.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/required_service_instance_impl.h"
#include "amsr/someip_binding_core/internal/required_service_instance_manager.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/server_manager.h"
#include "amsr/someip_binding_core/internal/server_receive_from_daemon.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_discovery_handler.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "amsr/someip_binding_core/internal/someip_daemon_client_interface.h"
#include "amsr/someip_binding_core/internal/someip_daemon_client_wrapper.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "vac/language/token_init.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief  Realizes a concrete Binding for the proxy/skeleton communication via SOME/IP and serves as an
 *         anchor between the application-side and the communication protocol of SOME/IP.
 *         For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
// VECTOR NC AutosarC++17_10-M3.2.2: MD_SOMEIPBINDING_AutosarC++17_10-M3.2.2_redefinition
// VECTOR NC Linker-ODR_Violation_In_Fields: MD_SOMEIPBINDING_Linker-ODR_Violation_In_Fields_redefinition
class SomeIpBindingCore final : public SomeIpBindingCoreInterface {
 public:
  /*!
   * \brief Type-alias for the ClientManager.
   */
  using ClientManager = ::amsr::someip_binding_core::internal::ClientManager;

  /*!
   * \brief Type-alias for the ServerManager.
   */
  using ServerManager = ::amsr::someip_binding_core::internal::ServerManager;

  /*!
   * \brief Type-alias for the LocalServerManager.
   */
  using LocalServerManager = ::amsr::someip_binding_core::internal::LocalServerManager;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief Type-alias for the LocalServer.
   */
  using LocalServer = ::amsr::someip_binding_core::internal::LocalServer;

  /*!
   * \brief       Constructor of this binding initializes the attributes of this class.
   * \param[in]   someip_daemon_client_wrapper  SomeIpdClient wrapper.
   * \param[in]   safe_ipc_polling             Contains reactor polling function to be called while waiting for
   *                                           a command response (only in Polling Mode).
   * \param[in]   is_polling_mode              A reference to a bool determining if polling mode is used.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpBindingCore(std::unique_ptr<SomeIpDaemonClientInterface> someip_daemon_client_wrapper,
                    std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling, bool const& is_polling_mode) noexcept;

  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SomeIpBindingCore() noexcept final;

  SomeIpBindingCore() = delete;
  SomeIpBindingCore(SomeIpBindingCore const&) = delete;
  SomeIpBindingCore(SomeIpBindingCore&&) = delete;
  SomeIpBindingCore& operator=(SomeIpBindingCore const&) & = delete;
  SomeIpBindingCore& operator=(SomeIpBindingCore&&) & = delete;

  /*!
   * \brief       Start all dynamic actions of the SOME/IP binding.
   * \details     In this API context all events triggered by the reactor thread must be activated / registered:
   *              - Register received events
   *              - Start any message transmission
   *              - Start timers
   * \param[in]   connection_retrial_enabled  Flag to enable connection retrial after a failed connection attempt.
   * \pre         Initialize() has been called.
   * \context     Init, Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \error kRuntimeConnectionTimeout The connection timed out.
   * \error any_error_code The connection failed.
   * \internal steady FALSE \endinternal
   * \return Connection result without any value.
   *
   */
  ::amsr::core::Result<void> Start(bool connection_retrial_enabled) noexcept final;

  /*!
   * \brief       Emplace a local server implementation with the given ID
   *              into the LocalServerManager.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   required_handler_set  Set of skeleton method request handlers for which request handlers have to be
   *                                    registered.
   *
   * \pre         The given provided service instance ID must not have been
   *              previously registered.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void EmplaceLocalServer(
      ProvidedServiceInstanceId const& service_instance_id,
      SomeIpBindingCoreInterface::RequiredSkeletonMethodHandlerSet const& required_handler_set) noexcept final;

  /*!
   * \brief       Create a server instance with the given ID.
   *
   * \param[in]   service_instance_id   The service instance id.
   *
   * \return      The created server instance.
   *
   * \error       ServerError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ServerError::kSystemConfigurationNotAvailable if this service instance is not configured.
   * \error       ServerError::kSystemPrivilegesNoAccess if access is denied for this service.
   *              configured.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         -
   *
   * \context     App (Skeleton ctor)
   * \threadsafe  TRUE (RequestLocalServer can be called from different threads in parallel)
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<LocalServer> RequestLocalServer(
      ProvidedServiceInstanceId const& service_instance_id) noexcept final;

  /*!
   * \brief       Emplace a required service instance implementation with the given ID
   *              into the RequiredServiceInstanceManager.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   service_information   Service interface information used in the service instance.
   *
   * \pre         The given required service instance ID must not have been previously registered.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void EmplaceRequiredServiceInstance(RequiredServiceInstanceId const& service_instance_id,
                                      ServiceInformation&& service_information) noexcept final;

  /*!
   * \brief        Creates a required service instance for a specific service id
   * \param[in]    service_instance_id The required service instance id
   *
   * \return       amsr::core::Result with the created required service instance or an error
   *
   * \error       ClientError::kSystemConfigurationNotAvailable If the given instance id is not configured.
   * \error       SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   *
   * \pre          -
   *
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<RequiredServiceInstance> RequestRequiredServiceInstance(
      RequiredServiceInstanceId const& service_instance_id) noexcept final;

  /*!
   * \copydoc SomeIpBindingCoreInterface::IsRequiredServiceInstanceIdConfigured
   */
  auto IsRequiredServiceInstanceIdConfigured(RequiredServiceInstanceId const& required_service_instance_id) noexcept
      -> bool final {
    return required_service_instance_manager_.IsRequiredServiceInstanceIdConfigured(required_service_instance_id);
  }

  /*!
   * \copydoc SomeIpBindingCoreInterface::IsProvidedServiceInstanceIdConfigured
   */
  auto IsProvidedServiceInstanceIdConfigured(ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> bool final {
    return local_server_manager_->IsProvidedServiceInstanceIdConfigured(provided_service_instance_id);
  }

  /*!
   * \brief       Start Service Discovery for all registered required service instances.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     Init, Reactor (If connection retrial is enabled), App (If RestartServiceDiscovery is used)
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void StartServiceDiscovery() noexcept final;

  /*!
   * \brief       Stop Service Discovery for all registered required service instances.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     Shutdown, App (If RestartServiceDiscovery is used)
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void StopServiceDiscovery() const noexcept final;

  /*!
   * \brief       Get whether the Daemon connection has succeeded. Is not reset if the connection is lost.
   *
   * \return      True if the Daemon connection has succeeded once.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::atomic_bool const& SomeIpDaemonConnectionSucceeded() const noexcept final;

 private:
  /*!
   * \brief SOME/IP daemon client wrapper
   */
  std::unique_ptr<SomeIpDaemonClientInterface> someip_daemon_client_wrapper_;

  /*!
   * \brief Optional of the SOME/IP daemon connection future.
   */
  ::amsr::core::Optional<::amsr::core::Future<void>> daemon_connection_future_{};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "SomeIpBindingCore"_sv};

  /*!
   * \brief Flag indicating that the connection to the SomeIpDaemon was successfully established. This value is not
   * updated on connection loss but only indicated that it was connected successfully once.
   */
  std::atomic_bool connection_succeeded_{false};

  /*!
   * \brief Check if polling mode is used.
   */
  bool const is_polling_mode_;

  /*!
   * \brief Flag to store if it was the first try which requires a blocking wait.
   */
  bool first_connection_try_{true};

  /*!
   * \brief Interface for polling wrappers.
   */
  std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling_;

  /*!
   * \brief Instance that is used to transport client communication to SOME/IP Daemon.
   */
  ClientTransportToDaemon client_transport_to_daemon_;

  /*!
   * \brief Instance that is used to transport server communication to SOME/IP Daemon.
   */
  ServerTransportToDaemon server_transport_to_daemon_;

  /*!
   * \brief ServerReceiveFromDaemon lives inside the scope of SomeIpBindingCore instance.
   */
  ServerReceiveFromDaemon server_receive_from_daemon_;

  /*!
   * \brief ServiceDiscoveryHandler lives inside the scope of SomeIpBindingCore instance.
   */
  service_discovery::ServiceDiscoveryHandler service_discovery_handler_{};

  /*!
   * \brief ClientManager lives inside the scope of SomeIpBindingCore instance.
   */
  ClientManager client_manager_{};

  /*!
   * \brief ServerManager lives inside the scope of  SomeIpBindingCore instance.
   */
  ServerManager server_manager_{};

  /*!
   * \brief ClientReceiveFromDaemon lives inside the scope of SomeIpBindingCore instance.
   */
  ClientReceiveFromDaemon client_receive_from_daemon_{};

  /*!
   * \brief LocalServerManager lives inside the scope of SomeIpBindingCore instance.
   *        The main purpose of this unit is to manage the resources associated with
   *        the concrete implementations for the local servers created upon initialization.
   *        This is done by requesting an specific provided service ID that will transfer
   *        ownership of the local server implementation, and that will later be released
   *        and returned to the LocalServerManager once not needed anymore.
   */
  std::shared_ptr<LocalServerManager> local_server_manager_{};

  /*!
   * \brief Container for all configured required service instances.
   *        The instances are added during init phase, before starting the binding.
   *        They are used to monitor SD updates and creating SOME/IP clients.
   */
  RequiredServiceInstanceManager required_service_instance_manager_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_H_
