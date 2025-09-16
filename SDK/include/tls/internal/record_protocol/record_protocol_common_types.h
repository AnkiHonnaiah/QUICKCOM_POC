/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!         \file  record_protocol_common_types.h
 *         \brief  This file contains using aliases, forward declarations of common types and definitions of constants
 *                 used by RecordProtocol.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_COMMON_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_COMMON_TYPES_H_

#include <cstdint>
#include <memory>
#include "ara/core/vector.h"

#include "tls/internal/common/common_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*! \brief The size of a TLS header. */
static constexpr std::uint8_t kTlsMessageHeaderSize{5U};

/*! \brief The size of a DTLS header. */
static constexpr std::uint8_t kDtlsMessageHeaderSize{13U};

/*!
 * \brief The size of the additional data added in AEAD algorithms, will be equal to
 * SEQ_NBR + TLSCOMPRESSES.TYPE + TLSCOMPRESSED.VERSION + TLSCOMPRESSED.LENGTH
 */
constexpr std::uint8_t kAeadAdditionalDataSize{kDtlsMessageHeaderSize};

/*!
 * \brief Size limit for received ciphertext records
 * \trace SPEC-6561846
 */
constexpr std::size_t kRecordOverflowLimit{18432};  // 2^14 + 2048

/*!
 * \brief The sequence number size, implicitly added to mac in TLS and explicitly as epoch + message seq in DTLS.
 * This size is large enough to contain a 2^64 number.
 */
constexpr std::uint8_t kSequenceNumberSize{8U};

/*!
 * \brief An enum class containing different types of content that a record can hold, for more information see
 *        https://tools.ietf.org/pdf/rfc5246.pdf
 * \trace SPEC-6477465
 * \vprivate This enum class is component private.
 */
enum class ContentType : std::uint8_t {
  kChangeCipherSpec = 20U,
  kAlert = 21U,
  kHandshake = 22U,
  kApplicationData = 23U
};

/*!
 * \brief An enum class containing types for read or write
 * \vprivate This enum class is component private.
 */
enum class ReadOrWrite : std::uint8_t { kRead = 0U, kWrite = 1U };

/*! \brief Forward declaration for amsr::tls::internal::record_protocol::SecurityParameters */
struct SecurityParameters;

/*! \brief Forward declaration for amsr::tls::internal::common::TlsType enumerator */
using TlsType = common::TlsType;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherText */
class CipherText;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherTextTls */
class CipherTextTls;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherTextDtls */
class CipherTextDtls;

/*! \brief Short alias for a unique pointer wrapping a CipherText */
using CipherTextUptr = std::unique_ptr<CipherText>;

/*! \brief Using a vector to represent a (D)TLS Record Protocol record. */
using Record = ara::core::Vector<std::uint8_t>;

/*! \brief Forward declaration for nstlcomm::record_protocol::CompressedText */
class CompressedText;

/*! \brief Forward declaration for nstlcomm::record_protocol::CompressedTextTls */
class CompressedTextTls;

/*! \brief Forward declaration for nstlcomm::record_protocol::CompressedTextDtls */
class CompressedTextDtls;

/*! \brief Forward declaration for nstlcomm::record_protocol::PlainText */
class PlainText;

/*! \brief Forward declaration for nstlcomm::record_protocol::PlainTextTls */
class PlainTextTls;

/*! \brief Forward declaration for nstlcomm::record_protocol::PlainTextDtls */
class PlainTextDtls;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherText */
class CipherText;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherTextTls */
class CipherTextTls;

/*! \brief Forward declaration for nstlcomm::record_protocol::CipherTextDtls */
class CipherTextDtls;

/*! \brief Short alias for a unique pointer wrapping a CompressedText */
using CompressedTextUptr = std::unique_ptr<CompressedText>;

/*! \brief Short alias for a unique pointer wrapping a PlainText */
using PlainTextUptr = std::unique_ptr<PlainText>;

/*! \brief Short alias for a unique pointer wrapping a CipherText */
using CipherTextUptr = std::unique_ptr<CipherText>;

/*! \brief Short alias for a ara::core::Vector<std::uint8_t> representing the payload container */
using Payload = ara::core::Vector<std::uint8_t>;

/*! \brief Short alias for ara::core::Vector<std::uint8_t> representing the input to MAC operations */
using MacInput = ara::core::Vector<std::uint8_t>;

/*! \brief Short alias for ara::core::Vector<std::uint8_t> representing the output to MAC operations */
using MacOutput = ara::core::Vector<std::uint8_t>;

/*! \brief Short alias for ara::core::Vector<std::uint8_t> representing a Nonce for decryption operations */
using Nonce = ara::core::Vector<std::uint8_t>;

/*! \brief Short alias for ara::core::Vector<std::uint8_t> representing AdditionalData for decryption operations */
using AdditionalData = ara::core::Vector<std::uint8_t>;

/*! \brief Short alias for ara::core::Vector<std::uint8_t> representing a Serialized record for sending */
using SerializedRecord = ara::core::Vector<uint8_t>;

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_COMMON_TYPES_H_
