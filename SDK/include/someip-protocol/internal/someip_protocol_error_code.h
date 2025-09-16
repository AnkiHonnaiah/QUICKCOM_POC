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
/*!        \file    someip_protocol_error_code.h
 *         \brief   Error handling related types.
 *         \details Contains Result alias, error domain & error codes.
 *         \unit    SomeIpProtocol::Messages::SomeIpErrorCode
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_PROTOCOL_ERROR_CODE_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_PROTOCOL_ERROR_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "ara/core/array.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_protocol {

/*!
 * \brief SOME/IP protocol specific error codes.
 * \vpublic
 */
enum class SomeIpProtocolErrc : amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief Default error code
   */
  error_not_ok = 0U,
  /*!
   * \brief Error code indicating that the buffer is too small to contain the Some/IP header.
   */
  buffer_too_small = 1U
};

/*!
 * \brief SOME/IP protocol specific exception
 * \vpublic
 */
class SomeIpProtocolException : public amsr::core::Exception {
 public:
  /*!
   * \brief   Inherited constructor
   * \pre     -
   * \context ANY
   */
  using Exception::Exception;
};

/*!
 * \brief domain for all SOME/IP protocol related errors.
 * \vpublic
 */
class SomeIpProtocolErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief Unique domain ID.
   */
  constexpr static IdType kId{0x2fb2886334acff8b};

 public:
  /*!
   * \brief Implements the Errc interface-type.
   */
  using Errc = SomeIpProtocolErrc;

  /*!
   * \brief Implements the Exception interface-type.
   */
  using Exception = SomeIpProtocolException;

  /*!
   * \brief   Constructs a SomeIpProtocolErrorDomain type.
   * \pre     -
   * \context ANY
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr SomeIpProtocolErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief   Gets the name of the ErrorDomain.
   * \return  The name as null terminated string, never nullptr.
   * \pre     -
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Name() const noexcept final { return "SomeIpProtocol"; }

  /*!
   * \brief   Converts an error code into a message.
   * \param   error_code the domain-specific error code.
   * \return  The text as a null-terminated string, never nullptr.
   * \pre     -
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  StringType Message(CodeType error_code) const noexcept final {
    constexpr static ara::core::Array<char const*, 2> kMessages{/* error_not_ok */ "Unknown Error Code",
                                                                /* buffer_too_small */ "Provided buffer is too small"};

    // VECTOR AV NC Architecture-ApiAnalysis Template_Argument: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
      if (code < kMessages.size()) {
        index = code;
      }
    }

    // VECTOR NL AutosarC++17_10-M5.0.16: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.0.16_PointerArithmetic
    return kMessages[index];
  }

  /*!
   * \brief   Converts an error code into an exception.
   * \param   error_code Error code to be thrown.
   * \throws  SomeIpProtocolException with the passed error code.
   * \remark  If the adaptive application is compiled without exceptions, this function call will terminate
   * \pre     -
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace internal {
/*!
 * \brief Global SomeIpProtocolErrorDomain instance.
 */
constexpr SomeIpProtocolErrorDomain kSomeIpProtocolErrorDomain;

/*!
 * \brief   SomeIpProtocolErrorDomain a reference to the global SomeIpProtocolErrorDomain instance.
 * \return  SomeIpProtocolErrorDomain instance.
 * \pre     -
 * \context ANY
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorDomain const& GetSomeIpProtocolDomain() noexcept {
  return internal::kSomeIpProtocolErrorDomain;
}

}  // namespace internal

/*!
 * \brief   Creates an error code from SomeIpProtocolErrorDomain.
 * \param   code The specific error code.
 * \param   data Vendor defined support data.
 * \param   message An optional message for this error.
 * \returns The constructed error code.
 * \pre     -
 * \context ANY
 * \spec
 *   requires true;
 * \endspec
 * \vprivate
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(SomeIpProtocolErrorDomain::Errc code,
                                                     SomeIpProtocolErrorDomain::SupportDataType data = {},
                                                     char const* message = nullptr) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code),
                               internal::GetSomeIpProtocolDomain(), data, message);
}

}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_PROTOCOL_ERROR_CODE_H_
