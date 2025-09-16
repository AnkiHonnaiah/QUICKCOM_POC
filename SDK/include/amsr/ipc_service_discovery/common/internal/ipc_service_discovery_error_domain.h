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
/*! \file
 *  \brief Error domain for ipc service discovery.
 *  \unit IpcServiceDiscovery::Common::IpcServiceDiscoveryErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_IPC_SERVICE_DISCOVERY_ERROR_DOMAIN_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_IPC_SERVICE_DISCOVERY_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Enumeration for all Error Code values of the IpcServiceDiscovery.
 * \vprivate Vector product internal API
 */
enum class IpcServiceDiscoveryErrc : ::ara::core::ErrorDomain::CodeType {
  kDefaultError = 0,              /*!< Default error. */
  kAlreadyProvided = 1,           /*!< Service instance is already provided. */
  kProvidedDifferentEndpoint = 2, /*!< Service instance is provided but with a different endpoint. */
  kProvidedDifferentClient = 3,   /*!< Service instance is or was provided by a different client. */
  kNotProvided = 4,               /*!< Service instance is currently not provided. */
  kNeverProvided = 5,             /*!< Service instance was never provided. */
  kAlreadyRequired = 6,           /*!< Service instance is already required. */
  kNotRequired = 7,               /*!< Service instance is currently not required. */
  kNeverRequired = 8              /*!< Service instance was never required. */
};

/*!
 * \brief An interface of the IpcServiceDiscovery.
 * \vprivate Vector product internal API
 */
class IpcServiceDiscoveryException : public ::ara::core::Exception {
 public:
  using Exception::Exception;
};

/*!
 * \brief IpcServiceDiscovery Error Domain class.
 * \details This class represents an error domain responsible for all errors occurring in IpcServiceDiscovery.
 * \vprivate Vector product internal API
 */
class IpcServiceDiscoveryErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief IpcServiceDiscovery error domain identifier (unique domain ID).
   */
  constexpr static ::ara::core::ErrorDomain::IdType kId{0xc0000000001e4c22};

  /*!
   * \brief Error code type definition.
   * \vprivate Vector product internal API
   */
  using Errc = IpcServiceDiscoveryErrc;

  /*!
   * \brief Constructor for IpcServiceDiscoveryErrorDomain.
   * \vprivate Vector product internal API
   */
  constexpr IpcServiceDiscoveryErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name for this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vprivate Vector product internal API
   */
  auto Name() const noexcept -> StringType final { return "IpcServiceDiscovery"; }

  /*!
   * \brief Return the textual description for the given error code.
   * \param[in] error_code The domain specific error code.
   * \return The text as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vprivate Vector product internal API
   */
  auto Message(::ara::core::ErrorDomain::CodeType error_code) const noexcept -> StringType final;

  /*!
   * \brief Throws the given ErrorCode as Exception.
   * \param[in] error_code The ErrorCode to be thrown.
   * \details If the code is compiled w/o exceptions, this function will call terminate instead.
   * \context           ANY
   * \pre               -
   * \throws            ThreadException for all error codes.
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector product internal API
   */
  [[noreturn]] void ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<IpcServiceDiscoveryException>(error_code);
  }
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
/*!
 * \brief Global IpcServiceDiscoveryErrorDomain instance.
 */
constexpr IpcServiceDiscoveryErrorDomain kIpcServiceDiscoveryErrorDomain;
}  // namespace internal

/*!
 * \brief Factory function of the IpcServiceDiscoveryErrorDomain.
 * \return Constant reference to the single instance of the IpcServiceDiscoveryErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           TRUE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto GetIpcServiceDiscoveryErrorDomain() -> ::ara::core::ErrorDomain const& {
  return internal::kIpcServiceDiscoveryErrorDomain;
}

/*!
 * \brief Make ErrorCode instances from the IpcServiceDiscoveryErrorDomain.
 * \param[in] code An error code identifier from the IpcServiceDiscoveryErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to IpcServiceDiscoveryErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto MakeErrorCode(IpcServiceDiscoveryErrorDomain::Errc code,
                                    ::ara::core::ErrorDomain::SupportDataType data, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), GetIpcServiceDiscoveryErrorDomain(), data, message};
}

/*!
 * \brief Make ErrorCode instances from the IpcServiceDiscoveryErrorDomain.
 * \param[in] code An error code identifier from the IpcServiceDiscoveryErrc enumeration.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to IpcServiceDiscoveryErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto MakeErrorCode(IpcServiceDiscoveryErrorDomain::Errc code, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), GetIpcServiceDiscoveryErrorDomain(), {}, message};
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_IPC_SERVICE_DISCOVERY_ERROR_DOMAIN_H_
