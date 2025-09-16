/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Contains the API for checking the certificate availability.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CERT_CHECKER_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CERT_CHECKER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "ara/crypto/x509/x509_provider.h"

namespace amsr {
namespace tls {

/*!
 * \brief           A class that checks if the certificates are available
 * \vprivate        product
 * \trace           SecCom::LibTLS::CertChecker
 */
class CertChecker final {
 public:
  /*!
   * \brief           Constructor
   * \pre             The Crypto client shall be initialized and the Crypto daemon shall be running.
   */
  CertChecker() noexcept;

  /*!
   * \brief           Default destructor
   */
  ~CertChecker() noexcept = default;

  /*!
   * \brief           Check if a certificate is available
   * \param[in]       label
   *                  The certificate label.
   * \return          True if the certificate is available, false otherwise.
   * \context         -
   * \pre             -
   * \threadsafe      TRUE
   * \trace           DSGN-SecCom-CertChecker
   */
  bool IsCertAvailable(amsr::core::StringView label) noexcept;

 private:
  // cannot be copied nor moved
  CertChecker(CertChecker const &) = delete;
  CertChecker(CertChecker &&) = delete;
  void operator=(CertChecker const &) = delete;
  void operator=(CertChecker &&) = delete;

  /*!
   * \brief           Constructor
   * \details         For testing purposes only.
   * \param[in]       x509_provider
   *                  The x509 provider.
   * \context         -
   * \pre             -
   * \threadsafe      TRUE
   */
  explicit CertChecker(ara::crypto::x509::X509Provider::Sptr x509_provider) noexcept : x509_provider_{x509_provider} {}

  /*!
   * \brief           The certificate provider
   */
  ara::crypto::x509::X509Provider::Sptr x509_provider_{nullptr};

  /*!
   * \brief           Friend test for accessing the private ctor
   */
  FRIEND_TEST(UT__CertChecker, IsCertAvailable_TestCtor);

  /*!
   * \brief           Friend test for accessing the private ctor
   */
  FRIEND_TEST(Cdd__CertChecker, IsCertAvailable_TestCtor);
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CERT_CHECKER_H_
