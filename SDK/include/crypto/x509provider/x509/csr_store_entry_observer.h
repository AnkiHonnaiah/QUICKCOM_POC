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
/*!        \file  csr_store_entry_observer.h
 *        \brief  Certificate sign request store entry observer interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_OBSERVER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_OBSERVER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "crypto/x509provider/x509/cert_sign_request.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Forward declaration of CSR store entry.
 */
class CsrStoreEntry;

/*!
 * \brief Certificate sign request store entry observer interface.
 * \vprivate Component Private
 */
class CsrStoreEntryObserver {
 public:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  CsrStoreEntryObserver() = default;

  /*!
   * \brief Default destructor.
   * \vprivate Component Private
   */
  virtual ~CsrStoreEntryObserver() noexcept = default;

  /*!
   * \brief Tries to persist the given CSR store entry.
   * \param[in] entry The entry to persist.
   * \return True if persisted successfully, otherwise false.
   * \vprivate Component Private
   */
  virtual bool OnPersist(CsrStoreEntry const& entry) const noexcept = 0;

  /*!
   * \brief Tries to set the pending state for the given CSR store entry.
   * \param[in] entry The entry for which the pending state shall be set.
   * \return True if pending state set successfully, otherwise false.
   * \vprivate Component Private
   */
  virtual bool OnSetPendingState(CsrStoreEntry const& entry) const noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  CsrStoreEntryObserver(CsrStoreEntryObserver const&) = default;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  CsrStoreEntryObserver(CsrStoreEntryObserver&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  CsrStoreEntryObserver& operator=(CsrStoreEntryObserver const&) & = default;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  CsrStoreEntryObserver& operator=(CsrStoreEntryObserver&&) & noexcept = default;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CSR_STORE_ENTRY_OBSERVER_H_
