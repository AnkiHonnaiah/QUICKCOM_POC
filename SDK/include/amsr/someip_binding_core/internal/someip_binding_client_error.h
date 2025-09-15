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
/*!        \file  someip_binding_client_error.h
 *        \brief  Definition of Binding Client Errors
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientManager
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CLIENT_ERROR_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CLIENT_ERROR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief SOMEIP Binding Client error codes
 */
enum class ClientError : std::uint32_t {
  kUnknownError = 0x00000000U,           // Unknown error
  kSystemConfigurationNotAvailable,      // Event not configured
  kSystemPrivilegesNoAccess,             // Access to the service has been denied
  kRuntimeConnectionTimeout,             // Timeout during waiting for response from SOME/IP Daemon
  kRuntimeConnectionTransmissionFailed,  // No connection to the SOME/IP Daemon has been established
  kRuntimeResourceOutOf,                 // IPC Queue from SOME/IP Daemon full, queue limit reached
  kClientIdsOverflow                     // Maximum number of simultaneous client Ids reached
};

/*!
 * \brief ara::com specific exception.
 */
class ClientException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding Client internal errors.
 */
class ClientErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = ClientError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = ClientException;

  /*!
   * \brief Constructor. Initializes the domain id with ClientErrorDomain::kId.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ClientErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Name() const noexcept final { return "SomeIpBindingClient"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Message(CodeType error_code) const noexcept final {
    /*!
     * \brief Error messages
     */
    static std::array<char const*, 6U> constexpr kClientErrorMessages{
        {"Unknown Error", "Configuration mismatch, event or method not configured",
         "Access to the service has been denied", "Timeout during waiting for response from SOME/IP Daemon",
         "No connection to the SOME/IP Daemon has been established",
         "Transmission to SomeIpDaemon failed, IPC queue full"}};

    static_assert(std::is_trivially_destructible<decltype(kClientErrorMessages)>::value,
                  "Destructor of object with static storage duration must be trivial!");
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};

    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};

      if (code < kClientErrorMessages.size()) {
        index = code;
      }
    }

    return kClientErrorMessages[index];
  }

  /*!
   * \brief Throws the given errorCode as Exception.
   * \param[in] error_code error code to be thrown.
   * \throws SomeIpBindingException for all error codes.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre          -
   * \context     ANY
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0xC237FC52CA26C6A3};
};

/*!
 * \brief Global ClientErrorDomain instance.
 */
constexpr ClientErrorDomain kClientErrorDomain;

/*!
 * \brief Get reference to the global ClientErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     ANY
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorDomain const& GetClientErrorDomain() noexcept { return kClientErrorDomain; }

/*!
 * \brief Creates an error code from ClientErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre          -
 * \context     ANY
 * \synchronous TRUE
 * \spec
 *   requires message != nullptr;
 * \endspec
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(ClientErrorDomain::Errc const code,
                                                     ClientErrorDomain::SupportDataType const data,
                                                     char const* message) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetClientErrorDomain(), data,
                               message);
}
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CLIENT_ERROR_H_
