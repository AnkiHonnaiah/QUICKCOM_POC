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
/*!        \file  amsr/asn1/structure/general_subtree.h
 *        \brief  Representation of parsed GeneralSubtree(RFC5280).
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_SUBTREE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_SUBTREE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/general_name.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief GeneralSubtree Structure.
 */
class GeneralSubtree {
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
  auto operator==(GeneralSubtree const& other) const -> bool {
    return std::tie(Base(), Minimum(), Maximum()) == std::tie(other.Base(), other.Minimum(), other.Maximum());
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
  auto operator!=(GeneralSubtree const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property Base.
   * \return Value of Base.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Base() const noexcept -> GeneralName const& { return base_; }

  /*!
   * \brief Access interface for property Base.
   * \return Value of Base.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Base() noexcept -> GeneralName& { return base_; }

  /*!
   * \brief Read-only access interface for property BaseDistance minimum.
   * \return Value of BaseDistance minimum.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Minimum() const noexcept -> Integer const& { return base_distance_min_; }

  /*!
   * \brief Access interface for property BaseDistance minimum.
   * \return Value of BaseDistance minimum.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Minimum() noexcept -> Integer& { return base_distance_min_; }

  /*!
   * \brief Read-only access interface for property BaseDistance maximum.
   * \return Value of BaseDistance maximum.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */

  auto Maximum() const noexcept -> amsr::core::Optional<Integer> const& { return base_distance_max_; }

  /*!
   * \brief Access interface for property BaseDistance maximum.
   * \return Value of BaseDistance maximum.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Maximum() noexcept -> amsr::core::Optional<Integer>& { return base_distance_max_; }

  /*!
   * \brief Method to encode GeneralSubtree class.
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
  GeneralName base_{};                                /*!< base. */
  Integer base_distance_min_{0x00};                   /*!< BaseDistanceMinimum*/
  amsr::core::Optional<Integer> base_distance_max_{}; /*!< BaseDistanceMaximum*/
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_SUBTREE_H_
