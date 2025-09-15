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
/*!       \file  performance_crypto_adapter.h
 *        \brief This file contains the declaration of PerformanceCryptoAdapter class, which is implemented and
 *               used in performance_crypto_adapter.cpp
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PERFORMANCE_CRYPTO_ADAPTER_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PERFORMANCE_CRYPTO_ADAPTER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "ara/crypto/common/guid.h"
#include "ara/crypto/cryp/authn_stream_cipher_ctx.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "ara/crypto/cryp/symmetric_key.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_common_impl.h"
#include "tls_crypto_adapter/internal/ed25519_signature_generator.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief Performance Crypto Adapter implementation.
 * \vprivate This class is Vector component internal API
 * \trace DSGN-SecCom-CryptoModes
 */
class PerformanceCryptoAdapter final : public CryptoAdapterInterface {
 public:
  /*!
   * \brief Constructs a new Crypto Adapter Implementation object, automatically setup crypto stack.
   * \param[in] ksp A shared pointer to the key storage provider used to load in keys.
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit PerformanceCryptoAdapter(ara::crypto::keys::KeyStorageProvider::Sptr ksp = nullptr);

  /*!
   * \brief Constructs a new Crypto Adapter implementation object, automatically setup crypto stack.
   * \param[in] trustzone_provider_uuid is the UUID of the trustzone provider
   * \param[in] ksp A shared pointer to the key storage provider used to load in keys.
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit PerformanceCryptoAdapter(ara::crypto::Uuid const& trustzone_provider_uuid,
                                    ara::crypto::keys::KeyStorageProvider::Sptr ksp = nullptr);

  /*! \brief Not copy constructible */
  PerformanceCryptoAdapter(PerformanceCryptoAdapter const&) = delete;

  /*! \brief Not copy assignable */
  PerformanceCryptoAdapter& operator=(PerformanceCryptoAdapter const&) = delete;

  /*! \brief Not move constructible */
  PerformanceCryptoAdapter(PerformanceCryptoAdapter&&) = delete;

  /*! \brief Not move assignable */
  PerformanceCryptoAdapter& operator=(PerformanceCryptoAdapter&&) = delete;

  /*! \brief Destructor to destroy the Crypto Adapter Implementation object. */
  ~PerformanceCryptoAdapter() final = default;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateHash
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied algorithm is not supported
   */
  amsr::core::Result<CryptoAdapterInterface::HashInterfaceUptr, CryptoAdapterErrorCode> CreateHash(
      EnHashAlgorithm algorithm) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateMacGenerator
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied MAC algorithm is not
   *        supported
   */
  amsr::core::Result<CryptoAdapterInterface::MacGeneratorInterfaceUptr, CryptoAdapterErrorCode> CreateMacGenerator(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateMacVerifier
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied MAC algorithm is
   *        not supported
   */
  amsr::core::Result<CryptoAdapterInterface::MacVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateMacVerifier(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateEncryptor
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kInvalidArgument is returned if the algorithm value is invalid.
   * \error EnCryptoAdapterErrors::kRuntimeError if the crypto provider fails any operation.
   */
  amsr::core::Result<CryptoAdapterInterface::EncryptorInterfaceUptr, CryptoAdapterErrorCode> CreateEncryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateDecryptor
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kInvalidArgument is returned if the algorithm value is invalid.
   * \error EnCryptoAdapterErrors::kRuntimeError if the crypto provider fails any operation.
   */
  amsr::core::Result<CryptoAdapterInterface::DecryptorInterfaceUptr, CryptoAdapterErrorCode> CreateDecryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateRng
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the current Prf algorithm is not
   * EnPrfAlgorithm::kTlsPrfSha256 or if current algorithm id is undefined
   */
  amsr::core::Result<CryptoAdapterInterface::RngInterfaceUptr, CryptoAdapterErrorCode> CreateRng() const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreatePrf
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied PRF algorithm is not either
   *        SHA256 or SHA384 or if the Algorithm ID for the TLS PRF algorithm or Master secret could not be resolved
   */
  amsr::core::Result<CryptoAdapterInterface::PrfInterfaceUptr, CryptoAdapterErrorCode> CreatePrf(
      EnPrfAlgorithm algorithm, ReadOnlyBufferView secret) const noexcept final;

  /*!
   * \copydoc amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::LoadPreSharedKey
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, tls_crypto_adapter::internal::CryptoAdapterErrorCode>
  LoadPreSharedKey(ara::crypto::Uuid const& uuid) noexcept final;

  /*!
   * \copydoc amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::DerivePreMasterSecretECDHE
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> DerivePreMasterSecretECDHE(
      ara::crypto::cryp::PrivateKey::Uptrc own_private_key,
      ara::crypto::ReadOnlyMemRegion partner_public_key) noexcept final;

  // VECTOR NC AutosarC++17_10-M9.3.1: MD_libseccom_AutosarC++17_10-M9.3.1_returning_non_const_reference_to_class_data
  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::GetKsp
   */
  ara::crypto::keys::KeyStorageProvider::Sptr GetKsp() const noexcept final { return ksp_; }

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::GenerateX25519KeyPair
   */
  amsr::core::Result<CryptoAdapterInterface::ECDHPrivatePublicKeyPair, CryptoAdapterErrorCode> GenerateX25519KeyPair()
      const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::GenerateMasterSecret
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the current algorithm id of the
   * TlsPrf or X25519 key agreement name is undefined
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if the generation of the master
   *        secret has failed
   */
  amsr::core::Result<void, CryptoAdapterErrorCode> GenerateMasterSecret(
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView seed,
      MasterSecretContainer& destination, EnPrfAlgorithm algorithm) noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateSignatureVerifier
   * \vprivate This function is Vector component internal API
   */
  amsr::core::Result<SignatureVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateSignatureVerifier(
      ara::crypto::cryp::PublicKey::Uptrc public_key) const noexcept final;

 private:
  /*!
   * \brief Internal crypto stack setup.
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  void SetupCryptoStack() const noexcept;

  /*! A constant StringView for the name of AES 128 Key  */
  amsr::core::StringView const kAlgNameAes128Key{"AES-128"};

  /*! A constant StringView for the name of AES 256 Key  */
  amsr::core::StringView const kAlgNameAes256Key{"AES-256"};

  /*! A constant StringView for the name of AES GCM 128 algorithm  */
  amsr::core::StringView const kAlgNameAesGcm128{"GCM/AES-128"};

  /*! A constant StringView for the name of AES GCM 256 algorithm  */
  amsr::core::StringView const kAlgNameAesGcm256{"GCM/AES-256"};

  /*! A constant StringView for the name of AES CBC 128 algorithm  */
  amsr::core::StringView const kAlgNameAesCbc128{"CBC/AES-128"};

  /*! A constant StringView for the name of AES CBC 256 algorithm  */
  ara::core::StringView const kAlgNameAesCbc256{"CBC/AES-256"};

  /*! A constant StringView for the name of HMAC Key  */
  amsr::core::StringView const kAlgNameHmacKey{"KEY_HMAC"};

  /*! A constant StringView for the name of HMAC SHA-256 algorithm  */
  amsr::core::StringView const kAlgNameHmacSha256{"HMAC/SHA-256"};

  /*! A constant StringView for the name of HMAC SHA1 algorithm  */
  amsr::core::StringView const kAlgNameHmacSha1{"HMAC/SHA-1"};

  /*! A constant StringView for the name of SHA-256 algorithm  */
  amsr::core::StringView const kAlgNameSha256{"SHA2-256"};

  /*! A constant StringView for the name of HMAC SHA-384 algorithm  */
  ara::core::StringView const kAlgNameHmacSha384{"HMAC/SHA-384"};

  /*!
   * \brief Creates a symmetric message authentication code context for supported MAC algorithm
   * \details Supports algorithm EnMacAlgorithm::kHmacSha256
   * \param[in] algorithm Target crypto MAC algorithm
   * \return Unique smart pointer encapsulating a symmetric message authentication code context
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied MAC algorithm is not supported
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::MessageAuthnCodeCtx::Uptr, CryptoAdapterErrorCode> CreateMessageAuthnCodeCtx(
      EnMacAlgorithm algorithm) const noexcept;

  /*!
   * \brief Creates a symmetric key from raw key data for supported MAC algorithm
   * \details Supports algorithm EnMacAlgorithm::kHmacSha256
   * \param[in] algorithm Target crypto MAC algorithm
   * \param[in] key_material Raw key data for symmetric key
   * \return Unique smart pointer encapsulating a symmetric key
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied MAC algorithm is not supported
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> CreateLibEsSymmetricKey(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key_material) const noexcept;

  /*!
   * \brief Creates a symmetric authenticated stream Cipher context for supported cipher algorithm
   * \details Supports algorithm EnCipherAlgorithm::kAes128Gcm
   * \param[in] algName Target crypto Cipher algorithm
   * \return Unique smart pointer encapsulating a symmetric authenticated stream Cipher context
   * \error EnCryptoAdapterErrors::kRuntimeError if the crypto provider cannot provide the context.
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::AuthnStreamCipherCtx::Uptr, CryptoAdapterErrorCode> CreateAuthnStreamCipherCtx(
      amsr::core::StringView const& algName) const noexcept;

  /*!
   * \brief Creates a symmetric block stream Cipher context for supported cipher algorithm
   * \details Supports algorithm EnCipherAlgorithm::kAes128Cbc
   * \param[in] algName Target crypto Cipher algorithm
   * \return Unique smart pointer encapsulating a symmetric block stream Cipher context
   * \error EnCryptoAdapterErrors::kRuntimeError if the crypto provider cannot provide the context.
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::StreamCipherCtx::Uptr, CryptoAdapterErrorCode> CreateStreamBlockCipherCtx(
      amsr::core::StringView const& algName) const noexcept;

  /*!
   * \brief Creates a symmetric key from raw key data for supported Cipher algorithm
   * \details Supports for algorithm  EnCipherAlgorithm::kAes128Gcm
   * \param[in] algorithm Target crypto Cipher algorithm
   * \param[in] key_material Raw key data for symmetric key creation
   * \return Unique smart pointer encapsulating the created symmetric key
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied Cipher algorithm is not supported
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> CreateLibEsSymmetricKey(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key_material) const noexcept;

  /*! \brief Universally Unique Identifier (UUID) of the trustzone provider */
  ara::crypto::Uuid trustzone_provider_uuid_;

  /*! \brief Shared smart pointer to the local provider interface */
  ara::crypto::cryp::CryptoProvider::Sptr const local_provider_;

  /*! \brief Shared smart pointer to the trustzone provider interface */
  ara::crypto::cryp::CryptoProvider::Sptr const trustzone_provider_;

  /*! \brief A shared pointer to the key storage provider */
  ara::crypto::keys::KeyStorageProvider::Sptr ksp_;

  /*! \brief Unique pointer wrapping a CryptoAdapterCommonInterface */
  std::unique_ptr<amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface> adapter_common_{
      std::make_unique<amsr::tls_crypto_adapter::internal::CryptoAdapterCommonImpl>()};

  /*! \brief Friend test declaration for PerformanceCryptoAdapter_InvalidArguments */
  FRIEND_TEST(Cdd__CryptoAdapter, PerformanceCryptoAdapter_InvalidArguments);

  /*! \brief Friend test declaration for PerformanceCryptoAdapter_InvalidKey */
  FRIEND_TEST(Cdd__CryptoAdapter, PerformanceCryptoAdapter_InvalidKey);

  /*! \brief Friend test declaration for GenerateMasterSecretFromPSK */
  FRIEND_TEST(Cdd__CryptoAdapter, GenerateMasterSecret);

  /*! \brief Friend test declaration for GetPskIdentity_EmptyId */
  FRIEND_TEST(Cdd__CryptoAdapter, GetPskIdentity_EmptyId);

  /*! \brief Friend test declaration for PerformanceCryptoAdapter */
  FRIEND_TEST(Cdd__CryptoAdapter, PerformanceCryptoAdapter);

  /*! \brief Friend test declaration for SetupCryptoStackWithWrongDefaultRng */
  FRIEND_TEST(Cdd__CryptoAdapter, SetupCryptoStackWithWrongDefaultRng);

  /*! \brief Friend test declaration for PerformanceCryptoAdapter_InvalidArguments */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, PerformanceCryptoAdapter_InvalidArguments);

  /*! \brief Friend test declaration for PerformanceCryptoAdapter_InvalidKey */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, PerformanceCryptoAdapter_InvalidKey);

  /*! \brief Friend test declaration for GenerateMasterSecretFromPSK */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, GenerateMasterSecret);

  /*! \brief Friend test declaration for GetPskIdentity_EmptyId */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, GetPskIdentity_EmptyId);

  /*! \brief Friend test declaration for PerformanceCryptoAdapter */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, PerformanceCryptoAdapter);

  /*! \brief Friend test declaration for SetupCryptoStackWithWrongDefaultRng */
  FRIEND_TEST(UT__PerformanceCryptoAdapter, SetupCryptoStackWithWrongDefaultRng);
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PERFORMANCE_CRYPTO_ADAPTER_H_
