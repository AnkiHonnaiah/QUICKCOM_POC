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
/*!        \file  initialization_error_domain.h
 *        \brief  Error domain for errors originating from initialization and deinitialization of components.
 *         \unit  ApplicationBase::InitializationDeinitializationErrorHandling
 *
 *********************************************************************************************************************/

#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INITIALIZATION_ERROR_DOMAIN_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INITIALIZATION_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <utility>
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace application_base {

/*!
 * \brief Specifies the internal errors that can occur upon calling ara::core::Initialize() and
 *        ara::core::Deinitialize().
 * \vpublic
 */
enum class InitializationErrc : ara::core::ErrorDomain::CodeType {
  /*!
   *  \brief Initialization or deinitialization was not executed in the correct order.
   */
  kWrongSequence = 101
};

/*!
 * \brief Exception type thrown by ara::core::Initialize() and ara::core::Deinitialize().
 * \vpublic
 */
class InitializationException : public ara::core::Exception {
 public:
  /*!
   * \brief Inheriting constructor.
   * \vpublic
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for errors originating from ara::core::Initialize() and ara::core::Deinitialize().
 * \vpublic
 *
 * \trace DSGN-ApplicationBase-InitializationDeinitializationErrorHandling
 */
class InitializationErrorDomain final : public ara::core::ErrorDomain {
  /*!
   * \brief The Initialization error domain id
   */
  constexpr static ErrorDomain::IdType kId{0x46273fd8e12edfea};

 public:
  /*!
   * \brief Alias for the exception base class.
   */
  using Exception = InitializationException;

  /*!
   * \brief Default constructor.
   * \vpublic
   */
  constexpr InitializationErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief  Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
   * \return "InitializationError".
   * \vpublic
   */
  char const* Name() const noexcept final { return "InitializationError"; }

  /*!
   * \brief  Translates an error code value into a text message.
   * \param  error_code The error code value to translate.
   * \return The text message, never nullptr.
   * \vpublic
   */
  char const* Message(ErrorDomain::CodeType error_code) const noexcept final {
    StringType return_msg{"Unknown Error Code"};
    if (error_code == static_cast<std::int32_t>(InitializationErrc::kWrongSequence)) {
      return_msg = "Initialization or deinitialization of components could not be made in the correct order";
    }
    return return_msg;
  }

  /*!
   * \brief Throw the exception type corresponding to the given ErrorCode.
   * \param error_code The ErrorCode instance.
   * \vpublic
   */
  void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace internal {
/*!
 * \brief The single global InitializationErrorDomain instance.
 * \vprivate
 */
constexpr InitializationErrorDomain g_InitializationErrorDomain;
}  // namespace internal

/*!
 * \brief  Obtain the reference to the single global InitializationErrorDomain instance.
 * \return Reference to the InitializationErrorDomain instance.
 * \vpublic
 */
inline constexpr ara::core::ErrorDomain const& GetInitializationErrorDomain() noexcept {
  return internal::g_InitializationErrorDomain;
}

/*!
 * \brief  Create a new ErrorCode for InitializationErrorDomain with the given support data type and message.
 * \param  code Enumeration value from InitializationErrc.
 * \param  data Vendor-defined supplementary value.
 * \param  message User-defined context message (can be nullptr).
 * \return The new ErrorCode instance.
 * \vpublic
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(InitializationErrc code,
                                                    ara::core::ErrorDomain::SupportDataType data,
                                                    char const* message) noexcept {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetInitializationErrorDomain(), data,
                              message);
}

}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INITIALIZATION_ERROR_DOMAIN_H_
