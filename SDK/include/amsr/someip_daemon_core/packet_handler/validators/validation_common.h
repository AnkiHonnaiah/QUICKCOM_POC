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
/*!        \file  validation_common.h
 *        \brief  Validation related helper functions
 *
 *      \details  Contains validation related helper functions that help in logging and error mapping
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_VALIDATION_COMMON_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_VALIDATION_COMMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "ara/log/logstream.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief Packet validation return codes.
 */
enum class PacketValidatorReturnCode : std::uint8_t {
  kOk = 0x00U,                                         /*! No error occurred. */
  kHeaderValidationError_WrongProtocolVersion = 0x01U, /*! Header validation failed: wrong protocol version */
  kHeaderValidationError_MalformedMessage = 0x02U,     /*! Header validation failed: malformed message */
  kHeaderValidationError_WrongMessageType = 0x03U,     /*! Header validation failed: wrong message type */
  kConfigurationError_UnknownMethod = 0x11U,           /*! Configuration check failed: method not configured */
  kConfigurationError_UnknownEvent = 0x12U,            /*! Configuration check failed: event not configured */
  kConfigurationError_WrongInterfaceVersion = 0x13U,   /*! Configuration check failed: wrong interface version */
  kConfigurationError_UnknownService = 0x14U,          /*! Configuration check failed: unknown service ID */
  kTransmissionProtocolError_MethodNotOk = 0x21U, /*! Transmission protocol error: method has wrong protocol version */
  kTransmissionProtocolError_EventNotOk = 0x22U,  /*! Transmission protocol error: event has wrong protocol version */
  kSecurityValidationError = 0x31U,               /*! IAM denied sending a method request */
  kNotOk = 0x50U,                                 /*! Generic error */
};

/*!
 * \brief Convert a packet validator's return code to its corresponding error message and log it.
 *
 * \param[in,out] s Log stream to use.
 * \param[in] return_code Packet validator's return code.
 *
 * \steady TRUE
 * \internal
 * - If the return code is of known value
 *   - Convert the return code to a representative message and log it.
 * - Else
 *   - Log an error message about an unknown return code.
 * \endinternal
 */
inline void LogValidationError(ara::log::LogStream& s, PacketValidatorReturnCode const return_code) {
  // PacketValidatorReturnCode::kNotOk is intentionally left out of the map.
  ara::core::Map<PacketValidatorReturnCode, char const*> log_validation_error_map{
      {PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion,
       "Message verification failed: WrongProtocolVersion"},
      {PacketValidatorReturnCode::kHeaderValidationError_MalformedMessage,
       "Message verification failed: MalformedMessage"},
      {PacketValidatorReturnCode::kHeaderValidationError_WrongMessageType,
       "Message verification failed: WrongMessageType"},
      {PacketValidatorReturnCode::kConfigurationError_UnknownMethod,
       "Message configuration compatibility validation failed: UnknownMethod"},
      {PacketValidatorReturnCode::kConfigurationError_UnknownEvent,
       "Message configuration compatibility validation failed: UnknownEvent"},
      {PacketValidatorReturnCode::kConfigurationError_WrongInterfaceVersion,
       "Message configuration compatibility validation failed: WrongInterfaceVersion"},
      {PacketValidatorReturnCode::kConfigurationError_UnknownService,
       "Message configuration compatibility validation failed: UnknownService"},
      {PacketValidatorReturnCode::kTransmissionProtocolError_MethodNotOk,
       "Transmission Protocol Error for Method: Unexpected Protocol"},
      {PacketValidatorReturnCode::kTransmissionProtocolError_EventNotOk,
       "Transmission Protocol Error for Event: Unexpected Protocol"},
      {PacketValidatorReturnCode::kSecurityValidationError,
       "SecurityValidationError: Access rights were denied by IAM."},
  };

  auto const iter = log_validation_error_map.find(return_code);

  if (iter != log_validation_error_map.end()) {
    s << iter->second;
  } else {
    s << "Encountered validation error: Client Id, Length or Return Code are invalid ("
      << static_cast<std::uint32_t>(return_code) << ").";
  }
}

/*!
 * \brief Convert a packet validator's return code to its corresponding SOME/IP return code.
 *
 * \param[in] return_code Packet validator's return code.
 *
 * \steady  TRUE
 * \return SOME/IP return code.
 *
 * \internal
 * - If the return code is of known value
 *   - Convert the return code to a corresponding SOME/IP return code
 * - Else
 *   - Set the return code to represent a not OK status
 * - Return the return code
 * \endinternal
 */
inline someip_protocol::internal::SomeIpReturnCode ToSomeIpReturnCode(PacketValidatorReturnCode return_code) {
  someip_protocol::internal::SomeIpReturnCode result{someip_protocol::internal::SomeIpReturnCode::kNotOk};
  switch (return_code) {
    case PacketValidatorReturnCode::kOk: {
      result = someip_protocol::internal::SomeIpReturnCode::kOk;
      break;
    }
    case PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion: {
      result = someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion;
      break;
    }
    case PacketValidatorReturnCode::kHeaderValidationError_MalformedMessage: {
      result = someip_protocol::internal::SomeIpReturnCode::kMalformedMessage;
      break;
    }
    case PacketValidatorReturnCode::kHeaderValidationError_WrongMessageType: {
      result = someip_protocol::internal::SomeIpReturnCode::kWrongMessageType;
      break;
    }
    case PacketValidatorReturnCode::kConfigurationError_UnknownMethod:
    case PacketValidatorReturnCode::kConfigurationError_UnknownEvent: {
      result = someip_protocol::internal::SomeIpReturnCode::kUnknownMethod;
      break;
    }
    case PacketValidatorReturnCode::kConfigurationError_WrongInterfaceVersion: {
      result = someip_protocol::internal::SomeIpReturnCode::kWrongInterfaceVersion;
      break;
    }
    case PacketValidatorReturnCode::kConfigurationError_UnknownService: {
      result = someip_protocol::internal::SomeIpReturnCode::kUnknownService;
      break;
    }
    case PacketValidatorReturnCode::kSecurityValidationError: {
      result = someip_protocol::internal::SomeIpReturnCode::kNotReachable;
      break;
    }
    case PacketValidatorReturnCode::kNotOk:  // Fall through unspecified error(s)
    case PacketValidatorReturnCode::kTransmissionProtocolError_MethodNotOk:
    case PacketValidatorReturnCode::kTransmissionProtocolError_EventNotOk:
    default: {
      result = someip_protocol::internal::SomeIpReturnCode::kNotOk;
      break;
    }
  }
  return result;
}

/*!
 * \brief Convert a SOME/IP return code to its corresponding packet validator's return code.
 *
 * \param[in] error_code SOME/IP return code to convert.
 *
 * \steady  TRUE
 * \return Packet validator's return code.
 *
 * \internal
 * - Set the return code to represent a not OK status
 * - If the return code is of known value
 *   - Convert the return code to a corresponding packet validator's return code
 * - Return the return code
 * \endinternal
 */
inline PacketValidatorReturnCode ToPacketValidatorReturnCode(
    someip_protocol::internal::SomeIpReturnCode const error_code) {
  PacketValidatorReturnCode return_code{PacketValidatorReturnCode::kNotOk};

  switch (error_code) {
    case someip_protocol::internal::SomeIpReturnCode::kOk: {
      return_code = PacketValidatorReturnCode::kOk;
      break;
    }
    case someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion: {
      return_code = PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion;
      break;
    }
    case someip_protocol::internal::SomeIpReturnCode::kMalformedMessage: {
      return_code = PacketValidatorReturnCode::kHeaderValidationError_MalformedMessage;
      break;
    }
    case someip_protocol::internal::SomeIpReturnCode::kWrongMessageType: {
      return_code = PacketValidatorReturnCode::kHeaderValidationError_WrongMessageType;
      break;
    }
    case someip_protocol::internal::SomeIpReturnCode::kNotOk: {
      return_code = PacketValidatorReturnCode::kNotOk;
      break;
    }
    default:
      // Nothing to do.
      break;
  }

  return return_code;
}

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_VALIDATION_COMMON_H_
