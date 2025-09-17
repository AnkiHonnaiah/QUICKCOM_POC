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
/*!        \file  amsr/asn1/structure/oraddress.h
 *        \brief  Representation of parsed ORAddress.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ORADDRESS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ORADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/builtin_domain_defined_attributes.h"
#include "amsr/asn1/structure/builtin_standard_attributes.h"
#include "amsr/asn1/structure/extension_attributes.h"

namespace amsr {
namespace asn1 {
namespace structure {
namespace ub {
/*!
 * \brief Upper bound for domain defined attributes
 */
constexpr std::uint64_t const kDomainDefinedAttributes{4};
}  // namespace ub

/*!
 * \brief ORAddress Structure.
 */
class ORAddress {
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
  auto operator==(ORAddress const& other) const -> bool {
    return std::tie(BuiltinStandardAttributes(), BuiltinDomainDefinedAttributes(), ExtensionAttributes()) ==
           std::tie(other.BuiltinStandardAttributes(), other.BuiltinDomainDefinedAttributes(),
                    other.ExtensionAttributes());
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
  auto operator!=(ORAddress const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property BuiltinStandardAttributes.
   * \return Value of BuiltinStandardAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BuiltinStandardAttributes() const noexcept -> structure::BuiltinStandardAttributes const& {
    return built_in_standard_attributes_;
  }

  /*!
   * \brief Access interface for property BuiltinStandardAttributes.
   * \return Value of BuiltinStandardAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BuiltinStandardAttributes() noexcept -> structure::BuiltinStandardAttributes& {
    return built_in_standard_attributes_;
  }

  /*!
   * \brief Read-only access interface for property BuiltinDomainDefinedAttributes.
   * \return Value of BuiltinDomainDefinedAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BuiltinDomainDefinedAttributes() const noexcept
      -> amsr::core::Optional<ara::core::Vector<structure::BuiltinDomainDefinedAttribute>> const& {
    return built_in_domain_defined_attributes_;
  }

  /*!
   * \brief Access interface for property BuiltinDomainDefinedAttributes.
   * \return Value of BuiltinDomainDefinedAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto BuiltinDomainDefinedAttributes() noexcept
      -> amsr::core::Optional<ara::core::Vector<structure::BuiltinDomainDefinedAttribute>>& {
    return built_in_domain_defined_attributes_;
  }

  /*!
   * \brief Read-only access interface for property ExtensionAttributes.
   * \return Value of ExtensionAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtensionAttributes() const noexcept
      -> amsr::core::Optional<ara::core::Vector<structure::ExtensionAttribute>> const& {
    return extension_attributes_;
  }

  /*!
   * \brief Access interface for property ExtensionAttributes.
   * \return Value of ExtensionAttributes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExtensionAttributes() noexcept -> amsr::core::Optional<ara::core::Vector<structure::ExtensionAttribute>>& {
    return extension_attributes_;
  }

  /*!
   * \brief Method to encode ORAddress class.
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
  structure::BuiltinStandardAttributes built_in_standard_attributes_{}; /*!< Builtin Standard Attributes. */
  amsr::core::Optional<ara::core::Vector<BuiltinDomainDefinedAttribute>>
      built_in_domain_defined_attributes_{}; /*!< Builtin Domain Defined Attributes. */
  amsr::core::Optional<ara::core::Vector<structure::ExtensionAttribute>>
      extension_attributes_{}; /*!< Extension Attributes. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_STRUCTURE_ORADDRESS_H_
