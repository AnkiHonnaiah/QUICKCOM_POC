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
/*!        \file  amsr/someip_binding_core/internal/required_service_instance_manager.h
 *        \brief  ara::com SOME/IP Binding Required Service Instance manager
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::RequiredServiceInstanceManager
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_MANAGER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>

#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/required_service_instance_impl.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_discovery_handler_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief    Manages life cycle of RequiredServiceInstanceImpl instances.
 * \details  This class has 3 phases:
 *           - Init: triggered during Binding initialization
 *              - APIs:
 *                - EmplaceRequiredServiceInstance: To prepare the class, all required service instances shall be
 *                                                  emplaced during initialization.
 *           - Runtime: Once initialization is done, runtime phase starts, where SD and communication functionality
 *                      could be used.
 *              - APIs:
 *                - StartServiceDiscovery: Start the service discovery for ALL registered required service instances.
 *                - StopServiceDiscovery: Stop the service discovery for ALL registered required service instances.
 *                - RequestRequiredServiceInstance: get a reference of one of the emplaced required service instances.
 *              Note: although it is possible to call RequestRequiredServiceInstance for different instances and then
 *                    call Start/StopServiceDiscovery on the retrieved instance, the Start/StopServiceDiscovery APIs
 *                    are introduced to allow the caller to start SD for all instances without keeping track of their
 *                    IDs.
 *           - Shutdown: Once StopServiceDiscovery is triggered for all instances, dtor can be called.
 *                       StopServiceDiscovery is also triggered implicitly within dtor.
 *           All APIs are triggered from application side, Runtime APIs could be triggered from multiple threads.
 */
class RequiredServiceInstanceManager final {
 private:
  /*!
   * \brief Type alias for shared prt of Required Service instance impl.
   */
  using RequiredServiceInstanceImplSharedPtr = std::shared_ptr<RequiredServiceInstanceImpl>;

  /*!
   * \brief Required Service instance map type.
   */
  using RequiredServiceInstanceMap = std::map<RequiredServiceInstanceId, RequiredServiceInstanceImplSharedPtr>;

 public:
  /*!
   * \brief       Constructor.
   *
   * \param[in]   service_discovery_handler    A reference to the service discovery handler. Used to listen to
   *                                           ServiceDiscovery updates.
   * \param[in]   client_manager               A reference to the client manager. Used to register a newly created
   *                                           proxy binding upon calling CreateBackend.
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
  RequiredServiceInstanceManager(service_discovery::ServiceDiscoveryHandlerInterface& service_discovery_handler,
                                 ClientManagerInterface& client_manager,
                                 ClientTransportToDaemonInterface& client_transport_to_daemon) noexcept;

  /*!
   * \brief       Destructor.
   * \details     Implicitly stop SD.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~RequiredServiceInstanceManager() noexcept;

  RequiredServiceInstanceManager(RequiredServiceInstanceManager&&) noexcept = delete;
  RequiredServiceInstanceManager& operator=(RequiredServiceInstanceManager&&) & noexcept = delete;
  RequiredServiceInstanceManager(RequiredServiceInstanceManager const&) = delete;
  RequiredServiceInstanceManager& operator=(RequiredServiceInstanceManager const&) & = delete;

  /*!
   * \brief        Create a new required service instance.
   * \param[in]    service_instance_id Required service instance ID.
   * \param[in]    service_information Service interface information used in the service instance.
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
  void EmplaceRequiredServiceInstance(RequiredServiceInstanceId const& service_instance_id,
                                      ServiceInformation&& service_information) noexcept;

  /*!
   * \brief        Creates a binding client for a specific service id
   * \param[in]    service_instance_id The required service instance id
   *
   * \return       amsr::core::Result with the created client or an error
   *
   * \error       ClientError::kSystemConfigurationNotAvailable If the given instance id is not configured.
   * \error       SomeIpDaemonClientErrc::kAccessDenied If the application has no right to request a service.
   * \error       SomeIpDaemonClientErrc::kIncorrectResponseMsgLength If the message deserialization failed.
   * \error       SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   * \error       SomeIpDaemonClientErrc::kPayloadEmpty If an empty response message payload is received.
   *
   * \pre          -
   *
   * \context       App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<RequiredServiceInstance> RequestRequiredServiceInstance(
      RequiredServiceInstanceId const& service_instance_id) noexcept;

  /*!
   * \brief        Check if a required service instance is configured in this instance.
   * \param[in]    required_service_instance_id The required service instance id.
   *
   * \return       true if the required service instance is configured.
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
  bool IsRequiredServiceInstanceIdConfigured(RequiredServiceInstanceId const& required_service_instance_id) noexcept;

  /*!
   * \brief       Start Service Discovery for all registered required service instances.
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
  void StartServiceDiscovery() const noexcept;

  /*!
   * \brief       Stop Service Discovery for all registered required service instances.
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
  void StopServiceDiscovery() const noexcept;

 private:
  /*!
   * \brief A reference to the service discovery handler
   */
  service_discovery::ServiceDiscoveryHandlerInterface& service_discovery_handler_;

  /*!
   * \brief Instance that is used to transport client communication to SOME/IP Daemon.
   */
  ClientTransportToDaemonInterface& client_transport_to_daemon_;

  /*!
   * \brief ClientManager lives inside the scope of SomeIpBindingCore instance.
   */
  ClientManagerInterface& client_manager_;

  /*!
   * \brief Required Service instance map
   */
  RequiredServiceInstanceMap required_service_instance_map_{};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_MANAGER_H_
