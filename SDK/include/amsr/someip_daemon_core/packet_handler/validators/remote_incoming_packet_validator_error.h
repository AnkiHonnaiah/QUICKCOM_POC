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
/**        \file  remote_incoming_packet_validator_error.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_REMOTE_INCOMING_PACKET_VALIDATOR_ERROR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_REMOTE_INCOMING_PACKET_VALIDATOR_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief Packet validation return codes.
 */
enum class RemoteIncomingPacketValidatorError : ara::core::ErrorDomain::CodeType {
  kHeaderValidationError_WrongProtocolVersion = 0x00U, /*! Header validation failed: wrong protocol version */
  kHeaderValidationError_MalformedMessage = 0x01U,     /*! Header validation failed: malformed message */
  kHeaderValidationError_WrongMessageType = 0x02U,     /*! Header validation failed: wrong message type */
  kConfigurationError_UnknownMethod = 0x03U,           /*! Configuration check failed: method not configured */
  kConfigurationError_UnknownEvent = 0x04U,            /*! Configuration check failed: event not configured */
  kConfigurationError_WrongInterfaceVersion = 0x05U,   /*! Configuration check failed: wrong interface version */
  kConfigurationError_UnknownService = 0x06U,          /*! Configuration check failed: unknown service ID */
  kTransmissionProtocolError_MethodNotOk = 0x07U, /*! Transmission protocol error: method has wrong protocol version */
  kTransmissionProtocolError_EventNotOk = 0x08U,  /*! Transmission protocol error: event has wrong protocol version */
  kSecurityValidationError = 0x09U,               /*! IAM denied sending a method request */
  kNotOk = 0x0AU,                                 /*! Generic error */
};

/*!
 * \brief Error messages
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
static std::array<char const*, 11U> constexpr kRemoteIncomingPacketValidatorErrorMessages{
    {"Message verification failed: WrongProtocolVersion", "Message verification failed: MalformedMessage",
     "Message verification failed: WrongMessageType",
     "Message configuration compatibility validation failed: UnknownMethod",
     "Message configuration compatibility validation failed: UnknownEvent",
     "Message configuration compatibility validation failed: WrongInterfaceVersion",
     "Message configuration compatibility validation failed: UnknownService",
     "Transmission Protocol Error for Method: Unexpected Protocol",
     "Transmission Protocol Error for Event: Unexpected Protocol",
     "SecurityValidationError: Access rights were denied by IAM",
     "Encountered validation error: Client Id, Length or Return Code are invalid"}};

/*!
 * \brief ara::com specific exception.
 */
class RemoteIncomingPacketValidatorException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class RemoteIncomingPacketValidatorErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = RemoteIncomingPacketValidatorError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = RemoteIncomingPacketValidatorException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   *
   * \steady      FALSE
   */
  constexpr RemoteIncomingPacketValidatorErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  StringType Name() const noexcept final { return "RemoteIncomingPacketValidatorError"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \synchronous TRUE
   * \steady      FALSE
   */
  StringType Message(CodeType error_code) const noexcept final {
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kRemoteIncomingPacketValidatorErrorMessages.size()) {
        index = code;
      }
    }
    return kRemoteIncomingPacketValidatorErrorMessages[index];
  }

  /*!
   * \brief Throw an exception based on an error code or abort.
   * \param[in] error_code Error code to cause a throw or an abort.
   * \throws SomeIpBindingException for all error codes.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre          -
   * \context     Network
   * \synchronous TRUE
   * \steady      FALSE
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x5D2205FC4794C7B1};
};

/*!
 * \brief Global SdEntryInterpretationErrorDomain instance.
 */
constexpr RemoteIncomingPacketValidatorErrorDomain kRemoteIncomingPacketValidatorErrorDomain;

/*!
 * \brief Get reference to the global SdEntryInterpretationErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     Network
 * \synchronous TRUE
 * \steady      FALSE
 */
inline constexpr ara::core::ErrorDomain const& GetRemoteEndpointsValidatorErrorDomain() noexcept {
  return kRemoteIncomingPacketValidatorErrorDomain;
}

/*!
 * \brief Creates an error code from SomeIpBindingErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre          -
 * \context     ANY
 * \synchronous TRUE
 * \steady      FALSE
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(
    RemoteIncomingPacketValidatorErrorDomain::Errc const code,
    RemoteIncomingPacketValidatorErrorDomain::SupportDataType const data, char const* message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code),
                              GetRemoteEndpointsValidatorErrorDomain(), data, message);
}

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_REMOTE_INCOMING_PACKET_VALIDATOR_ERROR_H_
