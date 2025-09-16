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
/**       \file
 *        \unit  SomeIpBinding::SomeIpBindingGenericLayer::GenericProxyUtil
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_UTIL_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_UTIL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief Class to translate between SomeIpBinding internal required and provided service instance IDs to the public API
 * ara::com::InstanceIdentifier type.
 * \internal
 * This is a copy of the Xf mapping function to avoid adding a common library or bad dependency from someipbinding to
 * socal or someip_binding_generic_layer to someip_binding_xf. We may add a library in future.
 * \endinternal
 */
class GenericProxyUtil final {
 public:
  /*!
   * \brief Parsed IDs from SomeIpBinding specific instance identifier.
   */
  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
  struct ParsedInstanceIdentifier {
    // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
    /*!
     * \brief  Constructor.
     * \details Explicit constructor to disallow default initialization and enable Result::EmplaceValue usage.
     * \param[in] service_identifier    Service ID.
     * \param[in] major                 Major version.
     * \param[in] instance_identifier   Instance ID.
     * \pre          -
     * \context      ANY
     * \threadsafe   FALSE
     * \reentrant    FALSE
     * \synchronous  TRUE
     * \steady      FALSE
     */
    explicit ParsedInstanceIdentifier(someip_protocol::internal::ServiceId service_identifier,
                                      someip_protocol::internal::MajorVersion major,
                                      someip_protocol::internal::InstanceId instance_identifier)
        // VCA_SOMEIPBINDING_STRUCT_CONSTRUCTOR_FALSE_POSITIVE
        : service_id{service_identifier},
          // VCA_SOMEIPBINDING_STRUCT_CONSTRUCTOR_FALSE_POSITIVE
          major_version{major},
          // VCA_SOMEIPBINDING_STRUCT_CONSTRUCTOR_FALSE_POSITIVE
          instance_id{instance_identifier} {}

    /*! \brief Service ID. */
    someip_protocol::internal::ServiceId service_id;
    /*! \brief Major version. */
    someip_protocol::internal::MajorVersion major_version;
    /*! \brief Instance ID. */
    someip_protocol::internal::InstanceId instance_id;
  };

  /*!
   * \brief Mapping function from the public API instance identifier to the SomeIpBinding fields stored therein.
   *
   * \details     Parses the string serialized IDs from the SomeIpBinding specific instance identifier string.
   * \param[in]   aracom_instance_identifier SomeIpBinding specific Instance Identifier string representation.
   * \return      Extracted instance identifier fields result.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto ComToBindingInstanceIdentifier(::ara::com::InstanceIdentifier const& aracom_instance_identifier) noexcept
      -> ::amsr::core::Result<ParsedInstanceIdentifier>;

  /*!
   * \brief Mapping function for SomeIpBinding specific instance identifier to the public API instance identifier type.
   *
   * \details     The function prepares and returns instance identifier string from a given instance id
   * \param[in]   required_service_instance_id Required service instance ID.
   * \return      Instance identifier in the format "SomeIp:<service id>:<major version>:<instance ID>"
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto BindingToComInstanceIdentifier(
      someip_binding_core::internal::RequiredServiceInstanceId const& required_service_instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;

  /*!
   * \brief Mapping function for SomeIpBinding specific instance identifier to the public API instance identifier type.
   *
   * \details     The function prepares and returns instance identifier string from a given instance id
   * \param[in]   provided_service_instance_id Provided service instance ID.
   * \return      Instance identifier in the format "SomeIp:<service id>:<major version>:<instance ID>"
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto BindingToComInstanceIdentifier(
      someip_binding_core::internal::ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;

  /*!
   * \brief       Map the configuration event subscription state type to the socal subscription state
   * \param[in]   someip_state Configuration event subscription state.
   * \return      Mapped subscription state type decoupled from the implementation specific type.
   *
   * \pre         Subscription state must be within enum set.
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BindingToComSubscriptionState(::amsr::someip_protocol::internal::SubscriptionState someip_state)
      -> ::ara::com::SubscriptionState;
};
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_UTIL_H_
