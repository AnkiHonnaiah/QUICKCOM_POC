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
/**        \file     someip_daemon_error_code.h
 *         \brief    Contains error handling related types.
 *
 *         \details  Contains Result alias, error domain & error codes.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIP_DAEMON_ERROR_CODE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIP_DAEMON_ERROR_CODE_H_

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

/*!
 * \brief SOME/IP daemon specific error codes.
 * \trace SPEC-8053547
 * \vpublic
 */
enum class SomeIpDaemonErrc : ara::core::ErrorDomain::CodeType {
  /*!
   * \brief Default error code
   */
  error_not_ok = 0U,

  /*!
   * \brief The user provided Json parser validation failure
   */
  json_parsing_failed = 1U,

  /*!
   * \brief The application does not have the right to execute the operation.
   */
  access_denied = 2U,

  /*!
   * \brief The client Id generator reaches the maximum number of simultaneous clients.
   */
  client_ids_overflow = 3U,

  /*!
   * \brief The remote server not found.
   */
  remote_server_not_found = 4U,

  /*!
   * \brief The required service instance not found.
   */
  required_service_instance_not_found = 5U,

  /*!
   * \brief Event id not found.
   */
  event_id_not_found = 6U,

  /*!
   * \brief Eventgroup id not found for the given event id.
   */
  eventgroup_id_not_found = 7U,

  /*!
   * \brief Event is already subscribed.
   */
  event_already_subscribed = 8U
};

/*!
 * \brief SOME/IP daemon specific exception
 * \vpublic
 */
class SomeIpDaemonException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherited constructor
   */
  using Exception::Exception;
};

/*!
 * \brief domain for all SOME/IP daemon related errors.
 * \vpublic
 */
class SomeIpDaemonErrorDomain final : public ara::core::ErrorDomain {
  /*!
   * \brief Unique domain ID.
   */
  constexpr static IdType kId{0x539083f874d86a36};

 public:
  /*!
   * \brief Implements the Errc interface-type.
   */
  using Errc = SomeIpDaemonErrc;

  /*!
   * \brief Implements the Exception interface-type.
   */
  using Exception = SomeIpDaemonException;

  /*!
   * \brief Constructs a SomeIpDaemonErrorDomain type.
   * \vpublic
   * \steady FALSE
   */
  constexpr SomeIpDaemonErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Gets the name of the ErrorDomain.
   * \steady FALSE
   */
  StringType Name() const noexcept final { return "SomeIpDaemon"; }

  /*!
   * \brief Converts an error code into a message.
   * \vpublic
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \steady FALSE
   */
  StringType Message(CodeType error_code) const noexcept final {
    constexpr static std::array<char const*, 9> kMessages{
        /* error_not_ok */ "Unknown Error Code",
        /*json_parsing_failed */ "Validation of JSON parser failed",
        /* access_denied */ "IAM access denied",
        /* client_ids_overflow */ "Client Id generator reaches the maximum number of simultaneous client Ids",
        /* remote_server_not_found */ "Remote server not found",
        /* required_service_instance_not_found */ "Required service instance not found",
        /* event_id_not_found */ "Event ID not found in the configuration",
        /* eventgroup_id_not_found */ "No required eventgroup ID found in the configuration for the requested event",
        /* event_already_subscribed */ "The requested event is already subscribed"};

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
   * \brief Throw an exception based on an error code or abort.
   * \vpublic
   * \param[in] error_code Error code to cause a throw or an abort.
   * \throws SomeIpDaemonException with the passed error code.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate
   * \steady FALSE
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }
};

namespace internal {
/*!
 * \brief Global SomeIpDaemonErrorDomain instance.
 */
constexpr SomeIpDaemonErrorDomain kSomeIpDaemonErrorDomain;

/*!
 * \brief SomeIpDaemonErrorDomain a reference to the global SomeIpDaemonErrorDomain instance.
 * \return SomeIpDaemonErrorDomain instance.
 * \steady FALSE
 */
inline constexpr ara::core::ErrorDomain const& GetSomeIpDaemonDomain() noexcept {
  return internal::kSomeIpDaemonErrorDomain;
}

}  // namespace internal

/*!
 * \brief Creates an error code from SomeIpDaemonErrorDomain.
 * \vprivate
 * \param code The specific error code.
 * \param data Vendor defined support data.
 * \param message An optional message for this error.
 * \returns The constructed error code.
 * \steady FALSE
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(SomeIpDaemonErrorDomain::Errc code,
                                                    SomeIpDaemonErrorDomain::SupportDataType data,
                                                    char const* message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), internal::GetSomeIpDaemonDomain(),
                              data, message);
}

}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SOMEIP_DAEMON_ERROR_CODE_H_
