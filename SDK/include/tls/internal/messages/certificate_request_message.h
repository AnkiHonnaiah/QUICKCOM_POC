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
/*!        \file  certificate_request_message.h
 *        \brief  Provides the definition of the CertificateRequestMessage class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_REQUEST_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_REQUEST_MESSAGE_H_

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t, uint16_t

#include "ara/core/vector.h"
#include "tls/internal/messages/common_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief Represents a "CertificateRequest" message.
 *
 * \trace SPEC-6561925
 *
 * \see   RFC 5246 7.4.4
 */
class CertificateRequestMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief The minimum number of bytes occupied by the certificate types.
   */
  static constexpr std::size_t kCertificateTypesMinLength{1};

  /*!
   * \brief The maximum number of bytes occupied by the certificate types.
   */
  static constexpr std::size_t kCertificateTypesMaxLength{255};  // 2^8-1

  /*!
   * \brief The minimum number of bytes occupied by the hash and signature algorithm pairs.
   */
  static constexpr std::size_t kSupportedSignatureAlgorithmsMinLength{2};

  /*!
   * \brief The maximum number of bytes occupied by the hash and signature algorithm pairs.
   */
  static constexpr std::size_t kSupportedSignatureAlgorithmsMaxLength{65'534};  // 2^16-2

  /*!
   * \brief The maximum number of bytes occupied by the certificate authority names.
   */
  static constexpr std::size_t kCertificateAuthoritiesMaxLength{65'535};  // 2^16-1

  /*!
   * \brief The number of bytes used by the size field for the certificate types.
   */
  static constexpr std::size_t kCertificateTypesSizeFieldLength{1};

  /*!
   * \brief The number of bytes used by the size field for the hash and signature algorithm pairs.
   */
  static constexpr std::size_t kSupportedSignatureAlgorithmsSizeFieldLength{2};

  /*!
   * \brief The number of bytes used by the size field for the certificate authority names.
   */
  static constexpr std::size_t kCertificateAuthoritiesSizeFieldLength{2};

  /*!
   * \brief     Creates an empty certificate request message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  CertificateRequestMessage() noexcept;

  CertificateRequestMessage(CertificateRequestMessage const&) noexcept = delete;
  CertificateRequestMessage(CertificateRequestMessage&&) noexcept = delete;

  auto operator=(CertificateRequestMessage const&) noexcept -> CertificateRequestMessage& = delete;
  auto operator=(CertificateRequestMessage&&) noexcept -> CertificateRequestMessage& = delete;

  /*!
   * \brief Destroys the certificate request message.
   */
  ~CertificateRequestMessage() noexcept final = default;

  /*!
   * \brief     Resets the internal buffers.
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
   * \brief         Attempts to set the associated client certificate types.
   *
   * \note          This function will validate the provided certificate types, and only store them if they
   *                are all deemed valid.
   *
   * \param[in,out] cxt               The flow context to use for error reporting.
   * \param[in]     certificate_types The client certificate types to use.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void SetCertificateTypes(common::TlsCommFlowContext& cxt,
                           ara::core::Vector<ClientCertificateType> certificate_types) noexcept;

  /*!
   * \brief     Returns the associated client certificate types.
   *
   * \return    A collection of client certificate types.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetCertificateTypes() const noexcept -> ara::core::Vector<ClientCertificateType> const&;

  /*!
   * \brief         Attempts to set the associated hash and signature algorithm pairs.
   *
   * \note          This function will validate the provided algorithms, and only store them if they
   *                are all deemed valid.
   *
   * \param[in,out] cxt                  The flow context to use for error reporting.
   * \param[in]     signature_algorithms The algorithm pairs to use.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void SetSupportedSignatureAlgorithms(common::TlsCommFlowContext& cxt,
                                       ara::core::Vector<SignatureAndHashAlgorithm> signature_algorithms) noexcept;

  /*!
   * \brief     Returns the associated hash and signature algorithm pairs.
   *
   * \return    A collection of hash and signature algorithm pairs.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetSupportedSignatureAlgorithms() const noexcept -> ara::core::Vector<SignatureAndHashAlgorithm> const&;

  /*!
   * \brief         Attempts to set the associated certificate authority names.
   *
   * \note          This function will validate the certificate authorities, and only store them if they
   *                are all deemed valid.
   *
   * \param[in,out] cxt                     The flow context to use for error reporting.
   * \param[in]     certificate_authorities The certificate authority names to use.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void SetCertificateAuthorities(common::TlsCommFlowContext& cxt,
                                 ara::core::Vector<DistinguishedName> certificate_authorities) noexcept;

  /*!
   * \brief     Returns the associated certificate authority names.
   *
   * \return    A collection of certificate authority names.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetCertificateAuthorities() const noexcept -> ara::core::Vector<DistinguishedName> const&;

  /*!
   * \brief     Returns the minimum number of bytes in a valid certificate request message payload.
   *
   * \return    The minimum payload size in bytes.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  constexpr static auto GetMinPayloadLength() noexcept -> std::size_t {
    return kCertificateTypesSizeFieldLength + kCertificateTypesMinLength +
           kSupportedSignatureAlgorithmsSizeFieldLength + kSupportedSignatureAlgorithmsMinLength +
           kCertificateAuthoritiesSizeFieldLength;
  }

  /*!
   * \brief     Returns the maximum number of bytes in a valid certificate request message payload.
   *
   * \return    The maximum payload size in bytes.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  constexpr static auto GetMaxPayloadLength() noexcept -> std::size_t {
    return kCertificateTypesSizeFieldLength + kCertificateTypesMaxLength +
           kSupportedSignatureAlgorithmsSizeFieldLength + kSupportedSignatureAlgorithmsMaxLength +
           kCertificateAuthoritiesSizeFieldLength + kCertificateAuthoritiesMaxLength;
  }

 private:
  /*!
   * \brief The associated client certificate types.
   */
  ara::core::Vector<ClientCertificateType> certificate_types_{};

  /*!
   * \brief The associated hash and signature algorithm pairs.
   */
  ara::core::Vector<SignatureAndHashAlgorithm> supported_signature_algorithms_{};

  /*!
   * \brief The associated certificate authority names.
   */
  ara::core::Vector<DistinguishedName> certificate_authorities_{};

  /*!
   * \brief         Attempts to deserialize certificate types from a serialized buffer.
   *
   * \param[in,out] cxt    The flow context to use for error reporting.
   * \param[in]     buffer The target buffer of serialized data.
   * \param[in]     index  An offset into the target buffer that marks the deserialization starting point.
   * \return        The number of deserialized bytes.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  auto DeserializeCertificateTypes(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer,
                                   std::size_t index) noexcept -> std::size_t;

  /*!
   * \brief         Attempts to deserialize hash and signature algorithm pairs from a serialized buffer.
   *
   * \param[in,out] cxt    The flow context to use for error reporting.
   * \param[in]     buffer The target buffer of serialized data.
   * \param[in]     index  An offset into the target buffer that marks the deserialization starting point.
   * \return        The number of deserialized bytes.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  auto DeserializeSignatureAlgorithms(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer,
                                      std::size_t index) noexcept -> std::size_t;

  /*!
   * \brief         Attempts to deserialize certificate authorities from a serialized buffer.
   *
   * \param[in,out] cxt    The flow context to use for error reporting.
   * \param[in]     buffer The target buffer of serialized data.
   * \param[in]     index  An offset into the target buffer that marks the deserialization starting point.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void DeserializeCertificateAuthorities(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer,
                                         std::size_t index) noexcept;

  /*!
   * \brief         Attempts to deserialize a single distinguished name from a serialized buffer.
   *
   * \param[in,out] cxt        The flow context to use for error reporting.
   * \param[in]     buffer     The target buffer of serialized data.
   * \param[in]     index      An offset into the target buffer that marks the deserialization starting point.
   * \param[in,out] out_buffer The buffer that the distinguished name will be written to.
   * \return        The number of deserialized bytes.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto DeserializeDistinguishedName(common::TlsCommFlowContext& cxt,
                                           ara::core::Vector<std::uint8_t> const& buffer, std::size_t index,
                                           ara::core::Vector<DistinguishedName>& out_buffer) noexcept -> std::size_t;

  /*!
   * \brief         Validates the message components, indicating whether the message can be serialized.
   *
   * \param[in,out] cxt The flow context to use for error reporting.
   * \return        True if the message can be serialized; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  auto IsSerializable(common::TlsCommFlowContext& cxt) const noexcept -> bool;

  /*!
   * \brief         Validates a collection of certificate types.
   *
   * \param[in,out] cxt               The flow context to use for error reporting.
   * \param[in]     certificate_types The certificate types to check.
   * \param[in]     error_code        The error code to use if the validation fails.
   * \return        True if all certificate types are valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateCertificateTypes(common::TlsCommFlowContext& cxt,
                                       ara::core::Vector<ClientCertificateType> const& certificate_types,
                                       TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a single certificate type identifier.
   *
   * \param[in,out] cxt                 The flow context to use for error reporting.
   * \param[in]     certificate_type_id The certificate type identifier to check.
   * \param[in]     error_code          The error code to use if the validation fails.
   * \return        True if the certificate type identifier is valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateCertificateTypeIdentifier(common::TlsCommFlowContext& cxt, std::uint8_t certificate_type_id,
                                                TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a collection of hash and signature algorithm pairs.
   *
   * \param[in,out] cxt                  The flow context to use for error reporting.
   * \param[in]     signature_algorithms The hash and signature algorithm pairs to check.
   * \param[in]     error_code           The error code to use if the validation fails.
   * \return        True if all hash and signature algorithm pairs are valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateSignatureAlgorithms(common::TlsCommFlowContext& cxt,
                                          ara::core::Vector<SignatureAndHashAlgorithm> const& signature_algorithms,
                                          TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a signature algorithm identifier.
   *
   * \param[in,out] cxt                    The flow context to use for error reporting.
   * \param[in]     signature_algorithm_id The signature identifier to check.
   * \param[in]     error_code             The error code to use if the validation fails.
   * \return        True if the signature algorithm identifier is valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateSignatureAlgorithmIdentifier(common::TlsCommFlowContext& cxt, std::uint8_t signature_algorithm_id,
                                                   TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a hash algorithm identifier.
   *
   * \param[in,out] cxt               The flow context to use for error reporting.
   * \param[in]     hash_algorithm_id The hash identifier to check.
   * \param[in]     error_code        The error code to use if the validation fails.
   * \return        True if the hash algorithm identifier is valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateHashAlgorithmIdentifier(common::TlsCommFlowContext& cxt, std::uint8_t hash_algorithm_id,
                                              TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a collection of certificate authority names.
   *
   * \param[in,out] cxt                     The flow context to use for error reporting.
   * \param[in]     certificate_authorities The certificate authority names to check.
   * \param[in]     error_code              The error code to use if the validation fails.
   * \return        True if all certificate authorities are valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateCertificateAuthorities(common::TlsCommFlowContext& cxt,
                                             ara::core::Vector<DistinguishedName> const& certificate_authorities,
                                             TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief         Validates a single distinguished name.
   *
   * \param[in,out] cxt                The flow context to use for error reporting.
   * \param[in]     distinguished_name The distinguished name to check.
   * \param[in]     error_code         The error code to use if the validation fails.
   * \return        True if the distinguished name is valid; false otherwise.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     TRUE
   */
  static auto ValidateDistinguishedName(common::TlsCommFlowContext& cxt, DistinguishedName const& distinguished_name,
                                        TlsCommErrorCode error_code) noexcept -> bool;

  /*!
   * \brief     Calculates the length of a list of distinguished names in bytes (including size fields).
   *
   * \param[in] distinguished_names The list of distinguished names to measure.
   * \return    The number of bytes occupied by the names.
   *
   * \pre       -
   * \context   ANY
   * \reentrant TRUE
   */
  static auto GetDistinguishedNamesTotalSizeInBytes(
      ara::core::Vector<DistinguishedName> const& distinguished_names) noexcept -> std::size_t;

  /*! \brief Used to access private data members. */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);

  /*! \brief Used to access private data members. */
  FRIEND_TEST(UT__MessageManager, Cleanup);

  /*! \brief Used to access the GetDistinguishedNamesTotalSizeInBytes function. */
  FRIEND_TEST(Cdd__CertificateRequestMessage__Parameterized, SerializeDeserializeRoundtrip);

  /*! \brief Used to access the GetDistinguishedNamesTotalSizeInBytes function. */
  FRIEND_TEST(UT__CertificateRequestMessage__Parameterized, SerializeDeserializeRoundtrip);

  /*! \brief Used to access non-public data members. */
  FRIEND_TEST(Cdd__CertificateRequestMessage, DefaultConstructor);

  /*! \brief Used to access non-public data members. */
  FRIEND_TEST(UT__CertificateRequestMessage, DefaultConstructor);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_REQUEST_MESSAGE_H_
