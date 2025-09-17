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
/*!        \file
 *        \brief  ComTrace error domain.
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_COM_TRACE_ERROR_DOMAIN_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_COM_TRACE_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace comtrace {

/*!
 * \brief Enumeration for all Error Code values of ComTrace.
 *
 * \vpublic
 *
 * \unit ComTrace::ErrorDomain
 */
enum class ComTraceErrc : ::ara::core::ErrorDomain::CodeType {  // NOLINT(performance-enum-size)
  /*!
   * \brief Default error code.
   */
  error_not_ok = 0,  // NOLINT(readability-identifier-naming)

  /*!
   * \brief JSON parsing failure.
   */
  json_parsing_failure = 1,  // NOLINT(readability-identifier-naming)
};

/*!
 * \brief ComTrace specific implementation of core Exception class.
 *
 * \vpublic
 *
 * \unit ComTrace::ErrorDomain
 */
class ComTraceException : public ::ara::core::Exception {
 public:
  /*!
   * \brief Construct a new Exception object with a specific ErrorCode.
   *
   * \param[in] error_code  The ErrorCode.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   */
  using ::ara::core::Exception::Exception;

  /*!
   * \brief Destroy the exception.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  ~ComTraceException() noexcept override = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \brief Copy-Construct the exception.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  ComTraceException(ComTraceException const&) noexcept = default;

  /*!
   * \brief Copy-Assign the exception.
   *
   * \return Reference to the copy-assigned exception.
   *
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  auto operator=(ComTraceException const&) & noexcept -> ComTraceException& = default;

  /*!
   * \brief Move-Construct the exception.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  ComTraceException(ComTraceException&&) noexcept = default;

  /*!
   * \brief Move-Assign the exception.
   *
   * \return Reference to the move-assigned exception.
   *
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  auto operator=(ComTraceException&&) & noexcept -> ComTraceException& = default;
};

/*!
 * \brief   Thread Error Domain class.
 * \details This class represents an error domain responsible for all errors occurring in ComTrace.
 *
 * \vpublic
 *
 * \unit ComTrace::ErrorDomain
 */
class ComTraceErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code type definition.
   *
   * \vprivate Vector product internal API.
   */
  using Errc = ComTraceErrc;

  /*!
   * \brief Constructor for ComTraceErrorDomain.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  constexpr ComTraceErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Destroy the error domain.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  ~ComTraceErrorDomain() noexcept = default;

  ComTraceErrorDomain(ComTraceErrorDomain const&) noexcept = delete;
  auto operator=(ComTraceErrorDomain const&) & noexcept -> ComTraceErrorDomain& = delete;
  ComTraceErrorDomain(ComTraceErrorDomain&&) noexcept = delete;
  auto operator=(ComTraceErrorDomain&&) & noexcept -> ComTraceErrorDomain& = delete;

  /*!
   * \brief Return the name for this error domain.
   *
   * \return The name as a null-terminated string, never nullptr.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  auto Name() const noexcept -> StringType final { return "ComTrace"; };

  /*!
   * \brief Return the textual description for the given error code.
   *
   * \param[in] error_code  The domain specific error code.
   *
   * \return The text as a null-terminated string, never nullptr.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  auto Message(::ara::core::ErrorDomain::CodeType error_code) const noexcept -> StringType final;

  /*!
   * \brief   Throws the given ErrorCode as Exception.
   * \details If the code is compiled w/o exceptions, this function will call terminate instead.
   *
   * \param[in] error_code  The ErrorCode to be thrown.
   *
   * \throws ThreadException  For all error codes.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  [[noreturn]] auto ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) -> void final {
    ::vac::language::ThrowOrTerminate<ComTraceException>(error_code);
  }

 private:
  /*!
   * \brief ComTrace error domain identifier (unique domain ID).
   */
  constexpr static ::ara::core::ErrorDomain::IdType kId{0xC0000000001E6468};
};

/*!
 * \brief Internal namespace.
 */
namespace internal {

/*!
 * \brief Global ComTraceErrorDomain instance.
 */
static constexpr ComTraceErrorDomain kComTraceErrorDomain;

/*!
 * \brief Factory function of the ComTraceErrorDomain.
 *
 * \return Constant reference to the single instance of the ComTraceErrorDomain.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \steady      FALSE
 */
constexpr auto GetComTraceErrorDomain() -> ::ara::core::ErrorDomain const& { return kComTraceErrorDomain; }

}  // namespace internal

/*!
 * \brief Make ErrorCode instances from the ComTraceErrorDomain.
 *
 * \param[in] code     An error code identifier from the ComTraceErrc enumeration.
 * \param[in] data     Supplementary data for the error description.
 * \param[in] message  Additional error message supplied by user code.
 *
 * \return ErrorCode instance always references to ComTraceErrorDomain.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
constexpr auto MakeErrorCode(ComTraceErrorDomain::Errc code, ::ara::core::ErrorDomain::SupportDataType data,
                             char const* message = nullptr) -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), internal::GetComTraceErrorDomain(), data, message};
}

/*!
 * \brief Make ErrorCode instances from the ComTraceErrorDomain.
 *
 * \param[in] code     An error code identifier from the ComTraceErrc enumeration.
 * \param[in] message  Additional error message supplied by user code.
 *
 * \return ErrorCode instance always references to ComTraceErrorDomain.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
constexpr auto MakeErrorCode(ComTraceErrorDomain::Errc code, char const* message = nullptr) -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), internal::GetComTraceErrorDomain(), {}, message};
}

}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_COM_TRACE_ERROR_DOMAIN_H_
