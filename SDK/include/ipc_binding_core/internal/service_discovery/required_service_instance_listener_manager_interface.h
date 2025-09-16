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
 *        \brief  Interface for the RequiredServiceInstanceListenerManager.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"

#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {
/*!
 * \brief Interface for the required service instance manager.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstanceManager
 */
class RequiredServiceInstanceListenerManagerInterface {
 public:
  /*!
   * \brief Type alias for socal's proxy service discovery interface.
   */
  using SocalProxyServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstanceListenerManagerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~RequiredServiceInstanceListenerManagerInterface() noexcept = default;

  RequiredServiceInstanceListenerManagerInterface(RequiredServiceInstanceListenerManagerInterface const&) noexcept =
      delete;
  auto operator=(RequiredServiceInstanceListenerManagerInterface const&) noexcept
      -> RequiredServiceInstanceListenerManagerInterface& = delete;
  RequiredServiceInstanceListenerManagerInterface(RequiredServiceInstanceListenerManagerInterface&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListenerManagerInterface&&) noexcept
      -> RequiredServiceInstanceListenerManagerInterface& = delete;

  /*!
   * \brief Construct a required service instance listener.
   *
   * \param[in,out] socal_service_discovery       Reference to socal's service discovery proxy. It must remain valid for
   *                                              the lifetime of this object.
   * \param[in]     required_service_instance_id  The required service instance id.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto CreateListener(SocalProxyServiceDiscovery& socal_service_discovery,
                              RequiredServiceInstanceId const& required_service_instance_id) noexcept -> void = 0;

  /*!
   * \brief Get the listener for a required service instance.
   *
   * \param[in] required_service_instance_id  The required service instance id.
   *
   * \return The required service instance listener.
   *
   * \pre         CreateListener() has been called for this required_service_instance_id.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetListener(RequiredServiceInstanceId const& required_service_instance_id) noexcept
      -> RequiredServiceInstanceListenerInterface& = 0;
};
}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_INTERFACE_H_
