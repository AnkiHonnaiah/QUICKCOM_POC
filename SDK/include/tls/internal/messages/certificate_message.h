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
/*!        \file  certificate_message.h
 *        \brief  This file contains the definition of the CertificateMessage class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_MESSAGE_H_

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>
#include <cstdint>

#include "ara/core/vector.h"
#include "ara/crypto/cryp/public_key.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Represents a "ASN.1Cert" according to the TLS specification.
 *
 * \see   RFC 5246 section 7.4.2.
 */
struct Asn1Certificate final {
  /*!
   * \brief The minimum number of bytes in a certificate.
   */
  static constexpr std::size_t kMinLength{1};

  /*!
   * \brief The maximum number of bytes in a certificate.
   */
  static constexpr std::size_t kMaxLength{16'777'215};  // 2^24-1

  /*!
   * \brief The number of bytes used by the size field for certificates.
   */
  static constexpr std::size_t kSizeFieldLength{3};

  /*!
   * \brief The binary certificate representation.
   */
  ara::core::Vector<std::uint8_t> data;
};

/*!
 * \brief   Represents a "Certificate" message according to the TLS specification.
 *
 * \details There are no structural differences between client and server certificate messages,
 *          so this class is intended to be used for both use cases.
 *
 * \see     RFC 5246 section 7.4.2 and 7.4.6.
 *
 * \trace   SPEC-6561904, SPEC-6561935
 */
class CertificateMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief The maximum number of bytes occupied by a certificate list.
   */
  static constexpr std::size_t kCertificateListMaxLength{16'777'215};  // 2^24-1

  /*!
   * \brief The number of bytes used by the size field for the certificate list.
   */
  static constexpr std::size_t kCertificateListSizeFieldLength{3U};

  /*!
   * \brief     Creates a certificate message.
   *
   * \param[in] message_seq The associated handshake message sequence number.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  explicit CertificateMessage(std::uint16_t message_seq) noexcept;

  CertificateMessage(CertificateMessage const&) noexcept = delete;

  CertificateMessage(CertificateMessage&&) noexcept = delete;

  auto operator=(CertificateMessage const&) noexcept -> CertificateMessage& = delete;

  auto operator=(CertificateMessage&&) noexcept -> CertificateMessage& = delete;

  /*!
   * \brief Destroys the certificate message.
   */
  ~CertificateMessage() noexcept final = default;

  /*!
   * \brief     Removes all associated certificates.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  void Cleanup() noexcept;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Serialize
   */
  auto Serialize(common::TlsCommFlowContext& cxt) const noexcept -> ara::core::Vector<std::uint8_t> final;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Deserialize
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief         Adds a certificate to the message.
   *
   * \note          The provided certificate is only added to the message if it's deemed valid.
   *
   * \param[in,out] cxt         The flow context to use for error reporting.
   * \param[in]     certificate The certificate that will be added.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void AddCertificate(common::TlsCommFlowContext& cxt, Asn1Certificate certificate) noexcept;

  /*!
   * \brief     Returns all of the associated certificates.
   *
   * \return    The associated certificate chain.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetCertificateList() const noexcept -> ara::core::Vector<Asn1Certificate> const&;

  /*!
   * \brief     Converts the associated certificates into a binary blob that represents a certificate chain.
   *
   * \return    A binary certificate chain.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto CreateCertificateChainBlob() const noexcept -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief     Sets the public key associated with the certificate message.
   *
   * \param[in] public_key The public key that will be stored.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  void SetPublicKey(ara::crypto::cryp::PublicKey::Uptrc public_key) noexcept;

  /*!
   * \brief     Takes the public key associated with the certificate message, if any.
   *
   * \return    A unique pointer to a public key.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto TakePublicKey() noexcept -> ara::crypto::cryp::PublicKey::Uptrc;

  /*!
   * \brief     Returns the minimum number of bytes in a valid certificate message payload.
   *
   * \return    The minimum payload size in bytes.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  constexpr static auto GetMinPayloadLength() noexcept -> std::size_t { return kCertificateListSizeFieldLength; }

  /*!
   * \brief     Returns the maximum number of bytes in a valid certificate message payload.
   *
   * \return    The maximum payload size in bytes.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  constexpr static auto GetMaxPayloadLength() noexcept -> std::size_t {
    return kCertificateListSizeFieldLength + kCertificateListMaxLength;
  }

 private:
  /*!
   * \brief The associated certificate chain.
   */
  ara::core::Vector<Asn1Certificate> certificates_{};

  /*!
   * \brief The associated public key, not included as a part of the serialized/deserialized message.
   */
  ara::crypto::cryp::PublicKey::Uptrc public_key_{};

  /*!
   * \brief         Deserializes a certificate list, storing all valid certificates.
   *
   * \param[in,out] cxt             The flow context to use for error reporting.
   * \param[in]     serialized_data The buffer of serialized data.
   * \param[in]     index           An offset into the serialized buffer to use as the starting point.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void DeserializeCertificateList(common::TlsCommFlowContext& cxt,
                                  ara::core::Vector<std::uint8_t> const& serialized_data, std::size_t index) noexcept;

  /*!
   * \brief         Deserializes a single certificate.
   *
   * \note          The deserialized certificate is not validated by this function, the caller is
   *                expected to do so themselves.
   *
   * \param[in,out] cxt             The flow context to use for error reporting.
   * \param[in]     serialized_data The buffer of serialized data.
   * \param[in]     index           An offset into the serialized buffer to use as the starting point.
   * \param[in,out] out_certificate Out parameter to which the deserialized certificate is written.
   * \return        The number of processed bytes.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  static auto DeserializeCertificate(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<std::uint8_t> const& serialized_data, std::size_t index,
                                     Asn1Certificate& out_certificate) noexcept -> std::size_t;

  /*!
   * \brief         Validates a list of certificates.
   *
   * \param[in,out] cxt          The flow context to use for error reporting.
   * \param[in]     certificates The list of certificates to validate.
   * \param[in]     error_code   The error code to use if the validation fails.
   * \return        True if the certificates are deemed to be valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  static auto ValidateCertificateList(common::TlsCommFlowContext& cxt,
                                      ara::core::Vector<Asn1Certificate> const& certificates,
                                      TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a single certificate.
   *
   * \param[in,out] cxt         The flow context to use for error reporting.
   * \param[in]     certificate The certificate to validate.
   * \param[in]     error_code  The error code to use if the validation fails.
   * \return        True if the certificate is deemed to be valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  static auto ValidateCertificate(common::TlsCommFlowContext& cxt, Asn1Certificate const& certificate,
                                  TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief     Calculates the total length of a list of certificates in bytes (including size fields).
   *
   * \param[in] certificates The list of certificate to measure.
   *
   * \return    The number of bytes occupied by the certificates.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  static auto GetTotalCertificateListSizeInBytes(ara::core::Vector<Asn1Certificate> const& certificates) noexcept
      -> std::size_t;

  /*!
   * \brief Used to access private data members.
   */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(UT__MessageManager, Cleanup);

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(Cdd__CertificateMessage, Constructor);

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(UT__CertificateMessage, Constructor);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_MESSAGE_H_
