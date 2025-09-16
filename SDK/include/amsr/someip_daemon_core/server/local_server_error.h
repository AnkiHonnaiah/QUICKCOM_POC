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
/**        \file  local_server_error.h
 *        \brief  Local Server Error.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_ERROR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief The possible errors returned from calling this class' APIs
 */
enum class LocalServerOperationError : ara::core::ErrorDomain::CodeType {
  kServiceNotConfigured = 0U,   /*! The service instance has not been configured. */
  kServiceAlreadyRequested = 1U /*! The service has already been requested. */
};

/*!
 * \brief The exception specific to the errors returned from calling this class' APIs
 */
class LocalServerErrorOperationException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all errors returned from calling this class' APIs
 */
class LocalServerOperationErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = LocalServerOperationError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = LocalServerErrorOperationException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   * \steady FALSE
   */
  constexpr LocalServerOperationErrorDomain() noexcept : ErrorDomain(kId) {}

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
  StringType Name() const noexcept override { return "LocalServerOperationError::Operation"; }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \synchronous TRUE
   * \steady      FALSE
   */
  StringType Message(CodeType error_code) const noexcept override {
    /*!
     * \brief Error messages
     */
    // VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
    std::array<char const*, 2U> constexpr OperationErrorMessages{
        {"The service instance has not been configured.", "The service has already been requested."}};

    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < OperationErrorMessages.size()) {
        index = code;
      }
    }
    return OperationErrorMessages[index];
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) override {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x17B16DDFDE1717A9};
};

/*!
 * \brief Global OperationErrorDomain instance.
 */
constexpr LocalServerOperationErrorDomain kOperationErrorDomain;

/*!
 * \brief Get reference to the global OperationErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     Network
 * \synchronous TRUE
 * \steady      FALSE
 */
inline constexpr ara::core::ErrorDomain const& GetOperationErrorDomain() noexcept { return kOperationErrorDomain; }

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_not_called_function
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
inline constexpr ara::core::ErrorCode MakeErrorCode(LocalServerOperationErrorDomain::Errc const code,
                                                    LocalServerOperationErrorDomain::SupportDataType const data,
                                                    char const* message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetOperationErrorDomain(), data,
                              message);
}

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_ERROR_H_
