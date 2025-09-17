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
/*!       \file   amsr/asn1/builder/constraint.h
 *        \brief  Base for constraint.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRAINT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRAINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include "amsr/asn1/asn1_builder.h"
#include "amsr/asn1/builder/transition.h"
#include "amsr/core/map.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Subtype constraint base.
 * \tparam Type Type to check
 */
template <typename Type>
class Constraint {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  Constraint() noexcept = default;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Constraint(Constraint const&) noexcept = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Constraint(Constraint&&) noexcept = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  auto operator=(Constraint&) noexcept -> Constraint& = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  auto operator=(Constraint&&) noexcept -> Constraint& = delete;

  /*!
   * \brief Checks if given input is accepted by constraint.
   * \param[in] value Value to be checked.
   * \return \c true if accepted, \c false otherwise.
   */
  virtual auto IsAccepted(Type const& value) const noexcept -> bool = 0;

  /*!
   * \brief Destructor.
   */
  virtual ~Constraint() noexcept = default;
};

/*!
 * \brief Value constraint.
 * \tparam Type Type to check
 */
template <typename Type>
class ValueConstraint : public Constraint<Type> {
 public:
  /*!
   * \brief Constructed form underlying type.
   * \param[in] values Values to be allowed.
   */
  explicit ValueConstraint(ara::core::Vector<Type> const& values) noexcept : Constraint<Type>{}, values_{values} {}

  /*!
   * \brief Checks if given input is accepted by constraint.
   * \param[in] value Value to be checked.
   * \return \c true if accepted, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsAccepted(Type const& value) const noexcept -> bool override {
    return (std::find(values_.cbegin(), values_.cend(), value) != values_.cend());
  }

 private:
  ara::core::Vector<Type> values_; /*!< List of values to be allowed. */
};

/*!
 * \brief Value range constraints.
 * \tparam Type Type to check
 */
template <typename Type>
class RangeConstraint : public Constraint<Type> {
 public:
  /*!
   * \brief Constructor.
   * \param[in] range Pair of (MIN, MAX) value.
   */
  explicit RangeConstraint(std::pair<Type, Type> const& range) noexcept : Constraint<Type>{}, range_{range} {
    if (range.first > range.second) {
      range_ = std::make_pair(range.second, range.first);
    }
  }

  /*!
   * \brief Checks if given input is accepted by constraint.
   * \param[in] value Value to be checked.
   * \return \c true if accepted, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsAccepted(Type const& value) const noexcept -> bool override {
    return ((value >= range_.first) && (value <= range_.second));
  }

 private:
  std::pair<Type, Type> range_; /*!< Range of value. */
};

/*!
 * \brief Size constraint.
 * \tparam Type Type to check
 */
template <typename Type>
class SizeConstraint : public Constraint<Type> {
 public:
  /*!
   * \brief Checks if given input is accepted by constraint.
   * \param[in] range Pair of (MIN, MAX) value.
   */
  explicit SizeConstraint(std::pair<std::uint64_t, std::uint64_t> const& range) noexcept
      : Constraint<Type>{}, range_{range} {
    if (range.first > range.second) {
      range_ = std::make_pair(range.second, range.first);
    }
  }

  /*!
   * \brief Checks if given input is accepted by constraint.
   * \param[in] value Value to be checked.
   * \return \c true if accepted, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsAccepted(Type const& value) const noexcept -> bool override {
    return ((value.size() >= range_.first) && (value.size() <= range_.second));
  }

 private:
  std::pair<std::uint64_t, std::uint64_t> range_; /*!< Range of value. */
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRAINT_H_
