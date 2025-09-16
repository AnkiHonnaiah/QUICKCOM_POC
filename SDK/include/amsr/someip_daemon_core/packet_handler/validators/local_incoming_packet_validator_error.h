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
/**        \file  local_incoming_packet_validator_error.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_LOCAL_INCOMING_PACKET_VALIDATOR_ERROR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_LOCAL_INCOMING_PACKET_VALIDATOR_ERROR_H_

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
enum class LocalIncomingPacketValidatorError : ara::core::ErrorDomain::CodeType {
  kNotOk = 0x00U,                                      /*! Generic error */
  kHeaderValidationError_WrongProtocolVersion = 0x01U, /*! Header validation failed: wrong protocol version */
  kHeaderValidationError_WrongMessageType = 0x02U,     /*! Header validation failed: wrong message type */
  kConfigurationError_UnknownMethod = 0x03U,           /*! Configuration check failed: method not configured */
  kConfigurationError_UnknownEvent = 0x04U,            /*! Configuration check failed: event not configured */
  kConfigurationError_WrongInterfaceVersion = 0x05U,   /*! Configuration check failed: wrong interface version */
  kConfigurationError_UnknownService = 0x06U,          /*! Configuration check failed: unknown service ID */
  kSecurityValidationError = 0x07U,                    /*! IAM denied sending a method request */
  kRuntimeError_ServiceNotOffered = 0x08U,             /*! Runtime check failed: service is not offered */
  kRuntimeError_ConnectionNotAvailable = 0x09U,        /*! Runtime check failed: connection is not established*/
  kRuntimeError_ConnectionTransmissionFailed = 0X0AU   /*! Runtime check failed: connection transmission failed*/
};

/*!
 * \brief Error messages
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
static std::array<char const*, 11U> constexpr kLocalIncomingPacketValidatorErrorMessages{
    {"Encountered validation error: Client Id, Length or Return Code are invalid",
     "Message verification failed: WrongProtocolVersion", "Message verification failed: WrongMessageType",
     "Message configuration compatibility validation failed: UnknownMethod",
     "Message configuration compatibility validation failed: UnknownEvent",
     "Message configuration compatibility validation failed: WrongInterfaceVersion",
     "Message configuration compatibility validation failed: UnknownService",
     "SecurityValidationError: Access rights were denied by IAM", "Runtime check failed: service is not offered",
     "Runtime check failed: connection is not established", "Runtime check failed: connection transmission failed"}};

/*!
 * \brief ara::com specific exception.
 */
class LocalIncomingPacketValidatorException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class LocalIncomingPacketValidatorErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = LocalIncomingPacketValidatorError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = LocalIncomingPacketValidatorException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   *
   * \steady      FALSE
   */
  constexpr LocalIncomingPacketValidatorErrorDomain() noexcept : ErrorDomain(kId) {}

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
  StringType Name() const noexcept final { return "LocalIncomingPacketValidatorError"; }

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
      if (code < kLocalIncomingPacketValidatorErrorMessages.size()) {
        index = code;
      }
    }
    return kLocalIncomingPacketValidatorErrorMessages[index];
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
  static constexpr ErrorDomain::IdType kId{0x5D2205FC4794C7B2};
};

/*!
 * \brief Global LocalIncomingPacketValidatorErrorDomain instance.
 */
constexpr LocalIncomingPacketValidatorErrorDomain kLocalIncomingPacketValidatorErrorDomain;

/*!
 * \brief Get reference to the global LocalIncomingPacketValidatorErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     Network
 * \synchronous TRUE
 * \steady      FALSE
 */
inline constexpr ara::core::ErrorDomain const& GetLocalEndpointsValidatorErrorDomain() noexcept {
  return kLocalIncomingPacketValidatorErrorDomain;
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
inline constexpr ara::core::ErrorCode MakeErrorCode(LocalIncomingPacketValidatorErrorDomain::Errc const code,
                                                    LocalIncomingPacketValidatorErrorDomain::SupportDataType const data,
                                                    char const* message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code),
                              GetLocalEndpointsValidatorErrorDomain(), data, message);
}

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_LOCAL_INCOMING_PACKET_VALIDATOR_ERROR_H_
