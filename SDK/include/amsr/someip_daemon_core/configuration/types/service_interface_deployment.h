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
/**        \file  service_interface_deployment.h
 *        \brief  Service Interface Deployment
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_INTERFACE_DEPLOYMENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_INTERFACE_DEPLOYMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_group.h"
#include "amsr/someip_daemon_core/configuration/types/someip_method_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_version.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Deployment ID
 */
struct SomeIpServiceInterfaceDeploymentId {
  /*!
   * \brief Unique Identifier that identifies the ServiceInterface in SOME/IP.
   *        This Identifier is sent as Service ID in SOME/IP Service Discovery messages.
   */
  ::amsr::someip_protocol::internal::ServiceId service_interface_id{};

  /*!
   * \brief The SOME/IP major and minor Version of the Service.
   */
  SomeIpServiceVersion service_interface_version{};
};

/*!
 * \brief Deployment ID used in communication between someipdaemons containing Interface Version and Major Version.
 * Minor version is not required for communication.
 */
struct SomeIpServiceInterfaceDeploymentIdCommunication {
  /*!
   * \brief Unique Identifier that identifies the ServiceInterface in SOME/IP.
   * This Identifier is sent as Service ID in SOME/IP Service Discovery messages.
   */
  ::amsr::someip_protocol::internal::ServiceId service_interface_id{};

  /*!
   * \brief The SOME/IP major version of the ServiceInterface.
   * \details For communication, MinorVersion is not needed
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief       Defines a strict weak ordering relationship for SomeIpServiceInterfaceDeploymentIdCommunication keys.
   * \param[in]   other A key to compare against.
   * \return      True if this key is less than the other one.
   *
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool operator<(SomeIpServiceInterfaceDeploymentIdCommunication const& other) const noexcept {
    return std::make_pair(this->service_interface_id, this->major_version) <
           std::make_pair(other.service_interface_id, other.major_version);
  }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief     Compare operator.
   *
   * \param[in] other the SomeIpServiceInterfaceDeploymentIdCommunication object to compare with.
   * \return    True if all the parameters are the same.
   *
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool operator==(SomeIpServiceInterfaceDeploymentIdCommunication const& other) const noexcept {
    return std::make_pair(this->service_interface_id, this->major_version) ==
           std::make_pair(other.service_interface_id, other.major_version);
  }
};

/*!
 * \brief SOME/IP configuration settings for a ServiceInterface communication deployment.
 */
struct SomeIpServiceInterfaceCommunicationDeployment {
  /*!
   * \brief SOME/IP Events that are defined within the SOME/IP deployment.
   */
  ara::core::Vector<SomeIpEventDeployment> events{};

  /*!
   * \brief SOME/IP Methods that are defined within the SOME/IP deployment.
   */
  ara::core::Vector<SomeIpMethodDeployment> methods{};

  /*!
   * \brief SOME/IP EventGroups that are defined within the SOME/IP deployment.
   */
  ara::core::Map<::amsr::someip_protocol::internal::EventgroupId, SomeIpEventGroup> event_groups{};
};

/*!
 * \brief SOME/IP configuration settings for a ServiceInterface.
 */
struct SomeIpServiceInterfaceDeployment {
  /*!
   * \brief Unique identifier for this service deployment.
   */
  SomeIpServiceInterfaceDeploymentId deployment_id{};

  /*!
   * \brief Encapsulation of communication deployment, containing deployed events, eventgroups and methods.
   */
  SomeIpServiceInterfaceCommunicationDeployment communication_deployment{};
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_INTERFACE_DEPLOYMENT_H_
