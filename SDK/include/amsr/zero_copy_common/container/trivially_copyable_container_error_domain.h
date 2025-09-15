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
/*!       \file
 *        \brief  Error domain for trivially copyable containers.
 *        \unit   ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableContainerErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_CONTAINER_ERROR_DOMAIN_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_CONTAINER_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

/*!
 * \brief Enumeration for all error code values of trivially copyable containers.
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableContainer-ReadOnlyWrapper
 * \vpublic
 */
enum class TriviallyCopyableContainerErrc : ::ara::core::ErrorDomain::CodeType {
  kDefaultError = 1,
  kContainerCorrupted = 2,
};

/*!
 * \brief Exception type of trivially copyable containers.
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableContainer-ReadOnlyWrapper
 * \vpublic
 */
class TriviallyCopyableContainerException : public ::ara::core::Exception {
 public:
  /*!
   * \brief Inherit base class constructor.
   * \vprivate
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for trivially copyable containers.
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableContainer-ReadOnlyWrapper
 * \vpublic
 */
class TriviallyCopyableContainerErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code type definition.
   * \vpublic
   */
  using Errc = TriviallyCopyableContainerErrc;

  /*!
   * \brief Construct the trivially copyable container error domain.
   * \vpublic
   */
  constexpr TriviallyCopyableContainerErrorDomain() noexcept : ErrorDomain(kId) {}

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
  auto Name() const noexcept -> StringType final { return "TriviallyCopyableContainer"; };

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
   * \throws TriviallyCopyableContainerException for all error codes.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  [[noreturn]] void ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) final {
    ::vac::language::ThrowOrTerminate<TriviallyCopyableContainerException>(error_code);
  }

 private:
  /*!
   * \brief Trivially copyable container error domain identifier (unique domain ID).
   * \vprivate
   */
  constexpr static ::ara::core::ErrorDomain::IdType kId{0x1ccb7968b5ad4995UL};
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
/*!
 * \brief Global TriviallyCopyableContainerErrorDomain instance.
 * \vprivate
 */
constexpr TriviallyCopyableContainerErrorDomain kTriviallyCopyableContainerErrorDomain{};
}  // namespace internal

/*!
 * \brief Factory function of the TriviallyCopyableContainerErrorDomain.
 * \return Constant reference to the single instance of the TriviallyCopyableContainerErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto GetTriviallyCopyableContainerErrorDomain() -> ::ara::core::ErrorDomain const& {
  return internal::kTriviallyCopyableContainerErrorDomain;
}

/*!
 * \brief Make ErrorCode instances from the TriviallyCopyableContainerErrorDomain.
 * \param[in] code An error code identifier from the TriviallyCopyableContainerErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user code (null-terminated).
 * \return ErrorCode instance always references to TriviallyCopyableContainerErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto MakeErrorCode(TriviallyCopyableContainerErrorDomain::Errc code,
                                    ::ara::core::ErrorDomain::SupportDataType data, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return {static_cast<::ara::core::ErrorDomain::CodeType>(code), GetTriviallyCopyableContainerErrorDomain(), data,
          message};
}

/*!
 * \brief Make ErrorCode instances from the TriviallyCopyableContainerErrorDomain (with default support data).
 * \param[in] code An error code identifier from the TriviallyCopyableContainerErrc enumeration.
 * \param[in] message Additional error message supplied by user code (null-terminated).
 * \return ErrorCode instance always references to TriviallyCopyableContainerErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate
 */
inline constexpr auto MakeErrorCode(TriviallyCopyableContainerErrorDomain::Errc code, char const* message = nullptr)
    -> ::ara::core::ErrorCode {
  return MakeErrorCode(code, {}, message);
}

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_CONTAINER_ERROR_DOMAIN_H_
