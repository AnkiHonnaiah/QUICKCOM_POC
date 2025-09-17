/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  amsr/container_error_domain.h
 *        \brief  Error domain for errors originating from container classes.
 *         \unit  VaCommonLib::ContainerLibrary
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CONTAINER_ERROR_DOMAIN_H_
#define LIB_VAC_INCLUDE_AMSR_CONTAINER_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <utility>
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {

/*!
 * \brief Specifies the types of internal errors that can occur upon using container data types.
 * \vpublic
 */
enum class ContainerErrorCode : amsr::core::ErrorDomain::CodeType {
  /*! \brief Tried to access an element out of range. */
  kOutOfRangeError = 101,
  /*! \brief Tried to allocate more elements than maximally allowed. */
  kLengthError = 102,
  /*! \brief Could not allocate memory. */
  kAllocationError = 103,
};

/*!
 * \brief Exception type thrown by container classes.
 * \vpublic
 */
class ContainerException : public amsr::core::Exception {
 public:
  /*!
   * \brief Inheriting constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for errors originating from container classes.
 * \vpublic
 */
class ContainerErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief The Container error domain ID.
   */
  constexpr static ErrorDomain::IdType kId{0xc0000000001e319aU};

 public:
  /*!
   * \brief Alias for the exception class.
   */
  using Exception = ContainerException;

  /*!
   * \brief Default constructor.
   * \context ANY
   * \pre -
   */
  constexpr ContainerErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the "shortname" of this error domain.
   * \return The "shortname" of this error domain.
   * \context ANY
   * \pre -
   */
  auto Name() const noexcept -> StringType final { return "Container"; }

  /*!
   * \brief Translates an error code value into a text message.
   * \param[in] error_code The error code value to translate.
   * \return The textual description of the error code if it is one of ContainerErrorCode, a generic message otherwise.
   * \context ANY
   * \pre -
   */
  auto Message(ErrorDomain::CodeType error_code) const noexcept -> StringType final {
    StringType result{"Unknown Error Code"};

    // VECTOR NL AutosarC++17_10-A7.2.1: MD_VAC_A7.2.1_valueIsNotWithinEnumeratorRange
    ContainerErrorCode const error_code_enum{static_cast<ContainerErrorCode>(error_code)};
    switch (error_code_enum) {
      case amsr::ContainerErrorCode::kOutOfRangeError: {
        result = "Data access out of range";
        break;
      }
      case amsr::ContainerErrorCode::kLengthError: {
        result = "Exceeded the maximally available memory";
        break;
      }
      case amsr::ContainerErrorCode::kAllocationError: {
        result = "Memory could not be allocated";
        break;
      }
    }

    return result;
  }

  /*!
   * \brief Throws the exception type corresponding to the given ErrorCode.
   * \param[in] error_code The ErrorCode instance.
   * \throws ContainerException with the given error code.
   * \context ANY
   * \pre -
   */
  void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace internal {
/*!
 * \brief The single global ContainerErrorDomain instance.
 * \vprivate
 */
constexpr ContainerErrorDomain kContainerErrorDomain;
}  // namespace internal

/*!
 * \brief Creates an ErrorCode instance from the given error.
 * \param[in] code The error code.
 * \param[in] data Context-specific supplementary data.
 * \param[in] message The error message.
 * \return The constructed ErrorCode instance.
 * \context ANY
 * \pre -
 * \vprivate
 */
constexpr auto MakeErrorCode(ContainerErrorCode code, amsr::core::ErrorDomain::SupportDataType data,
                             char const* message) noexcept -> amsr::core::ErrorCode {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), internal::kContainerErrorDomain,
                               data, message);
}

}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CONTAINER_ERROR_DOMAIN_H_
