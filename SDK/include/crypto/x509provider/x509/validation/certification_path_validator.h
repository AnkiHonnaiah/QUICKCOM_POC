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
/*!        \file  certification_path_validator.h
 *        \brief  Implementation of PKI certification path validation.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_CERTIFICATION_PATH_VALIDATOR_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_CERTIFICATION_PATH_VALIDATOR_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "crypto/x509provider/x509/certificate_store.h"
#include "crypto/x509provider/x509/validation/node.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace validation {

/*!
 * \brief           Implements PKI certification path validation.
 * \trace           DSGN-Crypto-X509_CertificationPathValidation
 * \vprivate        Component Private
 */
class CertificationPathValidator final {
 public:
  /*!
   * \brief         Enumeration of validation results.
   * \vprivate      Component Private
   */
  enum class ValidationResult : std::uint8_t {
    kValid = 0,                /*!< Validation step succeeded */
    kInvalid,                  /*!< Generic validation failure */
    kFuture,                   /*!< Certificate not valid yet */
    kExpired,                  /*!< Certificate is expired */
    kInvalidPath,              /*!< Current certification path is invalid */
    kNoTrust,                  /*!< No trust anchor found */
    kBasicConstraintsInvalid,  /*!< Validation of basic constraints failed */
    kKeyConstraintsInvalid,    /*!< Validation of key constraints failed */
    kAuthorityKeyIdInvalid,    /*!< Validation of authority key identifier failed */
    kSubjectKeyIdInvalid,      /*!< Validation of subject key identifier failed */
    kPathLimitExceeded,        /*!< Path limit exceeded */
    kUnknownCriticalExtension, /*!< Unknown critical extension(s) encountered */
    kKeyIdMismatch,            /*!< Authority key id of certificate does not match subject key id of issuing CA cert */
    kDistinguishedNameMismatch /*!< Issuer DN of certificate does not match subject DN of issuing CA cert */
  };

  /*!
   * \brief         Enumeration of validation modes.
   * \vprivate      Component Private
   */
  enum class ValidationMode : std::uint8_t {
    kPathBuilding, /*!< Validate potentially multiple candidate paths */
    kFixedPath     /*!< Validate a single, fixed path only */
  };

  /*!
   * \brief         Construct a new CertificationPathValidator instance.
   * \param[in]     certificate_store Access to locally stored certificates for path building
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  explicit CertificationPathValidator(x509::CertificateStore const& certificate_store) noexcept;

  /*!
   * \brief         Destroy this CertificationPathValidator instance.
   * \vprivate      Component Private
   */
  ~CertificationPathValidator() noexcept = default;

  /*!
   * \brief         Copy construct a CertificationPathValidator instance.
   * \vprivate      Component Private
   */
  CertificationPathValidator(CertificationPathValidator const&) noexcept = default;

  /*!
   * \brief         Move construct a CertificationPathValidator instance.
   * \vprivate      Component Private
   */
  CertificationPathValidator(CertificationPathValidator&&) noexcept = default;

  /*!
   * \brief         Deleted copy Assignment
   * \return        -
   * \vprivate      Component Private
   */
  auto operator=(CertificationPathValidator const&) & noexcept -> CertificationPathValidator& = delete;

  /*!
   * \brief         Deleted move Assignment
   * \return        -
   * \vprivate      Component Private
   */
  auto operator=(CertificationPathValidator&&) & noexcept -> CertificationPathValidator& = delete;

  /*!
   * \brief         Validate the given certificate.
   * \param[in]     target_certificate The certificate to be validated
   * \param[in]     known_extension_oids Vector of known extension OID strings
   * \param[in]     reference_timepoint The reference timepoint for validity checks
   * \return        The certificate validation status
   * \context       ANY
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto Validate(x509::Certificate const& target_certificate,
                ara::core::Vector<amsr::core::String> const& known_extension_oids,
                amsr::core::Optional<time_t> reference_timepoint) const noexcept -> ValidationResult;

  /*!
   * \brief         Validate the given certification path (chain).
   * \param[in]     certification_path The certification path to be validated
   * \param[in]     known_extension_oids Vector of known extension OID strings
   * \param[in]     reference_timepoint The reference timepoint for validity checks
   * \return        ValidationResult::kValid if whole path validated successfully, otherwise the ValidationResult::* of
   *                the first failed certificate
   * \context       ANY
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto Validate(ara::core::Vector<std::reference_wrapper<x509::Certificate const>> const& certification_path,
                ara::core::Vector<amsr::core::String> const& known_extension_oids,
                amsr::core::Optional<time_t> reference_timepoint) const noexcept -> ValidationResult;

  /*!
   * \brief         Map validation result to the corresponding certificate verification status.
   * \param[in]     validation_result The result to be mapped
   * \return        The corresponding certificate verification status. Will never return a Status::kUnknown
   * \context       ANY
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto MapValidationResultToCertificateStatus(ValidationResult validation_result) noexcept
      -> x509::Certificate::Status;

 private:
  /*!
   * \brief         Structure representing the context / state of the validation process.
   */
  struct ValidationContext final {
    Node::Sptr tip; /*!< The current tip of the certification path */
    ara::core::Vector<amsr::core::String> const&
        known_extension_oids;         /*!< A vector of custom extension OIDs that will be accepted if critical */
    time_t const reference_timepoint; /*!< The reference timepoint for the current validation process */
  };

  /*!
   * \brief         Validate the validity period.
   * \param[in]     certificate The certificate for which the validity period shall be validated
   * \param[in]     reference_timepoint The reference timepoint for validity period checks
   * \return        A validation result
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  static auto ValidateValidityPeriod(x509::Certificate const& certificate, time_t reference_timepoint) noexcept
      -> ValidationResult;

  /*!
   * \brief         Validate the extensions of a certificate.
   * \details       Any unknown critical extension found in \c certificate will lead to validation failure if the OID
   *                is not contained in \c known_extension_oids.
   * \param[in]     certificate The certificate for which the extensions shall be validated
   * \param[in]     known_extension_oids Vector of known extension OID strings
   * \return        A validation result
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  auto ValidateExtensions(x509::Certificate const& certificate,
                          ara::core::Vector<amsr::core::String> const& known_extension_oids) const noexcept
      -> ValidationResult;

  /*!
   * \brief         Validate a candidate path.
   * \param[in]     path The path to validate
   * \param[in]     known_extension_oids Vector of known extension OID strings
   * \param[in]     reference_timepoint The reference timepoint for validity checks
   * \param[in]     mode Validation mode
   * \return        A validation result
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  auto ValidatePath(ara::core::Vector<std::reference_wrapper<x509::Certificate const>> const& path,
                    ara::core::Vector<amsr::core::String> const& known_extension_oids, time_t reference_timepoint,
                    ValidationMode mode) const noexcept -> ValidationResult;

  /*!
   * \brief         Iterate & validate paths that can be reached from the current tip.
   * \details       A candidate path is found once a trust anchor has been reached. This candidate path will be
   *                validated once it was discovered. If a candidate path is validated successfully the processing
   *                stops, otherwise the process continues to look for potential candidates until all paths were
   *                discovered & checked.
   * \param[in]     context The current validation context
   * \return        A validation result
   * \context       ANY
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  auto IterateAndValidatePaths(ValidationContext context) const noexcept -> ValidationResult;

  /*!
   * \brief         Fetch all node elements from the local certificate cache matching given subject DN.
   * \param[in]     subject_dn The subject DN to be used for the certificate cache lookup
   * \return        A container of NodeElements matching given subject DN
   * \context       ANY
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  auto FetchNodeElements(x509::X509DN const& subject_dn) const noexcept -> Node::ElementContainer;

  /*!
   * \brief         Eliminate all duplicate elements from the path.
   * \param[in]     elements A container of node elements to be processed
   * \param[in]     context A validation context
   * \context       ANY
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  void EliminateDuplicates(Node::ElementContainer& elements, ValidationContext const& context) const noexcept;

  /*!
   * \brief         Sort the Node elements.
   * \details       Sorting is giving by weighing the elements with respect to their probability of providing a valid
   *                certification path in a descending order.
   * \param[in]     elements Container of NodeElements to be sorted
   * \context       ANY
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  static void SortElements(Node::ElementContainer& elements) noexcept;

  /*!
   * \brief         Calculate the weight of a certificate.
   * \details       The weight is used to sort the node elements such that certificates with higher successful path
   *                validation probability are preferred.
   * \param[in]     certificate The certificate for which the weight shall be calculated
   * \return        A unsigned 16-bit integer representing the weight
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \reentrant     TRUE
   */
  static auto CalculateWeight(x509::Certificate const& certificate) noexcept -> std::uint16_t;

  /*! Logger instance */
  crypto::common::util::Logger logger_;
  /*! Certificate store */
  x509::CertificateStore const& certificate_store_;
};

}  // namespace validation
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_VALIDATION_CERTIFICATION_PATH_VALIDATOR_H_
