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
/*!        \file   someip_binding_lite/error_domain.h
 *         \brief  Definition for SOME/IP binding lite specific error domain.
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_ERROR_DOMAIN_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_ERROR_DOMAIN_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/thread/error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "osabstraction/osab_error_domain.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief SOME/IP binding lite internal error codes.
 * \vpublic
 */
enum class SomeIpBindingLiteErrc : ara::core::ErrorDomain::CodeType {
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
class SomeIpBindingLiteException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for all SOME/IP binding lite related errors.
 * \vpublic
 */
class SomeIpBindingLiteErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enumerations of the domain.
   */
  using Errc = SomeIpBindingLiteErrc;

  /*!
   * \brief Exception type of the domain.
   */
  using Exception = SomeIpBindingLiteException;

  /*!
   * \brief Constructor.
   * \vpublic
   * \details Initialize the domain's ID.
   *
   * \steady      FALSE
   */
  constexpr SomeIpBindingLiteErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \vpublic
   * \return The name as a null-terminated string; never a "nullptr".
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Return the name.
   * \endinternal
   */
  StringType Name() const noexcept final { return "SomeIpBindingLite"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \vpublic
   * \details The returned value represents an unknown error if \p error_code
   *          is out of range of this error domain.
   * \param[in] error_code The domain-specific error code.
   * \return A null-terminated string; never a "nullptr".
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
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
   * \throws "SomeIpBindingLiteException" for all error codes.
   * \remark If the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
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
  static constexpr ErrorDomain::IdType kId{0x59b20d2ecd45222b};
};

/*!
 * \brief Global error domain instance.
 */
constexpr SomeIpBindingLiteErrorDomain kSomeIpBindingLiteErrorDomain;

/*!
 * \brief Get reference to the global error domain instance.
 * \vpublic
 * \return Error domain instance.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Return reference to the error domain's instance.
 * \endinternal
 */
inline constexpr ara::core::ErrorDomain const& GetSomeIpBindingLiteErrorDomain() noexcept {
  return kSomeIpBindingLiteErrorDomain;
}

/*!
 * \brief Create an error code from the error domain.
 * \vpublic
 * \param[in] code The specific error code.
 * \param[in] data Vendor defined support data.
 * \param[in] message An optional message for this error.
 * \return The constructed error code.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Create an error code based on the provided information and return that.
 * \endinternal
 */
// VECTOR NC VectorC++-V3.9.2: MD_SOMEIPBINDING_VectorC++-V3.9.2_builtin_types_false_positive
inline constexpr ara::core::ErrorCode MakeErrorCode(SomeIpBindingLiteErrorDomain::Errc const code,
                                                    SomeIpBindingLiteErrorDomain::SupportDataType const data,
                                                    char const* message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetSomeIpBindingLiteErrorDomain(),
                              data, message);
}

/*!
 * \brief Translate an error code to the domain's error enumeration value.
 * \vprivate Vector product internal API.
 * \param[in] error_code The error code to translate.
 * \return The domain's error enumeration value.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Translate the passed-in error code into an enumeration value specific to
 *   the error domain and return that.
 * \endinternal
 */
inline constexpr SomeIpBindingLiteErrorDomain::Errc TranslateErrorCodeThreadErrc(
    amsr::core::ErrorCode const& error_code) {
  using ThreadErrc = thread::ThreadErrc;
  using CodeType = amsr::core::ErrorDomain::CodeType;

  SomeIpBindingLiteErrorDomain::Errc return_code{SomeIpBindingLiteErrc::kUnknown};

  if (error_code.Value() == static_cast<CodeType>(ThreadErrc::kThreadCreationFailed)) {
    return_code = SomeIpBindingLiteErrc::kRuntimeThreadCreationFailed;

  } else if (error_code.Value() == static_cast<CodeType>(ThreadErrc::kInsufficientResources)) {
    return_code = SomeIpBindingLiteErrc::kRuntimeResourceOutOf;

  } else if (error_code.Value() == static_cast<CodeType>(ThreadErrc::kInvalidConfig)) {
    return_code = SomeIpBindingLiteErrc::kSystemConfigurationInvalid;

  } else if (error_code.Value() == static_cast<CodeType>(ThreadErrc::kInsufficientPermissions)) {
    return_code = SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess;

  } else if (error_code.Value() == static_cast<CodeType>(ThreadErrc::kJoinThreadFailed)) {
    return_code = SomeIpBindingLiteErrc::kRuntimeThreadJoiningFailed;

  } else {
    return_code = SomeIpBindingLiteErrc::kUnknown;
  }

  return return_code;
}

/*!
 * \brief Translate an error code to the domain's error enumeration value.
 * \vprivate Vector product internal API.
 * \param[in] error_code The error code to translate.
 * \return The domain's error enumeration value.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Translate the passed-in error code into an enumeration value specific to
 *   the error domain and return that.
 * \endinternal
 */
inline constexpr SomeIpBindingLiteErrorDomain::Errc TranslateErrorCodeClientError(
    amsr::core::ErrorCode const& error_code) {
  using ClientError = someip_binding_core::internal::ClientError;
  using CodeType = amsr::core::ErrorDomain::CodeType;

  SomeIpBindingLiteErrorDomain::Errc return_code{SomeIpBindingLiteErrc::kUnknown};

  if (error_code.Value() == static_cast<CodeType>(ClientError::kSystemConfigurationNotAvailable)) {
    return_code = SomeIpBindingLiteErrc::kSystemConfigurationNotAvailable;

  } else if (error_code.Value() == static_cast<CodeType>(ClientError::kSystemPrivilegesNoAccess)) {
    return_code = SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess;

  } else if (error_code.Value() == static_cast<CodeType>(ClientError::kRuntimeConnectionTransmissionFailed)) {
    return_code = SomeIpBindingLiteErrc::kRuntimeConnectionTransmissionFailed;

  } else {
    return_code = SomeIpBindingLiteErrc::kUnknown;
  }

  return return_code;
}

/*!
 * \brief Translate an error code to the domain's error enumeration value.
 * \vprivate Vector product internal API.
 * \param[in] error_code The error code to translate.
 * \return The domain's error enumeration value.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady      FALSE
 * \internal
 * - Translate the passed-in error code into an enumeration value specific to
 *   the error domain and return that.
 * \endinternal
 */
inline constexpr SomeIpBindingLiteErrorDomain::Errc TranslateErrorCodeOsabErrc(
    amsr::core::ErrorCode const& error_code) {
  using OsabErrc = osabstraction::OsabErrc;
  using CodeType = amsr::core::ErrorDomain::CodeType;

  SomeIpBindingLiteErrorDomain::Errc return_code{SomeIpBindingLiteErrc::kUnknown};

  if (error_code.Value() == static_cast<CodeType>(OsabErrc::kResource)) {
    return_code = SomeIpBindingLiteErrc::kRuntimeResourceOutOf;

  } else {
    return_code = SomeIpBindingLiteErrc::kUnknown;
  }

  return return_code;
}

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_ERROR_DOMAIN_H_
