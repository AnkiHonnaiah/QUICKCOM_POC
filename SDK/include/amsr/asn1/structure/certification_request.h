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
/*!        \file  certification_request.h
 *         \brief Representation of parsed CertificationRequest
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_H
#define AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "algorithm_identifier.h"
#include "asn1_primitive_types.h"
#include "certification_request_info.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief CertificationRequest Structure (RFC 2986).
 */
class CertificationRequest {
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
  auto operator==(CertificationRequest const& other) const -> bool {
    return std::tie(CertificationRequestInfo(), SignatureAlgorithm(), Signature()) ==
           std::tie(other.CertificationRequestInfo(), other.SignatureAlgorithm(), other.Signature());
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
  auto operator!=(CertificationRequest const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property CertificationRequestInfo.
   * \return Value of CertificationRequestInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CertificationRequestInfo() const noexcept -> structure::CertificationRequestInfo const& {
    return certification_request_info_;
  }
  /*!
   * \brief Access interface for property CertificationRequestInfo.
   * \return Value of CertificationRequestInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CertificationRequestInfo() noexcept -> structure::CertificationRequestInfo& {
    return certification_request_info_;
  }
  /*!
   * \brief Read-only access interface for property SignatureAlgorithm.
   * \return Value of SignatureAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureAlgorithm() const noexcept -> AlgorithmIdentifier const& { return signature_algorithm_; }
  /*!
   * \brief Access interface for property SignatureAlgorithm.
   * \return Value of SignatureAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SignatureAlgorithm() noexcept -> AlgorithmIdentifier& { return signature_algorithm_; }
  /*!
   * \brief Read-only access interface for property Signature.
   * \return Value of Signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() const noexcept -> BitString const& { return signature_; }
  /*!
   * \brief Access interface for property Signature.
   * \return Value of Signature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Signature() noexcept -> BitString& { return signature_; }
  /*!
   * \brief Method to encode CertificationRequest class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  structure::CertificationRequestInfo certification_request_info_; /*!< certificationRequestInfo. */
  AlgorithmIdentifier signature_algorithm_;                        /*!< signatureAlgorithm. */
  BitString signature_;                                            /*!< signature. */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_H
