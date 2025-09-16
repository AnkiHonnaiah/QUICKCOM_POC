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
/**       \file  someip_binding_xf/internal/ara_com_instance_id_xf.h
 *        \brief  -
 *        \unit  SomeIpBinding::SomeIpBindingXf::AraComInstanceIdXf
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/com/instance_identifier.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {

/*!
 * \brief AraCom_Instance_Id_Xf class to translate between ara::com::InstanceIdentifier and
 * amsr::someip_protocol::internal::InstanceId formats.
 */
class AraComInstanceIdXf final {
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
   * \brief       Builder function for public API instance identifier from SomeIpBinding specific identifiers.
   *
   * \details     The function prepares and returns instance id for a given instance_identifier string
   * \param[in]   service_id    Service ID.
   * \param[in]   major_version Major version.
   * \param[in]   instance_id   Instance ID.
   * \return      Public API instance identifier representation.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto BuildAraComInstanceIdentifier(someip_protocol::internal::ServiceId service_id,
                                            someip_protocol::internal::MajorVersion major_version,
                                            someip_protocol::internal::InstanceId instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;

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
   * \internal steady FALSE \endinternal
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
   * \internal steady FALSE \endinternal
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
   * \internal steady FALSE \endinternal
   * \spec
   *   requires true;
   * \endspec
   */
  static auto BindingToComInstanceIdentifier(
      someip_binding_core::internal::ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;
};

}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_
