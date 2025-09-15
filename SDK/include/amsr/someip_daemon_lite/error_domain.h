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
/*!        \file   someip_daemon_lite/error_domain.h
 *         \brief  Definition for SOME/IP daemon lite specific error domain.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpDaemonLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_ERROR_DOMAIN_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_ERROR_DOMAIN_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_lite {

/*!
 * \brief SOME/IP daemon lite internal error codes.
 * \vpublic
 */
enum class SomeIpDaemonLiteErrc : ara::core::ErrorDomain::CodeType {
  /*!
   * \brief Unknown error used as default.
   */
  kUnknown = 0,

  /*!
   * \brief System configuration not available.
   */
  kSystemConfigurationNotAvailable = 1,

  /*!
   * \brief System configuration invalid.
   */
  kSystemConfigurationInvalid = 2,

  /*!
   * \brief System access failure due to limited privileges.
   */
  kSystemPrivilegesNoAccess = 3,

  /*!
   * \brief Runtime connection setup error.
   */
  kRuntimeConnectionSetupError = 4,

  /*!
   * \brief Runtime connection transmission failed.
   */
  kRuntimeConnectionTransmissionFailed = 5,

  /*!
   * \brief Runtime connection lost.
   */
  kRuntimeConnectionLost = 6,

  /*!
   * \brief Runtime thread creation failed.
   */
  kRuntimeThreadCreationFailed = 7,

  /*!
   * \brief Runtime thread joining failed.
   */
  kRuntimeThreadJoiningFailed = 8,

  /*!
   * \brief Runtime out of resources.
   */
  kRuntimeResourceOutOf = 9,

  /*!
   * \brief Runtime buffer deserialization error.
   */
  kRuntimeBufferDeserializationError = 10,

  /*!
   * \brief Runtime buffer invalid.
   */
  kRuntimeBufferInvalid = 11
};

/*!
 * \brief "ara::com" specific exception.
 * \vpublic
 */
class SomeIpDaemonLiteException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for all SOME/IP daemon lite related errors.
 * \vpublic
 */
class SomeIpDaemonLiteErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enumerations of the domain.
   */
  using Errc = SomeIpDaemonLiteErrc;

  /*!
   * \brief Exception type of the domain.
   */
  using Exception = SomeIpDaemonLiteException;

  /*!
   * \brief Constructor.
   * \vpublic
   * \details Initialize the domain's ID.
   * \steady  FALSE
   */
  constexpr SomeIpDaemonLiteErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \vpublic
   * \return The name as a null-terminated string; never a "nullptr".
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Return the name.
   * \endinternal
   */
  StringType Name() const noexcept final { return "SomeIpDaemonLite"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \vpublic
   * \details The returned value represents an unknown error if \p error_code
   *          is out of range of this error domain.
   * \param[in] error_code The domain-specific error code.
   * \return A null-terminated string; never a "nullptr".
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Return the message mapped to the error code or a message indicating an
   *   unknown error if the error code is out of the bounds of this error domain.
   * \endinternal
   */
  StringType Message(CodeType error_code) const noexcept final {
    static std::array<char const*, 12u> constexpr kMessages{{
        "Unknown error",                                    // kUnknown
        "System configuration not available",               // kSystemConfigurationNotAvailable
        "System configuration invalid",                     // kSystemConfigurationInvalid
        "System access failure due to limited privileges",  // kSystemPrivilegesNoAccess
        "Runtime connection setup error",                   // kRuntimeConnectionSetupError
        "Runtime connection transmission failed",           // kRuntimeConnectionTransmissionFailed
        "Runtime connection lost",                          // kRuntimeConnectionLost
        "Runtime thread creation failed",                   // kRuntimeThreadCreationFailed
        "Runtime thread joining failed",                    // kRuntimeThreadJoiningFailed
        "Runtime out of resources",                         // kRuntimeResourceOutOf
        "Runtime buffer deserialization error",             // kRuntimeBufferDeserializationError
        "Runtime buffer invalid"                            // kRuntimeBufferInvalid
    }};

    static_assert(std::is_trivially_destructible<decltype(kMessages)>::value,
                  "Destructor of an object with static storage duration shall be trivial.");

    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType shall not exceed the range of \"std::size_t\".");

    std::size_t index{0u};

    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};

      if (code < kMessages.size()) {
        index = code;
      }
    }

    return kMessages[index];
  }

  /*!
   * \brief Throws the given error code as exception.
   * \vpublic
   * \param[in] error_code error code to be thrown.
   * \throws "SomeIpDaemonLiteException" for all error codes.
   * \remark If the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Throw the error as an exception if exceptions are enabled or abort.
   * \endinternal
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0xc0000000001e411e};
};

/*!
 * \brief Global error domain instance.
 */
constexpr SomeIpDaemonLiteErrorDomain kSomeIpDaemonLiteErrorDomain;

/*!
 * \brief Get reference to the global error domain instance.
 * \vpublic
 * \return Error domain instance.
 * \pre -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Return reference to the error domain's instance.
 * \endinternal
 */
inline constexpr ara::core::ErrorDomain const& GetSomeIpDaemonLiteErrorDomain() noexcept {
  return kSomeIpDaemonLiteErrorDomain;
}

/*!
 * \brief Create an error code from the error domain.
 * \vpublic
 * \param[in] code The specific error code.
 * \param[in] data Vendor defined support data.
 * \param[in] message An optional message for this error.
 * \return The constructed error code.
 * \pre -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Create an error code based on the provided information and return that.
 * \endinternal
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(SomeIpDaemonLiteErrorDomain::Errc const code,
                                                    SomeIpDaemonLiteErrorDomain::SupportDataType const data,
                                                    char const* message) noexcept {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetSomeIpDaemonLiteErrorDomain(),
                              data, message);
}

/*!
 * \brief Translate an error code to the domain's error enumeration value.
 * \vprivate Vector product internal API.
 * \param[in] error_code The error code to translate.
 * \return The domain's error enumeration value.
 * \details
 *    The error codes are translated as follow (SomeIpDaemonErrc:: on the left, SomeIpDaemonLiteErrc:: on the right):
 *    - error_not_ok                        -> kUnknown
 *    - json_parsing_failed                 -> kSystemConfigurationNotAvailable
 *    - access_denied                       -> kSystemPrivilegesNoAccess
 *    - client_ids_overflow                 -> kRuntimeResourceOutOf
 *    - remote_server_not_found             -> kSystemConfigurationNotAvailable
 *    - required_service_instance_not_found -> kSystemConfigurationNotAvailable
 *    - event_id_not_found                  -> kSystemConfigurationNotAvailable
 *    - eventgroup_id_not_found             -> kSystemConfigurationNotAvailable
 *    - event_already_subscribed            -> kSystemConfigurationNotAvailable
 * \pre -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Translate the passed-in error code into an enumeration value specific to
 *   the error domain and return that.
 * \endinternal
 */
inline constexpr SomeIpDaemonLiteErrorDomain::Errc TranslateErrorCodeSomeIpDaemonCoreErrc(
    ara::core::ErrorCode const& error_code) noexcept {
  using SomeIpDaemonErrc = someip_daemon_core::SomeIpDaemonErrc;

  SomeIpDaemonLiteErrorDomain::Errc return_code{SomeIpDaemonLiteErrc::kUnknown};

  switch (static_cast<SomeIpDaemonErrc>(error_code.Value())) {
    case SomeIpDaemonErrc::error_not_ok:
      // Intentionally left empty.
      break;
    case SomeIpDaemonErrc::json_parsing_failed:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
    case SomeIpDaemonErrc::access_denied:
      return_code = SomeIpDaemonLiteErrc::kSystemPrivilegesNoAccess;
      break;
    case SomeIpDaemonErrc::client_ids_overflow:
      return_code = SomeIpDaemonLiteErrc::kRuntimeResourceOutOf;
      break;
    case SomeIpDaemonErrc::remote_server_not_found:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
    case SomeIpDaemonErrc::required_service_instance_not_found:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
    case SomeIpDaemonErrc::event_id_not_found:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
    case SomeIpDaemonErrc::eventgroup_id_not_found:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
    case SomeIpDaemonErrc::event_already_subscribed:
      return_code = SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable;
      break;
  }

  return return_code;
}

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_ERROR_DOMAIN_H_
