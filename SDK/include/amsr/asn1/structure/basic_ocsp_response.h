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
/*!        \file  basic_ocsp_response.h
 *        \brief  Representation of parsed BasicOCSPResponse
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_BASIC_OCSP_RESPONSE_H
#define AMSR_VECTOR_FS_VAASN1_BASIC_OCSP_RESPONSE_H
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "algorithm_identifier.h"
#include "response_data.h"
#include "x509_certificate.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief BasicOCSPResponse Structure (RFC 6960).
 */
class BasicOcspResponse {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator==(BasicOcspResponse const& other) const -> bool {
    return std::tie(TbsResponseData(), SignatureAlgorithm(), Signature(), Certs()) ==
           std::tie(other.TbsResponseData(), other.SignatureAlgorithm(), other.Signature(), other.Certs());
  }
  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator!=(BasicOcspResponse const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property tbsResponseData.
   * \return Value of tbsResponseData.
   */
  auto TbsResponseData() const noexcept -> ResponseData const& { return tbs_response_data_; }
  /*!
   * \brief Access interface for property tbsResponseData.
   * \return Value of tbsResponseData.
   */
  auto TbsResponseData() noexcept -> ResponseData& { return tbs_response_data_; }
  /*!
   * \brief Read-only access interface for property signatureAlgorithm.
   * \return Value of signatureAlgorithm.
   */
  auto SignatureAlgorithm() const noexcept -> AlgorithmIdentifier const& { return signature_algorithm_; }
  /*!
   * \brief Access interface for property signatureAlgorithm.
   * \return Value of signatureAlgorithm.
   */
  auto SignatureAlgorithm() noexcept -> AlgorithmIdentifier& { return signature_algorithm_; }
  /*!
   * \brief Read-only access interface for property signature.
   * \return Value of signature.
   */
  auto Signature() const noexcept -> BitString const& { return signature_; }
  /*!
   * \brief Access interface for property signature.
   * \return Value of signature.
   */
  auto Signature() noexcept -> BitString& { return signature_; }
  /*!
   * \brief Read-only access interface for property certs.
   * \return Value of certs.
   */
  auto Certs() const noexcept -> amsr::core::Optional<ara::core::Vector<X509Certificate>> const& { return certs_; }
  /*!
   * \brief Access interface for property certs.
   * \return Value of certs.
   */
  auto Certs() noexcept -> amsr::core::Optional<ara::core::Vector<X509Certificate>>& { return certs_; }
  /*!
   * \brief Method to encode BasicOcspResponse class.
   * \return encoded data.
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  ResponseData tbs_response_data_;                                 /*!< TBS response data */
  AlgorithmIdentifier signature_algorithm_;                        /*!< Signature algorithm */
  BitString signature_;                                            /*!< Signature */
  amsr::core::Optional<ara::core::Vector<X509Certificate>> certs_; /*!< Certificates */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_BASIC_OCSP_RESPONSE_H
