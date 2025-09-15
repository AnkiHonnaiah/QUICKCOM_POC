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
/**        \file  required_service_instance_deployment.h
 *        \brief  Required Service Instance Deployment
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_SERVICE_INSTANCE_DEPLOYMENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_SERVICE_INSTANCE_DEPLOYMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/required_someip_service_version.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_group.h"
#include "amsr/someip_daemon_core/configuration/types/someip_method_deployment.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Deployment ID for required service instances, containing the service interface ID and a required version,
 *        which specifies the criteria for the server minor version that shall be accepted.
 */
struct RequiredSomeIpServiceInstanceDeploymentId {
  /*!
   * \brief Unique Identifier that identifies the ServiceInterface in SOME/IP.
   *        This Identifier is sent as Service ID in SOME/IP Service Discovery messages.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_interface_id;

  /*!
   * \brief The SOME/IP major and minor Version of the Service.
   */
  RequiredSomeIpServiceVersion const service_interface_version;
};

/*!
 * \brief SOME/IP configuration settings for a ServiceInterface.
 */
struct RequiredSomeIpServiceInstanceDeployment {
  /*!
   * \brief Unique identifier for this service deployment.
   */
  RequiredSomeIpServiceInstanceDeploymentId deployment_id;

  /*!
   * \brief Encapsulation of communication deployment, containing deployed events, eventgroups and methods.
   */
  SomeIpServiceInterfaceCommunicationDeployment communication_deployment;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_SERVICE_INSTANCE_DEPLOYMENT_H_
