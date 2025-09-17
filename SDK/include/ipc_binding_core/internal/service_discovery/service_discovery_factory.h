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
 *        \brief  Factory for ServiceDiscovery.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_centralized.h"

#include "ipc_binding_core/internal/service_discovery/service_discovery_factory_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief A factory able to create ServiceDiscovery instances.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::ServiceDiscoveryFactory
 */
class ServiceDiscoveryFactory final : public ServiceDiscoveryFactoryInterface {
 public:
  /*!
   * \brief Construct the factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ServiceDiscoveryFactory() noexcept = default;

  /*!
   * \brief Destroy the factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServiceDiscoveryFactory() noexcept final = default;  // VCA_IPCB_MOLE_1298

  ServiceDiscoveryFactory(ServiceDiscoveryFactory const&) noexcept = delete;
  auto operator=(ServiceDiscoveryFactory const&) noexcept -> ServiceDiscoveryFactory& = delete;
  ServiceDiscoveryFactory(ServiceDiscoveryFactory&&) noexcept = delete;
  auto operator=(ServiceDiscoveryFactory&&) noexcept -> ServiceDiscoveryFactory& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::service_discovery::ServiceDiscoveryFactoryInterface::Create
   */
  auto Create(ReactorInterface& reactor, TimerManagerInterface& timer_manager) noexcept
      -> amsr::UniquePtr<ServiceDiscoveryInterface> final;

 private:
  /*!
   * \brief Type-alias for concrete ServiceDiscovery.
   */
  using ServiceDiscovery = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized;

  /*!
   * \brief Type-alias for BindingType.
   */
  using BindingType = ::amsr::ipc_service_discovery::internal::BindingType;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_FACTORY_H_
