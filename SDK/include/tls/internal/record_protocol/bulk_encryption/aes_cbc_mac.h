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
/*!          \file    aes_cbc_mac.h
 *          \brief    This file contains the class declaration for the bulk encryption with AesCbcMac
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_CBC_MAC_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_CBC_MAC_H_

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
 *          Mode: CBC
 *          Hash: Secure Hash Algorithm 256 (SHA256)
 * \vprivate This class is component private.
 */
class AesCbcMac final : public BulkEncryptionInterface {
 public:
  /*!
   * \brief Constructor
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] cipher_alg The cipher algorithm.
   * \param[in] mac_alg The MAC algorithm.
   */
  AesCbcMac(tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
            tls_crypto_adapter::internal::EnCipherAlgorithm const cipher_alg,
            tls_crypto_adapter::internal::EnMacAlgorithm const mac_alg) noexcept;
  /*! \brief Default Destructor. */
  ~AesCbcMac() final = default;
  /*! \brief Not copy constructible. */
  AesCbcMac(AesCbcMac const &) = delete;
  /*! \brief Not copy assignable. */
  AesCbcMac &operator=(AesCbcMac const &) = delete;
  /*! \brief Not move constructible. */
  AesCbcMac(AesCbcMac &&) = delete;
  /*! \brief Not move assignable. */
  AesCbcMac &operator=(AesCbcMac &&) = delete;

  /*!
   * \brief Creates a BulkEncryptionInterface with AesCbcMac.
   * \return The created encryptor.
   * \param[in] crypto_adapter The crypto adapter.
   * \param[in] cipher_alg The cipher algorithm.
   * \param[in] mac_alg The MAC algorithm.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  static std::unique_ptr<BulkEncryptionInterface> Create(
      tls_crypto_adapter::internal::CryptoAdapterInterface const &crypto_adapter,
      tls_crypto_adapter::internal::EnCipherAlgorithm const cipher_alg,
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

  /*!
   * \brief The MAC algorithm to use for this instance.
   */
  tls_crypto_adapter::internal::EnMacAlgorithm const mac_alg_;
  /*! \brief The pointer to the encryption interface */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::EncryptorInterfaceUptr encryptor_;
  /*! \brief The pointer to the decryption interface */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::DecryptorInterfaceUptr decryptor_;
  /*! \brief Unique pointer to a MAC generator context */
  tls_crypto_adapter::internal::CryptoAdapterInterface::MacGeneratorInterfaceUptr mac_generator_;
  /*! \brief Unique pointer to a MAC verificator context */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::MacVerifierInterfaceUptr mac_verifier_;
  /*! \brief Unique pointer to a RNG  context */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::RngInterfaceUptr rng_;
  /*!
   * \brief Verifies MAC attached to ciphertext.
   * \param[in] cipher_text ciphertext.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac verification
   */
  amsr::core::Result<void> VerifyMac(CipherText &cipher_text, SecurityParameters const &sp) noexcept;
  /*!
   * \brief Performs mac verification if EncryptThenMac, otherwise nothing.
   * \param[in] cipher_text ciphertext.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac verification
   */
  amsr::core::Result<void> StartVerifyMac(CipherText &cipher_text, SecurityParameters const &sp) noexcept;
  /*!
   * \brief Performs mac verification if MacThenEncrypt, otherwise nothing.
   * \param[in] cipher_text ciphertext.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac verification
   */
  amsr::core::Result<void> EndVerifyMac(CipherText &cipher_text, SecurityParameters const &sp) noexcept;
  /*!
   * \brief Decrypts CipherText using configured decryptor.
   * \param[in] cipher_text ciphertext.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful decryption
   */
  amsr::core::Result<void> DecryptCipherText(CipherText &cipher_text, SecurityParameters const &sp) const noexcept;
  /*!
   * \brief Performs mac generation if MacThenEncrypt, otherwise nothing.
   * \param[in] comp_text compressed text.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac generation
   */
  amsr::core::Result<void> StartGenerateMac(CompressedText &comp_text, SecurityParameters const &sp) const noexcept;
  /*!
   * \brief Performs mac generation if EncryptThenMac, otherwise nothing.
   * \param[in] comp_text compressed text.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac generation
   */
  amsr::core::Result<void> EndGenerateMac(CompressedText &comp_text, SecurityParameters const &sp) const noexcept;

  /*!
   * \brief Generates a MAC for the compressedtext.
   * \param[in] comp_text compressed text.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> if sucessful mac generation
   */
  amsr::core::Result<void> GenerateMac(CompressedText &comp_text, SecurityParameters const &sp) const noexcept;
  /*!
   * \brief Performs encryption of compressed text using configured encryptor.
   * \param[in] comp_text compressed text.
   * \param[in] sp security parameters config.
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \return result<void> on sucessful encryption
   */
  amsr::core::Result<void> EncryptCompressedText(CompressedText &comp_text, SecurityParameters const &sp) noexcept;
};

}  // namespace bulk_encryption
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_AES_CBC_MAC_H_
