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
/*!          \file    null_mac.h
 *          \brief    This file contains the class declaration for the bulk encryption with "NullMac".
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_MAC_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_MAC_H_

#include <memory>

#include "tls/internal/record_protocol/bulk_encryption/bulk_encryption_interface.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace bulk_encryption {

/*!
 * \brief This classification of bulk encryption provides authentication but no encryption.
 * \details Encryption: NULL
 *          Hash: Secure Hash Algorithm 256 (SHA256)
 * \vprivate This method is component private.
 */
class NullMac final : public BulkEncryptionInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] mac_alg The MAC algorithm.
   */
  NullMac(tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
          tls_crypto_adapter::internal::EnMacAlgorithm const mac_alg) noexcept;
  /*! \brief Default Destructor. */
  ~NullMac() final = default;
  /*! \brief Not copy constructible. */
  NullMac(NullMac const &) = delete;
  /*! \brief Not copy assignable. */
  NullMac &operator=(NullMac const &) = delete;
  /*! \brief Not move constructible. */
  NullMac(NullMac &&) = delete;
  /*! \brief Not move assignable. */
  NullMac &operator=(NullMac &&) = delete;

  /*!
   * \brief Creates a NullMac bulk encryptor.
   * \return The created encryptor.
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] mac_alg The MAC algorithm.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  static std::unique_ptr<BulkEncryptionInterface> Create(
      tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
      tls_crypto_adapter::internal::EnMacAlgorithm const mac_alg) noexcept;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::InitializeEncryptor
   * \vprivate This method is component private.
   */
  amsr::core::Result<void> InitializeEncryptor(SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::InitializeDecryptor
   * \vprivate This method is component private.
   */
  amsr::core::Result<void> InitializeDecryptor(SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::Decrypt
   * \details The returning compressed text will take ownership over the payload in the passed cipher text upon
   * successful MAC verification.
   * \vprivate This method is component private.
   */
  CompressedTextUptr Decrypt(CipherTextUptr &&cipher_text, SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::Encrypt
   * \details The returning cipher text will take ownership over the payload in the passed compressed text upon
   * successful MAC generation.
   * \trace SPEC-6493113, SPEC-6493114
   * \vprivate This method is component private.
   */
  CipherTextUptr Encrypt(CompressedTextUptr &&comp_text, SecurityParameters const &sp) noexcept final;

 private:
  /*!
   * \brief A reference to the crypto adapter.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter_;

  /*!
   * \brief The MAC algorithm to use for this instance.
   */
  tls_crypto_adapter::internal::EnMacAlgorithm const mac_alg_;
  /*! \brief Unique pointer to a MAC generator context */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::MacGeneratorInterfaceUptr mac_generator_;
  /*! \brief Unique pointer to a MAC verificator context */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::MacVerifierInterfaceUptr mac_verifier_;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_AutosarC++17_10-M7.1.2_parameter_is_unique_pointer
  /*!
   * \brief Get the MAC verification tag from cipher text and finish verification
   * \param[in] cipher_text The cipher text to be decrypted
   * \param[in] verification_tag The tag to verify against.
   * \param[in] start_of_mac The size to indicate the size of payload after extracting the MAC verification tag
   * \return A unique pointer wrapping the new TLS CompressedText object
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \return  Compressed text or nullptr.
   * \vprivate
   */
  CompressedTextUptr FinishVerification(CipherTextUptr &&cipher_text, ReadOnlyBufferView const verification_tag,
                                        std::size_t const start_of_mac) const noexcept;
};

}  // namespace bulk_encryption
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_MAC_H_
