/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!          \file    aes_gcm_mac.h
 *          \brief    This file contains the class declaration for the bulk encryption with AesGcmMac
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_GCM_MAC_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_GCM_MAC_H_

#include <memory>

#include "tls/internal/record_protocol/bulk_encryption/bulk_encryption_interface.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace bulk_encryption {

/*!
 * \brief This classification of bulk encryption provides both authentication and encryption.
 * \details Encryption: Advanced Encryption Standard
 *          Hash: Secure Hash Algorithm 256 (SHA256)
 * \vprivate This class is component private.
 */
class AesGcmMac final : public BulkEncryptionInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] cipher_alg The cipher algorithm.
   */
  AesGcmMac(tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
            tls_crypto_adapter::internal::EnCipherAlgorithm const cipher_alg) noexcept;
  /*! \brief Default Destructor. */
  ~AesGcmMac() noexcept final = default;
  /*! \brief Not copy constructible. */
  AesGcmMac(AesGcmMac const &) = delete;
  /*! \brief Not copy assignable. */
  AesGcmMac &operator=(AesGcmMac const &) = delete;
  /*! \brief Not move constructible. */
  AesGcmMac(AesGcmMac &&) = delete;
  /*! \brief Not move assignable. */
  AesGcmMac &operator=(AesGcmMac &&) = delete;

  /*!
   * \brief Creates a AesGcmMac bulk encryptor.
   * \return The created encryptor.
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] cipher_alg The cipher algorithm.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  static std::unique_ptr<BulkEncryptionInterface> Create(
      tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
      tls_crypto_adapter::internal::EnCipherAlgorithm const cipher_alg) noexcept;

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
   * successful decryption.
   * \trace SPEC-6562038
   * \vprivate This method is component private.
   */
  CompressedTextUptr Decrypt(CipherTextUptr &&cipher_text, SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::Encrypt
   * \details The returning cipher text will take ownership over the payload in the passed compressed text upon
   * successful encryption.
   * \trace SPEC-6493119, SPEC-6493124, SPEC-6562038
   * \vprivate This method is component private.
   */
  CipherTextUptr Encrypt(CompressedTextUptr &&comp_text, SecurityParameters const &sp) noexcept final;

 private:
  /*!
   * \brief A reference to the crypto adapter.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter_;

  /*!
   * \brief The cipher algorithm to use for this instance.
   */
  tls_crypto_adapter::internal::EnCipherAlgorithm const cipher_alg_;

  /*! \brief The pointer to the encryption interface */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::EncryptorInterfaceUptr encryptor_;
  /*! \brief The pointer to the decryption interface */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::DecryptorInterfaceUptr decryptor_;
};

}  // namespace bulk_encryption
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_GCM_MAC_H_
