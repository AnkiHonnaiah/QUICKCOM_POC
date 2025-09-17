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
/*!        \file  certificate_store.h
 *        \brief  Provides certificate storage handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/map.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/x509/certificate.h"
#include "crypto/common/util/logger.h"
#include "crypto/x509provider/x509/certificate_store_entry.h"
#include "crypto/x509provider/x509/certificate_store_interface.h"
#include "crypto/x509provider/x509/persistency_load_callback_handler.h"
#include "crypto/x509provider/x509/persistency_provider.h"

namespace crypto {
namespace x509provider {
namespace x509 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Provides certificate storage handling.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class CertificateStore final : public CertificateStoreEntryObserver,
                               public PersistencyLoadCallbackHandler,
                               public CertificateStoreInterface {
 public:
  /*!
   * \brief Type alias for a container of certificate store entries.
   */
  using EntryContainer = ara::core::Vector<std::reference_wrapper<CertificateStoreEntry const>>;

  /*!
   * \brief Persistency group identifier for trusted certificates.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kTrustedGroupIdentifier{"trusted"_sv};

  /*!
   * \brief Persistency group identifier for untrusted certificates.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kUntrustedGroupIdentifier{"untrusted"_sv};

  /*!
   * \brief Item type string identifying certificates.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kCertificateItemType{"cer"_sv};

  /*!
   * \brief Creates an empty certificate store.
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] hash Hash function to be used for fingerprint / key calculation.
   * \param[in] persistency_provider persistency provider to be used.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CertificateStore(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                   ara::crypto::cryp::HashFunctionCtx::Uptr hash, PersistencyProvider& persistency_provider);

  /*!
   * \brief Adds the given certificate to the store.
   * \param[in] certificate The certificate to be added.
   * \param[in] to_volatile Volatile status.
   * \param[in] label The certificate label.
   * \return A pointer to the store entry for the given certificate if the element was added OR a null pointer if an
   *         existing element was persisted.
   * \context ANY
   * \pre -
   * \exceptionsafety BASIC The hash function context state may change.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kContentDuplication if the certificate already exists and cannot be persisted
   * \error ara::crypto::SecurityErrc::kRuntimeFault if the certificate store is volatile but the certificate is not
   * \error ara::crypto::SecurityErrc::kInvalidArgument if the label is not a valid Posix file name
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<CertificateStoreEntry*> AddCertificate(Certificate const& certificate, bool to_volatile = false,
                                                            amsr::core::StringView label = amsr::core::StringView{});

  /*!
   * \brief Removes the given certificate from the store.
   * \param[in] certificate The certificate to be removed.
   * \return True if certificate was removed successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool RemoveCertificate(ara::crypto::x509::Certificate const& certificate) noexcept;

  /*!
   * \brief Remove certificate with specified label from the store.
   * \param[in] label the certificate's label.
   * \return True if certificate was removed successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool RemoveCertificate(amsr::core::StringView label) noexcept;

  /*!
   * \brief Tries to find the given certificate entry inside the store.
   * \param[in] certificate The certificate to look for.
   * \return A reference to the entry in the store or an error code if none was found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the certificate is not found.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto FindEntry(ara::crypto::x509::Certificate const& certificate) noexcept
      -> amsr::core::Result<CertificateStoreEntry&>;

  /*!
   * \brief Tries to find the given certificate entry inside the store.
   * \param[in] label The label of the certificate to look for.
   * \return A reference to the entry in the store or an error code if none was found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the certificate is not found.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto FindEntry(amsr::core::StringView label) const noexcept -> amsr::core::Result<CertificateStoreEntry const&>;

  /*!
   * \brief Find all certificates for an entity by its subject distinguished name.
   * \param[in] subject_dn Subject distinguished name of the entity
   * \return A container of certificate store entries with matching subject distinguished name
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto FindEntries(X509DN const& subject_dn) const noexcept -> EntryContainer;

  /*!
   * \brief Returns the current content of the certificate store
   * \return Content of this certificate store
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetContent() const noexcept -> EntryContainer;

  /*!
   * \brief Checks whether the store contains a CA certificate issued by the given authority.
   * \param[in] authority_dn The authority's distinguished name.
   * \return true if the such a certificate is stored, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto HasCaCertificateForDn(X509DN const& authority_dn) const noexcept -> bool;

  /*!
   * \brief Tries to persist the given certificate store entry.
   * \param[in] entry The entry to persist.
   * \return True if persisted successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnPersist(CertificateStoreEntry const& entry) const noexcept -> bool final;

  /*!
   * \brief Tries to promote the given certificate store entry to the trust anchor group.
   * \param[in] entry The entry to promote.
   * \return True if promoted successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnMakeRootOfTrust(CertificateStoreEntry const& entry) const noexcept -> bool final;

  /*!
   * \brief Callback handling a single item loaded from the persistency provider.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data content of the item.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier Subgroup identifier.
   *
   * \return True to continue load operation, false to abort.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto HandleLoadedItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                        amsr::core::Span<std::uint8_t const> data, amsr::core::StringView item_type,
                        amsr::core::StringView subgroup_identifier) noexcept -> bool final;

  /*!
   * \brief         Determine if given certificate is stored & marked as root of trust.
   * \param[in]     certificate The certificate to check
   * \return        True if \c certificate is root of trust, false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CheckIsRoot(Certificate const& certificate) const noexcept -> bool final;

 private:
  /*!
   * \brief Calculates the key string for the given certificate.
   * \param[in] certificate The certificate for which the key shall be calculated.
   * \return The calculated key.
   * \context ANY
   * \pre -
   * \throws ara::crypto::RuntimeException Failed to encode the X509 certificate to the DER format.
   * \throws ara::crypto::LogicException Failed to create hex string.
   * \exceptionsafety BASIC The hash function context state may change.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto CalculateKey(ara::crypto::x509::Certificate const& certificate) const -> amsr::core::String;

  /*!
   * \brief Certificate store entry map type definition.
   */
  using EntryMap = amsr::core::Map<amsr::core::String, CertificateStoreEntry>;

  crypto::common::util::Logger logger_;               /*!< Logging instance */
  ara::crypto::cryp::HashFunctionCtx::Uptr key_hash_; /*!< Hash function for fingerprint hashing */
  PersistencyProvider& persistency_;                  /*!< Persistency provider */
  EntryMap certificates_;                             /*!< Map containing certificate store entries */
  std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider_; /*!< Default crypto provider */
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_H_
