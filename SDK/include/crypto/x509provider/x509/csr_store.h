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
/*!        \file  csr_store.h
 *        \brief  Provides certificate sign request storage.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/map.h"
#include "amsr/core/string.h"
#include "crypto/x509provider/x509/cert_sign_request.h"
#include "crypto/x509provider/x509/certificate.h"
#include "crypto/x509provider/x509/csr_store_entry.h"
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
 * \brief Provides certificate sign request storage.
 * \trace CREQ-Crypto-X509Provider_X509_Certificate_Management
 * \vprivate Component Private
 */
class CsrStore final : public CsrStoreEntryObserver, public PersistencyLoadCallbackHandler {
 public:
  /*!
   * \brief Type alias for a container of CSR store entries.
   */
  using EntryContainer = ara::core::Vector<std::reference_wrapper<CsrStoreEntry const>>;

  // VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static
  /*!
   * \brief Persistency group identifier for new CSRs.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kNewCsrGroupIdentifier{"new"_sv};

  // VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static
  /*!
   * \brief Persistency group identifier for pending CSRs.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kPendingGroupIdentifier{"pending"_sv};

  /*!
   * \brief Item type string identifying CSRs.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kCsrItemType{"csr"_sv};

  /*!
   * \brief Item type string identifying DNs.
   * \vprivate Component Private
   */
  static constexpr amsr::core::StringView const kDnItemType{"dn"_sv};

  /*!
   * \brief Creates an empty CSR store.
   * \param[in] default_crypto_provider Default crypto provider.
   * \param[in] persistency_provider Persistency provider to be used.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit CsrStore(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                    PersistencyProvider& persistency_provider) noexcept;

  /*!
   * \brief Adds the given CSR to the store.
   * \param[in] sign_request The CSR to be added.
   * \param[in] authority_dn Distinguished name of the requested authority.
   * \return A pair of a pointer to the store entry for the given CSR and a boolean that holds true if the
   *         element was added and false if it already existed in the store.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AddCertSignRequest(CertSignRequest::Uptr sign_request, X509DN const& authority_dn) noexcept
      -> std::pair<CsrStoreEntry*, bool>;

  /*!
   * \brief Removes the given CSR store entry.
   * \param[in] csr_store_entry The CSR store entry to be removed.
   * \return True if the CSR store entry was removed successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto RemoveEntry(CsrStoreEntry const& csr_store_entry) noexcept -> bool;

  /*!
   * \brief Tries to find the given CSR entry inside the store.
   * \param[in] csr The CSR to look for.
   * \return A reference to the entry in the store or an error code if none was found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the csr is not found.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto FindEntry(CertSignRequest const& csr) noexcept -> amsr::core::Result<CsrStoreEntry&>;

  /*!
   * \brief Tries to find a CSR store entry matching the given certificate.
   * \details Search criteria are: matching public key, matching subject dn, matching authority dn.
   * \param[in] certificate The certificate to match.
   * \param[in] crypto_provider Crypto provider to use for key matching.
   * \return A reference to the matching entry in the store or an error code if none was found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the csr is not found.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto FindEntryByCertificate(Certificate const& certificate,
                              ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) noexcept
      -> amsr::core::Result<CsrStoreEntry&>;

  /*!
   * \brief Returns the current content of the CSR store.
   * \return Content of this CSR store.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetContent() const noexcept -> EntryContainer;

  /*!
   * \brief Tries to persist the given CSR store entry.
   * \param[in] entry The entry to persist.
   * \return True if persisted successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnPersist(CsrStoreEntry const& entry) const noexcept -> bool final;

  /*!
   * \brief Tries to set the pending state for the given CSR store entry.
   * \param[in] entry The entry for which the pending state shall be set.
   * \return True if pending state set successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnSetPendingState(CsrStoreEntry const& entry) const noexcept -> bool final;

  /*!
   * \brief Callback handling a single item loaded from the persistency provider.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data content of the item.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier Subgroup identifier.
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

 private:
  /*!
   * \brief Calculates the key string for the given CSR.
   * \param[in] csr The CSR for which the key shall be calculated.
   * \return The calculated key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The csr argument must not be accessed by another thread at the same time.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static auto CalculateKey(CertSignRequest const& csr) noexcept -> amsr::core::String;

  /*!
   * \brief Persists all information of single entry.
   * \param[in] entry The entry to persist.
   * \return True if successful, otherwise false.
   * \vprivate Component Private
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto PersistEntry(CsrStoreEntry const& entry) const noexcept -> bool;

  /*!
   * \brief Persists the CSR represented by given entry.
   * \param[in] entry The entry to persist
   * \param[in] group_identifier The group identifier of the entry
   * \return -
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRuntimeFault If the Request could not be persisted
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto PersistCsr(CsrStoreEntry const& entry, amsr::core::StringView group_identifier) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Persists the authority DNs of the entry.
   * \param[in] entry The entry to persist the DNs for
   * \param[in] group_identifier The group identifier of the entry
   * \return -
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRuntimeFault If the DN could not be persisted
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto PersistDns(CsrStoreEntry const& entry, amsr::core::StringView group_identifier) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Parse a DER encoded CSR.
   * \details If parsing succeeded adds the resulting object to \c csrs_
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data content of the item.
   * \param[in] subgroup_identifier Subgroup identifier.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto ParseCsr(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                amsr::core::Span<std::uint8_t const> data, amsr::core::String const& subgroup_identifier) noexcept
      -> void;

  /*!
   * \brief Parse a DER encoded DN.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] data Data content of the item.
   * \param[in] subgroup_identifier Subgroup identifier.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto ParseDn(amsr::core::StringView item_identifier, amsr::core::Span<std::uint8_t const> data,
               amsr::core::String const& subgroup_identifier) noexcept -> void;

  using EntryMap = amsr::core::Map<amsr::core::String, CsrStoreEntry>; /*!< Map for CsrStoreEntry */
  using DnMap = amsr::core::Map<amsr::core::String,
                                amsr::core::Map<amsr::core::String, X509DN::Uptr>>; /*!< Map of CSR keys to pending
                                                                                       authority DNs */

  crypto::common::util::Logger logger_; /*!< Logging instance */
  PersistencyProvider& persistency_;    /*!< Persistency provider */
  EntryMap csrs_;                       /*!< CSR store entry container */
  DnMap pending_dns_;                   /*!< DN container holding authority DNs during CSR load */
  ara::crypto::cryp::CryptoProvider::Sptr default_crypto_provider_; /*!< Default crypto provider. */
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_H_
