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
/*!        \file  amsr/asn1/structure/validation_parms.h
 *        \brief  Representation of parsed validation param.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDATION_PARMS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDATION_PARMS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Validation Params structure as per RFC 5912.
 */
class ValidationParms {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(ValidationParms const& other) const noexcept -> bool {
    return std::tie(Seed(), PgenCounter()) == std::tie(other.Seed(), other.PgenCounter());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(ValidationParms const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property seed.
   * \return Value of seed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::BitString const& Seed() const noexcept { return seed_; }

  /*!
   * \brief Access interface for property seed.
   * \return Value of seed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::BitString& Seed() noexcept { return seed_; }

  /*!
   * \brief Read-only access interface for property pgenCounter.
   * \return Value of pgenCounter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer const& PgenCounter() const noexcept { return pgen_counter_; }

  /*!
   * \brief Access interface for property pgenCounter.
   * \return Value of pgenCounter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer& PgenCounter() noexcept { return pgen_counter_; }

  /*!
   * \brief Method to encode ValidationParms class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \error Asn1Errc::kInvalidContent  If any of the integer member is empty.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode() const noexcept;

 private:
  structure::BitString seed_;         /*!< seed property. */
  structure::Integer pgen_counter_{}; /*!< pgenCounter property. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_VALIDATION_PARMS_H_
