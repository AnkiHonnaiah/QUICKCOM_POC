/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  signature_generator_impl.h
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_SIGNATURE_GENERATOR_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_SIGNATURE_GENERATOR_IMPL_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "tls_crypto_adapter/internal/ed25519_signature_generator.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Helper function to load in a Private Key.
 * \param[in] crypto_provider A pointer to the crypto provider.
 * \param[in] ksp A pointer to the key storage provider.
 * \param[in] slot_number Slot number indicating key slots.
 * \return A private key.
 */
ara::crypto::cryp::PrivateKey::Uptrc LoadPrivateKey(ara::crypto::cryp::CryptoProvider *crypto_provider,
                                                    ara::crypto::keys::KeyStorageProvider *ksp,
                                                    ara::crypto::keys::SlotNumber const &slot_number) noexcept;

/*!
 * \brief  Helper function to create a SignerPrivateCtx and PrivateKey pair
 * \param[in] crypto_provider A pointer to the crypto provider.
 * \param[in] ksp A pointer to the key storage provider.
 * \param[in] slot_number A const reference to the Slot number indicating key slots.
 * \return std::pair with SignerPrivateCtx and PrivateKey.
 */
std::pair<ara::crypto::cryp::SignerPrivateCtx::Uptr, ara::crypto::cryp::PrivateKey::Uptrc>
CreateSignerAndPrivateKeyPair(ara::crypto::cryp::CryptoProvider *crypto_provider,
                              ara::crypto::keys::KeyStorageProvider *ksp,
                              ara::crypto::keys::SlotNumber const &slot_number) noexcept;

/*!
 * \brief Get the crypto provider and slot number.
 * \param[in] key_uuid A string view for the UUID of the key.
 * \param[in] ksp A pointer to the key storage provider.
 * \param[out] slot_number A const reference to the private key slot related to the passed uuid.
 * \return The CryptoProvider.
 */
ara::crypto::cryp::CryptoProvider::Sptr ObtainCryptoProvider(amsr::core::StringView const &key_uuid,
                                                             ara::crypto::keys::KeyStorageProvider const *ksp,
                                                             ara::crypto::keys::SlotNumber &slot_number) noexcept;

/*!
 * \brief Create an instance for a Ed25519SignatureGenerator.
 * \param[in] key_uuid A const reference of string view for the UUID of the key.
 * \param[in] ksp A pointer to the key storage provider.
 * \return The generator returned by its interface type.
 */
amsr::core::Result<CryptoAdapterInterface::SignatureGeneratorInterfaceUptr, CryptoAdapterErrorCode>
CreateEd25519SignatureGenerator(amsr::core::StringView const &key_uuid,
                                ara::crypto::keys::KeyStorageProvider *ksp) noexcept;

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr
#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_SIGNATURE_GENERATOR_IMPL_H_
