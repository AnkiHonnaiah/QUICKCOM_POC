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
/**        \file     someip_daemon_client_error_code.h
 *         \brief    Contains error handling related types.
 *
 *         \details  Contains Result alias, error domain & error codes.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_ERROR_CODE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_ERROR_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief SOME/IP daemon client specific error codes.
 * \vpublic
 */
enum class SomeIpDaemonClientErrc : amsr::core::ErrorDomain::CodeType {
  kOk = 0U,
  kNotOk = 1U,
  kAccessDenied = 2U,
  kNotConnected = 3U,
  kDisconnected = 4U,
  kPayloadEmpty = 5U,
  kIncorrectResponseMsgLength = 6U,
  kRuntimeResourceOutOf = 7U,
  kAlreadyConnected = 8U,
  kClientIdsOverflow = 9U,
  kConfigurationError = 10U,
  kPacketNull = 11U
};

/*!
 * \brief SOME/IP daemon client specific exception
 * \vpublic
 */
class SomeIpDaemonClientException : public amsr::core::Exception {
 public:
  /*!
   * \brief Inherited constructor
   */
  using Exception::Exception;
};

/*!
 * \brief domain for all SOME/IP daemon client related errors.
 * \vpublic
 */
class SomeIpDaemonClientErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief Unique domain ID.
   */
  constexpr static IdType kId{0x5360fe374fd8c4c5};

 public:
  /*!
   * \brief Implements the Errc interface-type.
   */
  using Errc = SomeIpDaemonClientErrc;

  /*!
   * \brief Implements the Exception interface-type.
   */
  using Exception = SomeIpDaemonClientException;

  /*!
   * \brief Constructs a SomeIpDaemonClientErrorDomain type.
   * \vpublic
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr SomeIpDaemonClientErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the name of the ErrorDomain.
   * \return the name of the ErrorDomain as a string.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto Name() const noexcept -> StringType final { return "SomeIpDaemonClient"; }

  /*!
   * \brief Converts an error code into a message.
   * \vpublic
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto Message(CodeType error_code) const noexcept -> StringType final {
    constexpr static ara::core::Array<char const*, 11> kMessages{
        /* kOk */ "Unknown Error Code",
        /* kNotOk */ "Failed to request the service as no remote server is found.",
        /* kAccessDenied */ "Access Denied",
        /* kNotConnected */ "Failed to communication with SOME/IP Daemon.",
        /* kDisconnected */ "Disconnect() called before receiving response for the RequestService command request.",
        /* kPayloadEmpty */ "Received empty response message payload.",
        /* kIncorrectResponseMsgLength */ "Failed to deserialize the RequestService command control response.",
        /* kRuntimeResourceOutOf */ "Failed to send message queue full.",
        // kAlreadyConnected
        "Connection to SOME/IP daemon is already established or is currently being established.",
        /* kClientIdsOverflow */ "Application reaches the maximum number of simultaneous client Ids.",
        /* kConfigurationError */ "Configuration mismatch when requesting a local server."};

    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {
        index = code;
      }
    }

    return kMessages.at(index);
  }

  /*!
   * \brief Throw an exception based on an error code or abort.
   * \vpublic
   * \param[in] error_code Error code to cause a throw or an abort.
   * \throws SomeIpDaemonClientException with the passed error code.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace internal {
/*!
 * \brief Global SomeIpDaemonClientErrorDomain instance.
 */
constexpr SomeIpDaemonClientErrorDomain kSomeIpDaemonClientErrorDomain;

}  // namespace internal

/*!
 * \brief SomeIpDaemonClientErrorDomain a reference to the global SomeIpDaemonClientErrorDomain instance.
 * \return SomeIpDaemonClientErrorDomain instance.
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr auto GetSomeIpDaemonClientErrorDomain() noexcept -> amsr::core::ErrorDomain const& {
  return internal::kSomeIpDaemonClientErrorDomain;
}

/*!
 * \brief Creates an error code from SomeIpDaemonClientErrorDomain.
 * \vprivate
 * \param code The specific error code.
 * \param data Vendor defined support data.
 * \param message An optional message for this error.
 * \returns The constructed error code.
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr auto MakeErrorCode(SomeIpDaemonClientErrorDomain::Errc code,
                                    SomeIpDaemonClientErrorDomain::SupportDataType data, char const* message)
    -> amsr::core::ErrorCode {
  return amsr::core::ErrorCode{static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSomeIpDaemonClientErrorDomain(),
                               data, message};
}

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_ERROR_CODE_H_
