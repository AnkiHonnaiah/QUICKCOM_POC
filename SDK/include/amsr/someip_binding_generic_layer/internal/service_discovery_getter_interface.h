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
 *        \brief  Interface for Service Discovery Getter
 *
 *      \details  Interface enables the possibility to test GenericProxyServiceDiscovery independend from
 *                GenericProxyFactory.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_GETTER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_GETTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief  ServiceDiscoveryGetterInterface.
 * \details Interface is only needed to be able to test GenericProxyServiceDiscovery independend from
 * GenericProxyFactory.
 */
class ServiceDiscoveryGetterInterface {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ServiceDiscoveryGetterInterface() = default;

  /*!
   * \brief Use default destructor.
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  virtual ~ServiceDiscoveryGetterInterface() = default;

  ServiceDiscoveryGetterInterface(ServiceDiscoveryGetterInterface&&) = delete;
  ServiceDiscoveryGetterInterface& operator=(ServiceDiscoveryGetterInterface&&) = delete;
  ServiceDiscoveryGetterInterface(ServiceDiscoveryGetterInterface const&) = delete;
  ServiceDiscoveryGetterInterface& operator=(ServiceDiscoveryGetterInterface const&) = delete;

  /*!
   * \brief       Getter for the required service instance.
   * \return      required service instance.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual someip_binding_core::internal::RequiredServiceInstance const& GetRequiredServiceInstance() const noexcept = 0;

  /*!
   * \brief       Getter for the required service instance id.
   * \return      required service instance id.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual someip_binding_core::internal::RequiredServiceInstanceId const GetRequiredInstanceIdentifier()
      const noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_SERVICE_DISCOVERY_GETTER_INTERFACE_H_
