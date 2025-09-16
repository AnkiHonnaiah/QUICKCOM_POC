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
/*!        \file  provider_manager.h
 *        \brief  Registry for keeping track of already loaded providers.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROVIDER_MANAGER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROVIDER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>

#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "ara/crypto/x509/x509_provider.h"

namespace crypto {
namespace client {

/*!
 * \brief Registry for keeping track of already loaded providers.
 * \vprivate Component Private
 */
class ProviderManager final {
 public:
  /*!
   * \brief Constructor - creates the instance of ProviderManager.
   * \param[in] ksp Key Storage Provider.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit ProviderManager(std::shared_ptr<ara::crypto::keys::KeyStorageProvider> ksp) noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~ProviderManager() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ProviderManager(ProviderManager const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ProviderManager(ProviderManager&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  ProviderManager& operator=(ProviderManager const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  ProviderManager& operator=(ProviderManager&& /*other*/) & noexcept = default;

  /*!
   * \brief Registers a crypto provider under given UID.
   * \param[in] cp_uid The crypto provider UID.
   * \param[in] crypto_provider The crypto provider to register.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RegisterCryptoProvider(ara::crypto::CryptoProviderUid const& cp_uid,
                              std::weak_ptr<ara::crypto::cryp::CryptoProvider> crypto_provider) noexcept;

  /*!
   * \brief Returns a crypto provider for a given crypto provider UID.
   * \param[in] cp_uid The crypto provider UID.
   * \return The crypto provider for given UID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::shared_ptr<ara::crypto::cryp::CryptoProvider> GetCryptoProvider(
      ara::crypto::CryptoProviderUid const& cp_uid) noexcept;

  /*!
   * \brief Returns the registered key storage provider.
   * \return The key storage provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::shared_ptr<ara::crypto::keys::KeyStorageProvider> GetKeyStorageProvider() const noexcept;

 private:
  std::map<ara::crypto::CryptoProviderUid, std::weak_ptr<ara::crypto::cryp::CryptoProvider>>
      crypto_providers_; /*!< Already loaded crypto providers */
  std::shared_ptr<ara::crypto::keys::KeyStorageProvider>
      key_storage_provider_;                                     /*!< Already loaded key storage provider */
  std::weak_ptr<ara::crypto::x509::X509Provider> x509_provider_; /*!< Already loaded x509 provider */
};

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROVIDER_MANAGER_H_
