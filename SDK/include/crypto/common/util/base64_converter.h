/*!********************************************************************************************************************
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
/*!        \file
 *        \brief  Convert base64 encoded data to DER encoded data
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_BASE64_CONVERTER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_BASE64_CONVERTER_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace common {
namespace util {

/*!
 * \brief Convert base64 encoded data to DER encoded data
 * \vprivate Product Private
 */
class Base64Converter {
 public:
  /*!
   * \brief Converts base64 encoded data to DER encoded data.
   * \param[in] base64_encoded Base64 encoded data.
   * \return DER encoded data
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If input size is invalid or if an input character is invalid.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto Convert(ara::core::Span<char> base64_encoded) noexcept
      -> ara::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Checks if character is a valid base64 character.
   * \param[in] ch Character to check.
   * \return True or false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto CharIsBase64(char ch) noexcept -> bool;

 private:
  /*!
   * \brief Checks if all characters in span are valid base64 characters or 1-2 padding characters at the end.
   * \param[in] span Span to check.
   * \return True or false.
   * \context ANY
   * \pre span size is at least 4.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SpanIsBase64OrPadding(ara::core::Span<char> span) noexcept -> bool;

  crypto::common::util::Logger logger_{util::kCommonLoggingContext, ""_sv}; /*!< logging instance */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__util__Base64Converter, SpanIsBase64OrPadding__True);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__util__Base64Converter, SpanIsBase64OrPadding__False);
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_BASE64_CONVERTER_H_
