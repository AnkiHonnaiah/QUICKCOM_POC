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
/*!        \file  certificate_store_entry.h
 *        \brief  Represents a single certificate store entry.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/x509/certificate.h"
#include "crypto/x509provider/x509/certificate.h"
#include "crypto/x509provider/x509/certificate_store_entry_observer.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Represents a single certificate store entry.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class CertificateStoreEntry {
 public:
  /*!
   * \brief Constructs a certificate store entry.
   * \param[in] certificate The certificate to be represented by this entry.
   * \param[in] key The key for this entry.
   * \param[in] observer A reference to an entry observer instance.
   * \param[in] as_volatile Whether this should be created as volatile.
   * \param[in] as_root_of_trust Whether this should be created as trust anchor.
   * \param[in] label Certificate label.
   * \context ANY
   * \pre certificate != nullptr
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CertificateStoreEntry(Certificate::Uptr certificate, amsr::core::String const& key,
                        CertificateStoreEntryObserver const& observer, bool as_volatile = true,
                        bool as_root_of_trust = false,
                        amsr::core::StringView label = amsr::core::StringView{}) noexcept;

  /*!
   * \brief Default destructor.
   * \vprivate Component Private
   */
  virtual ~CertificateStoreEntry() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   * \vprivate Component Private
   */
  CertificateStoreEntry(CertificateStoreEntry const&) = delete;

  /*!
   * \brief Deleted move constructor.
   * \vprivate Component Private
   */
  CertificateStoreEntry(CertificateStoreEntry&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateStoreEntry const&) & noexcept -> CertificateStoreEntry& = delete;

  /*!
   * \brief Deleted move assignment.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateStoreEntry&&) & noexcept -> CertificateStoreEntry& = delete;

  /*!
   * \brief Determines whether this entry is marked as volatile.
   * \return True if this entry is volatile, false if it shall be persistent.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsVolatile() const noexcept;

  /*!
   * \brief Determines whether this entry is marked as root of trust.
   * \return True if this entry is root of trust, otherwise false
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsRootOfTrust() const noexcept -> bool;

  /*!
   * \brief Persist this entry.
   * \return True if persisted successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Persist() noexcept -> bool;

  /*!
   * \brief Tries to promote the entry to a root of / trust anchor.
   * \return True if promoted successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto MakeRootOfTrust() noexcept -> bool;

  /*!
   * \brief Gets the key of this entry.
   * \return The key of this entry.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetKey() const noexcept -> amsr::core::String const&;

  /*!
   * \brief Gets the certificate represented by this entry.
   * \return The certificate object this entry represents.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetCertificate() const noexcept -> Certificate const&;

  /*!
   * \brief Gets the label of the certificate.
   * \return The certificate's label.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetLabel() const noexcept -> amsr::core::StringView;

  /*!
   * \brief Set the label of the certificate.
   * \param[in] new_label The certificate's label.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetLabel(amsr::core::StringView new_label) noexcept;

  /*!
   * \brief Returns the identifier string under which to store the entry (either fingerprint or label).
   * \returns The file name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetIdentifier() const noexcept -> amsr::core::StringView;

 private:
  Certificate::Uptr certificate_; /*!< The certificate represented by this entry */
  amsr::core::String key_;        /*!< The key for this entry */
  amsr::core::String label_;      /*!< A description by which this certificate can be identified */
  CertificateStoreEntryObserver const& state_observer_; /*!< A reference to an entry observer instance */
  bool is_volatile_;                                    /*!< Whether the entry is volatile */
  bool is_root_of_trust_;                               /*!< Whether the entry is trust anchor */
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_H_
