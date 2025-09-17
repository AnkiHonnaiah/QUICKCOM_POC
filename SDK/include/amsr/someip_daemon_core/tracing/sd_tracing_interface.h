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
/*!        \file  someip_daemon_core/tracing/sd_tracing_interface.h
 *        \brief  SOME/IP daemon specific interface for providing tracing services for service discovery messages.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_SD_TRACING_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_SD_TRACING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "osabstraction/process/process_native_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace tracing {

/*!
 * \brief Service discovery tracing interface.
 */
class SdTracingInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady  FALSE
   */
  SdTracingInterface() noexcept = default;

  /*!
   * \brief Define default destructor.
   *
   * \steady  FALSE
   */
  virtual ~SdTracingInterface() noexcept = default;
  SdTracingInterface(SdTracingInterface const &) = delete;
  SdTracingInterface(SdTracingInterface &&) = delete;
  SdTracingInterface &operator=(SdTracingInterface const &) & = delete;
  SdTracingInterface &operator=(SdTracingInterface &&) & = delete;

  /*!
   * \brief Called every time a remote service is offered.
   * \details This API is called once per adaptive application that is requesting the service.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void TraceServiceInstanceUpSync(osabstraction::process::ProcessId const process_id,
                                          someip_protocol::internal::ServiceId const service_id,
                                          someip_protocol::internal::MajorVersion const major_version,
                                          someip_protocol::internal::InstanceId const instance_id) noexcept = 0;

  /*!
   * \brief Called every time a remote service is stopped.
   * \details This API is called once per adaptive application that is requesting the service.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void TraceServiceInstanceDownSync(osabstraction::process::ProcessId const process_id,
                                            someip_protocol::internal::ServiceId const service_id,
                                            someip_protocol::internal::MajorVersion const major_version,
                                            someip_protocol::internal::InstanceId const instance_id) noexcept = 0;
};

}  // namespace tracing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_SD_TRACING_INTERFACE_H_
