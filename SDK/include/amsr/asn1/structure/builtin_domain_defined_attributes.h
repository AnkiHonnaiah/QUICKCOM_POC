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
/*!        \file  amsr/asn1/structure/builtin_domain_defined_attributes.h
 *        \brief  Representation of parsed builtin domain defined attributes.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_DOMAIN_DEFINED_ATTRIBUTES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_DOMAIN_DEFINED_ATTRIBUTES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {
namespace ub {

/*!
 * \brief Upper bound for type
 */
constexpr std::uint64_t const kDomainDefinedAttributeType{8};

/*!
 * \brief Upper bound for value
 */
constexpr std::uint64_t const kDomainDefinedAttributeValue{128};

}  // namespace ub
/*!
 * \brief BuiltIn Domain Defined Attribute Structure.
 */
class BuiltinDomainDefinedAttribute {
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
  auto operator==(BuiltinDomainDefinedAttribute const& other) const -> bool {
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
  auto operator!=(BuiltinDomainDefinedAttribute const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access API for Type.
   * \return Value of property Type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Type() const noexcept -> PrintableString const& { return type_; }

  /*!
   * \brief Access API for Type.
   * \return Value of property Type.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Type() noexcept -> PrintableString& { return type_; }

  /*!
   * \brief Read-Only access API for Value.
   * \return Value of Value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Value() const noexcept -> PrintableString const& { return value_; }

  /*!
   * \brief Access API for Value.
   * \return Value of Value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Value() noexcept -> PrintableString& { return value_; }

  /*!
   * \brief Method to encode BuiltinDomainDefinedAttribute class.
   * \param[in] context_specific If context-specific \c true, otherwise \c false.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto Encode(bool context_specific = false) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  PrintableString type_{};  /*!< Type. */
  PrintableString value_{}; /*!< Value. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_DOMAIN_DEFINED_ATTRIBUTES_H_
