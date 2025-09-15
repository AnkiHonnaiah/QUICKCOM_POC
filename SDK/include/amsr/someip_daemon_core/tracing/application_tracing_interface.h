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
/*!        \file  someip_daemon_core/tracing/application_tracing_interface.h
 *        \brief  SOME/IP daemon specific interface for providing tracing services for messages received from the
 *                application.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_APPLICATION_TRACING_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_APPLICATION_TRACING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "osabstraction/process/process_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace tracing {

/*!
 * \brief Application tracing interface.
 */
class ApplicationTracingInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady  FALSE
   */
  ApplicationTracingInterface() noexcept = default;

  /*!
   * \brief Define default destructor.
   *
   * \steady  FALSE
   */
  virtual ~ApplicationTracingInterface() noexcept = default;

  ApplicationTracingInterface(ApplicationTracingInterface const &) = delete;
  ApplicationTracingInterface(ApplicationTracingInterface &&) = delete;
  ApplicationTracingInterface &operator=(ApplicationTracingInterface const &) & = delete;
  ApplicationTracingInterface &operator=(ApplicationTracingInterface &&) & = delete;

  /*!
   * \brief Called every time a SOME/IP event notification has been transmitted.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] event_id       The event id.
   * \param[in] message_size   The size of the complete message (header + payload).
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceSomeIpEventSendSync(osabstraction::process::ProcessId const process_id,
                                        someip_protocol::internal::ServiceId const service_id,
                                        someip_protocol::internal::MajorVersion const major_version,
                                        someip_protocol::internal::InstanceId const instance_id,
                                        someip_protocol::internal::EventId const event_id,
                                        std::size_t const message_size) noexcept = 0;

  /*!
   * \brief Called every time a PDU notification has been transmitted.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] event_id       The event id.
   * \param[in] message_size   The size of the complete message (header + payload).
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TracePduEventSendSync(osabstraction::process::ProcessId const process_id,
                                     someip_protocol::internal::ServiceId const service_id,
                                     someip_protocol::internal::InstanceId const instance_id,
                                     someip_protocol::internal::EventId const event_id,
                                     std::size_t const message_size) noexcept = 0;

  /*!
   * \brief Called every time a method request has been transmitted.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] method_id      The method id.
   * \param[in] message_size   The size of the complete message (header + payload).
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodRequestSendSync(osabstraction::process::ProcessId const process_id,
                                          someip_protocol::internal::ServiceId const service_id,
                                          someip_protocol::internal::MajorVersion const major_version,
                                          someip_protocol::internal::InstanceId const instance_id,
                                          someip_protocol::internal::MethodId const method_id,
                                          std::size_t const message_size) noexcept = 0;

  /*!
   * \brief     Called every time a method response has been transmitted.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] method_id      The method id.
   * \param[in] message_size   The size of the complete message (header + payload).
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodResponseSendSync(osabstraction::process::ProcessId const process_id,
                                           someip_protocol::internal::ServiceId const service_id,
                                           someip_protocol::internal::MajorVersion const major_version,
                                           someip_protocol::internal::InstanceId const instance_id,
                                           someip_protocol::internal::MethodId const method_id,
                                           std::size_t const message_size) noexcept = 0;
};

}  // namespace tracing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_APPLICATION_TRACING_INTERFACE_H_
