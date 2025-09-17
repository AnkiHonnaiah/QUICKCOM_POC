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
/*!        \file  log_builder.h
 *         \unit  SomeIpBinding::SomeIpBindingCore::Utilities
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOG_BUILDER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOG_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/log/logging.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Helper class for SomeIpBinding types logging.
 */
class LogBuilder {
 public:
  /*!
   * \brief Log SOME/IP required service instance ID.
   * \param[in] s  The logstream to write to.
   * \param[in] id The required service instance ID to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogRequiredServiceInstanceId(ara::log::LogStream& s, RequiredServiceInstanceId const& id) noexcept;

  /*!
   * \brief Log SOME/IP required service instance ID to string stream.
   * \param[in] s  The string stream to write to.
   * \param[in] id The required service instance ID to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogRequiredServiceInstanceIdToStringStream(
      ::amsr::someip_binding_core::internal::logging::StringStream& s, RequiredServiceInstanceId const& id) noexcept;

  /*!
   * \brief Log SOME/IP provided service instance ID.
   * \param[in] s  The logstream to write to.
   * \param[in] id The required service instance ID to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogProvidedServiceInstanceId(ara::log::LogStream& s, ProvidedServiceInstanceId const& id) noexcept;

  /*!
   * \brief Log SOME/IP provided service instance ID to string stream.
   * \param[in] s  The string stream to write to.
   * \param[in] id The provided service instance ID to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogProvidedServiceInstanceIdToStringStream(
      ::amsr::someip_binding_core::internal::logging::StringStream& s, ProvidedServiceInstanceId const& id) noexcept;

  /*!
   * \brief Log SOME/IP event ID.
   * \param[in] s                      The logstream to write to
   * \param[in] someip_event_identity  Data structure containing service_id, instance_id, major_version and event_id
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogEventId(
      ara::log::LogStream& s,
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity) noexcept;
  /*!
   * \brief Log SOME/IP method ID.
   * \param[in] s           The logstream to write to
   * \param[in] service_id  The service ID to be logged.
   * \param[in] method_id   The method ID to be logged.
   * \param[in] client_id   The client ID to be logged.
   * \param[in] instance_id The instance ID to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogMethodId(ara::log::LogStream& s, ::amsr::someip_protocol::internal::ServiceId const service_id,
                          ::amsr::someip_protocol::internal::MethodId const method_id,
                          ::amsr::someip_protocol::internal::ClientId const client_id,
                          ::amsr::someip_protocol::internal::InstanceId const instance_id) noexcept;

  /*!
   * \brief Log subscription state as string
   * \param[in] s     The logstream to write to.
   * \param[in] state subscription state
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogSubscriptionStateAsString(ara::log::LogStream& s,
                                           ::amsr::someip_protocol::internal::SubscriptionState const& state) noexcept;

  /*!
   * \brief Log message type as string
   * \tparam Stream          Stream type
   * \param[in] s            The stream to write to.
   * \param[in] message_type Message Type
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Stream>
  static void LogMessageTypeAsString(Stream& s,
                                     ::amsr::someip_protocol::internal::SomeIpMessageType const message_type) noexcept {
    switch (message_type) {
      case ::amsr::someip_protocol::internal::SomeIpMessageType::kNotification: {
        s << "Notification";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpMessageType::kRequest: {
        s << "Request";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpMessageType::kRequestNoReturn: {
        s << "Request No Return";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpMessageType::kError: {
        s << "Error";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpMessageType::kResponse: {
        s << "Response";
        break;
      }
      default: {
        s << "Unknown message type";
        break;
      }
    }
  }

  /*!
   * \brief Log return code as string
   * \tparam Stream          Stream type
   * \param[in] s           The stream to write to.
   * \param[in] return_code Return code
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC Metric-HIS.VG: MD_SOMEIPBINDING_Metric-HIS.VG_CodeToMessageTranslation
  template <typename Stream>
  static void LogReturnCodeAsString(Stream& s,
                                    ::amsr::someip_protocol::internal::SomeIpReturnCode const return_code) noexcept {
    switch (return_code) {
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kOk: {
        s << "OK";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kNotReady: {
        s << "Not Ready";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kNotReachable: {
        s << "Not Reachable";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk: {
        s << "Not OK";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kMalformedMessage: {
        s << "Malformed Message";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kUnknownMethod: {
        s << "Unknown Method";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kUnknownService: {
        s << "Unknown Service";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kWrongMessageType: {
        s << "Wrong Message Type";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion: {
        s << "Wrong Protocol Version";
        break;
      }
      case ::amsr::someip_protocol::internal::SomeIpReturnCode::kWrongInterfaceVersion: {
        s << "Wrong Interface Version";
        break;
      }
      default: {
        s << "Unknown return code";
        break;
      }
    }
  }

  /*!
   * \brief Log SOME/IP header.
   * \param[in] s       The logstream to write to
   * \param[in] header  The header to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogSomeIpHeader(ara::log::LogStream& s,
                              ::amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept;

  /*!
   * \brief Log SOME/IP header into a string stream.
   * \param[in] s  The string stream to write to.
   * \param[in] header  The header to be logged.
   * \pre -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static void LogSomeIpHeader(::amsr::someip_binding_core::internal::logging::StringStream& s,
                              ::amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept;
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOG_BUILDER_H_
