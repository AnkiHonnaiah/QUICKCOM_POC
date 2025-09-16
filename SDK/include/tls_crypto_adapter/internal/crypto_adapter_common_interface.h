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
/*!        \file crypto_adapter_common_interface.h
 *        \brief Class declaration of crypto adapter common interface.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_INTERFACE_H_

#include <utility>
#include "amsr/core/abort.h"
#include "crypto/common/constants.h"
#include "crypto/softwareprovider/constants.h"
#include "crypto/softwareprovider/cryp/internal/symmetric_key_impl.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Interface for Crypto Adapter Common.
 */
class CryptoAdapterCommonInterface {
 public:
  /*! \brief Default constructor. */
  CryptoAdapterCommonInterface() = default;

  /*! \brief Not copy constructible. */
  CryptoAdapterCommonInterface(CryptoAdapterCommonInterface const &) = delete;

  /*! \brief Not move constructible. */
  CryptoAdapterCommonInterface(CryptoAdapterCommonInterface &&) = delete;

  /*! \brief Not copy assignable. */
  CryptoAdapterCommonInterface &operator=(CryptoAdapterCommonInterface const &) = delete;

  /*! \brief Not move assignable. */
  CryptoAdapterCommonInterface &operator=(CryptoAdapterCommonInterface &&) = delete;

  /*! \brief Virtual default destructor. */
  virtual ~CryptoAdapterCommonInterface() = default;

  /*!
   * \brief A factory method that creates a hash function context.
   * \param[in] algorithm The hash algorithm to be used.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \return A unique pointer to the newly created hash function context or an error code.
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the supplied algorithm is not supported
   * \context ANY
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::Result<CryptoAdapterInterface::HashInterfaceUptr, CryptoAdapterErrorCode> CreateHash(
      EnHashAlgorithm algorithm, ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept = 0;

  /*!
   * \brief Factory method that creates the RNG context.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \return A unique pointer to the newly created RNG context or an error code.
   * \vprivate This function is Vector component internal API.
   * \error "EnCryptoAdapterErrors::kUnsupportedAlgorithm" is returned if the current PRF algorithm is not
   *        "EnPrfAlgorithm::kTlsPrfSha256" or if current algorithm ID is undefined.
   * \context ANY
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual core::Result<CryptoAdapterInterface::RngInterfaceUptr, CryptoAdapterErrorCode> CreateRng(
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) const noexcept = 0;

  /*!
   * \brief Loads a pre-master key from the KeyStorageProvider for our CryptoProvider when the algorithm is
   *        Diffie-Hellman.
   * \param[in] own_private_key Our own private key which is used for generating pre-master secret.
   * \param[in] partner_public_key The client's public key which is used for generating pre-master secret.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \return Pre-master secret or the related crypto error code.
   * \vprivate This function is Vector component internal API.
   * \context ANY
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> DerivePreMasterSecretECDHE(
      ara::crypto::cryp::PrivateKey::Uptrc own_private_key, ara::crypto::ReadOnlyMemRegion partner_public_key,
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) noexcept = 0;

  /*!
   * \brief Generates the master secret.
   * \param[in] crypto_provider A shared pointer to the crypto provider.
   * \param[in] pre_master_secret Used to generate the master secret key.
   * \param[in] seed Seeding data.
   * \param[out] destination Reference to the destination buffer to fill with the generated master secret.
   * \param[in] algorithm The hash algorithm to be used.
   * \return On success returns "kCryptoAdapterErrOk", on failure returns "kCryptoProviderException".
   * \vprivate This function is Vector component internal API
   * \error EnCryptoAdapterErrors::kUnsupportedAlgorithm is returned if the current algorithm id of the
   *        TlsPrf or X25519 key agreement name is undefined
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if the generation of the master
   *        secret has failed
   * \context ANY
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::Result<void, CryptoAdapterErrorCode> GenerateMasterSecret(
      ara::crypto::cryp::CryptoProvider::Sptr const crypto_provider,
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView seed,
      MasterSecretContainer &destination, EnPrfAlgorithm algorithm) noexcept = 0;

  /*!
   * \brief Factory function for creating an object of Ed25519SignatureVerifier type.
   * \param[in] crypto_provider A pointer to the used crypto provider.
   * \param[in] public_key A unique pointer to a constant PublicKey.
   * \context Handshake
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate This function is Vector component internal API
   * \return A base pointer to the created Ed25519SignatureVerifier object.
   */
  virtual amsr::core::Result<CryptoAdapterInterface::SignatureVerifierInterfaceUptr, CryptoAdapterErrorCode>
  CreateSignatureVerifier(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                          ara::crypto::cryp::PublicKey::Uptrc public_key) const noexcept = 0;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_COMMON_INTERFACE_H_
