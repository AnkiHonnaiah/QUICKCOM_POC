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
/*!          \file  record_protocol_validator.h
 *          \brief  This file contains the class declaration for the RecordProtocolValidator
 *        \details  The RecordProtocolValidator validates received records for both TLS and DTLS.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_VALIDATOR_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_VALIDATOR_H_

#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief Enumeration class defining return values for validation checks.
 * \vprivate This enum class is component private.
 */
enum class ReturnTypeValidator : ::std::uint8_t {
  kFailed = 0U,                  // returned if a check failed (protocol independent failure return value)
  kPassed = 1U,                  // returned if it passed all validation checks
  kDrop = 2U,                    // returned (only for DTLS) if the record must be dropped
  kFatalUnexpectedMessage = 3U,  // returned if the record contains an unexpected message (TLS only)
  kUseNullCipher = 4U,           // returned if the record contains a unencrypted handshake message
  kContainsClientHello = 5U,     // returned (only for DTLS) if the record contains a ClientHello message,
                                 // must use Null cipher too
  kRetransmit = 6U,              // returned if the record is a retransmit
  kOverflow = 7U                 // returned if the size of the ciphertext exceeds 18432 bytes
};

/*! \brief The RecordProtcolValidator verifies (D)TLS CipherTexts based on the current state of the owning
 * RecordProtocol
 * \vprivate This class is component private.
 */
class RecordProtocolValidator {
 public:
  /*!
   * \brief Validates an instance of CipherTextTls using unencrypted header information.
   * \details If validation succeeded ReturnTypeValue::kPassed is returned.
   * If it returns with ReturnTypeValue::kOverflow the CipherText is exceeding the accepted size limit, severity fatal.
   * If it returns with ReturnTypeValue::kFatalUnexpectedMessage an unexpected message was received, severity fatal.
   * If it returns with ReturnTypeValue::kDrop the cipher text MUST be dropped.
   * \param[in] cipherTextTls A const reference to the CipherTextTls instance to be validated
   * \return The status code indicating if validation succeeded or failed
   * \reentrant FALSE
   * \context RecordProtocolDeserializer
   * \synchronous TRUE
   * \vprivate This method is component private.
   * \pre -
   */
  static ReturnTypeValidator ValidateCipherTextTls(CipherTextTls const& cipherTextTls) noexcept;
  /*!
   * \brief Validates an instance of CipherTextDtls using unencrypted header information.
   * \details If validations succeeded ReturnTypeValue::kPassed is returned.
   * - If it returns with ReturnTypeValue::kDrop the cipher text MUST be dropped.
   * - If it returns with ReturnTypeValue::kUseNullCipher the null cipher MUST be used for this cipher text.
   * - If it returns with ReturnTypeValue::kContainsClientHello the null cipher MUST be used for this cipher text and
   * the sequence number of this record must be saved as the most recent received client hello sequence number.
   * \param[in] cipherTextDtls A const reference to the CipherTextDtls instance to be validated
   * \param[in] rpContext A const reference to the current RecordProtocolContext
   * \return The status code indicating if validation succeeded or failed
   * \reentrant FALSE
   * \context RecordProtocolDeserializer
   * \synchronous TRUE
   * \vprivate This method is component private.
   * \pre -
   */
  static ReturnTypeValidator ValidateCipherTextDtls(CipherTextDtls const& cipherTextDtls,
                                                    RecordProtocolContext const& rpContext) noexcept;
  /*!
   * \brief Validates an instance of PlainText using potentially encrypted data.
   * \details If validations succeeded ReturnTypeValue::kPassed is returned.
   * - If it returns with ReturnTypeValue::kDrop the plain text MUST be dropped.
   * \param[in] plainText A const reference to the PlainText instance to be validated.
   * \param[in] rpContext A const reference to the current RecordProtocolContext.
   * \return The status code indicating if validation succeeded or failed.
   * \reentrant FALSE
   * \context RecordProtocolDeserializer
   * \synchronous TRUE
   * \vprivate This method is component private.
   * \pre -
   */
  static ReturnTypeValidator PostValidatePlainText(PlainText const& plainText,
                                                   RecordProtocolContext const& rpContext) noexcept;
};
/*!
 * \brief The validator_internal namespace encapsulates check functions used for validation of (D)TLS cipher texts and
 * they are intended to only be used internally by the RecordProtocolValidator
 */
namespace validator_internal {

/*!
 * \brief Checks if the size of the passed CipherText does not exceed 18432 bytes.
 * \details If the value of previousCheck is not kPassed this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherText to be checked
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the size of the CipherText is exceeding its limit kOverflow is returned, otherwise kPassed.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckCipherTextSize(CipherText const& cipherText, ReturnTypeValidator const previousCheck) noexcept;

/*!
 * \brief Checks if the size of the passed PlainText does not exceed 2^14 bytes.
 * \param[in] plain_text A const reference to the PlainText to be checked.
 * \return If the size of the PlainText is exceeding its limit kOverflow is returned, otherwise kPassed.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckPlainTextSize(PlainText const& plain_text) noexcept;

/*!
 * \brief Checks the content type of the passed CipherText
 * \details If the value of previousCheck is not kPassed this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherText to be checked
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the content type of the cipher text is supported kPassed is returned, otherwise kFailed is returned.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckContentType(CipherText const& cipherText, ReturnTypeValidator const previousCheck) noexcept;
/*!
 * \brief Checks the protocol version of the passed PlainText.
 * \details  If the plain text contains a ClientHello, ServerHello or HelloVerifyRequest handshake message the protocol
 * version check is skipped and kPassed is returned.
 * \param[in] isTls flag indicating which protocol version to check against, TLS or DTLS.
 * \param[in] plainText A const reference to the PlainText to be checked.
 * \return If the protocol version of the plain text is supported kPassed is returned, other wise kDrop is returned.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckProtocolVersion(bool isTls, PlainText const& plainText) noexcept;

/*!
 * \brief Check that if the content type is a Handshake message that the payload is not empty
 * \param[in] cipherText A const reference to the CipherText to be checked
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the payload of a handshake message is empty kFailed is returned, otherwise kPassed is returned
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckHandshakePayload(CipherText const& cipherText,
                                          ReturnTypeValidator const previousCheck) noexcept;

/*!
 * \brief Checks the epoch of the passed CipherTextDtls
 * \details If the value of previousCheck is not kPassed this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherTextDtls to be checked
 * \param[in] rpContext A const reference to the current RecordProtocolContext
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the epoch of the cipher text is valid kPassed is returned, otherwise kDrop is returned.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckEpoch(CipherTextDtls const& cipherText, RecordProtocolContext const& rpContext,
                               ReturnTypeValidator const previousCheck) noexcept;
/*!
 * \brief Checks the explicit sequence number of the passed CipherTextDtls against the current sliding window provided
 * by the RecordProtocolContext.
 * \details If the value of previousCheck is not kPassed this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherTextDtls to be checked
 * \param[in] rpContext A const reference to the current RecordProtocolContext
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the explicit sequence number is a duplicate or behind the sliding window kDrop is returned, otherwise
 * kPassed is returned.
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckSlidingWindow(CipherTextDtls const& cipherText, RecordProtocolContext const& rpContext,
                                       ReturnTypeValidator const previousCheck) noexcept;
/*!
 * \brief Checks if the epoch of the passed CipherTextDtls is zero and it contains a handshake message
 * \details If the value of previousCheck is not kPassed this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherText to be checked
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the CipherText has epoch zero and contains a handshake message kUseNullCipher is returned, otherwise
 * kPassed is returned
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckForEpochZeroAndHandshakeMessage(CipherTextDtls const& cipherText,
                                                         ReturnTypeValidator const previousCheck) noexcept;
/*!
 * \brief Checks if the passed CipherTextDtls contains a ClientHello handshake message
 * \details If the value of previousCheck is not kUseNullCipher this check is skipped and its value is returned.
 * \param[in] cipherText A const reference to the CipherText to be checked
 * \param[in] previousCheck The outcome of the previous check(s)
 * \return If the CipherText contains a ClientHello handshake message kContainsClientHello is returned, otherwise
 * the value of previousCheck is returned
 * \reentrant FALSE
 * \context RecordProtocolValidator
 * \synchronous TRUE
 * \vprivate This method is component private.
 * \pre -
 */
ReturnTypeValidator CheckForClientHelloMessage(CipherTextDtls const& cipherText,
                                               ReturnTypeValidator const previousCheck) noexcept;
}  // namespace validator_internal

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_VALIDATOR_H_
