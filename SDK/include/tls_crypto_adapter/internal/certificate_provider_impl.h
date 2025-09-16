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
 *        \brief  This file contains the declaration of CertificateProviderImpl class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_PROVIDER_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_PROVIDER_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/string.h"
#include "ara/crypto/x509/entry_point.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_error_definitions.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief           CertificateProvider adapter implementation
 * \vprivate        This class is component private.
 */
class CertificateProviderImpl final : public CertificateProviderInterface {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       get_cert_time_fptr
   *                  Callback for a time getter.
   */
  explicit CertificateProviderImpl(tls::TlsCommConfigurationInformation::CertConf::GetTimeFptr get_cert_time_fptr);
  /*!
   * \copydoc         CertificateProviderInterface::GetBinaryCertificate
   * \vprivate        This method is component private.
   */
  amsr::core::Result<std::size_t, CryptoAdapterErrorCode> GetBinaryCertificate(
      amsr::core::String const& label, ara::core::Vector<std::uint8_t>& target) const noexcept final;

  /*!
   * \copydoc         CertificateProviderInterface::ParseCertificateChain
   * \vprivate        This method is component private.
   */
  amsr::core::Result<void> ParseCertificateChain(ara::core::Vector<ara::crypto::x509::Certificate::Uptr>& parsed_certs,
                                                 ara::crypto::ReadOnlyMemRegion const& cert_chain) const noexcept final;

  /*!
   * \copydoc         CertificateProviderInterface::LoadRootCertificateAndSetAsRootOfTrust
   * \vprivate        This method is component private.
   * \trace           DSGN-SecCom-GetCertTime
   */
  amsr::core::Result<ara::crypto::x509::Certificate::Uptr, CryptoAdapterErrorCode>
  LoadRootCertificateAndSetAsRootOfTrust(amsr::core::String const& root_label) const noexcept final;

  /*!
   * \copydoc         amsr::tls_crypto_adapter::internal::CertificateProviderInterface::LoadCertificate
   */
  auto LoadCertificate(ara::core::StringView label) const noexcept
      -> ara::core::Result<ara::crypto::x509::Certificate::Uptr, CryptoAdapterErrorCode> final;

  /*!
   * \copydoc         CertificateProviderInterface::VerifyCertificateChain
   * \vprivate        This method is component private.
   * \trace           DSGN-SecCom-GetCertTime
   */
  amsr::core::Result<void> VerifyCertificateChain(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const& cert_chain) const noexcept final;

  /*!
   * \copydoc         CertificateProviderInterface::GetCertificatePublicKey
   * \vprivate        This method is component private.
   */
  ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc, CryptoAdapterErrorCode> GetCertificatePublicKey(
      ara::crypto::x509::Certificate const& cert) const noexcept final;

  /*!
   * \copydoc         amsr::tls_crypto_adapter::internal::CertificateProviderInterface::DecodeDn
   */
  auto DecodeDn(ara::crypto::ReadOnlyMemRegion distinguished_name,
                ara::crypto::Serializable::FormatId format_id) noexcept
      -> ara::core::Result<ara::crypto::x509::X509DN::Uptrc> final;

 private:
  /*!
   * A shared smart pointer to CertificateProvider
   */
  std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider_{nullptr};

  /*!
   * \brief           Callback for setting the certificate time
   */
  tls::TlsCommConfigurationInformation::CertConf::GetTimeFptr get_cert_time_fptr_;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr
#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_PROVIDER_IMPL_H_
