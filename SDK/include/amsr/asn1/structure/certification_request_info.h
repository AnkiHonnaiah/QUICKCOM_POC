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
/*!        \file  certification_request_info.h
 *         \brief Representation of parsed CertificationRequestInfo (RFC5912)
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_INFO_H
#define AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_INFO_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/attribute.h"
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/subject_public_key_info.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief CertificationRequestInfo Structure (RFC 2986).
 */
class CertificationRequestInfo {
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
  auto operator==(CertificationRequestInfo const& other) const -> bool {
    return std::tie(Version(), Subject(), SubjectPkInfo(), Attributes()) ==
           std::tie(other.Version(), other.Subject(), other.SubjectPkInfo(), other.Attributes());
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
  auto operator!=(CertificationRequestInfo const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property Version.
   * \return Value of Version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() const noexcept -> Integer const& { return version_; }
  /*!
   * \brief Access interface for property Version.
   * \return Value of Version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() noexcept -> Integer& { return version_; }
  /*!
   * \brief Read-only access interface for property Subject.
   * \return Value of Subject.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Subject() const noexcept -> Name const& { return subject_; }
  /*!
   * \brief Access interface for property Subject.
   * \return Value of Subject.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Subject() noexcept -> Name& { return subject_; }
  /*!
   * \brief Read-only access interface for property SubjectPKInfo.
   * \return Value of SubjectPKInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPkInfo() const noexcept -> SubjectPublicKeyInfo const& { return subject_pk_info_; }
  /*!
   * \brief Access interface for property SubjectPKInfo.
   * \return Value of SubjectPKInfo.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SubjectPkInfo() noexcept -> SubjectPublicKeyInfo& { return subject_pk_info_; }
  /*!
   * \brief Read-only access interface for property Attributes.
   * \return Value of Attributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Attributes() const noexcept -> ara::core::Vector<AttributeSet> const& { return attributes_; }
  /*!
   * \brief Access interface for property Attributes.
   * \return Value of Attributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Attributes() noexcept -> ara::core::Vector<AttributeSet>& { return attributes_; }
  /*!
   * \brief Method to encode CertificationRequestInfo class.
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
  Integer version_;                            /*!< version */
  Name subject_;                               /*!< subject */
  SubjectPublicKeyInfo subject_pk_info_;       /*!< subjectPkInfo */
  ara::core::Vector<AttributeSet> attributes_; /*!< attributes */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_CERTIFICATION_REQUEST_INFO_H
