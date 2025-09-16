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
 *         \brief  Generic layer initialization
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericLayerInitializer
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_LAYER_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_LAYER_INITIALIZER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief Static helper class for initialization of the generic layer SomeIpBinding functionality.
 */
class GenericLayerInitializer final {
 public:
  /*!
   * \brief Collection of require service instance IDs for which generic proxy factories are created and registered.
   */
  using RequiredServiceInstanceIds = ::ara::core::Vector<someip_binding_core::internal::RequiredServiceInstanceId>;

  /*!
   * \brief         Initializes all generic proxy factory and service discovery instances and register them in the
   *                provided SOME/IP core.
   *
   * \param[in]     required_service_instance_ids Required service instance ID for generic proxy factories are created
   *                                              and registered.
   * \param[inout]  someip_binding_core           SomeIpBindingCore to request the required service instance from.
   *
   * \pre           -
   *
   * \context       Init
   *
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Request the required service instance from the associated SomeIpBindingCore.
   * - Create service discovery listener and register it in the RSI.
   * - Create generic proxy service discovery instance and put the listener in it.
   * - Create generic proxy factory and put the GP SD instance in it.
   * - Move the generic proxy factory ownership to socal.
   * \endinternal
   */
  static void RegisterRequiredServiceInstances(
      RequiredServiceInstanceIds const& required_service_instance_ids,
      // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_const_ref
      someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core) noexcept;

  /*!
   * \brief         Deinitializes all generic proxy factory and service discovery instances and deregisters from socal.
   *
   * \param[in]     required_service_instance_ids Required service instance ID for socal factory deregistration.
   *
   * \pre           -
   *
   * \context       Shutdown
   *
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Deregister from socal for each required service instance.
   * \endinternal
   */
  static void DeregisterRequiredServiceInstances(
      RequiredServiceInstanceIds const& required_service_instance_ids) noexcept;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_LAYER_INITIALIZER_H_
