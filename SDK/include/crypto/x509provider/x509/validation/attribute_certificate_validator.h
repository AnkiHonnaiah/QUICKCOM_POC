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
/*!        \file  attribute_certificate_validator.h
 *        \brief  Implementation of attribute certificate validation.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_ATTRIBUTE_CERTIFICATE_VALIDATOR_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_ATTRIBUTE_CERTIFICATE_VALIDATOR_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "crypto/x509provider/x509/attribute_certificate.h"
#include "crypto/x509provider/x509/certificate_store.h"
#include "vac/language/unique_function.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace validation {

/*!
 * \brief           Implementation of attribute certificate validation.
 */
class AttributeCertificateValidator final {
 public:
  /*!
   * \brief         Type alias for certificate status check functor.
   * \details       This callback will be used to validate candidate issuer & holder certificates.
   */
  using CertificateStatusCheckFunc = vac::language::UniqueFunction<bool(x509::Certificate const&)>;

  /*!
   * \brief         Construct a new AttributeCertificateValidator instance.
   * \param[in]     certificate_store Access to locally stored certificates for path building
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  explicit AttributeCertificateValidator(x509::CertificateStore const& certificate_store) noexcept;

  /*!
   * \brief         Destroy this AttributeCertificateValidator instance.
   */
  ~AttributeCertificateValidator() noexcept = default;

  /*!
   * \brief         Copy construct a AttributeCertificateValidator instance.
   */
  AttributeCertificateValidator(AttributeCertificateValidator const&) noexcept = default;

  /*!
   * \brief         Move construct a AttributeCertificateValidator instance.
   */
  AttributeCertificateValidator(AttributeCertificateValidator&&) noexcept = default;

  /*!
   * \brief Deleted copy Assignment
   * \return -
   * \vprivate Component Private
   */
  auto operator=(AttributeCertificateValidator const&) & noexcept -> AttributeCertificateValidator& = delete;

  /*!
   * \brief Deleted move Assignment
   * \return -
   * \vprivate Component Private
   */
  auto operator=(AttributeCertificateValidator&&) & noexcept -> AttributeCertificateValidator& = delete;

  /*!
   * \brief         Validate the given attribute certificate.
   * \param[in]     attribute_certificate The certificate to be validated
   * \param[in]     certificate_validator Functor to be used for issuer & holder candidate certificate validation
   * \param[in]     reference_time_point (optional) a time point to be used as reference for validity verification
   * \return        The final certificate verification status, will not return Status::kUnknown
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \trace         DSGN-Crypto-X509_AttributeCertificate_Validation
   * \vprivate      Component Private
   */
  auto Validate(x509::AttributeCertificate const& attribute_certificate,
                CertificateStatusCheckFunc&& certificate_validator,
                ara::core::Optional<time_t> reference_time_point = ara::core::nullopt) const noexcept
      -> ara::crypto::x509::AttributeCertificate::Status;

 private:
  /*! Certificate store entry container type alias */
  using EntryContainer = ara::core::Vector<std::reference_wrapper<CertificateStoreEntry const>>;

  /*!
   * \brief         Find issuer candidate certificates.
   * \param[in]     attribute_certificate The attribute certificate for which the issuer cert shall be found
   * \return        Vector of certificates that could be the issuer
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  auto FindAttributeCertIssuerCandidates(x509::AttributeCertificate const& attribute_certificate) const noexcept
      -> EntryContainer;

  /*!
   * \brief         Find holder candidate certificates.
   * \param[in]     attribute_certificate The attribute certificate for which the holder cert shall be found
   * \return        Vector of certificates that could be the holder
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  auto FindAttributeCertHolderCandidates(x509::AttributeCertificate const& attribute_certificate) const noexcept
      -> EntryContainer;

  /*! Logger instance */
  crypto::common::util::Logger logger_;
  /*! Certificate store */
  x509::CertificateStore const& certificate_store_;
};

}  // namespace validation
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_ATTRIBUTE_CERTIFICATE_VALIDATOR_H_
