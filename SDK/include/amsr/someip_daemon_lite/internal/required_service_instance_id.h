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
/*!        \file  internal/required_service_instance_id.h
 *        \brief  ID of a required SOME/IP service instance.
 *        \unit   SomeIpDaemon::CommunicationExtensionPoints::RequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

#include "amsr/someip_daemon_lite/data_types.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief ID of a required SOME/IP service instance.
 * \vprivate Component Private
 */
class RequiredServiceInstanceId {
 public:
  /*!
   * \brief Constructor.
   * \param[in] service_id Service ID of a SOME/IP service.
   * \param[in] instance_id Instance ID of a SOME/IP service.
   * \param[in] major_interface_version Major interface version of a SOME/IP service.
   * \param[in] minor_interface_version Minor interface version of a SOME/IP service.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  RequiredServiceInstanceId(ServiceId const service_id, InstanceId const instance_id,
                            MajorInterfaceVersion const major_interface_version,
                            MinorInterfaceVersion const minor_interface_version) noexcept
      : service_id_{service_id}, instance_id_{instance_id}, major_interface_version_{major_interface_version} {
    static_cast<void>(minor_interface_version);
  }

  /*!
   * \brief Getter for the service ID.
   * \return Service ID.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  ServiceId GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Getter for the instance ID.
   * \return Instance ID.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  InstanceId GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Getter for the major interface version.
   * \return Major interface version.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  MajorInterfaceVersion GetMajorInterfaceVersion() const noexcept { return major_interface_version_; }

 private:
  /*!
   * \brief Service ID of the SOME/IP service.
   */
  ServiceId const service_id_;

  /*!
   * \brief Instance ID of the SOME/IP service.
   */
  InstanceId const instance_id_;

  /*!
   * \brief Major interface version of the service interface.
   */
  MajorInterfaceVersion const major_interface_version_;
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_
