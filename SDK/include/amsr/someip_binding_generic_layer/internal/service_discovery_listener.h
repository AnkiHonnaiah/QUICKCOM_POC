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
 *         \brief  Forwards notification of new available and shutdown instances.
 *
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericProxyServiceDiscovery
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_LISTENER_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_LISTENER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief   Listener class to handle and forward service status updates.
 */
class ServiceDiscoveryListener final
    : public someip_binding_core::internal::service_discovery::ServiceListenerInterface {
 public:
  /*!
   * \brief Constructor.
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
   * \copydoc someip_binding_core::internal::service_discovery::ServiceListenerInterface::OnServiceInstanceUp
   */
  void OnServiceInstanceUp(
      someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

  /*!
   * \copydoc someip_binding_core::internal::service_discovery::ServiceListenerInterface::OnServiceInstanceDown
   */
  void OnServiceInstanceDown(
      someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final;

 private:
  /*!
   * \brief Reference to the service listener interface to delegate the calls.
   */
  someip_binding_core::internal::service_discovery::ServiceListenerInterface& listener_;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_LISTENER_H_
