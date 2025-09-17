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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  crypto_provider_factory.h
 *        \brief  Factory class that handles the creation of the CryptoProvider for every implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_PROVIDERFACTORY_CRYPTO_PROVIDER_FACTORY_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_PROVIDERFACTORY_CRYPTO_PROVIDER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "ara/crypto/cryp/crypto_provider.h"
#include "crypto/common/constants.h"
#include "crypto/common/keys/key_storage_provider.h"

namespace crypto {
namespace server {

/*!
 * \brief Factory class that maps UUIDs to the correct provider for creation of CryptoProvider and KeyStorageProvider
 * \trace CREQ-Crypto-CryptoProviderManagement
 * \vprivate Component Private
 * \trace DSGN-Crypto-CustomCryptoProviders, DSGN-Crypto-Factory-Pattern
 */
class CryptoProviderFactory {
 public:
  /*!
   * \brief Constructor
   * \param[in] ksp KeyStorageProvider to be used for new registrations
   * \vprivate Component Private
   * \trace DSGN-Crypto-Adapter-Pattern
   */
  explicit CryptoProviderFactory(std::shared_ptr<crypto::common::keys::KeyStorageProvider> ksp);

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~CryptoProviderFactory() noexcept = default;

  /*!
   * \brief Factory function for retrieving known crypto providers.
   * \param[in] uuid UUID corresponding to the crypto provider to be retrieved
   * \return CryptoProvider for the given UUID. If the UUID is not configured, it returns nullptr
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetCryptoProvider(ara::crypto::Uuid const& uuid) const noexcept
      -> ara::crypto::cryp::CryptoProvider::Sptr;

  // VECTOR NC AutosarC++17_10-M9.3.1: MD_CRYPTO_AutosarC++17_10-M9.3.1_shared_ptr
  /*!
   * \brief Gives access to the key storage provider instance.
   * \return A shared pointer to the contained key storage provider instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetKeyStorageProvider() const noexcept -> std::shared_ptr<crypto::common::keys::KeyStorageProvider> {
    return ksp_;
  }

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  CryptoProviderFactory(CryptoProviderFactory const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  CryptoProviderFactory(CryptoProviderFactory&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  CryptoProviderFactory& operator=(CryptoProviderFactory const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  CryptoProviderFactory& operator=(CryptoProviderFactory&& /*other*/) & noexcept = default;

 private:
  /*!
   * \brief Initializes crypto provider factory.
   * \context ANY
   * \pre -
   * \throws std::bad_alloc Memory allocation failed.
   * \exceptionsafety STRONG
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-DefaultCryptoProvider_NoIsolation
   * \vprivate Component Private
   */
  void InitCryptoProvidersInternal() noexcept;

  std::map<ara::crypto::Uuid const, ara::crypto::cryp::CryptoProvider::Sptr>
      crypto_providers_;                                          /*!< Map containing known crypto providers */
  std::shared_ptr<crypto::common::keys::KeyStorageProvider> ksp_; /*!< Key storage provider instance */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_PROVIDERFACTORY_CRYPTO_PROVIDER_FACTORY_H_
