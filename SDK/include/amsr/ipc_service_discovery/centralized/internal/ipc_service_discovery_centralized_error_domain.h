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
 *  \brief Error domain for ipc service discovery centralized.
 *  \unit IpcServiceDiscovery::Centralized::IpcServiceDiscoveryCentralizedErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_ERROR_DOMAIN_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Enumeration for all Error Code values of the IpcServiceDiscoveryCentralized.
 * \vprivate Vector product internal API
 */
enum class IpcServiceDiscoveryCentralizedErrc : ::ara::core::ErrorDomain::CodeType {
  kDefaultError = 0,            /*!< Default error. */
  kDisconnected = 1,            /*!< Disconnected from peer. */
  kSendBufferFull = 2,          /*!< Send buffer of Ipc connection is full. */
  kMessageCorrupted = 3,        /*!< Received message is corrupted. */
  kMessageSizeMaximumError = 4, /*!< Violation of the maximum message size. */
  kNoSuchConnection = 5,        /*!< No such connection exists. */
  kNoSuchEntry = 6,             /*!< No such entry in the ServiceRegistry exists. */
  kClientNotRegistered = 7,     /*!< The client is not registered. */
  kClientAlreadyRegistered = 8, /*!< The client is already registered. */
  kProtocolError = 9            /*!< Violation of the IpcSd protocol. */
};

/*!
 * \brief An interface of the IpcServiceDiscoveryCentralized.
 * \vprivate Vector product internal API
 */
class IpcServiceDiscoveryCentralizedException : public ::ara::core::Exception {
 public:
  using Exception::Exception;
};

/*!
 * \brief IpcServiceDiscoveryCentralized Error Domain class.
 * \details This class represents an error domain responsible for all errors occurring in
 * IpcServiceDiscoveryCentralized.
 *
 * \vprivate Vector product internal API
 */
class IpcServiceDiscoveryCentralizedErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief IpcServiceDiscoveryCentralized error domain identifier (unique domain ID).
   */
  constexpr static ::ara::core::ErrorDomain::IdType kId{0xc0000000001e0ab4};

  /*!
   * \brief Error code type definition.
   * \vprivate Vector product internal API
   */
  using Errc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Constructor for IpcServiceDiscoveryCentralizedErrorDomain.
   * \vprivate Vector product internal API
   */
  constexpr IpcServiceDiscoveryCentralizedErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name for this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vprivate Vector product internal API
   */
  // clang-format off
  auto Name() const noexcept -> StringType final /* COV_IpcSD_unused */ {
    // clang-format on
    return "IpcServiceDiscoveryCentralized";
  }

  /*!
   * \brief Return the textual description for the given error code.
   * \param[in] error_code The domain specific error code.
   * \return The text as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
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
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \throws            ThreadException for all error codes.
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vprivate Vector product internal API
   */
  // clang-format off
  [[noreturn]] void ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) final /* COV_IpcSD_unused */ {
    // clang-format on
    vac::language::ThrowOrTerminate<IpcServiceDiscoveryCentralizedException>(error_code);
  }
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
/*!
 * \brief Global IpcServiceDiscoveryCentralizedErrorDomain instance.
 */
constexpr IpcServiceDiscoveryCentralizedErrorDomain kIpcServiceDiscoveryCentralizedErrorDomain;
}  // namespace internal

/*!
 * \brief Factory function of the IpcServiceDiscoveryCentralizedErrorDomain.
 * \return Constant reference to the single instance of the IpcServiceDiscoveryCentralizedErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           TRUE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto GetIpcServiceDiscoveryCentralizedErrorDomain() -> ::ara::core::ErrorDomain const& {
  return internal::kIpcServiceDiscoveryCentralizedErrorDomain;
}

/*!
 * \brief Make ErrorCode instances from the IpcServiceDiscoveryCentralizedErrorDomain.
 * \param[in] code An error code identifier from the IpcServiceDiscoveryCentralizedErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to IpcServiceDiscoveryCentralizedErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto MakeErrorCode(IpcServiceDiscoveryCentralizedErrorDomain::Errc code,
                                    ::ara::core::ErrorDomain::SupportDataType data, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), GetIpcServiceDiscoveryCentralizedErrorDomain(), data,
          message};
}

/*!
 * \brief Make ErrorCode instances from the IpcServiceDiscoveryCentralizedErrorDomain.
 * \param[in] code An error code identifier from the IpcServiceDiscoveryCentralizedErrc enumeration.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to IpcServiceDiscoveryCentralizedErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vprivate Vector product internal API
 */
inline constexpr auto MakeErrorCode(IpcServiceDiscoveryCentralizedErrorDomain::Errc code, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code),
          GetIpcServiceDiscoveryCentralizedErrorDomain(),
          {},
          message};
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_ERROR_DOMAIN_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_unused
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is part of the public unit interface and supposed to be helpful for future extensions of
//            the component and tested by unit tests, no component test function coverage needs to be achieved for this
//            function. Usage of the function is strictly forbidden by precondition and compliance with this
//            precondition is checked by review.
// COV_JUSTIFICATION_END
