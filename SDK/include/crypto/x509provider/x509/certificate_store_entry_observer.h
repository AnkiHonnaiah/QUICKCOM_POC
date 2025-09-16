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
/*!        \file  certificate_store_entry_observer.h
 *        \brief  Certificate store entry observer interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_OBSERVER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_OBSERVER_H_

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Forward declaration of CertificateStoreEntry.
 */
class CertificateStoreEntry;

/*!
 * \brief Certificate store entry observer interface.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class CertificateStoreEntryObserver {
 public:
  /*!
   * \brief Default destructor
   * \vprivate Component Private
   */
  virtual ~CertificateStoreEntryObserver() noexcept = default;

  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  CertificateStoreEntryObserver() = default;

  /*!
   * \brief Tries to persist the given certificate store entry.
   * \param[in] entry The entry to persist.
   * \return True if persisted successfully, otherwise false.
   * \vprivate Component Private
   */
  virtual bool OnPersist(CertificateStoreEntry const& entry) const noexcept = 0;

  /*!
   * \brief Tries to promote the given certificate store entry to the trust anchor group.
   * \param[in] entry The entry to promote.
   * \return True if promoted successfully, otherwise false.
   * \vprivate Component Private
   */
  virtual bool OnMakeRootOfTrust(CertificateStoreEntry const& entry) const noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  CertificateStoreEntryObserver(CertificateStoreEntryObserver const&) = default;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  CertificateStoreEntryObserver(CertificateStoreEntryObserver&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  CertificateStoreEntryObserver& operator=(CertificateStoreEntryObserver const&) & = default;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  CertificateStoreEntryObserver& operator=(CertificateStoreEntryObserver&&) & noexcept = default;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_ENTRY_OBSERVER_H_
