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
/*!        \file  amsr/asn1/structure/notice_reference.h
 *        \brief  Representation of parsed Notice Reference.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NOTICE_REFERENCE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NOTICE_REFERENCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/display_text.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Notice Reference Structure.
 */
class NoticeReference {
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
  auto operator==(NoticeReference const& other) const -> bool {
    return std::tie(Organization(), NoticeNumbers()) == std::tie(other.Organization(), other.NoticeNumbers());
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
  auto operator!=(NoticeReference const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property organization.
   * \return Value of organization.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Organization() const noexcept -> structure::DisplayText const& { return organization_; }

  /*!
   * \brief Access interface for property organization.
   * \return Value of organization.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Organization() noexcept -> structure::DisplayText& { return organization_; }

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
  auto NoticeNumbers() const noexcept -> ara::core::Vector<structure::Integer> const& { return notice_numbers_; }

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
  auto NoticeNumbers() noexcept -> ara::core::Vector<structure::Integer>& { return notice_numbers_; }

  /*!
   * \brief Method to encode NoticeReference class.
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
  structure::DisplayText organization_{};                  /*!< Organization Identifier. */
  ara::core::Vector<structure::Integer> notice_numbers_{}; /*!< NoticeNumbers Identifier. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NOTICE_REFERENCE_H_
