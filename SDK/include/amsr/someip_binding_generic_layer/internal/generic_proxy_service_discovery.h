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
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericProxyServiceDiscovery
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_SERVICE_DISCOVERY_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_SERVICE_DISCOVERY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_generic_layer/internal/service_discovery_getter_interface.h"
#include "amsr/someip_binding_generic_layer/internal/service_discovery_listener.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief GenericProxyServiceDiscovery class that forwards service discovery notifications of instances to the socal
 * layer from instance up and instance down messages.
 */
class GenericProxyServiceDiscovery final
    : public someip_binding_core::internal::service_discovery::ServiceListenerInterface,
      public ServiceDiscoveryGetterInterface {
 public:
  /*!
   * \brief Alias for the proxy service discovery interface.
   */
  using GenericProxyServiceDiscoveryInterface =
      ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface;

  /*!
   * \brief Constructor.
   * \param[in] required_service_instance_id Required service instance ID.
   * \param[in] required_service_instance    Required service instance.
   * \param[in] service_discovery_access     The SingletonAccess object for the socal service discovery singleton.
   *
   * \pre           -
   *
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Register as a listener to the binding for service updates.
   * \endinternal
   */
  explicit GenericProxyServiceDiscovery(
      someip_binding_core::internal::RequiredServiceInstanceId const& required_service_instance_id,
      someip_binding_core::internal::RequiredServiceInstance&& required_service_instance,
      ::amsr::generic::SingletonAccess<GenericProxyServiceDiscoveryInterface>&& service_discovery_access) noexcept;

  /*!
   * \brief       Destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GenericProxyServiceDiscovery() noexcept final;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  GenericProxyServiceDiscovery(GenericProxyServiceDiscovery const&) = delete;

  /*!
   * \brief       Default copy constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericProxyServiceDiscovery(GenericProxyServiceDiscovery&&) noexcept = default;

  auto operator=(GenericProxyServiceDiscovery const&) & -> GenericProxyServiceDiscovery& = delete;

  /*!
   * \brief       Default move assignment operator.
   * \return      Reference to self.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(GenericProxyServiceDiscovery&&) & noexcept -> GenericProxyServiceDiscovery& = default;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface::OnServiceInstanceUp
   */
  void OnServiceInstanceUp(
      someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface::OnServiceInstanceDown
   */
  void OnServiceInstanceDown(
      someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final;

  // clang-format off
  /*!
   * \copydoc amsr::someip_binding_generic_layer::internal::ServiceDiscoveryGetterInterface::GetRequiredServiceInstance
   */
  // clang-format on
  someip_binding_core::internal::RequiredServiceInstance const& GetRequiredServiceInstance() const noexcept final;

  // clang-format off
  /*!
   * \copydoc amsr::someip_binding_generic_layer::internal::ServiceDiscoveryGetterInterface::GetRequiredInstanceIdentifier
   */
  // clang-format on
  someip_binding_core::internal::RequiredServiceInstanceId const GetRequiredInstanceIdentifier() const noexcept final;

 private:
  /*!
   * \brief SingletonAccess for the service discovery listener singleton.
   */
  ::amsr::generic::SingletonAccess<GenericProxyServiceDiscoveryInterface> service_discovery_access_;

  /*!
   * \brief Service discovery listener to be registered into the RequiredServiceInstance to listen for service status
   *        updates. The RequiredServiceInstance will hold a shared pointer to this listener in order to forward the
   *        calls. This object will be registered upon construction of this class and later deregistered during
   *    destruction.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  std::shared_ptr<ServiceDiscoveryListener> sd_listener_;

  /*!
   * \brief The required service instance.
   */
  someip_binding_core::internal::RequiredServiceInstance required_service_instance_;

  /*!
   * \brief The required service instance indentifier bound to the service discovery instance in socal type format.
   */
  ::amsr::socal::internal::RequiredInstanceIdentifier rsi_id_socal_type_;

  /*!
   * \brief The required service instance indentifier bound to the service discovery instance.
   */
  someip_binding_core::internal::RequiredServiceInstanceId rsi_id_;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_SERVICE_DISCOVERY_H_
