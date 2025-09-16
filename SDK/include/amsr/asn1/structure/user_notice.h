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
/*!        \file  amsr/asn1/structure/user_notice.h
 *        \brief  Representation of parsed User notice structure.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_USER_NOTICE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_USER_NOTICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/display_text.h"
#include "amsr/asn1/structure/notice_reference.h"
#include "amsr/core/optional.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief User Notice Structure.
 */
class UserNotice {
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
  auto operator==(UserNotice const& other) const -> bool {
    return std::tie(NoticeRef(), ExplicitText()) == std::tie(other.NoticeRef(), other.ExplicitText());
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
  auto operator!=(UserNotice const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property NoticeRef.
   * \return Value of NoticeRef.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NoticeRef() const noexcept -> amsr::core::Optional<NoticeReference> const& { return notice_ref_; }

  /*!
   * \brief Access interface for property NoticeRef.
   * \return Value of NoticeRef.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NoticeRef() noexcept -> amsr::core::Optional<NoticeReference>& { return notice_ref_; }

  /*!
   * \brief Read-only access interface for property explicitText.
   * \return Value of explicitText.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExplicitText() const noexcept -> amsr::core::Optional<DisplayText> const& { return explicit_text_; }

  /*!
   * \brief Access interface for property explicitText.
   * \return Value of explicitText.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ExplicitText() noexcept -> amsr::core::Optional<DisplayText>& { return explicit_text_; }

  /*!
   * \brief Method to Encode UserNotice class.
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
  amsr::core::Optional<NoticeReference> notice_ref_{}; /*!< Notice reference Identifier. */
  amsr::core::Optional<DisplayText> explicit_text_{};  /*!< Explicit text Identifier. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_USER_NOTICE_H_
