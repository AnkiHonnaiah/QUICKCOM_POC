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
/*!        \file  amsr/asn1/structure/validity.h
 *        \brief  Representation of parsed validity.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDITY_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDITY_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Validity Structure.
 */
class Validity {
 public:
  /*!
   * \brief Time Structure.
   * \details Variant of UTcTime and GeneralizedTime.
   */
  using Time = amsr::core::Variant<UtcTime, GeneralizedTime>;

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
  auto operator==(Validity const& other) const -> bool {
    return std::tie(NotBefore(), NotAfter()) == std::tie(other.NotBefore(), other.NotAfter());
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
  auto operator!=(Validity const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property NotBefore.
   * \return Value of NotBefore.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotBefore() const noexcept -> Time const& { return not_before_; }
  /*!
   * \brief access interface for property NotBefore.
   * \return Value of NotBefore.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotBefore() noexcept -> Time& { return not_before_; }

  /*!
   * \brief Read-only access interface for property NotAfter.
   * \return Value of NotAfter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotAfter() const noexcept -> Time const& { return not_after_; }

  /*!
   * \brief access interface for property NotAfter.
   * \return Value of NotAfter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotAfter() noexcept -> Time& { return not_after_; }

  /*!
   * \brief Method to encode Validity class.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> ara::core::Vector<std::uint8_t>;

 private:
  Time not_before_{}; /*!< Data member to store not before time. */
  Time not_after_{};  /*!< Data member to store not before time. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDITY_H_
