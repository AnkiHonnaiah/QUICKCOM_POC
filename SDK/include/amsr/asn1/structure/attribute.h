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
/*!        \file  amsr/asn1/structure/attribute.h
 *        \brief  Representation of parsed attribute.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <amsr/core/variant.h>
#include <set>
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/attribute/attribute_encoder.h"
#include "amsr/asn1/structure/attribute/constants.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief A template for representations of ATTRIBUTEs, Attributes, SingleAttributes and AttributeSets.
 * \details In RFC 5912, the term "attribute" is defined, leaving many things that have names that include the word
 *      "attribute" in some capacity. \c ATTRIBUTE refers to an abstract description of what an attribute looks like.
 *      In essence, this describes the concept of a "type-value" tuple.
 *      \n\n This concept can be instantiated, e.g.
 *      by declaring that the OID \c 2.5.4.6 defines a country name and assigning it the type
 *      <tt>PrintableString</tt>. This defines an instance of \c ATTRIBUTE but is still not an encodable data packet,
 *      but an instance (object) of the information object class \c ATTRIBUTE. \n\n Now, \c SingleAttribute and \c
 *      AttributeSet are \c SEQUENCE types defining a way to serialize said abstract attribute values. They are of a
 *      similar structure: One carries an OID and an item of the type specified by said OID, and the other has an OID
 *      and a \c SET \c OF elements of said type. \n\n This class serves as a template for representing these two ASN.1
 *      structures. <b>Beware:</b> RFC 2986 defines a structure named \c Attribute which is of the same structure as \c
 *      AttributeSet so it is not the intuitive default case of what an "Attribute" would be. This is also the reason
 *      why this class is not named "Attribute".
 * \tparam V A template template over possible values' types. For single attributes, this will be a variant. For sets,
 *      this is intended to be a variant of vectors.
 */
template <template <typename...> class V>
class AttributeStructure {
 public:
  /*!
   * \brief The list of allowed attribute element types. Add to here if new attributes are required.
   */
  using AllowedTypesVariant = V<DirectoryString, PrintableString, Ia5String, RawData>;

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
  auto operator==(AttributeStructure const& other) const -> bool {
    return std::tie(Type(), Value()) == std::tie(other.Type(), other.Value());
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
  auto operator!=(AttributeStructure const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property type.
   * \return Value of type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Type() const noexcept -> Oid const& { return type_; }

  /*!
   * \brief Access interface for property type.
   * \return Value of type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Type() noexcept -> Oid& { return type_; }

  /*!
   * \brief Read-only access interface for property value.
   * \return Value of value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Value() const noexcept -> AllowedTypesVariant const& { return value_; }

  /*!
   * \brief Access interface for property value.
   * \return Value of value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Value() noexcept -> AllowedTypesVariant& { return value_; }

  /*!
   * \brief Encodes the attribute by DER.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kIncompleteInput  Content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    return attribute::AttributeEncoder::Encode(type_, value_);
  }

 private:
  Oid type_;                  /*!< Attribute type. */
  AllowedTypesVariant value_; /*!< Attribute value. */
};

/*!
 * \brief Alias for a variant over vector types, <b>not</b> a vector of variants.
 */
template <typename... Ts>
using VariantVector = amsr::core::Variant<ara::core::Vector<Ts>...>;

/*!
 * \brief SingleAttribute as per RFC 5912. All element types are always allowed, so there is no limitation by the formal
 *      \c AttrSet parameter. If you want to limit allowed types, use a constraint over the attribute's type OID.
 */
using SingleAttribute = AttributeStructure<amsr::core::Variant>;

/*!
 * \brief AttributeSet as per RFC 5912. All element types are always allowed, so there is no limitation by the formal
 *      \c AttrSet parameter. If you want to limit allowed types, use a constraint over the attribute's type OID.
 */
using AttributeSet = AttributeStructure<VariantVector>;

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_H_
