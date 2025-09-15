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
 *        \brief  Interface for a ServiceDiscovery factory.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/memory.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief Interface for a ServiceDiscovery factory.
 *
 * \unit IpcBinding::IpcBindingCore::ServiceDiscoveryFactory
 */
class ServiceDiscoveryFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Type-alias for TimerManagerInterface.
   */
  using TimerManagerInterface = ::amsr::steady_timer::TimerManagerInterface;

  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~ServiceDiscoveryFactoryInterface() noexcept = default;

  ServiceDiscoveryFactoryInterface(ServiceDiscoveryFactoryInterface const&) noexcept = delete;
  auto operator=(ServiceDiscoveryFactoryInterface const&) noexcept -> ServiceDiscoveryFactoryInterface& = delete;
  ServiceDiscoveryFactoryInterface(ServiceDiscoveryFactoryInterface&&) noexcept = delete;
  auto operator=(ServiceDiscoveryFactoryInterface&&) noexcept -> ServiceDiscoveryFactoryInterface& = delete;

  /*!
   * \brief Constructs a service discovery returns a unique pointer to its interface.
   *
   * \param[in] reactor        Reference to the reactor the service discovery should use. Must be valid for the entire
   *                           lifetime of the IpcServiceDiscovery.
   * \param[in] timer_manager  Reference to the timer manager the service discovery should use. Must be valid for the
   *                           entire lifetime of the IpcServiceDiscovery.
   *
   * \return Unique pointer to a service discovery object.
   *
   * \context Init
   * \steady  FALSE
   */
  virtual auto Create(ReactorInterface& reactor, TimerManagerInterface& timer_manager) noexcept
      -> amsr::UniquePtr<ServiceDiscoveryInterface> = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceDiscoveryFactoryInterface() noexcept = default;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_INTERFACE_H_
