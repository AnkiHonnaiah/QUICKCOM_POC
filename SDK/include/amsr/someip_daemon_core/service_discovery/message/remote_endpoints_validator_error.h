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
/**        \file  remote_endpoints_validator_error.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_REMOTE_ENDPOINTS_VALIDATOR_ERROR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_REMOTE_ENDPOINTS_VALIDATOR_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * \brief SD message interpretation error messages
 */
enum class RemoteEndpointsValidatorError : ara::core::ErrorDomain::CodeType {
  kEndpointRequiredButNotReferenced = 0U, /*! An endpoint is required but not referenced in the entry */
  kUdpRequiredButNotReferenced = 1U,      /*! A UDP endpoint is required but not referenced in the entry */
  kTcpRequiredButNotReferenced = 2U,      /*! A TCP endpoint is required but not referenced in the entry */
  kAddressNotInSubnet = 3U                /*! The address is not topologically correct */
};

/*!
 * \brief Error messages
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
static std::array<char const *, 4U> constexpr kRemoteEndpointsValidatorErrorMessages{
    {"Endpoint required but not referenced", "UDP endpoint required but not reference in the SD entry",
     "TCP endpoint required but not reference in the SD entry", "The reference address does not belong to our subnet"}};

/*!
 * \brief ara::com specific exception.
 */
class RemoteEndpointsValidatorException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class RemoteEndpointsValidatorErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = RemoteEndpointsValidatorError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = RemoteEndpointsValidatorException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   * \steady FALSE
   */
  constexpr RemoteEndpointsValidatorErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RemoteEndpointsValidatorErrorDomain() noexcept = default;

  RemoteEndpointsValidatorErrorDomain(RemoteEndpointsValidatorErrorDomain const &) = delete;
  RemoteEndpointsValidatorErrorDomain(RemoteEndpointsValidatorErrorDomain &&) = delete;
  RemoteEndpointsValidatorErrorDomain &operator=(RemoteEndpointsValidatorErrorDomain const &) & = delete;
  RemoteEndpointsValidatorErrorDomain &operator=(RemoteEndpointsValidatorErrorDomain &&) & = delete;

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
  StringType Name() const noexcept override { return "RemoteEndpointValidation"; }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \synchronous TRUE
   * \steady      FALSE
   */
  StringType Message(CodeType error_code) const noexcept override {
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kRemoteEndpointsValidatorErrorMessages.size()) {
        index = code;
      }
    }
    return kRemoteEndpointsValidatorErrorMessages[index];
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
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const &error_code) const noexcept(false) override {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x5D2205FC4794C7B0};
};

/*!
 * \brief Global SdEntryInterpretationErrorDomain instance.
 */
constexpr RemoteEndpointsValidatorErrorDomain kRemoteEndpointsValidatorErrorDomain;

/*!
 * \brief Get reference to the global SdEntryInterpretationErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     Network
 * \synchronous TRUE
 * \steady      FALSE
 */
inline constexpr ara::core::ErrorDomain const &GetRemoteEndpointsValidatorErrorDomain() noexcept {
  return kRemoteEndpointsValidatorErrorDomain;
}

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
inline constexpr ara::core::ErrorCode MakeErrorCode(RemoteEndpointsValidatorErrorDomain::Errc const code,
                                                    RemoteEndpointsValidatorErrorDomain::SupportDataType const data,
                                                    char const *message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code),
                              GetRemoteEndpointsValidatorErrorDomain(), data, message);
}

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_REMOTE_ENDPOINTS_VALIDATOR_ERROR_H_
