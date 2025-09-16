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
/*!       \file   certificate_verify_message.h
 *        \brief  Class describing a TLS/DTLS Certificate Verify message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_VERIFY_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_VERIFY_MESSAGE_H_

#include <cstdint>

#include "ara/core/vector.h"
#include "tls/internal/messages/common_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Certificate Verify message
 * \trace SPEC-6561961
 * \vprivate This class is component private.
 */
class CertificateVerifyMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief The maximum number of bytes of a signature, which is (2^16)-1.
   */
  static constexpr std::uint32_t kCertificateVerifySignatureMaxLength{65'535U};

  /*!
   * \brief Creates an empty CertificateVerify message.
   */
  CertificateVerifyMessage() noexcept;

  /*!
   * \brief Default destructor.
   */
  ~CertificateVerifyMessage() final = default;

  CertificateVerifyMessage(CertificateVerifyMessage const&) noexcept = delete;
  CertificateVerifyMessage(CertificateVerifyMessage&&) noexcept = delete;

  auto operator=(CertificateVerifyMessage const&) noexcept -> CertificateVerifyMessage& = delete;
  auto operator=(CertificateVerifyMessage&&) noexcept -> CertificateVerifyMessage& = delete;

  /*!
   * \brief Clears the content of this message.
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
   * \brief     Sets the hash algorithm associated with the message.
   *
   * \param[in] hash_algorithm The new hash algorithm.
   */
  void SetHashAlgorithm(HashAlgorithm hash_algorithm) noexcept;

  /*!
   * \brief     Sets the signature algorithm associated with the message.
   *
   * \param[in] signature_algorithm The new signature algorithm.
   */
  void SetSignatureAlgorithm(SignatureAlgorithm signature_algorithm) noexcept;

  /*!
   * \brief     Sets the signature associated with the message.
   *
   * \param[in] signature The new signature.
   */
  void SetSignature(ara::core::Vector<std::uint8_t> signature) noexcept;

  /*!
   * \brief  Returns the associated hash algorithm.
   *
   * \return A hash algorithm identifier.
   */
  auto GetHashAlgorithm() const noexcept -> HashAlgorithm;

  /*!
   * \brief  Returns the associated signature algorithm.
   *
   * \return A signature algorithm identifier.
   */
  auto GetSignatureAlgorithm() const noexcept -> SignatureAlgorithm;

  /*!
   * \brief  Returns the signature associated with this message.
   *
   * \return A signature.
   */
  auto GetSignature() const noexcept -> ara::core::Vector<std::uint8_t> const&;

 private:
  /*! \brief The used hash algorithm. */
  HashAlgorithm hash_algorithm_{HashAlgorithm::kNone};

  /*! \brief The used signature algorithm. */
  SignatureAlgorithm signature_algorithm_{SignatureAlgorithm::kAnonymous};

  /*! \brief The associated signature. */
  ara::core::Vector<std::uint8_t> signature_{};

  /*! \brief Used to access non-public data members. */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);

  /*! \brief Used to access non-public data members. */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CERTIFICATE_VERIFY_MESSAGE_MESSAGE_H_
