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
/**        \file  someip_service_instance_id.h
 *        \brief  Datatype containing service id, instance id, major version and minor version.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SERVICE_INSTANCE_ID_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SERVICE_INSTANCE_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief A SOME/IP service instance id to use in communication. It contains Service id, Major Version and Instance Id.
 * Minor Version is not needed for communication between someipdaemons, it is only used in Service Discovery.
 */
struct SomeIpServiceInstanceIdCommunication {
  /*!
   * \brief A SOME/IP service deployment containing Service Id and Major Version information.
   */
  amsr::someip_daemon_core::configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication deployment_id{};

  /*!
   * \brief A SOME/IP service instance ID.
   */
  amsr::someip_protocol::internal::InstanceId instance_id{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief       Defines a strict weak ordering relationship for SomeIpServiceInstanceIdCommunication keys.
   * \param[in]   other A key to compare against.
   * \return      True if this key is less than the other one, false otherwise.
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool operator<(SomeIpServiceInstanceIdCommunication const& other) const noexcept {
    return std::make_tuple(this->deployment_id.service_interface_id, this->deployment_id.major_version,
                           this->instance_id) < std::make_tuple(other.deployment_id.service_interface_id,
                                                                other.deployment_id.major_version, other.instance_id);
  }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief     Compare operator.
   *
   * \param[in] other the SomeIpServiceInstanceIdCommunication object to compare with.
   * \return    True if all the parameters are the same, or if all parameters are the same.
   *
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   */
  bool operator==(SomeIpServiceInstanceIdCommunication const& other) const noexcept {
    return (std::make_pair(this->deployment_id, this->instance_id) ==
            std::make_pair(other.deployment_id, other.instance_id));
  }
};

/*!
 * \brief Service instance ID. Contains deployment id and instance id.
 */
struct SomeIpServiceInstanceId {
  /*!
   * \brief Service interface deployment id. Contains service id and version information.
   */
  amsr::someip_daemon_core::configuration::types::SomeIpServiceInterfaceDeploymentId deployment_id{};

  /*!
   * \brief The SOME/IP instance id of the Service.
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief       Defines a strict weak ordering relationship for SomeIpServiceInstanceId keys.
   * \param[in]   other A key to compare against.
   * \return      True if this key is less than the other one, false otherwise.
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool operator<(SomeIpServiceInstanceId const& other) const noexcept {
    return std::make_tuple(this->deployment_id.service_interface_id,
                           this->deployment_id.service_interface_version.major_version,
                           this->deployment_id.service_interface_version.minor_version, this->instance_id) <
           std::make_tuple(other.deployment_id.service_interface_id,
                           other.deployment_id.service_interface_version.major_version,
                           other.deployment_id.service_interface_version.minor_version, other.instance_id);
  }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief     Compare operator.
   *
   * \param[in] other the SomeIpServiceInstanceId object to compare with.
   * \return    true if all the parameters are the same otherwise false.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool operator==(SomeIpServiceInstanceId const& other) const noexcept {
    return std::make_tuple(this->deployment_id.service_interface_id,
                           this->deployment_id.service_interface_version.major_version,
                           this->deployment_id.service_interface_version.minor_version, this->instance_id) ==
           std::make_tuple(other.deployment_id.service_interface_id,
                           other.deployment_id.service_interface_version.major_version,
                           other.deployment_id.service_interface_version.minor_version, other.instance_id);
  }
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_SERVICE_INSTANCE_ID_H_
