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
/*!        \file  amsr/asn1/structure/extension_attributes.h
 *        \brief  Representation of parsed extension attributes.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_ATTRIBUTES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_ATTRIBUTES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief Extension Attribute Structure.
 * \vprivate Product Private
 */
class ExtensionAttribute {
 public:
  /*!
   * \brief Type alias containing known variant alternatives.
   */
  using Value = amsr::core::Variant<structure::RawData>;

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
  auto operator==(ExtensionAttribute const& other) const -> bool {
    return std::tie(ExtensionAttributeType(), ExtensionAttributeValue()) ==
           std::tie(other.ExtensionAttributeType(), other.ExtensionAttributeValue());
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
  auto operator!=(ExtensionAttribute const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access API for Extension Attribute Type.
   * \return Type of property Extension Attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtensionAttributeType() const noexcept -> Integer const& { return extension_attribute_type_; }

  /*!
   * \brief Access API for Extension Attribute Type.
   * \return Type of property Extension Attribute.
   */
  auto ExtensionAttributeType() noexcept -> Integer& { return extension_attribute_type_; }

  /*!
   * \brief Read-Only access API for Extension Attribute Value.
   * \return Value of Extension Attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtensionAttributeValue() const noexcept -> Value const& { return extension_attribute_value_; }

  /*!
   * \brief Access API for Extension Attribute Value.
   * \return Value of Extension Attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtensionAttributeValue() noexcept -> Value& { return extension_attribute_value_; }

  /*!
   * \brief Method to encode ExtensionAttribute class with a universal header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  /*!
   * \brief Method to encode ExtensionAttribute class without a header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto EncodeWithoutHeader() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  Integer extension_attribute_type_{}; /*!< Member: Extension Attribute Type. */
  Value extension_attribute_value_{};  /*!< Member: Extension Attribute Value. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EXTENSION_ATTRIBUTES_H_
