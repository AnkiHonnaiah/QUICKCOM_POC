/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  cert_id.h
 *        \brief  Representation of parsed CertID(RFC 6960)
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CERT_ID_H
#define AMSR_VECTOR_FS_VAASN1_CERT_ID_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "algorithm_identifier.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief CertID Structure (RFC 6960).
 */
class CertId {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(CertId const& other) const -> bool {
    return std::tie(HashAlgorithm(), IssuerNameHash(), IssuerKeyHash(), SerialNumber()) ==
           std::tie(other.HashAlgorithm(), other.IssuerNameHash(), other.IssuerKeyHash(), other.SerialNumber());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(CertId const& other) const -> bool { return !(*this == other); }
  /*!
   * \brief Read-only access interface for property hashAlgorithm.
   * \return Value of hashAlgorithm.
   */
  auto HashAlgorithm() const noexcept -> AlgorithmIdentifier const& { return hash_algorithm_; }
  /*!
   * \brief Access interface for property hashAlgorithm.
   * \return Value of hashAlgorithm.
   */
  auto HashAlgorithm() noexcept -> AlgorithmIdentifier& { return hash_algorithm_; }
  /*!
   * \brief Read-only access interface for property issuerNameHash.
   * \return Value of issuerNameHash.
   */
  auto IssuerNameHash() const noexcept -> OctetString const& { return issuer_name_hash_; }
  /*!
   * \brief Access interface for property issuerNameHash.
   * \return Value of issuerNameHash.
   */
  auto IssuerNameHash() noexcept -> OctetString& { return issuer_name_hash_; }
  /*!
   * \brief Read-only access interface for property issuerKeyHash.
   * \return Value of issuerKeyHash.
   */
  auto IssuerKeyHash() const noexcept -> OctetString const& { return issuer_key_hash_; }
  /*!
   * \brief Access interface for property issuerKeyHash.
   * \return Value of issuerKeyHash.
   */
  auto IssuerKeyHash() noexcept -> OctetString& { return issuer_key_hash_; }
  /*!
   * \brief Read-only access interface for property serialNumber.
   * \return Value of serialNumber.
   */
  auto SerialNumber() const noexcept -> Integer const& { return serial_number_; }
  /*!
   * \brief Access interface for property serialNumber.
   * \return Value of serialNumber.
   */
  auto SerialNumber() noexcept -> Integer& { return serial_number_; }
  /*!
   * \brief Method to encode CertID class.
   * \return encoded data.
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  AlgorithmIdentifier hash_algorithm_; /*!< AlgorithmIdentifier data */
  OctetString issuer_name_hash_;       /*!< OctetString data */
  OctetString issuer_key_hash_;        /*!< OctetString data */
  Integer serial_number_;              /*!< Integer data */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_CERT_ID_H
