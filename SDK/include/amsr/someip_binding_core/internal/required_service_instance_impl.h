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
/*!        \file  amsr/someip_binding_core/internal/required_service_instance_impl.h
 *        \brief  ara::com SOME/IP Binding Required Service Instance implementation
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::RequiredServiceInstance
 *
 *      \details  RequiredServiceInstance allows monitoring the status of found remote services
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_discovery_handler_interface.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/service_listener_registry_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/core/vector.h"

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace amsr {
namespace someip_binding_core {
namespace internal {
/*!
 * \brief Realizes a concrete RequiredServiceInstance and allows service listeners to receive
 *        notifications about the required service instance being up and down.
 *        For each SdProxyXf, there is only one object of this type.
 */
class RequiredServiceInstanceImpl final : public service_discovery::ServiceListenerInterface,
                                          public ServiceListenerRegistryInterface {
 public:
  /*!
   * \brief       Constructor.
   * \param[in]   required_service_instance_id The required service instance id
   * \param[in]   service_information          Service interface information used in the service instance.
   * \param[in]   client_manager               A reference to the client manager. Used to register a newly created
   *                                           proxy binding upon calling CreateBackend.
   * \param[in]   service_discovery_handler    A reference to the service discovery handler. Used to listen to
   *                                           ServiceDiscovery updates.
   * \param[in]   client_transport_to_daemon   A reference to the client transport to daemon. Used to access
   *                                           SOME/IP Daemon to request service.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  RequiredServiceInstanceImpl(RequiredServiceInstanceId required_service_instance_id,
                              ServiceInformation&& service_information, ClientManagerInterface& client_manager,
                              service_discovery::ServiceDiscoveryHandlerInterface& service_discovery_handler,
                              ClientTransportToDaemonInterface& client_transport_to_daemon) noexcept;

  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl const&) = delete;
  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl&&) = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl const&) & = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl&&) & = delete;

  /*!
   * \brief       Destructor.
   * \pre         There must be no registered listeners left
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~RequiredServiceInstanceImpl() noexcept final;

  /*!
   * \brief        Registers a listener
   * \param[in]    listener The listener to register
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires listener != nullptr;
   * \endspec
   */
  void RegisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept final;

  /*!
   * \brief        Unregisters a listener
   * \param[in]    listener The listener to unregister
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires listener != nullptr;
   * \endspec
   */
  void UnregisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept final;

  /*!
   * \brief        Creates a binding client for a specific service id
   * \param[in]    service_id The service id
   * \return       amsr::core::Result with the created client or an error
   * \error       ClientError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ClientError::kSystemConfigurationNotAvailable if this required service instance is not configured.
   * \error       ClientError::kSystemPrivilegesNoAccess if access is denied for this service.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<LocalClient> CreateClient(ProvidedServiceInstanceId const& service_id) noexcept;

  /*!
   * \brief       Start Service Discovery for the given service.
   *
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     Init, App (If RestartServiceDiscovery is used)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \vprivate
   */
  amsr::core::Result<void> StartServiceDiscovery() noexcept;

  /*!
   * \brief       Stop Service Discovery for the given service.
   *
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     Shutdown, App (If RestartServiceDiscovery is used)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \vprivate
   */
  amsr::core::Result<void> StopServiceDiscovery() noexcept;

 private:
  /*!
   * \brief         Inform all listeners about the availability change of the given service.
   * \param[in]     offered_service The provided service instance ID of the offered service.
   * \pre           -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

  /*!
   * \brief         Callback function to be called from binding whenever one service instance is stopped.
   * \param[in]     stopped_service The provided service instance ID of the stopped service.
   * \pre           -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceDown(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final;

  /*!
   * \brief The required service instance id
   */
  RequiredServiceInstanceId const required_service_instance_id_;

  /*!
   * \brief Service interface information used in the service instance.
   */
  ServiceInformation service_information_;

  /*!
   * \brief A reference to the client manager
   */
  ClientManagerInterface& client_manager_;

  /*!
   * \brief A reference to the service discovery handler
   */
  service_discovery::ServiceDiscoveryHandlerInterface& service_discovery_handler_;

  /*!
   * \brief A reference to the client transport to daemon
   */
  ClientTransportToDaemonInterface& client_transport_to_daemon_;

  /*!
   * \brief Container for found service instances.
   * \details This container shall contain at most one entry when a specific service instance id is
   *          configured, and more than one entries in case RequiredInstanceIdAll is configured
   */
  ara::core::Vector<ProvidedServiceInstanceId> found_instances_{};

  /*!
   * \brief Container of service listeners
   * \details By design, every listener must be removed from this container before destruction.
   * Using shared pointers to guard against implementation issues could create deadlocks.
   * Therefore using weak pointes to detect bugs.
   */
  ara::core::Vector<std::weak_ptr<service_discovery::ServiceListenerInterface>> listeners_{};

  /*!
   * \brief Lock for avoiding concurrent read/write of listeners_
   */
  std::mutex service_listeners_lock_{};

  /*!
   * \brief   true if service discovery is started (i.e. StartServiceDiscovery is called and StopServiceDiscovery is not
   *          called), false otherwise.
   * \details This is only accessed from App thread to avoid duplicate start or stop of SD and to implicitly stop it
   *          within dtor.
   */
  bool is_sd_started_{false};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::RequiredServiceInstance::service_listeners_lock_
 *                Ensures consistency while read and write access to the container of service listeners.
 *
 * \protects ::amsr::someip_binding_core::internal::RequiredServiceInstanceImpl::listeners_
 *           is the protected resource and contains all registered service listeners.
 *
 * \usedin OnServiceInstanceUp(read), OnServiceInstanceDown(read), RegisterListener(write)
 *
 * \length MEDIUM Limited to a map lookup, an emplacement if a listener is added to the vector,
 *                a removal of a listener from the vector,
 *                a reception of a service state update callback.
 *                The exclusive area starts and ends always in the smallest possible scope within the protected methods.
 *                This exclusive area covers calls to at most one method. The complexity is logarithmic in the size of
 *                the container.
 * \endexclusivearea
 */
