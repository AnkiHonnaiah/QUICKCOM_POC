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
/*!        \file remote_crypto_adapter.h
 *        \brief Class declaration of remote crypto adapter.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_CRYPTO_ADAPTER_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_CRYPTO_ADAPTER_H_

#include <memory>
#include <utility>

#include "ara/crypto/common/guid.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "tls_crypto_adapter/internal/crypto_adapter_common_impl.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"
#include "tls_crypto_adapter/internal/ed25519_signature_generator.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief RemoteCryptoAdapeter provides cryptographic functionality via a remote CryptoDaemon.
 * \trace DSGN-SecCom-CryptoModes
 */
class RemoteCryptoAdapter final : public CryptoAdapterInterface {
 public:
  /*!
   * \brief Constructs a new Crypto Adapter implementation object, automatically setup crypto stack.
   * \param[in] trustzone_provider_uuid UUID of the trustzone provider.
   * \param[in] ksp A shared pointer to the key storage provider used to load in keys.
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit RemoteCryptoAdapter(ara::crypto::Uuid const& trustzone_provider_uuid,
                               ara::crypto::keys::KeyStorageProvider::Sptr ksp = nullptr);

  /*!
   * \brief Constructs a new Crypto Adapter implementation object
   * \param[in] crypto_provider A pointer to the crypto provider interface.
   * \param[in] ksp A shared pointer to the key storage provider used to load in keys.
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit RemoteCryptoAdapter(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                               ara::crypto::keys::KeyStorageProvider::Sptr ksp = nullptr);

  RemoteCryptoAdapter(RemoteCryptoAdapter const&) = delete;

  RemoteCryptoAdapter(RemoteCryptoAdapter&&) = delete;

  /*! \brief Default destructor. */
  ~RemoteCryptoAdapter() noexcept final = default;

  RemoteCryptoAdapter& operator=(RemoteCryptoAdapter const&) = delete;

  RemoteCryptoAdapter& operator=(RemoteCryptoAdapter&&) = delete;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateHash
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied algorithm is not supported.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<HashInterfaceUptr, CryptoAdapterErrorCode> CreateHash(EnHashAlgorithm algorithm) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateMacGenerator
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied MAC algorithm is not supported.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<MacGeneratorInterfaceUptr, CryptoAdapterErrorCode> CreateMacGenerator(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateMacVerifier
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied MAC algorithm is
   *        not supported.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<MacVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateMacVerifier(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreatePrf
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied PRF algorithm is not either
   *        SHA256 or SHA384 or if the algorithm ID for the TLS PRF algorithm or master secret cannot be resolved.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<PrfInterfaceUptr, CryptoAdapterErrorCode> CreatePrf(EnPrfAlgorithm algorithm,
                                                                   ReadOnlyBufferView secret) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateEncryptor
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied cipher algorithm is
   *        not supported.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<EncryptorInterfaceUptr, CryptoAdapterErrorCode> CreateEncryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateDecryptor
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the supplied cipher algorithm is
   *        not supported.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<DecryptorInterfaceUptr, CryptoAdapterErrorCode> CreateDecryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateRng
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the current PRF algorithm is not
   *        "EnPrfAlgorithm::kTlsPrfSha256" or if current algorithm ID is undefined.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  core::Result<RngInterfaceUptr, CryptoAdapterErrorCode> CreateRng() const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::GenerateMasterSecret
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kPskIdentityNotFound" is returned if the slot UID cannot be found
   *        with the supplied key identity.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the current algorithm ID of the
   *        "TlsPrf" or master secret name is undefined.
   * \error "EnCryptoAdapterErrors::kRuntimeError" is returned if the generation of the master
   *        secret has failed.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<void, CryptoAdapterErrorCode> GenerateMasterSecret(
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView seed,
      MasterSecretContainer& destination, EnPrfAlgorithm algorithm) noexcept final;

  /*!
   * \brief Imports a symmetric key to the remote crypto provider.
   * \param[in] targetAlgId The target algorithm for the imported key.
   * \param[in] raw The raw key data.
   * \return A symmetric key or any error from the crypto provider.
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> ImportSymmetricKeyFromRaw(
      ara::crypto::cryp::CryptoProvider::AlgId const targetAlgId, ReadOnlyBufferView raw) const noexcept;

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
  LoadPreSharedKey(ara::crypto::Uuid const& uuid) final;

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
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterInterface::CreateSignatureVerifier
   * \vprivate This function is Vector component internal API
   */
  amsr::core::Result<SignatureVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateSignatureVerifier(
      ara::crypto::cryp::PublicKey::Uptrc public_key) const noexcept final;

 private:
  /*!
   * \brief The Trustzone UUID used for loading keys.
   */
  ara::crypto::CryptoProviderUid trustzone_uuid_;

  /*!
   * \brief Shared smart pointer to the local provider interface.
   */
  ara::crypto::cryp::CryptoProvider::Sptr const crypto_provider_;

  /*! \brief A shared pointer to the key storage provider */
  ara::crypto::keys::KeyStorageProvider::Sptr const ksp_;

  /*! \brief Unique pointer wrapping a CryptoAdapterCommonInterface */
  std::unique_ptr<amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface> adapter_common_{
      std::make_unique<amsr::tls_crypto_adapter::internal::CryptoAdapterCommonImpl>()};
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_CRYPTO_ADAPTER_H_
