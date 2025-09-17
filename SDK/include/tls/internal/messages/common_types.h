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
/*!        \file  common_types.h
 *        \brief  Provides definition of common types related to TLS messages.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_COMMON_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_COMMON_TYPES_H_

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t

#include "ara/core/vector.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Provides "HashAlgorithm" identifiers according to the TLS specification.
 *
 * \see   RFC 5246 7.4.1.4.1
 * \see   RFC 8422 5.1.3
 */
enum class HashAlgorithm : std::uint8_t {
  // RFC 5246
  kNone = 0,
  kMd5 = 1,
  kSha1 = 2,
  kSha224 = 3,
  kSha256 = 4,
  kSha384 = 5,
  kSha512 = 6,

  // RFC 8422
  kIntrinsic = 8,
};

/*!
 * \brief     Indicates whether a hash algorithm identifier is valid.
 *
 * \param[in] value The hash algorithm identifier to check.
 *
 * \return    True if the identifier is valid; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
constexpr auto IsValidHashAlgorithm(std::uint8_t const value) noexcept -> bool { return (value <= 6) || (value == 8); }

/*!
 * \brief     Indicates whether a hash algorithm identifier is reserved.
 *
 * \param[in] value The hash algorithm identifier to check.
 *
 * \return    True if the identifier is reserved; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
constexpr auto IsReservedHashAlgorithm(std::uint8_t const value) noexcept -> bool {
  return (value == 7) || (value >= 9);
}

/*!
 * \brief Provides "SignatureAlgorithm" identifiers according to the TLS specification.
 *
 * \see   RFC 5246 7.4.1.4.1
 * \see   RFC 8422 5.4
 * \see   RFC 9189 5
 */
enum class SignatureAlgorithm : std::uint8_t {
  // RFC 5246
  kAnonymous = 0,
  kRsa = 1,
  kDsa = 2,
  kEcdsa = 3,

  // RFC 8422
  kEd25519 = 7,
  kEd448 = 8,

  // RFC 9189
  kGostr34102012_256 = 64,
  kGostr34102012_512 = 65
};

/*!
 * \brief     Indicates whether a signature algorithm identifier is valid.
 *
 * \param[in] value The signature algorithm identifier to check.
 *
 * \return    True if the identifier is valid; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
constexpr auto IsValidSignatureAlgorithm(std::uint8_t const value) noexcept -> bool {
  return (value == 0) || (value == 1) || (value == 2) || (value == 3) || (value == 7) || (value == 8) ||
         (value == 64) || (value == 65);
}

/*!
 * \brief     Indicates whether a signature algorithm identifier is reserved.
 *
 * \param[in] value The signature algorithm identifier to check.
 *
 * \return    True if the identifier is reserved; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
constexpr auto IsReservedSignatureAlgorithm(std::uint8_t const value) noexcept -> bool {
  return ((value >= 4) && (value <= 6)) || ((value >= 9) && (value <= 63)) || (value >= 66);
}

/*!
 * \brief Provides "ClientCertificateType" identifiers according to the TLS specification.
 *
 * \see   RFC 5246 7.4.4
 * \see   RFC 8422 5.5
 * \see   RFC 9189 7
 */
enum class ClientCertificateType : std::uint8_t {
  // RFC 5246
  kRsaSign = 1,
  kDssSign = 2,
  kRsaFixedDh = 3,
  kDssFixedDh = 4,

  // RFC 8422
  kEcdsaSign = 64,
  kRsaFixedEcdh = 65,    // Deprecated, provided for compatibility.
  kEcdsaFixedEcdh = 66,  // Deprecated, provided for compatibility.

  // RFC 9189
  kGostSign256 = 67,
  kGostSign512 = 68
};

/*!
 * \brief     Indicates whether a client certificate type identifier is valid.
 *
 * \param[in] value The client certificate type identifier to check.
 *
 * \return    True if the identifier is valid; false otherwise.
 *
 * \pre       -
 */
constexpr auto IsValidClientCertificateType(std::uint8_t const value) noexcept -> bool {
  // VECTOR NL AutosarC++17_10-M0.1.2: MD_libseccom_M0.1.2_falsepositive
  return ((value >= 1) && (value <= 4)) || ((value >= 64) && (value <= 68));
}

/*!
 * \brief     Indicates whether a client certificate type is deprecated.
 *
 * \param[in] certificate_type The client certificate type to check.
 *
 * \return    True if the identifier is deprecated; false otherwise.
 *
 * \pre       -
 */
constexpr auto IsDeprecatedClientCertificateType(ClientCertificateType const certificate_type) noexcept -> bool {
  return (certificate_type == ClientCertificateType::kRsaFixedEcdh) ||
         (certificate_type == ClientCertificateType::kEcdsaFixedEcdh);
}

/*!
 * \brief     Indicates whether a client certificate type is reserved.
 *
 * \details   Values formally considered "unassigned" are deemed reserved by this function.
 *
 * \param[in] value The client certificate type to check.
 *
 * \return    True if the identifier is reserved; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
constexpr auto IsReservedClientCertificateType(std::uint8_t const value) noexcept -> bool {
  return (value == 0) || ((value >= 5) && (value <= 63)) || (value >= 69);
}

/*!
 * \brief Represents a hash and signature algorithm pair according to the TLS specification.
 *
 * \see   RFC 5246 7.4.1.4.1
 */
struct SignatureAndHashAlgorithm final {
  /*!
   * \brief A hash algorithm identifier.
   */
  HashAlgorithm hash;

  /*!
   * \brief A signature algorithm identifier.
   */
  SignatureAlgorithm signature;
};

/*!
 * \brief Represents a "DistinguishedName" according to the TLS specification.
 *
 * \see   RFC 5246 7.4.4
 */
struct DistinguishedName final {
  /*!
   * \brief The minimum number of bytes in a distinguished name.
   */
  static constexpr std::size_t kMinLength{1};

  /*!
   * \brief The maximum number of bytes in a distinguished name.
   */
  static constexpr std::size_t kMaxLength{65'535};  // 2^16-1

  /*!
   * \brief The number of bytes used by the size field for distinguished names.
   */
  static constexpr std::size_t kSizeFieldLength{2};

  /*!
   * \brief The binary name data.
   */
  ara::core::Vector<std::uint8_t> data{};
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_COMMON_TYPES_H_
