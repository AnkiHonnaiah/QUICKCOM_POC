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
/*!        \file  someip_daemon_core/tracing/network_tracing_interface.h
 *        \brief  SOME/IP daemon specific interface for providing tracing services when messages are received from the
 *                network.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_NETWORK_TRACING_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_NETWORK_TRACING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "osabstraction/process/process_native_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace tracing {

/*!
 * \brief Network tracing interface.
 */
class NetworkTracingInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady  FALSE
   */
  NetworkTracingInterface() noexcept = default;

  /*!
   * \brief Define default destructor.
   *
   * \steady  FALSE
   */
  virtual ~NetworkTracingInterface() noexcept = default;

  NetworkTracingInterface(NetworkTracingInterface const &) = delete;
  NetworkTracingInterface(NetworkTracingInterface &&) = delete;
  NetworkTracingInterface &operator=(NetworkTracingInterface const &) & = delete;
  NetworkTracingInterface &operator=(NetworkTracingInterface &&) & = delete;

  /*!
   * \brief Called every time a SOME/IP event notification has been received
   * \details This API is called once per adaptive application that is subscribed to the event.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] event_id       The event id.
   * \param[in] message_size   The size of the complete message (header + payload) in bytes.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceSomeIpEventReceiveSync(osabstraction::process::ProcessId const process_id,
                                           someip_protocol::internal::ServiceId const service_id,
                                           someip_protocol::internal::MajorVersion const major_version,
                                           someip_protocol::internal::InstanceId const instance_id,
                                           someip_protocol::internal::EventId const event_id,
                                           std::size_t const message_size) noexcept = 0;

  /*!
   * \brief Called every time a PDU event notification has been received
   * \details This API is called once per adaptive application that is subscribed to the event.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] event_id       The event id.
   * \param[in] message_size   The size of the complete message (header + payload) in bytes.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TracePduEventReceiveSync(osabstraction::process::ProcessId const process_id,
                                        someip_protocol::internal::ServiceId const service_id,
                                        someip_protocol::internal::InstanceId const instance_id,
                                        someip_protocol::internal::EventId const event_id,
                                        std::size_t const message_size) noexcept = 0;

  /*!
   * \brief Called every time a method request has been received.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] method_id      The method id.
   * \param[in] message_size   The size of the complete message (header + payload) in bytes.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodRequestReceiveSync(osabstraction::process::ProcessId const process_id,
                                             someip_protocol::internal::ServiceId const service_id,
                                             someip_protocol::internal::MajorVersion const major_version,
                                             someip_protocol::internal::InstanceId const instance_id,
                                             someip_protocol::internal::MethodId const method_id,
                                             std::size_t const message_size) noexcept = 0;

  /*!
   * \brief Called every time a method response has been received.
   *
   * \param[in] process_id     The service interface id of the service in question.
   * \param[in] service_id     The service interface id of the service in question.
   * \param[in] major_version  Major version of the service in question.
   * \param[in] instance_id    The service instance id of the service in question.
   * \param[in] method_id      The method id.
   * \param[in] message_size   The size of the complete message (header + payload) in bytes.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodResponseReceiveSync(osabstraction::process::ProcessId const process_id,
                                              someip_protocol::internal::ServiceId const service_id,
                                              someip_protocol::internal::MajorVersion const major_version,
                                              someip_protocol::internal::InstanceId const instance_id,
                                              someip_protocol::internal::MethodId const method_id,
                                              std::size_t const message_size) noexcept = 0;
};

}  // namespace tracing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_NETWORK_TRACING_INTERFACE_H_
