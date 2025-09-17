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
/*!         \file
 *         \brief  Common general types.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_COMMON_GENERAL_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_COMMON_GENERAL_TYPES_H_

#include <cstdint>

#include "amsr/core/string.h"
#include "tls/public/tls_comm_error_definitions.h"

namespace amsr {
namespace tls {
namespace internal {
namespace common {
/*!
 * \brief Contains line number.
 * \vprivate This enum class is component private.
 */
struct LineNumber {
  /*! \brief Line number. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains index number.
 * \vprivate This enum class is component private.
 */
struct Index {
  /*! \brief Index number. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains amount of size field bytes.
 * \vprivate This enum class is component private.
 */
struct SizeFieldBytes {
  /*! \brief Amount of size field bytes. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains maximum field size.
 * \vprivate This enum class is component private.
 */
struct MaxFieldSize {
  /*! \brief Maximum field size. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains field size.
 * \vprivate This enum class is component private.
 */
struct FieldSize {
  /*! \brief Field size. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains the size of a buffer.
 * \vprivate This enum class is component private.
 */
struct BufferSize {
  /*! \brief Size of a buffer. */
  std::uint32_t const value_;
};
/*!
 * \brief Contains the file name.
 * \vprivate This enum class is component private.
 */
struct FileName {
  /*! \brief File name. */
  char const* value_;
};
/*!
 * \brief Contains the function name.
 * \vprivate This enum class is component private.
 */
struct FunctionName {
  /*! \brief The function name. */
  char const* value_;
};
/*!
 * \brief Contains the number of bytes.
 * \vprivate This enum class is component private.
 */
struct NrOfBytes {
  /*! \brief The number of bytes. */
  std::uint32_t const value_;
};

/*!
 * \brief Enum class for TLS type
 * \vprivate This enum class is component private.
 */
enum class TlsType : ::std::uint8_t { kTls = 0U, kDtls };

/*!
 * \brief The maximum allowed size of a TLS and DTLS plain text fragment, is 2^14 bytes according to TLS 1.2
 * \details This applies to the PlainText only. The CompressedText and CipherText are allowed to be slightly bigger.
 * Compressed text can be 1024 bytes longer (but we only support NULL compression so the max size should be 2^14).
 * CipherText are allowed to be 2048 bytes longer, see kMaxRecordExpansion.
 * \trace SPEC-6561834
 */
constexpr uint16_t const kMaxFragmentSize{16384U};  // 2^14

/*! \brief The major version for TLS */
constexpr uint8_t const kTlsMajorVersion{3U};

/*! \brief The minor version for TLS */
constexpr uint8_t const kTlsMinorVersion{3U};

/*! \brief The major version for DTLS 1.2 */
constexpr uint8_t const kDtlsMajorVersion{254U};

/*! \brief The minor version for DTLS 1.2 */
constexpr uint8_t const kDtlsMinorVersion{253U};

/*! \brief The minor version for DTLS 1.0 */
constexpr uint8_t const kDtlsMinorVersionOneDotZero{255U};

/*!
 * \brief The maximum expansion of a record when encrypting.
 * \details Encrypting a plain text fragment should maximum add 2048 Bytes.
 * See chapter "6.2.3 Payload protection for record protocol" in the TLS RFC
 * \trace SPEC-6493112
 * */
constexpr uint16_t const kMaxRecordExpansion{2048U};

/*! \brief the alert message size */
constexpr uint8_t const kAlertMessageSize{2U};

/*! \brief the content of change cipher spec message */
constexpr uint8_t const kChangeCipherSpecMessageContent{1U};

/*! \brief the size of change cipher spec message */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_libseccom_M3.4.1_GlobalVariableCanBeDeclaredInsideFunction
constexpr uint8_t const kChangeCipherSpecMessageSize{1U};

/*! \brief the maximum allowed size of argument of a buffer from network or commParty */
constexpr uint32_t const kMaxDataSize{0xFFFFFFFFU - 1U};  // 2^32 - 1

/*! \brief The size for the sliding window */
constexpr uint8_t const kSlidingWindowSize{64U};

/*! \brief The max value for the sequence number used in DTLS records */
constexpr uint64_t const kDtlsSequenceNumberLimit{0xFFFFFFFFFFFDU};  // (2^48 - 1) - 2

/*! \brief The max value for the write sequence counter used in TLS records */
constexpr uint64_t const kTlsSequenceNumberLimit{0xFFFFFFFFFFFFFFFDU};  // (2^64 - 1) - 2

/*!
 * \brief This is a passive information carrier. Includes information that is of value in an execution flow.
 * \details
 * The flow context includes information that is passed along in an execution flow and carries information that
 * is used in different scopes but are common for a flow. Example of information is traceability identifiers,
 * common logging information and error information. See also FlowContextUniquePtr.
 * Example of usage:
 * When starting a handshake, a common name and trace id should be used during the full handshake. This will make
 * it traceable in the log information.
 * THIS TRACE CAUSES DOCUMENTATION BUILD ERROR: trace DSGN-SecCom-FlowContext
 * \vprivate This struct is component private.
 */
struct TlsCommFlowContext final {
  /*! \brief The name of the message flow that is currently executing. */
  amsr::core::String message_flow_name{};

  /*! \brief The traceability unique identifier of the currently executing flow. */
  amsr::core::String uid{};

  /*! \brief Indicates if the call returned an error. Set to false if the call was successful. */
  bool has_error{false};

  /*! \brief A string containing information about the error. Shall be ignored if has_error is false. */
  amsr::core::String error_string{};

  /*! \brief Error code of the discovered error. Ignored if "has_error" property is false. */
  TlsCommErrorCode error_code{TlsCommErrorCode::kTlsErrOk};
};

}  // namespace common
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_COMMON_GENERAL_TYPES_H_
