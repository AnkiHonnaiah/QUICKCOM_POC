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
/*!        \file  amsr/asn1/structure/domain_parameters.h
 *        \brief  Representation of parsed domain parameters.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DOMAIN_PARAMETERS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DOMAIN_PARAMETERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/validation_parms.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Domain Parameters structure as per RFC 5912.
 */
class DomainParameters {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(DomainParameters const& other) const noexcept -> bool {
    return std::tie(P(), G(), Q(), J(), ValidationParms()) ==
           std::tie(other.P(), other.G(), other.Q(), other.J(), other.ValidationParms());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(DomainParameters const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Read only access API for P value.
   * \return Value of P.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer const& P() const noexcept { return p_; }

  /*!
   * \brief Access API for P value.
   * \return Value of P.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer& P() noexcept { return p_; }

  /*!
   * \brief Read only access API for G value.
   * \return Value of G.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer const& G() const noexcept { return g_; }

  /*!
   * \brief Access API for G value.
   * \return Value of G.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer& G() noexcept { return g_; }

  /*!
   * \brief Read only access API for Q value.
   * \return Value of Q.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer const& Q() const noexcept { return q_; }

  /*!
   * \brief Access API for Q value.
   * \return Value of Q.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Integer& Q() noexcept { return q_; }

  /*!
   * \brief Read only access API for J value.
   * \return Value of J.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<Integer> const& J() const noexcept { return j_; }

  /*!
   * \brief Access API for J value.
   * \return Value of J.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<Integer>& J() noexcept { return j_; }

  /*!
   * \brief Read only access API for ValidationParms value.
   * \return Value of ValidationParms.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<structure::ValidationParms> const& ValidationParms() const noexcept { return validation_parms_; }

  /*!
   * \brief Access API for ValidationParms value.
   * \return Value of ValidationParms.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Optional<structure::ValidationParms>& ValidationParms() noexcept { return validation_parms_; }

  /*!
   * \brief Method to encode DomainParameters class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \error Asn1Errc::kInvalidContent  If any of the integer member is empty.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_VAASN1_AutosarC++17_10-M3.2.4_without_definition_false_positive
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode() const noexcept;

 private:
  Integer p_;                                                         /*!< Odd prime, p=jq +1. */
  Integer g_;                                                         /*!< Generator, g. */
  Integer q_;                                                         /*!< Factor of p-1. */
  amsr::core::Optional<Integer> j_;                                   /*!< Subgroup factor. */
  amsr::core::Optional<structure::ValidationParms> validation_parms_; /*!< validation Parms value. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DOMAIN_PARAMETERS_H_
