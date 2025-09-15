/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/generic/generic_error_domain.h
 *        \brief  Error code enum and domain class for GenericErrorDomain
 *         \unit  VaCommonLib::ErrorHandling::GenericErrorDomain
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_GENERIC_GENERIC_ERROR_DOMAIN_H_
#define LIB_VAC_INCLUDE_AMSR_GENERIC_GENERIC_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iterator>
#include <limits>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace generic {

/*!
 * \brief Generic error code enum.
 * \vpublic
 */
enum class GenErrc : amsr::core::ErrorDomain::CodeType {

  kProtocol,

  kProtocolWrongSequence,
  kProtocolWrongFormat,
  kProtocolInvalidParameter,

  kSystem,

  kSystemConfiguration,
  kSystemConfigurationNotAvailable,
  kSystemConfigurationInvalid,

  kSystemPrivileges,
  kSystemPrivilegesNoAccess,

  kSystemCommandLine,
  kSystemCommandLineMissingParameter,
  kSystemCommandLineUnknownParameter,
  kSystemCommandLineInvalidParameter,
  kSystemEnvVariable,
  kSystemEnvVariableMissing,
  kSystemEnvVariableInvalid,

  kRuntime,

  kRuntimeResource,
  kRuntimeResourceNotAvailable,
  kRuntimeResourceNotInitialized,
  kRuntimeResourceInvalidHandle,
  kRuntimeResourceBusy,
  kRuntimeResourceOutOf,

  kRuntimeThread,
  kRuntimeThreadCreationFailed,

  kRuntimeBuffer,
  kRuntimeBufferTooSmall,
  kRuntimeBufferEmpty,
  kRuntimeBufferSerializationError,
  kRuntimeBufferInvalid,

  kRuntimeConnection,
  kRuntimeConnectionSetupError,
  kRuntimeConnectionTransmissionFailed,
  kRuntimeConnectionTimeout,
  kRuntimeConnectionLost,

  kRuntimeEndpoint,
  kRuntimeEndpointInvalid,
  kRuntimeEndpointNotAvailable,
  kRuntimeEndpointAlreadyUsed,
  kRuntimeEndpointOpenError
};

/*!
 * \brief Exception type for GenericErrorDomain.
 * \vpublic
 */
class GenericException : public ara::core::Exception {
 public:
  /*!
   * \brief Inheriting constructor.
   */
  using ara::core::Exception::Exception;
};

/*!
 * \brief Error domain for generic errors.
 * \trace DSGN-VaCommonLib-ErrorHandling
 * \vpublic
 */
class GenericErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief ID of the domain.
   */
  static constexpr amsr::core::ErrorDomain::IdType kId{0xc0000000001e5e9f};

 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = GenErrc;

  /*!
   * \brief Exception type of the domain.
   */
  using Exception = GenericException;

  /*!
   * \brief Constructor. Initializes the domain id with GenericErrorDomain::kId.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr GenericErrorDomain() noexcept : amsr::core::ErrorDomain(kId) {}

  /*!
   * \brief Returns the name of the domain.
   * \pre       -
   * \return "Generic".
   * \spec
   *   requires true;
   * \endspec
   */
  char const* Name() const noexcept final { return "Generic"; }

  /*!
   * \brief     Translates the given error code into a text message.
   * \param[in] error_code The domain-specific error code.
   * \pre       -
   * \return    The textual representation of the given error code.
   *            Default message if the error code is unknown.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC Metric-HIS.VG: MD_VAC_Metric-HIS.VG
  char const* Message(amsr::core::ErrorDomain::CodeType const error_code) const noexcept final {
    StringType return_msg{"Unknown error code"};
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_VAC_A7.2.1_valueIsNotWithinEnumeratorRange
    GenErrc const errc{static_cast<GenErrc>(error_code)};
    switch (errc) {
      case GenErrc::kProtocol:
        return_msg = "Protocol error";
        break;
      case GenErrc::kProtocolWrongSequence:
        return_msg = "Wrong protocol sequence";
        break;
      case GenErrc::kProtocolWrongFormat:
        return_msg = "Wrong protocol format";
        break;
      case GenErrc::kProtocolInvalidParameter:
        return_msg = "Invalid protocol parameter";
        break;
      case GenErrc::kSystem:
        return_msg = "System error";
        break;
      case GenErrc::kSystemConfiguration:
        return_msg = "Configuration error";
        break;
      case GenErrc::kSystemConfigurationNotAvailable:
        return_msg = "Configuration not available";
        break;
      case GenErrc::kSystemConfigurationInvalid:
        return_msg = "Invalid configuration";
        break;
      case GenErrc::kSystemPrivileges:
        return_msg = "Privileges error";
        break;
      case GenErrc::kSystemPrivilegesNoAccess:
        return_msg = "No access to privileges";
        break;
      case GenErrc::kSystemCommandLine:
        return_msg = "Command line error";
        break;
      case GenErrc::kSystemCommandLineMissingParameter:
        return_msg = "Missing command line parameter";
        break;
      case GenErrc::kSystemCommandLineUnknownParameter:
        return_msg = "Unknown command line parameter";
        break;
      case GenErrc::kSystemCommandLineInvalidParameter:
        return_msg = "Invalid command line parameter";
        break;
      case GenErrc::kSystemEnvVariable:
        return_msg = "Environment variable error";
        break;
      case GenErrc::kSystemEnvVariableMissing:
        return_msg = "Missing environment variable";
        break;
      case GenErrc::kSystemEnvVariableInvalid:
        return_msg = "Invalid environment variable";
        break;
      case GenErrc::kRuntime:
        return_msg = "Runtime error";
        break;
      case GenErrc::kRuntimeResource:
        return_msg = "Resource error";
        break;
      case GenErrc::kRuntimeResourceNotAvailable:
        return_msg = "Resource not available";
        break;
      case GenErrc::kRuntimeResourceNotInitialized:
        return_msg = "Resource not initialized";
        break;
      case GenErrc::kRuntimeResourceInvalidHandle:
        return_msg = "Invalid resource handle";
        break;
      case GenErrc::kRuntimeResourceBusy:
        return_msg = "Resource busy";
        break;
      case GenErrc::kRuntimeResourceOutOf:
        return_msg = "Out of resources";
        break;
      case GenErrc::kRuntimeThread:
        return_msg = "Thread error";
        break;
      case GenErrc::kRuntimeThreadCreationFailed:
        return_msg = "Thread creation failed";
        break;
      case GenErrc::kRuntimeBuffer:
        return_msg = "Buffer error";
        break;
      case GenErrc::kRuntimeBufferTooSmall:
        return_msg = "Buffer size too small";
        break;
      case GenErrc::kRuntimeBufferEmpty:
        return_msg = "Buffer is empty";
        break;
      case GenErrc::kRuntimeBufferSerializationError:
        return_msg = "Buffer serialization error";
        break;
      case GenErrc::kRuntimeBufferInvalid:
        return_msg = "Invalid buffer";
        break;
      case GenErrc::kRuntimeConnection:
        return_msg = "Connection error";
        break;
      case GenErrc::kRuntimeConnectionSetupError:
        return_msg = "Connection setup error";
        break;
      case GenErrc::kRuntimeConnectionTransmissionFailed:
        return_msg = "Connection transmission failed";
        break;
      case GenErrc::kRuntimeConnectionTimeout:
        return_msg = "Connection timeout";
        break;
      case GenErrc::kRuntimeConnectionLost:
        return_msg = "Connection lost";
        break;
      case GenErrc::kRuntimeEndpoint:
        return_msg = "Endpoint error";
        break;
      case GenErrc::kRuntimeEndpointInvalid:
        return_msg = "Invalid endpoint";
        break;
      case GenErrc::kRuntimeEndpointNotAvailable:
        return_msg = "Endpoint not available";
        break;
      case GenErrc::kRuntimeEndpointAlreadyUsed:
        return_msg = "Endpoint already used";
        break;
      case GenErrc::kRuntimeEndpointOpenError:
        return_msg = "Endpoint open error";
        break;
      default:
        // Do nothing for unknown error codes.
        break;
    }
    return return_msg;
  }

  /*!
   * \brief  Throws the given errorCode as Exception.
   * \param  error_code Error code to be thrown.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \throw  GenericException for all error codes.
   * \remark If the is compiled without exceptions, this function call will terminate.
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<GenericException>(error_code);
  }
};

namespace internal {
/*!
 * \brief Global GenericErrorDomain instance.
 * \trace DSGN-VaCommonLib-ErrorHandling
 */
// VECTOR Next Line AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
constexpr GenericErrorDomain g_genericErrorDomain;
}  // namespace internal

/*!
 * \brief  Returns a reference to the global GenericErrorDomain instance.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return GenericErrorDomain instance.
 * \vpublic
 */
inline constexpr amsr::core::ErrorDomain const& GetGenericDomain() noexcept { return internal::g_genericErrorDomain; }

/*!
 * \brief     Creates an ErrorCode from the GenericErrorDomain.
 * \param[in] code error code enum value.
 * \param[in] data support data.
 * \param     message user message.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    the created error code.
 * \vpublic
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(GenericErrorDomain::Errc code,
                                                     amsr::core::ErrorDomain::SupportDataType data,
                                                     char const* message) noexcept {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetGenericDomain(), data, message);
}

// Helper functions for generic handling of errors.

/*!
 * \brief Checks if error code is ProtocolError.
 * \param ec error code enum value.
 * \return true if ec is a ProtocolError, false otherwise.
 * \vpublic
 */
constexpr bool IsProtocolError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kProtocol) && (ec < GenErrc::kSystem);
}

/*!
 * \brief Checks if error code is SystemConfigurationError.
 * \param ec error code enum value.
 * \return true if ec is a SystemConfigurationError, false otherwise.
 * \vpublic
 */
constexpr bool IsSystemConfigurationError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kSystemConfiguration) && (ec < GenErrc::kSystemPrivileges);
}

/*!
 * \brief Checks if error code is SystemPrivilegesError.
 * \param ec error code enum value.
 * \return true if ec is a SystemPrivilegesError, false otherwise.
 * \vpublic
 */
constexpr bool IsSystemPrivilegesError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kSystemPrivileges) && (ec < GenErrc::kSystemCommandLine);
}

/*!
 * \brief Checks if error code is SystemCommandError.
 * \param ec error code enum value.
 * \return true if ec is a SystemCommandError, false otherwise.
 * \vpublic
 */
constexpr bool IsSystemCommandError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kSystemCommandLine) && (ec < GenErrc::kSystemEnvVariable);
}

/*!
 * \brief Checks if error code is SystemEnv error.
 * \param ec error code enum value.
 * \return true if ec is a SystemEnv error, false otherwise.
 * \vpublic
 */
constexpr bool IsSystemEnvError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kSystemEnvVariable) && (ec < GenErrc::kRuntime);
}

/*!
 * \brief Checks if error code is SystemError.
 * \param ec error code enum value.
 * \return true if ec is a SystemError, false otherwise.
 * \vpublic
 */
constexpr bool IsSystemError(GenErrc const ec) noexcept {
  return (ec == GenErrc::kSystem) || IsSystemConfigurationError(ec) || IsSystemPrivilegesError(ec) ||
         IsSystemCommandError(ec) || IsSystemEnvError(ec);
}

/*!
 * \brief Checks if error code is RuntimeResourceError.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeResourceRrror, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeResourceError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kRuntimeResource) && (ec < GenErrc::kRuntimeThread);
}

/*!
 * \brief Checks if error code is RuntimeThreadError.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeThreadError, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeThreadError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kRuntimeThread) && (ec < GenErrc::kRuntimeBuffer);
}

/*!
 * \brief Checks if error code is a RuntimeBuffError.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeBufferError, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeBufferError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kRuntimeBuffer) && (ec < GenErrc::kRuntimeConnection);
}

/*!
 * \brief Checks if error code is a RuntimeConnectionError.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeConnectionError, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeConnectionError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kRuntimeConnection) && (ec < GenErrc::kRuntimeEndpoint);
}

/*!
 * \brief Checks if error code is a RuntimeEndpointError.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeEndpointError, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeEndpointError(GenErrc const ec) noexcept {
  return (ec >= GenErrc::kRuntimeEndpoint) && (ec <= GenErrc::kRuntimeEndpointOpenError);
}

/*!
 * \brief Checks if error code is a RuntimeRrror.
 * \param ec error code enum value.
 * \return true if ec is a RuntimeError, false otherwise.
 * \vpublic
 */
constexpr bool IsRuntimeError(GenErrc const ec) noexcept {
  return (ec == GenErrc::kRuntime) || IsRuntimeResourceError(ec) || IsRuntimeThreadError(ec) ||
         IsRuntimeBufferError(ec) || IsRuntimeConnectionError(ec) || IsRuntimeEndpointError(ec);
}
}  // namespace generic
}  // namespace amsr
#endif  // LIB_VAC_INCLUDE_AMSR_GENERIC_GENERIC_ERROR_DOMAIN_H_
