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
/*!        \file  amsr/asn1/structure/object_digest_info.h
 *        \brief  Representation of parsed ObjectDigestInfo.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OBJECT_DIGEST_INFO_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OBJECT_DIGEST_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/algorithm_identifier.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/optional.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief Enumerators for DigestedObjectType (RFC 3281)
 */
enum class DigestedObjectType : std::uint8_t { kPublicKey = 0x00, kPublicKeyCert = 0x01, kOtherObjectTypes = 0x02 };

/*!
 * \brief ObjectDigestInfo structure.
 */
class ObjectDigestInfo {
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
  auto operator==(ObjectDigestInfo const& other) const -> bool {
    return std::tie(DigestedObjectType(), OtherObjectTypeID(), DigestAlgorithm(), ObjectDigest()) ==
           std::tie(other.DigestedObjectType(), other.OtherObjectTypeID(), other.DigestAlgorithm(),
                    other.ObjectDigest());
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
  auto operator!=(ObjectDigestInfo const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property digestedObjectType.
   * \return Value of digestedObjectType.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DigestedObjectType() const noexcept -> Enumerated const& { return digested_object_type_; }

  /*!
   * \brief Access interface for property digestedObjectType.
   * \return Value of digestedObjectType.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DigestedObjectType() noexcept -> Enumerated& { return digested_object_type_; }

  /*!
   * \brief Read-only access interface for property OtherObjectTypeID.
   * \return Value of OtherObjectTypeID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OtherObjectTypeID() const noexcept -> amsr::core::Optional<structure::Oid> const& {
    return other_object_type_id_;
  }

  /*!
   * \brief Access interface for property OtherObjectTypeID.
   * \return Value of OtherObjectTypeID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OtherObjectTypeID() noexcept -> amsr::core::Optional<structure::Oid>& { return other_object_type_id_; }

  /*!
   * \brief Read-only access interface for property DigestAlgorithm.
   * \return Value of DigestAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DigestAlgorithm() const noexcept -> AlgorithmIdentifier const& { return digest_algorithm_; }

  /*!
   * \brief Access interface for property DigestAlgorithm.
   * \return Value of DigestAlgorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DigestAlgorithm() noexcept -> AlgorithmIdentifier& { return digest_algorithm_; }

  /*!
   * \brief Read-only access interface for property ObjectDigest.
   * \return Value of ObjectDigest.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ObjectDigest() const noexcept -> BitString const& { return object_digest_; }

  /*!
   * \brief Access interface for property ObjectDigest.
   * \return Value of ObjectDigest.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ObjectDigest() noexcept -> BitString& { return object_digest_; }

  /*!
   * \brief Method to encode ObjectDigestInfo class.
   * \param[in] context_specific If context-specific \c true, otherwise \c false.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode(bool context_specific = false) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  Enumerated digested_object_type_;                           /*!< DigestedObjectType. */
  amsr::core::Optional<structure::Oid> other_object_type_id_; /*!< OtherObjectTypeID. */
  AlgorithmIdentifier digest_algorithm_;                      /*!< DigestAlgorithm. */
  BitString object_digest_;                                   /*!< ObjectDigest. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OBJECT_DIGEST_INFO_H_
