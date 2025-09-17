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
/*!        \file  certificate_store_interface.h
 *        \brief  Interface to the certificate store.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_INTERFACE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_INTERFACE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "crypto/x509provider/x509/certificate.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief           Interface to the certificate store.
 */
class CertificateStoreInterface {
 public:
  /*!
   * \brief         Default construct a CertificateStoreInterface instance.
   */
  CertificateStoreInterface() noexcept = default;

  /*!
   * \brief         Destroy this CertificateStoreInterface instance.
   */
  virtual ~CertificateStoreInterface() noexcept = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  CertificateStoreInterface(CertificateStoreInterface const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  CertificateStoreInterface(CertificateStoreInterface&&) noexcept = delete;

  /*!
   * \brief Deleted copy Assignment
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateStoreInterface const&) & noexcept -> CertificateStoreInterface& = delete;

  /*!
   * \brief Deleted move Assignment
   * \return -
   * \vprivate Component Private
   */
  auto operator=(CertificateStoreInterface&&) & noexcept -> CertificateStoreInterface& = delete;

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
  virtual auto CheckIsRoot(Certificate const& certificate) const noexcept -> bool = 0;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_STORE_INTERFACE_H_
