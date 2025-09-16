/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights Reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license Conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/asn1/structure/reason_flags.h
 *        \brief  Representation of Reason Flags extension Structure.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_REASON_FLAGS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_REASON_FLAGS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/bit_string_wrapper.h"
#include "ara/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Reason Flags extension Structure.
 */
class ReasonFlags final : public BitStringWrapper {
 public:
  /*!
   * \brief Default no-args constructor
   */
  ReasonFlags() = default;
  /*!
   * \brief Default copy constructor
   */
  ReasonFlags(ReasonFlags const&) = default;
  /*!
   * \brief Default move constructor
   */
  ReasonFlags(ReasonFlags&&) noexcept = default;
  /*!
   * \brief Default copy assignment
   */
  auto operator=(ReasonFlags const&) & -> ReasonFlags& = default;
  /*!
   * \brief Default move assignment
   */
  auto operator=(ReasonFlags&&) & noexcept -> ReasonFlags& = default;
  /*!
   * \brief Default destructor
   */
  ~ReasonFlags() final = default;

  /*!
   * \brief Readonly access API for Unused.
   * \return Stored Unused.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean Unused() const noexcept { return BitAt(0); }

  /*!
   * \brief Readonly access API for KeyCompromise.
   * \return Stored KeyCompromise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean KeyCompromise() const noexcept { return BitAt(1); }

  /*!
   * \brief Readonly access API for KeyCompromise.
   * \return Stored KeyCompromise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean CACompromise() const noexcept { return BitAt(2); }

  /*!
   * \brief Readonly access API for AffiliationChanged.
   * \return Stored AffiliationChanged.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean AffiliationChanged() const noexcept { return BitAt(3); }

  /*!
   * \brief Readonly access API for Superseded.
   * \return Stored Superseded.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean Superseded() const noexcept { return BitAt(4); }

  /*!
   * \brief Readonly access API for CessationOfOperation.
   * \return Stored CessationOfOperation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean CessationOfOperation() const noexcept { return BitAt(5); }

  /*!
   * \brief Readonly access API for CertificateHold.
   * \return Stored CertificateHold.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean CertificateHold() const noexcept { return BitAt(6); }

  /*!
   * \brief Readonly access API for PrivilegeWithdrawn.
   * \return Stored PrivilegeWithdrawn.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean PrivilegeWithdrawn() const noexcept { return BitAt(7); }

  /*!
   * \brief Readonly access API for AACompromise.
   * \return Stored AACompromise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean AACompromise() const noexcept { return BitAt(8); }
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_REASON_FLAGS_H_
