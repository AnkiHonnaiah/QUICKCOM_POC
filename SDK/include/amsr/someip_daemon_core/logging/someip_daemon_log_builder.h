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
/*!        \file  someip_daemon_log_builder.h
 *        \brief  This file contains helper class for Ipc-Binding values logging.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_SOMEIP_DAEMON_LOG_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_SOMEIP_DAEMON_LOG_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/log/logging.h"

namespace amsr {
namespace someip_daemon_core {
namespace logging {

/*!
 * \brief Helper class for Ipc-Binding values logging.
 * \vprivate Vector API Component private
 */
class SomeipDaemonLogBuilder {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Log SOME/IP required service instance ID in the format
   *       "(RequiredServiceInstance: Service Id = 0x1, Instance Id = 0x2, Major Version = 0x3, Minor Version = 0x4)"
   *
   * \param[in] s      The logstream to write to.
   * \param[in] req_si The required service instance ID to be logged.
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void LogRequiredServiceInstanceId(
      ara::log::LogStream& s,
      configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const& req_si) noexcept {
    s << "(RequiredServiceInstance: ";
    s << "Service Id = 0x" << ara::log::HexFormat(req_si.service_id_);
    s << ", Instance Id = 0x" << ara::log::HexFormat(req_si.instance_id_);
    s << ", Major Version = 0x" << ara::log::HexFormat(req_si.major_version_);
    s << ", Minor Version = 0x" << ara::log::HexFormat(req_si.minor_version_);
    s << ")";
  }

  /*!
   * \brief Log SOME/IP required service instance ID in the format
   *       "(RequiredServiceInstance: Service Id = 0x1, Instance Id = 0x2, Major Version = 0x3, Minor Version = 0x4)"
   *
   * \param[in] s                      The logstream to write to.
   * \param[in] service_deployment_id  The service deployment id, containing service id and version information
   * \param[in] instance_id            The service instance id
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void LogRequiredServiceInstanceId(
      ara::log::LogStream& s, configuration::types::SomeIpServiceInterfaceDeploymentId const& service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id) noexcept {
    s << "(RequiredServiceInstance: ";
    s << "Service Id = 0x" << ara::log::HexFormat(service_deployment_id.service_interface_id);
    s << ", Instance Id = 0x" << ara::log::HexFormat(instance_id);
    s << ", Major Version = 0x" << ara::log::HexFormat(service_deployment_id.service_interface_version.major_version);
    s << ", Minor Version = 0x" << ara::log::HexFormat(service_deployment_id.service_interface_version.minor_version);
    s << ")";
  }

  /*!
   * \brief Log SOME/IP required service instance ID in the format
   *       "(RequiredServiceInstance: Service Id = 0x1, Instance Id = 0x2, Major Version = 0x3)"
   *
   * \param[in] s                      The logstream to write to.
   * \param[in] service_deployment_id  The service deployment id, containing service id and version information
   * \param[in] instance_id            The service instance id
   *
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void LogRequiredServiceInstanceIdCommunication(
      ara::log::LogStream& s,
      configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication const& service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id) noexcept {
    s << "(RequiredServiceInstance: ";
    s << "Service Id = 0x" << ara::log::HexFormat(service_deployment_id.service_interface_id);
    s << ", Instance Id = 0x" << ara::log::HexFormat(instance_id);
    s << ", Major Version = 0x" << ara::log::HexFormat(service_deployment_id.major_version);
    s << ")";
  }
};

}  // namespace logging
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_SOMEIP_DAEMON_LOG_BUILDER_H_
