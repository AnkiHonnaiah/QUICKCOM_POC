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
/*!        \file  amsr/asn1/structure/other_name.h
 *        \brief  Representation of parsed AnotherName as per RFC 5280.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OTHER_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OTHER_NAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief OtherName Structure.
 */
class OtherName {
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
  auto operator==(OtherName const& other) const -> bool {
    return std::tie(TypeId(), Value()) == std::tie(other.TypeId(), other.Value());
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
  auto operator!=(OtherName const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property TypeId.
   * \return Value of TypeId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TypeId() const noexcept -> Oid const& { return type_id_; }

  /*!
   * \brief Access interface for property TypeId.
   * \return Value of TypeId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TypeId() noexcept -> Oid& { return type_id_; }

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
  auto Value() const noexcept -> amsr::core::Variant<structure::RawData> const& { return value_; }

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
  auto Value() noexcept -> amsr::core::Variant<structure::RawData>& { return value_; }

  /*!
   * \brief Method to encode OtherName class with a universal header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode OtherName class with a context specific header.
   * \param [in] tag The context-specific tag to apply to the header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto EncodeContextSpecific(std::uint64_t tag) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  /*!
   * \brief Method to encode OtherName class without a header.
   * \return encoded data.
   */
  auto EncodeWithoutHeader() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  Oid type_id_{};                                   /*!< type-id */
  amsr::core::Variant<structure::RawData> value_{}; /*!< value */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_OTHER_NAME_H_
