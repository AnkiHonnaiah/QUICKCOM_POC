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
/*!        \file  amsr/asn1/structure/dss_parms.h
 *        \brief  Representation of parsed Field ID for EC algorithm.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DSS_PARMS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DSS_PARMS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief DSS-Parms structure as per RFC 5480.
 */
class DssParms {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(DssParms const& other) const noexcept -> bool {
    return std::tie(P(), Q(), G()) == std::tie(other.P(), other.Q(), other.G());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(DssParms const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property P.
   * \return Value of p.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer const& P() const noexcept { return p_; }

  /*!
   * \brief Access interface for property p.
   * \return Value of p.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer& P() noexcept { return p_; }

  /*!
   * \brief Read-only access interface for property Q.
   * \return Value of Q.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer const& Q() const noexcept { return q_; }

  /*!
   * \brief Access interface for property q.
   * \return Value of q.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer& Q() noexcept { return q_; }

  /*!
   * \brief Read-only access interface for property g.
   * \return Value of g.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer const& G() const noexcept { return g_; }

  /*!
   * \brief Access interface for property g.
   * \return Value of g.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  structure::Integer& G() noexcept { return g_; }

  /*!
   * \brief Method to encode DssParms class.
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
  structure::Integer p_{}; /*!< Odd prime, p=jq +1. */
  structure::Integer q_{}; /*!< Generator, g. */
  structure::Integer g_{}; /*!< Factor of p-1. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DSS_PARMS_H_
