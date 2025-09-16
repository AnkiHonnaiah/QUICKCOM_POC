/*!********************************************************************************************************************
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
/*!        \file  e2e_error_domain.h
 *        \brief  Error domain for E2e
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_ERROR_DOMAIN_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_ERROR_DOMAIN_H_

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace e2e {

/*!
 * \brief Enumeration with raw error codes for E2e.
 */
enum class E2eErrc : ara::core::ErrorDomain::CodeType { kNullInput = 0x13u, kWrongInput = 0x17u };

/*!
 * \brief Default Exception for E2e error handling.
 */
class E2eException final : public ara::core::Exception {
 public:
  using Exception::Exception;
};

/*!
 * \brief Error domain for E2e.
 * \unit amsr::e2e::E2eErrorDomain
 */
class E2eErrorDomain final : public ara::core::ErrorDomain {
  /*!
   * \brief Global Id of this error domain.
   */
  constexpr static IdType kId{0x46005f6043340606};

 public:
  /*!
   * \brief Constructor.
   * \steady TRUE
   */
  constexpr E2eErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the name of this error domain.
   * \return name
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  StringType Name() const noexcept final { return "E2eErrorDomain"; }

  /*!
   * \brief Gets the standard message for an error with error_code.
   * \details An error with error_code always contains this message and optionally an user message.
   * \param[in] error_code The raw error code.
   * \return Standard message for errors with error_code.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  StringType Message(CodeType error_code) const noexcept final {
    StringType msg{"No message defined for this error code!"};
    if (error_code == static_cast<CodeType>(E2eErrc::kNullInput)) {
      msg = "Null input.";
    } else if (error_code == static_cast<CodeType>(E2eErrc::kWrongInput)) {
      msg = "Wrong Input.";
    } else {
      // msg is already assigned above
    }
    return msg;
  }

  /*!
   * \brief Throws the default exception for E2e error handling or terminates.
   * \details Do not use it!
   * \param error_code The raw error code.
   * \context ANY
   * \pre -
   * \throws amsr::vector::fs::e2e::E2eException Could be thrown if ThrowOrTerminate shall issue an exception.
   * \threadsafe FALSE
   * \exceptionsafety NO
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<E2eException>(error_code);
  }
};

namespace detail {
/*!
 * \brief E2e error domain.
 */
constexpr E2eErrorDomain e2e_error_domain;
}  // namespace detail

/*!
 * \brief Gets the E2E error domain.
 * \return E2e error domain.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \unit amsr::e2e::E2eErrorDomain
 */
inline constexpr ara::core::ErrorDomain const& GetE2EDomain() noexcept { return detail::e2e_error_domain; }

/*!
 * \brief Create an error code.
 * \param code Raw error code.
 * \param user_message Optional user message for this error.
 * \param support_data Optional support data.
 * \return A error code.
 * \context ANY
 * \pre -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 * \unit amsr::e2e::E2eErrorDomain
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(
    E2eErrorDomain::CodeType code, amsr::core::ErrorDomain::StringType user_message = nullptr,
    amsr::core::ErrorDomain::SupportDataType support_data = {}) noexcept {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetE2EDomain(), support_data, user_message};
}

}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_ERROR_DOMAIN_H_
