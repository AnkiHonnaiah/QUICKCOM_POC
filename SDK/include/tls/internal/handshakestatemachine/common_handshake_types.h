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
/*!        \file  common_handshake_types.h
 *         \brief This file contains enumerations and constant values
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_COMMON_HANDSHAKE_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_COMMON_HANDSHAKE_TYPES_H_

#include <array>
#include <cstdint>

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

// VECTOR Disable AutosarC++17_10-M3.4.1: MD_libseccom_M3.4.1_GlobalVariableCanBeDeclaredInsideFunction

/*! \brief String containing "client finished". */
static char const* const client_finished_string{"client finished"};
/*! \brief String containing "server finished". */
static char const* const server_finished_string{"server finished"};

/*! \brief Size of the FinishedMessage hash for SHA256 */
static constexpr std::uint8_t kMessageHashSizeSha256{32U};
/*! \brief Size of the FinishedMessage hash for SHA384 */
static constexpr std::uint8_t kMessageHashSizeSha384{48U};

/*! \brief Maximum size of the list of CipherSuites */
static constexpr std::uint8_t kMaxSizeOfListOfCipherSuites{10U};

/*! \brief Length of the mt_unix_time and random_bytes fields of ClientHelloMessage */
static constexpr std::uint8_t kClientHelloRandomFieldLength{32U};
/*! \brief Length of the mt_unix_time and random_bytes fields of ServerHelloMessage */
static constexpr std::uint8_t kServerHelloRandomFieldLength{32U};
/*! \brief Size of the size field for PSK id */
static constexpr std::uint8_t kPskIdSizeFieldSize{2U};
/*! \brief Size of the size field for PSK id hint */
static constexpr std::uint8_t kPskIdHintSizeFieldSize{2U};
/*! \brief Size of the field for Elliptic Curve curve type */
static constexpr std::uint8_t kEcCurveTypeSize{1U};
/*! \brief Size of the field for Elliptic Curve curve name */
static constexpr std::uint8_t kEcCurveNameSize{2U};
/*! \brief Size of the field for digitally signed parameters */
static constexpr std::uint8_t kSignedParamSize{2U};
// VECTOR Enable AutosarC++17_10-M3.4.1

/*!
 * \brief Constant for the size of a TLS handshake message header, 4 bytes,
 * type + length (1 byte + 3 byte)
 */
static constexpr std::uint32_t kHandshakeMessageHeaderSizeTls{4U};

/*!
 * \brief Constant for the size DTLS handshake message header,
 * 12 bytes type + length + message + fragment_offset + fragment_length (1 + 3 + 2 + 3 + 3)
 */
static constexpr std::uint32_t kHandshakeMessageHeaderSizeDtls{12U};

/*!
 * \brief Constant for Hello Message Random Byte
 * random_bytes[28] + uint32 gmt_unix_time
 */
static constexpr std::uint8_t kRandomBytesSize{32U};
/*! \brief Size of the field for protocol version */
static constexpr std::uint8_t kProtocolVersionSize{2U};
/*! \brief Maximum size of session id */
static constexpr std::uint8_t kSessionIdMaxSize{32U};

/*!
 * \brief The size of cookies used for verification, 32 bytes for greater compatibility.
 * \trace SPEC-6562059
 */
static constexpr std::uint32_t kCookieMaxSize{32U};
// VECTOR Disable AutosarC++17_10-M3.4.1: MD_libseccom_M3.4.1_GlobalVariableCanBeDeclaredInsideFunction
/*! \brief Maximum length of CipherSuite which is equal to (2^16)-2 */
static constexpr std::uint16_t kCipherSuitesMaxLength{65'534U};
/*! \brief Maximum length of compression field which is equal to (2^8)-1 */
static constexpr std::uint32_t kCompressionMethodsFieldMaxLength{255U};
/*! \brief Maximum length of HelloMessage extension which is equal to (2^16)-1 */
static constexpr std::uint32_t kHelloExtensionsFieldMaxLength{65'535U};

/*! \brief Length of the MAC key when sha1 */
static constexpr std::uint8_t kMacKeyLengthSha1{20U};
/*! \brief Length of the MAC when sha1 */
static constexpr std::uint8_t kMacLengthSha1{20U};

/*! \brief Length of the MAC key when sha256 */
static constexpr std::uint8_t kMacKeyLengthSha256{32U};
/*! \brief Length of the MAC when sha256 */
static constexpr std::uint8_t kMacLengthSha256{32U};
/*! \brief Length of the MAC key when sha384 */
static constexpr std::uint8_t kMacKeyLengthSha384{48U};
/*! \brief Length of the MAC when sha384 */
static constexpr std::uint8_t kMacLengthSha384{48U};
/*! \brief Length for verify data field */
static constexpr std::uint8_t kVerifyDataLength{12U};

/*! \brief Length of the MAC key when GCM */
static constexpr std::uint8_t kMacKeyLengthGcm{0U};
/*! \brief Length of the encryption key when aes128 */
static constexpr std::uint8_t kEncryptionKeyLengthAes128{16U};
/*! \brief Initial vector salt when aes128 */
static constexpr std::uint8_t kIvSaltAes128{4U};
/*! \brief Length of the initial vector when aes128 */
static constexpr std::uint8_t kIvLengthAes128{12U};
/*! \brief Length of the initial vector when aesCbc128 */
static constexpr std::uint8_t kIvLengthAesCbc128{16U};
/*! \brief Length of the initial vector when aesCbc256 */
static constexpr std::uint8_t kIvLengthAesCbc256{16U};
/*! \brief Length of the encryption key when aes256 */
static constexpr std::uint8_t kEncryptionKeyLengthAes256{32U};
/*! \brief Initial vector salt when aes256 */
static constexpr std::uint8_t kIvSaltAes256{4U};
/*! \brief Length of the initial vector when aes256 */
static constexpr std::uint8_t kIvLengthAes256{12U};
// VECTOR Enable AutosarC++17_10-M3.4.1

/*! \brief Cipher suite TLS_NULL_WITH_NULL_NULL */
static constexpr std::uint16_t kTlsNullWithNullNull{0x0000U};

/*!
 * \brief Cipher suite TLS_PSK_WITH_NULL_SHA_256
 * \trace SPEC-6181151
 * \trace SPEC-6562824
 * \trace SPEC-6562850
 */
static constexpr std::uint16_t kTlsPskWithNullSha256{0x00b0U};

/*!
 * \brief Cipher suite TLS_PSK_WITH_AES_128_GCM_SHA_256
 * \trace SPEC-6181147
 * \trace SPEC-6562836
 * \trace SPEC-6562842
 */
static constexpr std::uint16_t kTlsPskWithAes128GcmSha256{0x00a8U};

/*!
 * \brief The message sequence of ClientHello Message
 * \trace SPEC-6562068
 */
static constexpr std::uint16_t kMessageSeqClientHello{0U};
/*! \brief The message sequence number of the client certificate message. */
static constexpr std::uint16_t kMessageSeqClientCertificate{1U};  // TODO(ARCTOR-6590)
/*! \brief The message sequence of ClientKeyExchange Message */
static constexpr std::uint16_t kMessageSeqClientKeyExchange{1U};
/*! \brief The message sequence of CertificateVerify Message */
static constexpr std::uint16_t kMessageSeqCertificateVerify{2U};  // TODO(ARCTOR-6590)
/*! \brief The message sequence of ClientFinished Message */
static constexpr std::uint16_t kMessageSeqClientFinished{2U};
/*! \brief The message sequence of ServerHello Message */
static constexpr std::uint16_t kMessageSeqServerHello{0U};
/*! \brief The message sequence of ServerKeyExchange Message */
static constexpr std::uint16_t kMessageSeqServerKeyExchange{1U};
/*! \brief The message sequence of CertificateRequest Message */
static constexpr std::uint16_t kMessageSeqCertificateRequest{2U};  // TODO(ARCTOR-6590)
/*! \brief The message sequence of ServerHelloDone Message */
static constexpr std::uint16_t kMessageSeqServerHelloDone{2U};
/*! \brief The message sequence of ServerFinished Message */
static constexpr std::uint16_t kMessageSeqServerFinished{3U};
/*! \brief The message sequence number of the server certificate message. */
static constexpr std::uint16_t kMessageSeqServerCertificate{2U};  // TODO(ARCTOR-6590)

/*! \brief The message sequence of ServerHelloVerifyRequest Message */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_libseccom_M3.4.1_GlobalVariableCanBeDeclaredInsideFunction
static constexpr std::uint16_t kMessageSeqServerHelloVerifyRequest{0U};

/*!
 * \brief A HandshakeStateHandle is an enum used to easily fetch an instance of
 * a handshake state from the HandshakeStatePool. There exists one handle for
 * each possible state.
 * \vprivate This enum class is component private.
 */
enum class HandshakeStateHandle : std::uint8_t {
  kClientStateDisconnected = 0U,
  kClientStateClientHello,
  kClientStateServerHello,
  kClientStateServerCertificateExchange,
  kClientStateServerKeyExchange,
  kClientStateCertificateRequest,
  kClientStateServerHelloDone,
  kClientStateServerChangeCipherSpec,
  kClientStateConnected,
  kServerStateDisconnected,
  kServerStateClientHello,
  kServerStateClientKeyExchange,
  kServerStateClientChangeCipherSpec,
  kServerStateConnected,
};

/*!
 * \brief Enum with HandshakeMessage types
 * Values are taken from RFC 5246
 * TLS Protocol Version 1.2, Chapter 7.4
 * \trace SPEC-6561859, SPEC-6562056, SPEC-6562098
 * \vprivate This enum class is component private.
 */
enum class HandshakeMessageHandle : std::uint8_t {
  kHelloRequest = 0U,
  kClientHello = 1U,
  kServerHello = 2U,
  kHelloVerifyRequest = 3U,
  kCertificate = 11U,
  kServerKeyExchange = 12U,
  kCertificateRequest = 13U,
  kServerHelloDone = 14U,
  kCertificateVerify = 15U,
  kClientKeyExchange = 16U,
  kFinished = 20U,
  kUndefined = 0xFFU
};

/*! \brief Type alias for array holding cipher suites */
using ArrOfSuites = std::array<std::uint16_t, kMaxSizeOfListOfCipherSuites>;

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_COMMON_HANDSHAKE_TYPES_H_
