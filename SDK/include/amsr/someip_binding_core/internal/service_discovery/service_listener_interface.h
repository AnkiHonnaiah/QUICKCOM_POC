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
/**        \file  amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h
 *        \brief  Interface for Service Listener
 *
 *      \details  Interface for classes that need to listen to service updates from SomeIpBinding.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_LISTENER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_LISTENER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Service listener interface. It listens to the binding for service updates.
 * \details This interface shall be implemented by classes that need to listen to service updates
 *          (Service offered or stopped) from SomeIpBinding.
 */
class ServiceListenerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceListenerInterface() = default;

  /*!
   * \brief Use default destructor.
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServiceListenerInterface() = default;

  ServiceListenerInterface(ServiceListenerInterface&&) = delete;
  ServiceListenerInterface& operator=(ServiceListenerInterface&&) = delete;
  ServiceListenerInterface(ServiceListenerInterface const&) = delete;
  ServiceListenerInterface& operator=(ServiceListenerInterface const&) = delete;

  /*!
   * \brief         Callback function to be called from binding whenever one service instance gets offered.
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
  virtual void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept = 0;

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
  virtual void OnServiceInstanceDown(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept = 0;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_LISTENER_INTERFACE_H_
