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
 *        \brief  Interface to monitor a provided service instance's availability of the ServiceDiscovery.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_INSTANCE_LISTENER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_INSTANCE_LISTENER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief Interface to monitor a provided service instance's availability of the ServiceDiscovery.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstanceListener
 */
class ServiceInstanceListenerInterface {
 public:
  /*!
   * \brief Type alias for the remote server address.
   */
  using IpcUnicastAddress = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Default constructor.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ServiceInstanceListenerInterface() noexcept = default;

  ServiceInstanceListenerInterface(ServiceInstanceListenerInterface const&) noexcept = delete;
  ServiceInstanceListenerInterface(ServiceInstanceListenerInterface&&) noexcept = delete;
  auto operator=(ServiceInstanceListenerInterface const&) noexcept -> ServiceInstanceListenerInterface& = delete;
  auto operator=(ServiceInstanceListenerInterface&&) noexcept -> ServiceInstanceListenerInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ServiceInstanceListenerInterface() noexcept = default;

  /*!
   * \brief Notification that the monitored provided service instance is now available.
   *
   * \param[in] address  Remote server address of the provided service instance.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  virtual auto OnServiceInstanceUp(IpcUnicastAddress const& address) noexcept -> void = 0;

  /*!
   * \brief Notification that the monitored provided service instance is not available anymore.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto OnServiceInstanceDown() noexcept -> void = 0;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_INSTANCE_LISTENER_INTERFACE_H_
