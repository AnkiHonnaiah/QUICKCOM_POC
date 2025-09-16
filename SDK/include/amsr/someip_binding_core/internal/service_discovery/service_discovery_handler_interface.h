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
/**        \file  amsr/someip_binding_core/internal/service_discovery/service_discovery_handler_interface.h
 *        \brief  Interface for Service Discovery Handler
 *
 *      \details  Interface for service discovery handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Service discovery handler interface. Central point where all instances can get their sd updates.
 * \details This interface shall be implemented by classes that will listen to service updates
 *          (Service offered or stopped) from the network, and used by the listeners.
 *          It is mainly useful to decouple the listener units from the handler implementation.
 */
class ServiceDiscoveryHandlerInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  ServiceDiscoveryHandlerInterface() = default;

  /*!
   * \brief Use default destructor.
   * \context ANY
   */
  virtual ~ServiceDiscoveryHandlerInterface() = default;

  ServiceDiscoveryHandlerInterface(ServiceDiscoveryHandlerInterface&&) = delete;
  ServiceDiscoveryHandlerInterface& operator=(ServiceDiscoveryHandlerInterface&&) = delete;
  ServiceDiscoveryHandlerInterface(ServiceDiscoveryHandlerInterface const&) = delete;
  ServiceDiscoveryHandlerInterface& operator=(ServiceDiscoveryHandlerInterface const&) = delete;

  /*!
   * \brief Type alias for service listener pointer.
   */
  using ServiceListenerPtr = typename std::add_pointer<
      ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface>::type;

  /*!
   * \brief       Start listening to service updates with this listener.
   * \param[in]   service_instance  The binding specific service interface identifier.
   * \param[in]   listener             Service listener that wants to listen to the service updates.
   *                                   Parameter must not be a nullptr.
   *
   * \pre         All Listeners shall be registered before StartServiceDiscovery is being called
   * \pre         Same instance id cannot be registered twice.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void StartListening(::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& service_instance,
                              ServiceListenerPtr listener) noexcept = 0;

  /*!
   * \brief       Stop listening to service updates.
   * \param[in]   service_instance  The binding specific service interface identifier.
   *
   * \pre         All Listeners shall stop listening after StopServiceDiscovery has been called
   * \pre         Listener must have been already registered
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void StopListening(
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& service_instance) noexcept = 0;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_INTERFACE_H_
