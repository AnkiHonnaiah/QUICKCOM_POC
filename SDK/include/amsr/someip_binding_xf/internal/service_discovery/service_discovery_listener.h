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
/**        \file  someip_binding_xf/internal/service_discovery/service_discovery_listener.h
 *        \brief  Listener class to handle and forward service status updates.
 *        \unit   SomeIpBinding::SomeIpBindingXf::ServiceDiscovery::ServiceDiscoveryListener
 *
 *         \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_LISTENER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Listener class to handle and forward service status updates.
 */
class ServiceDiscoveryListener final
    : public ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface {
 public:
  /*!
   * \brief Constructor
   * \param[in] listener The reference to the ServiceListenerInterface to which the calls will be forwarded.
   *
   * \context       Init
   * \pre           -
   *
   */
  explicit ServiceDiscoveryListener(ServiceListenerInterface& listener) noexcept;

  ServiceDiscoveryListener() = delete;
  ServiceDiscoveryListener(ServiceDiscoveryListener const&) = delete;
  ServiceDiscoveryListener(ServiceDiscoveryListener&&) = delete;
  ServiceDiscoveryListener& operator=(ServiceDiscoveryListener const&) = delete;
  ServiceDiscoveryListener& operator=(ServiceDiscoveryListener&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ServiceDiscoveryListener() noexcept final = default;

  /*!
   * \brief         Forwards the service instance up status update to the actual ServiceListenerInterface
   * implementation.
   * \param[in]     offered_service The provided service instance ID of the offered service.
   * \pre -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   */
  void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

  /*!
   * \brief         Forwards the service instance down status update to the actual ServiceListenerInterface
   * implementation.
   * \param[in]     stopped_service The provided service instance ID of the stopped service.
   * \pre -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   */
  void OnServiceInstanceDown(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final;

 private:
  /*!
   * \brief Reference to the service listener interface to delegate the calls.
   */
  ServiceListenerInterface& listener_;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr
#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_LISTENER_H_
