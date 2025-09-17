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
/**        \file  amsr/someip_binding_core/internal/communication_error_domain.h
 *        \brief  Definition for SOME/IP Binding communication specific error domain.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientManager
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_COMMUNICATION_ERROR_DOMAIN_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_COMMUNICATION_ERROR_DOMAIN_H_

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
 * \brief SOME/IP binding internal error codes.
 */
enum class CommunicationErrc : amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief default error code
   */
  error_not_ok = 0U,

  /*!
   * \brief Wrong SOME/IP message type.
   */
  wrong_message_type = 1U,

  /*!
   * \brief Wrong SOME/IP protocol version.
   */
  wrong_protocol_version = 2U,

  /*!
   * \brief Value in the length field is smaller than the minimum value (< length of the header).
   */
  payload_length_too_short = 3U,

  /*!
   * \brief Wrong SOME/IP return code.
   */
  wrong_return_code = 4U,

  /*!
   * \brief Wrong SOME/IP client ID.
   */
  wrong_client_id = 5U,

  /*!
   * \brief Wrong SOME/IP method ID.
   */
  wrong_method_id = 6U,

  /*!
   * \brief Service is not available.
   */
  not_ready = 7U,

  /*!
   * \brief Service is not known.
   */
  unknown_service = 8U,

  /*!
   * \brief Malformed SOME/IP message.
   */
  malformed_message = 9U,

  /*!
   * \brief IAM access denied.
   */
  access_denied = 10U,
};

/*!
 * \brief ara::com specific exception.
 */
class CommunicationException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class CommunicationErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = CommunicationErrc;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = CommunicationException;

  /*!
   * \brief Constructor. Initializes the domain id with CommunicationErrorDomain::kId.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr CommunicationErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Name() const noexcept final { return "Communication"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Message(CodeType error_code) const noexcept final {
    static std::array<char const*, 11U> constexpr kMessages{{
        "Unknown Error Code",                                                           // error_not_ok
        "Wrong message type",                                                           // wrong_message_type
        "Wrong protocol version",                                                       // wrong_protocol_version
        "Length field is smaller than the minimum value (< length of SOME/IP header)",  // payload_length_too_short
        "Wrong SOME/IP return code",                                                    // wrong_return_code
        "Wrong SOME/IP client ID",                                                      // wrong_client_id
        "Wrong SOME/IP method ID",                                                      // wrong_method_id
        "Service is not available",                                                     // not_ready
        "Service is not known",                                                         // unknown_service
        "Malformed SOME/IP message",                                                    // malformed_message
        "IAM access denied"                                                             // access_denied
    }};

    static_assert(std::is_trivially_destructible<decltype(kMessages)>::value,
                  "Destructor of object with static storage duration must be trivial!");
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {
        index = code;
      }
    }
    return kMessages[index];
  }

  /*!
   * \brief Throws the given errorCode as Exception.
   * \param[in] error_code error code to be thrown.
   * \throws CommunicationException for all error codes.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
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
  static constexpr ErrorDomain::IdType kId{0x22efef5f6ce1d4b2};
};

/*!
 * \brief Global SomeBindingErrorDomain instance.
 */
constexpr CommunicationErrorDomain kCommunicationErrorDomain;

/*!
 * \brief Get reference to the global SomeBindingErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorDomain const& GetCommunicationErrorDomain() noexcept {
  return kCommunicationErrorDomain;
}

/*!
 * \brief Creates an error code from CommunicationErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre          -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(CommunicationErrorDomain::Errc const code,
                                                     CommunicationErrorDomain::SupportDataType const data,
                                                     char const* message) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetCommunicationErrorDomain(),
                               data, message);
}

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_COMMUNICATION_ERROR_DOMAIN_H_
