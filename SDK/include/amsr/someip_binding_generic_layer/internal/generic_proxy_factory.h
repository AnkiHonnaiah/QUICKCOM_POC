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
/*!        \file   someip_binding_generic_layer/internal/generic_proxy_factory.h
 *         \brief  Factory class for generic proxy
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericProxyFactory
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_FACTORY_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_FACTORY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/socal/generic_proxy.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/someip_binding_generic_layer/internal/generic_proxy.h"
#include "amsr/someip_binding_generic_layer/internal/generic_proxy_service_discovery.h"
#include "amsr/someip_binding_generic_layer/internal/service_discovery_getter_interface.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief GenericProxyFactory class
 */
// VECTOR NC Compiler-#1444: MD_SOMEIPBINDING_Compiler-#1444_beta_code_dependency
// VECTOR NC AutosarC++17_10-A1.1.1: MD_SOMEIPBINDING_AutosarC++17_10-A1.1.1_beta_code_dependency
class GenericProxyFactory final : public amsr::socal::GenericProxy::BackendFactoryInterface {
 public:
  /*!
   * \brief GenericProxyBackendInterface unique pointer alias.
   */
  using BackendInterfacePtr = std::unique_ptr<::amsr::socal::internal::GenericProxyBackendInterface>;

  /*!
   * \brief Constructs the GenericProxyFactory instance.
   * \param[in] generic_proxy_service_discovery       The generic proxy service discovery.
   * \pre      -
   * \context  Init
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate Product Private.
   * \spec   requires true; \endspec
   */
  explicit GenericProxyFactory(std::unique_ptr<ServiceDiscoveryGetterInterface>&& generic_proxy_service_discovery);

  /*!
   * \brief Destroys the GenericProxyFactory instance.
   * \pre      -
   * \context  Shutdown
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate Product Private.
   * \spec   requires true; \endspec
   */
  ~GenericProxyFactory() final = default;

  GenericProxyFactory(GenericProxyFactory&&) = delete;
  GenericProxyFactory(GenericProxyFactory const&) = delete;
  GenericProxyFactory& operator=(GenericProxyFactory const&) = delete;
  GenericProxyFactory& operator=(GenericProxyFactory&&) = delete;

  /*!
   * \brief   Creates the proxy backend.
   * \details For each proxy instance a distinct proxy backend instance shall be created to ensure thread-safety.
   * \param[in] provided_instance_id  Identifier of the provided instance the backend shall be created for.
   * \return Result containing a pointer to the created proxy backend or an error. The returned pointer must not be
   *         null.
   * \error ComErrc::kGrantEnforcementError  If the request is refused by IAM.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate    Product Private.
   * \spec   requires true; \endspec
   */
  auto CreateBackend(socal::internal::ProvidedInstanceIdentifier const& provided_instance_id) noexcept
      -> ::amsr::core::Result<BackendInterfacePtr> final;

 private:
  /*!
   * \brief Service discovery unique ptr
   */
  std::unique_ptr<ServiceDiscoveryGetterInterface> generic_proxy_service_discovery_;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_FACTORY_H_
