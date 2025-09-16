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
/*!        \file  ocsp_response_validator.h
 *        \brief  Implementation of OCSP response validation.
 *
 **********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_OCSP_RESPONSE_VALIDATOR_H_
#define LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_OCSP_RESPONSE_VALIDATOR_H_

#include "crypto/x509provider/x509/certificate_store.h"
#include "crypto/x509provider/x509/ocsp_response.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace validation {

/*!
 * \brief           Implementation of OCSP response validation.
 * \trace           DSGN-Crypto-X509_OCSP_Response_Validation
 * \vprivate        Component Private
 */
class OcspResponseValidator final {
 public:
  /*!
   * \brief         Construct a new OcspResponseValidator instance.
   * \param[in]     certificate_store Access to locally stored certificates for path building
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  explicit OcspResponseValidator(x509::CertificateStore const& certificate_store) noexcept;

  /*!
   * \brief         Destroy this OcspResponseValidator instance.
   */
  ~OcspResponseValidator() noexcept = default;

  /*!
   * \brief         Copy construct a OcspResponseValidator instance.
   */
  OcspResponseValidator(OcspResponseValidator const&) noexcept = default;

  /*!
   * \brief         Move construct a OcspResponseValidator instance.
   */
  OcspResponseValidator(OcspResponseValidator&&) noexcept = default;

  /*!
   * \brief         Deleted copy assignment operator.
   */
  auto operator=(OcspResponseValidator const&) & noexcept -> OcspResponseValidator& = delete;

  /*!
   * \brief         Deleted move assignment operator.
   */
  auto operator=(OcspResponseValidator&&) & noexcept -> OcspResponseValidator& = delete;

  /*!
   * \brief         Validate an OCSP response.
   * \details       Checks if the OCSP responder is trusted and if the response's signature is verifiable.
   * \param[in]     ocsp_response The OCSP response to validate
   * \return        \c true iff the OCSP response is accepted as valid.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault if an unexpected error happens during validation. This
   *                may include errors in parsing or retrieving data, missing mandatory field values and the like.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto Validate(crypto::x509provider::x509::OcspResponse const& ocsp_response) const noexcept
      -> ara::core::Result<bool>;

  /*!
   * \brief         Tries to verify the signature in the OCSP response via the passed certificate's public key.
   * \see           ContainsCertificateWithMatchingSignature
   * \param[in]     certificate The certificate to test
   * \param[in]     client_ocsp_response The signed OCSP response to verify
   * \param[in]     crypto_provider The crypto provider used for signature verification
   * \return        \c true iff \c certificate can be used to verify the response's signature
   * \context       ANY
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto TryVerifyingSignature(Certificate const& certificate,
                                    crypto::x509provider::x509::OcspResponse const& client_ocsp_response,
                                    ara::crypto::cryp::CryptoProvider& crypto_provider) noexcept
      -> ara::core::Result<bool>;

 private:
  /*!
   * \brief         Local type alias for brevity.
   */
  using EntryContainer = ara::core::Vector<std::reference_wrapper<CertificateStoreEntry const>>;

  /*!
   * \brief         Collect all valid certificates that might belong to the responder of the OCSP response.
   * \details       OCSP responses contain a field called \c responderID. This field can be an X.509 distinguished
   *                name or a hashed key. This methods searches the certificate store for known (and thus implicitly
   *                trusted) certificates associated to said \c responderID and returns them if they are, by themselves,
   *                valid at the moment.
   * \see           https://www.ietf.org/rfc/rfc6960.txt
   * \param[in]     client_ocsp_response The OCSP response that provides the \c responderId value to look for.
   * \return        A subset of the certificate store entries that are \b valid and \b associated to \c responderId.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault if an unexpected error happens during validation. This
   *                may include errors in parsing or retrieving data, missing mandatory field values and the like.
   * \context       ANY
   * \pre           The \c responderId field must be present.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CollectCandidateCertificates(crypto::x509provider::x509::OcspResponse const& client_ocsp_response) const noexcept
      -> ara::core::Result<EntryContainer>;

  /*!
   * \brief         Collect certificates by X.509 distinguished name.
   * \see           CollectCandidateCertificates
   * \param[in]     client_ocsp_response The OCSP response that provides the \c responderId value to look for.
   * \return        A subset of the certificate store entries that are \b valid and \b associated to \c responderId.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault if an unexpected error happens during validation. This
   *                may include errors in parsing or retrieving data, missing mandatory field values and the like.
   * \context       ANY
   * \pre           The \c responderId field must be present.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CollectCandidateCertificatesByName(crypto::x509provider::x509::OcspResponse const& client_ocsp_response)
      const noexcept -> ara::core::Result<EntryContainer>;

  /*!
   * \brief         Collect certificates by key hash.
   * \details       Since keys are unique, the returned collection will contain no more than one entry.
   * \see           CollectCandidateCertificates
   * \param[in]     client_ocsp_response The OCSP response that provides the \c responderId value to look for.
   * \return        A subset of the certificate store entries that are \b valid and \b associated to \c responderId.
   *                Contains at most one element.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault if an unexpected error happens during validation. This
   *                may include errors in parsing or retrieving data, missing mandatory field values and the like.
   * \context       ANY
   * \pre           The \c responderId field must be present.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CollectCandidateCertificatesByKeyHash(crypto::x509provider::x509::OcspResponse const& client_ocsp_response)
      const noexcept -> ara::core::Result<EntryContainer>;

  /*!
   * \brief         Evaluate the local validity criteria for a certificate
   * \details       In the above definitions, certificates are said to be \b valid if this method evaluates to \c true.
   * \see           CollectCandidateCertificates
   * \param[in]     entry The certificate store entry to validate
   * \return        \c true iff the certificate is valid
   * \context       ANY
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto IsCertificateValid(CertificateStoreEntry const& entry) const noexcept -> bool;

  /*!
   * \brief         Tries to verify the signature in the OCSP response via the passed certificates' public keys.
   * \see           CollectCandidateCertificates
   * \param[in]     valid_signer_candidate_certs The certificates to iterate
   * \param[in]     client_ocsp_response The signed OCSP response to verify
   * \param[in]     crypto_provider The crypto provider used for signature verification
   * \return        \c true iff there is at least one certificate that can be used to verify the response's signature
   * \context       ANY
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto ContainsCertificateWithMatchingSignature(
      EntryContainer const& valid_signer_candidate_certs,
      crypto::x509provider::x509::OcspResponse const& client_ocsp_response,
      ara::crypto::cryp::CryptoProvider& crypto_provider) noexcept -> ara::core::Result<bool>;

  /*! Logger instance */
  crypto::common::util::Logger logger_;
  /*! Certificate store */
  x509::CertificateStore const& certificate_store_;
};

}  // namespace validation
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_OCSP_RESPONSE_VALIDATOR_H_
