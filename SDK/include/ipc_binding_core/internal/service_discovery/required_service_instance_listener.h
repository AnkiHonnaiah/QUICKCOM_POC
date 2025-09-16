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
/**        \file
 *        \brief  Listener for service discovery updates about required service instances.
 *
 *      \details  Per- required service instance listener registered with the ipc service discovery. It forwards
 *                updates to Socal's service discovery and to the registered provided service instance listeners.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief Listener and dispatcher for service discovery updates about required service instances.
 *
 * \unit IpcBinding::IpcBindingCore::RequiredServiceInstanceListener
 */
class RequiredServiceInstanceListener final : public RequiredServiceInstanceListenerInterface {
 public:
  /*!
   * \brief Type alias for IpcServiceDiscoveryInterface.
   */
  using IpcServiceDiscovery = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type alias for socal's proxy service discovery interface.
   */
  using SocalServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface;

  /*!
   * \brief Construct a required service instance listener and start listening for updates.
   *
   * \param[in]     required_service_instance_id  The required service instance id to listen for.
   * \param[in,out] socal_service_discovery       Reference to the socal service discovery. It must remain valid for the
   *                                              lifetime of the object.
   * \param[in,out] ipc_service_discovery         Reference to the ipc service discovery. It must remain valid for the
   *                                              lifetime of the object.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstanceListener(RequiredServiceInstanceId const& required_service_instance_id,
                                  SocalServiceDiscovery& socal_service_discovery,
                                  IpcServiceDiscovery& ipc_service_discovery) noexcept;

  /*!
   * \brief Stop listening and destroy the required instance listener.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RequiredServiceInstanceListener() final;

  RequiredServiceInstanceListener(RequiredServiceInstanceListener const&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListener const&) noexcept -> RequiredServiceInstanceListener& = delete;
  RequiredServiceInstanceListener(RequiredServiceInstanceListener&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListener&&) noexcept -> RequiredServiceInstanceListener& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerInterface::ListenProvidedServiceInstance
   */
  auto ListenProvidedServiceInstance(ProvidedServiceInstanceId const& provided_service_instance_id,
                                     amsr::WeakPtr<ProvidedServiceInstanceListener> provided_service_instance_listener)
      -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerInterface::StopListenProvidedServiceInstance
   */
  auto StopListenProvidedServiceInstance(ProvidedServiceInstanceId const& provided_service_instance_id) -> void final;

 private:
  /*!
   * \brief Type alias for IpcUnicastAddress.
   */
  using IpcUnicastAddress = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Type alias for RequiredServiceInstanceIdentifier.
   */
  using RequiredServiceInstanceIdentifier = ::amsr::ipc_service_discovery::internal::RequiredServiceInstanceIdentifier;

  /*!
   * \brief Type alias for ServiceInstanceEndpoint.
   */
  using ServiceInstanceEndpoint = ::amsr::ipc_service_discovery::common::internal::ServiceInstanceEndpoint;

  /*!
   * \brief Type alias for ProvidedState.
   */
  using ProvidedState = ::amsr::ipc_service_discovery::common::internal::ProvidedState;

  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the LogBuilder.
   */
  using LogBuilder = ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for the map of provided service instance listeners.
   */
  using ProvidedServiceInstanceListenerMap =
      ara::core::Map<ProvidedServiceInstanceId, amsr::WeakPtr<ProvidedServiceInstanceListener>>;

  /*!
   * \brief Called when an update about this required service instance is received from the ipc service discovery.
   *
   * \param[in] service_instance_endpoint  Information about the specific provided service instance that was updated.
   * \param[in] provided_state             The updated provided state.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ListenServiceCallback(ServiceInstanceEndpoint const& service_instance_endpoint,
                             ProvidedState const& provided_state) -> void;

  /*!
   * \brief The required service instance id for this listener.
   */
  RequiredServiceInstanceId const required_service_instance_id_;

  /*!
   * \brief The socal required service instance id.
   */
  amsr::socal::internal::RequiredInstanceIdentifier const socal_required_instance_id_;

  /*!
   * \brief Reference to the socal service discovery.
   */
  SocalServiceDiscovery& socal_service_discovery_;

  /*!
   * \brief Reference to the ipc service discovery.
   */
  IpcServiceDiscovery& ipc_service_discovery_;

  /*!
   * \brief The map of service instance listeners (RemoteServer instances) for each provided service instance associated
   *        with this required service instance listener.
   */
  ProvidedServiceInstanceListenerMap provided_service_instance_listeners_{};

  /*!
   * \brief Used logger.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 amsr::core::StringView{"RequiredServiceInstanceListener"}};
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_H_
