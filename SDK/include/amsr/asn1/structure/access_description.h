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
/*!        \file  amsr/asn1/structure/access_description.h
 *        \brief  Representation of parsed Access Description.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ACCESS_DESCRIPTION_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ACCESS_DESCRIPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/general_name.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Access Description structure.
 */
class AccessDescription {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(AccessDescription const& other) const -> bool {
    return std::tie(AccessMethod(), AccessLocation()) == std::tie(other.AccessMethod(), other.AccessLocation());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(AccessDescription const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property Access Method.
   * \return Value of Access Method.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Oid const& AccessMethod() const noexcept { return access_method_; }

  /*!
   * \brief Access interface for property Access Method.
   * \return Value of Access Method.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Oid& AccessMethod() noexcept { return access_method_; }

  /*!
   * \brief Read-only access interface for property Access Location.
   * \return Value of Access Location.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::GeneralName const& AccessLocation() const noexcept { return access_location_; }

  /*!
   * \brief Access interface for property Access Location.
   * \return Value of Access Location.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::GeneralName& AccessLocation() noexcept { return access_location_; }

  /*!
   * \brief Method to encode 'AccessDescription' class.
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
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode() const noexcept;

  /*!
   * \brief Amends the access description class.
   * \details Set the each data members with the corresponding member from the input access description.
   * \param[in] access_description Input access description class.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  amsr::core::Result<void> Amend(AccessDescription const& access_description) noexcept;

  /*!
   * \brief Amends the access method member.
   * \details Set the access method with the input parameter.
   * \param[in] access_method Input access method structure.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void AmendMethod(structure::Oid const& access_method) noexcept;

  /*!
   * \brief Amends the access location member.
   * \details Set the access location with the input parameter.
   * \param[in] access_location Input access location structure.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  amsr::core::Result<void> AmendLocation(structure::GeneralName const& access_location) noexcept;

 private:
  structure::Oid access_method_{};           /*!< Access Method. */
  structure::GeneralName access_location_{}; /*!<  Access Location. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ACCESS_DESCRIPTION_H_
