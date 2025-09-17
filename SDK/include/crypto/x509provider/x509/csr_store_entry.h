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
/*!        \file  csr_store_entry.h
 *        \brief  Single entry in the CSR store.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "crypto/x509provider/x509/cert_sign_request.h"
#include "crypto/x509provider/x509/csr_store_entry_observer.h"
#include "crypto/x509provider/x509/x509_dn.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Single entry in the CSR store.
 * \vprivate Component Private
 */
class CsrStoreEntry {
 public:
  /*!
   * \brief X509DN container type.
   * \vprivate Component Private
   */
  using X509DNContainer = std::vector<X509DN>;

  /*!
   * \brief Constructs a CSR store entry.
   * \param[in] csr The CSR to be represented by this entry.
   * \param[in] key The key for this entry.
   * \param[in] observer A reference to a entry observer instance.
   * \param[in] as_volatile Whether this should be created as volatile.
   * \param[in] as_pending Whether this should be created as pending.
   * \context ANY
   * \pre -
   * \threadsafe TRUE If key and observer are not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CsrStoreEntry(CertSignRequest::Uptr csr, amsr::core::String key, CsrStoreEntryObserver const& observer,
                bool as_volatile = true, bool as_pending = false) noexcept;

  /*!
   * \brief Constructs a CSR store entry.
   * \param[in] csr The CSR to be represented by this entry.
   * \param[in] authority_dn Distinguished name of the requested authority.
   * \param[in] key The key for this entry.
   * \param[in] observer A reference to a entry observer instance.
   * \context ANY
   * \pre -
   * \throws std::bad_alloc Memory allocation failed.
   * \exceptionsafety STRONG
   * \threadsafe TRUE If authority_dn, key and observer are not accessed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CsrStoreEntry(CertSignRequest::Uptr csr, X509DN const& authority_dn, amsr::core::String const& key,
                CsrStoreEntryObserver const& observer) noexcept;

  /*!
   * \brief Default destructor.
   * \vprivate Component Private
   */
  virtual ~CsrStoreEntry() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   * \vprivate Component Private
   */
  CsrStoreEntry(CsrStoreEntry const&) = delete;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  CsrStoreEntry(CsrStoreEntry&&) noexcept = default;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   * \vprivate Component Private
   */
  CsrStoreEntry& operator=(CsrStoreEntry const&) & = delete;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  CsrStoreEntry& operator=(CsrStoreEntry&&) & noexcept = default;

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
   * \brief Gets the status of the CSR.
   * \return True if the CSR is pending, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsPending() const noexcept;

  /*!
   * \brief Gets status of the CSR.
   * \return True if the CSR is new, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsNew() const noexcept;

  /*!
   * \brief Sets the CSR status to pending.
   * \return True if set successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool SetPending() noexcept;

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
  bool Persist() noexcept;

  /*!
   * \brief Adds an authority to which the request is addressed.
   * \param[in] authority_dn The distinguished name of the authority.
   * \return True if added successfully, false if authority already added before.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool AddAuthority(X509DN const& authority_dn) noexcept;

  /*!
   * \brief Removes an authority to which the request is addressed.
   * \param authority_dn The distinguished name of the authority.
   * \return True if added successfully, false if not found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool RemoveAuthority(X509DN const& authority_dn) noexcept;

  /*!
   * \brief Gets the key of this entry.
   * \return The key of this entry.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::String const& GetKey() const noexcept;

  /*!
   * \brief Gets the CSR represented by this entry.
   * \return The CSR object this entry represents.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  CertSignRequest& GetCertSignRequest() const noexcept;

  /*!
   * \brief Gets the authorities this request is addressed to.
   * \return Container of the authorities' X509 distinguished names.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  X509DNContainer const& GetAuthorities() const noexcept;

 private:
  CertSignRequest::Uptr csr_;                                          /*!< Certificate Sign Request */
  X509DNContainer authorities_;                                        /*!< X509DNContainer */
  amsr::core::String key_;                                             /*!< Key */
  std::reference_wrapper<CsrStoreEntryObserver const> state_observer_; /*!< Observer */
  bool is_pending_;                                                    /*!< Flag to check pending status */
  bool is_volatile_;                                                   /*!< Flag to check volatile status */
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_H_
