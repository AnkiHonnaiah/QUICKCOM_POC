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
/**        \file
 *        \brief  Definition for IPC Binding specific error domain.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief IPC binding internal error codes.
 */
enum class IpcBindingErrc : ::amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief Default error code.
   */
  error_not_ok = 0,

  /*!
   * \brief Malformed instance identifier string.
   */
  malformed_instance_identifier_string = 1,

  /*!
   * \brief JSON configuration file not found or loadable.
   */
  json_loading_failure = 2,

  /*!
   * \brief JSON configuration parsing failure.
   */
  json_parsing_failure = 3,

  /*!
   * \brief Deserialization error.
   */
  deserialization_error = 4,

  /*!
   * \brief Initialization/Deinitialization is performed in wrong order.
   */
  wrong_init_sequence = 5,

  /*!
   * \brief Memory allocation failed.
   */
  memory_allocation_failure = 6,

  /*!
   * \brief Unicast communication failed.
   */
  communication_failure = 7
};

/*!
 * \brief IpcBinding specific implementation of core Exception class.
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
class IpcBindingException : public ::amsr::core::Exception {
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
  using ::amsr::core::Exception::Exception;

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
  ~IpcBindingException() noexcept override = default;  // VCA_IPCB_MOLE_1298

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
  IpcBindingException(IpcBindingException const&) noexcept = default;

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
  auto operator=(IpcBindingException const&) & noexcept -> IpcBindingException& = default;

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
  IpcBindingException(IpcBindingException&&) noexcept = default;

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
  auto operator=(IpcBindingException&&) & noexcept -> IpcBindingException& = default;
};

/*!
 * \brief Error Domain for all IPC Binding internal errors.
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
class IpcBindingErrorDomain final : public ::amsr::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = IpcBindingErrc;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = IpcBindingException;

  /*!
   * \brief Construct the IpcBinding error domain.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  constexpr IpcBindingErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Destroy the error domain.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcBindingErrorDomain() noexcept = default;

  IpcBindingErrorDomain(IpcBindingErrorDomain const&) noexcept = delete;
  auto operator=(IpcBindingErrorDomain const&) noexcept -> IpcBindingErrorDomain& = delete;
  IpcBindingErrorDomain(IpcBindingErrorDomain&&) noexcept = delete;
  auto operator=(IpcBindingErrorDomain&&) noexcept -> IpcBindingErrorDomain& = delete;

  /*!
   * \brief Returns the name of this error domain.
   *
   * \return Error domain name as a null-terminated string, never nullptr.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto Name() const noexcept -> StringType final {
    return "IpcBinding";
  }  // COV_IpcBinding_ComponentOverall_tooling_issue

  /*!
   * \brief   Returns the textual representation of the given error code.
   * \details The return value is undefined if the given error code did not originate from this error domain.
   *
   * \param[in] error_code  The domain-specific error code.
   *
   * \return The error message text as a null-terminated string, never nullptr.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - If the given error code is valid for this error domain:
   *   - Return the corresponding error message.
   * - Otherwise return the default error message.
   * \endinternal
   */
  auto Message(CodeType error_code) const noexcept -> StringType final {
    constexpr static std::array<char const*, 8> kMessages{{
        "Unknown Error Code",                                           // error_not_ok
        "Malformed InstanceIdentifierString",                           // malformed_instance_identifier_string
        "JSON file load failure",                                       // json_loading_failure
        "JSON parsing failure",                                         // json_parsing_failure
        "Deserialization Error",                                        // Deserialization Error
        "Initialization/Deinitialization is performed in wrong order",  // wrong_init_sequence
        "Memory allocation failure",                                    // memory_allocation_failure
        "Unitcast communication failure",                               // communication_failure
    }};

    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {
        index = code;
      }
    }
    return kMessages.at(index);
  }  // COV_IpcBinding_ComponentOverall_tooling_issue

  /*!
   * \brief   Throws the given error code as exception.
   * \details If the adaptive application is compiled without exceptions, this function call will terminate.
   *
   * \param[in] error_code  Error code to be thrown.
   *
   * \throws IpcBindingException  For all error codes.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  [[noreturn]] void ThrowAsException(::amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }  // COV_IpcBinding_ComponentOverall_tooling_issue

 private:
  /*!
   * \brief ID of the error domain.
   */
  static constexpr ErrorDomain::IdType kId{0x269651118FBA832E};
};

/*!
 * \brief Global IpcBindingErrorDomain instance.
 */
constexpr IpcBindingErrorDomain kIpcBindingErrorDomain;

/*!
 * \brief Gets the reference to the global IpcBindingErrorDomain instance.
 *
 * \return Common error domain instance.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
inline constexpr auto GetIpcBindingErrorDomain() noexcept -> ::amsr::core::ErrorDomain const& {
  return kIpcBindingErrorDomain;
}

/*!
 * \brief Creates an error code from IpcBindingErrorDomain.
 *
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 *
 * \return The constructed error code.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
inline constexpr auto MakeErrorCode(IpcBindingErrorDomain::Errc code, IpcBindingErrorDomain::SupportDataType data,
                                    char const* message) -> ::amsr::core::ErrorCode {
  return {static_cast<::amsr::core::ErrorDomain::CodeType>(code), GetIpcBindingErrorDomain(), data, message};
}

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_ComponentOverall_tooling_issue
//   \ACCEPT  XX
//   \REASON  CodeCoverageComponentOverall gaps are reported due to VectorCast tooling issues (see DI-4937).
//            It is manually verified that the APIs tested on component test level.
// COV_JUSTIFICATION_END
