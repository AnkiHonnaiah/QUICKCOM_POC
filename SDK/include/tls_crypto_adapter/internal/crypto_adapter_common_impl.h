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
/*!        \file crypto_adapter_common_impl.h
 *        \brief Class declaration of crypto adapter common implementation.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_IMPL_H_

#include "crypto_adapter_common_interface.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Crypto Adapter Common implementation.
 * \vprivate This class is Vector component internal API
 */
class CryptoAdapterCommonImpl : public CryptoAdapterCommonInterface {
 public:
  /*! \brief Default constructor. */
  CryptoAdapterCommonImpl() = default;

  CryptoAdapterCommonImpl(CryptoAdapterCommonImpl const&) = delete;

  CryptoAdapterCommonImpl(CryptoAdapterCommonImpl&&) = delete;

  /*! \brief Default destructor. */
  ~CryptoAdapterCommonImpl() final = default;

  CryptoAdapterCommonImpl& operator=(CryptoAdapterCommonImpl const&) = delete;

  CryptoAdapterCommonImpl& operator=(CryptoAdapterCommonImpl&&) = delete;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface::CreateHash
   */
  amsr::core::Result<CryptoAdapterInterface::HashInterfaceUptr, CryptoAdapterErrorCode> CreateHash(
      EnHashAlgorithm algorithm, ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface::CreateRng
   */
  core::Result<CryptoAdapterInterface::RngInterfaceUptr, CryptoAdapterErrorCode> CreateRng(
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept final;

  /*!
   * \copydoc amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface::DerivePreMasterSecretECDHE
   */
  amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> DerivePreMasterSecretECDHE(
      ara::crypto::cryp::PrivateKey::Uptrc own_private_key, ara::crypto::ReadOnlyMemRegion partner_public_key,
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface::GenerateMasterSecret
   */
  amsr::core::Result<void, CryptoAdapterErrorCode> GenerateMasterSecret(
      ara::crypto::cryp::CryptoProvider::Sptr const crypto_provider,
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView seed,
      MasterSecretContainer& destination, EnPrfAlgorithm algorithm) noexcept final;

  /*!
   * \copydoc ::amsr::tls_crypto_adapter::internal::CryptoAdapterCommonInterface::CreateSignatureVerifier
   */
  amsr::core::Result<CryptoAdapterInterface::SignatureVerifierInterfaceUptr, CryptoAdapterErrorCode>
  CreateSignatureVerifier(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                          ara::crypto::cryp::PublicKey::Uptrc public_key) const noexcept final;

 private:
  /*!
   * \brief Derives a master secret key from a pre-shared key and given seed.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \param[in] pre_master_secret A pre-shared key used for key derivation.
   * \param[in] seed A seed used during key derivation.
   * \param[in] algorithm The hash algorithm to be used.
   * \return A derived key representing the master secret.
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  static amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> DeriveMasterSecretKey(
      ara::crypto::cryp::CryptoProvider::Sptr const crypto_provider,
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView const seed,
      EnPrfAlgorithm algorithm) noexcept;

  /*!
   * \brief Exports the master secret from the remote to a local destination buffer.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \param[in] masterSecretKey The key representing the master secret.
   * \param[in,out] destination A destination buffer to store exported key material.
   * \return On success returns "kCryptoAdapterErrOk", on failure returns "kCryptoProviderException".
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  amsr::core::Result<void, CryptoAdapterErrorCode> ExportMasterSecretKey(
      ara::crypto::cryp::CryptoProvider::Sptr const crypto_provider,
      ara::crypto::cryp::SymmetricKey::Uptrc masterSecretKey, MasterSecretContainer& destination) const noexcept;

  /*! \brief Friend test declaration for DeriveMasterSecret */
  FRIEND_TEST(Cdd__CryptoAdapter, DeriveMasterSecret_BadKey);

  /*! \brief Friend test declaration for DeriveMasterSecret */
  FRIEND_TEST(Cdd__CryptoAdapter, DeriveMasterSecretKeyError);

  /*! \brief Friend test declaration for ExportMasterSecret */
  FRIEND_TEST(Cdd__CryptoAdapter, ExportMasterSecret_BadKey);

  /*! \brief Friend test declaration for ExportMasterSecret */
  FRIEND_TEST(Cdd__CryptoAdapter, ExportMasterSecretKeyError);

  /*! \brief Friend test declaration for ExportMasterSecret */
  FRIEND_TEST(UT__CryptoAdapterCommonImpl, ExportMasterSecret_BadKey);

  /*! \brief Friend test declaration for ExportMasterSecret */
  FRIEND_TEST(UT__CryptoAdapterCommonImpl, DeriveMasterSecret_BadKey);

  /*! \brief Friend test declaration for ExportMasterSecret */
  FRIEND_TEST(UT__CryptoAdapterCommonImpl__DeathTest, ExportMasterSecret_NullCryptoProvider);
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_IMPL_H_
