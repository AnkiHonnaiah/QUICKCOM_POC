/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *         \brief  Error Domain for the ZeroCopyCommon component.
 *         \unit  ZeroCopyCommon::Common::ZeroCopyCommonErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_ZERO_COPY_COMMON_ERROR_DOMAIN_H_
#define LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_ZERO_COPY_COMMON_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace zero_copy_common {
namespace common {

/*!
 * \brief Enumeration for all error code values of ZeroCopyCommon.
 * \vpublic
 */
enum class ZeroCopyCommonErrc : ::ara::core::ErrorDomain::CodeType {
  kDefaultError = 1,
  kQueueError = 2,
  kProtocolError = 3,
  kMessageCorrupted = 4,
  kMemoryError = 5,
  kInvalidConfiguration = 6,
  kUnexpectedState = 7,
  kUnexpectedReceiverState = 8,
  kReceiverError = 9,
  kPeerCrashedError = 10,
  kPeerDisconnectedError = 11,
  kDroppedNotificationError = 12,
  kDisconnected = 13
};

/*!
 * \brief Exception type of ZeroCopyCommon.
 * \vpublic
 */
class ZeroCopyCommonException : public ::ara::core::Exception {
 public:
  /*!
   * \brief Inherit base class constructor.
   * \vprivate
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for ZeroCopyCommon.
 * \vpublic
 */
class ZeroCopyCommonErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code type definition.
   * \vpublic
   */
  using Errc = ZeroCopyCommonErrc;

  /*!
   * \brief Construct the ZeroCopyCommon error domain.
   * \vpublic
   */
  constexpr ZeroCopyCommonErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name for this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  auto Name() const noexcept -> StringType final { return "ZeroCopyCommon"; };

  /*!
   * \brief Return the textual description for the given error code.
   * \param[in] error_code The domain specific error code.
   * \return The error text as a null-terminated string for error codes defined in this domain.
   * \return A standard error text as a null-terminated string for unknown error codes not defined in this domain.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  auto Message(::ara::core::ErrorDomain::CodeType error_code) const noexcept -> StringType final;

  /*!
   * \brief Throw the given ErrorCode as Exception.
   * \param[in] error_code The ErrorCode to be thrown.
   * \details If the code is compiled w/o exceptions, this function will call terminate instead.
   * \context ANY
   * \pre -
   * \throws ZeroCopyCommonException for all error codes.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  [[noreturn]] void ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) final {
    ::vac::language::ThrowOrTerminate<ZeroCopyCommonException>(error_code);
  }

 private:
  /*!
   * \brief ZeroCopyCommon error domain identifier (unique domain ID).
   * \vprivate
   */
  constexpr static ::ara::core::ErrorDomain::IdType kId{0xc0000000001e5cd8UL};
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
/*!
 * \brief Global ZeroCopyCommonErrorDomain instance.
 * \vprivate
 */
constexpr ZeroCopyCommonErrorDomain kZeroCopyCommonErrorDomain{};
}  // namespace internal

/*!
 * \brief Factory function of the ZeroCopyCommonErrorDomain.
 * \return Constant reference to the single instance of the ZeroCopyCommonErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto GetZeroCopyCommonErrorDomain() -> ::ara::core::ErrorDomain const& {
  return internal::kZeroCopyCommonErrorDomain;
}

/*!
 * \brief Make ErrorCode instances from the ZeroCopyCommonErrorDomain.
 * \param[in] code An error code identifier from the ZeroCopyCommonErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user code (null-terminated).
 * \return ErrorCode instance always references to ZeroCopyCommonErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto MakeErrorCode(ZeroCopyCommonErrorDomain::Errc code,
                                    ::ara::core::ErrorDomain::SupportDataType data, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), GetZeroCopyCommonErrorDomain(), data, message};
}

/*!
 * \brief Make ErrorCode instances from the ZeroCopyCommonErrorDomain (with default support data).
 * \param[in] code An error code identifier from the ZeroCopyCommonErrc enumeration.
 * \param[in] message Additional error message supplied by user code (null-terminated).
 * \return ErrorCode instance always references to ZeroCopyCommonErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto MakeErrorCode(ZeroCopyCommonErrorDomain::Errc code, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return MakeErrorCode(code, {}, message);
}

}  // namespace common
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_COMMON_ZERO_COPY_COMMON_ERROR_DOMAIN_H_
