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
/**        \file  service_discovery_client_state_owner.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_pool.h"
#include "vac/statemachine/state_owner.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief A Service Discovery Client state owner.
 */
class ServiceDiscoveryClientStateOwner final : public vac::statemachine::StateOwner<ServiceDiscoveryClientState> {
 public:
  /*!
   * \brief Constructor of ServiceDiscoveryClientStateOwner.
   *
   * \param[in,out] state_pool A state pool.
   * \param[in,out] context A state context.
   *
   * \context ANY (Init or during runtime for Instance ID = ALL)
   * \steady FALSE
   */
  ServiceDiscoveryClientStateOwner(ServiceDiscoveryClientStatePool& state_pool,
                                   ServiceDiscoveryClientStateContext& context) noexcept
      : vac::statemachine::StateOwner<
            ServiceDiscoveryClientState>::StateOwner{state_pool, ServiceDiscoveryClientStateHandle::kDownPhase},
        context_{context} {}

  /*!
   * \brief Destructor of ServiceDiscoveryClientStateOwner.
   * \context Shutdown
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryClientStateOwner() noexcept override = default;

  ServiceDiscoveryClientStateOwner(ServiceDiscoveryClientStateOwner const&) = delete;
  ServiceDiscoveryClientStateOwner(ServiceDiscoveryClientStateOwner&&) = delete;
  ServiceDiscoveryClientStateOwner& operator=(ServiceDiscoveryClientStateOwner const&) & = delete;
  ServiceDiscoveryClientStateOwner& operator=(ServiceDiscoveryClientStateOwner&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns the context for states.
   *
   * \return The state context.
   *
   * \context      App
   * \steady       FALSE
   */
  ServiceDiscoveryClientStateContext& GetContext() override { return context_; }

 private:
  /*!
   * \brief A state context.
   */
  ServiceDiscoveryClientStateContext& context_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_
