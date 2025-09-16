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
/*!        \file  amsr/asn1/structure/numerical_type.h
 *        \brief  Representation of numerical type.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NUMERICAL_TYPE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NUMERICAL_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstring>
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Represents container type.
 * \tparam Type Element type
 */
template <typename Type>
class NumericalType {
  static_assert(std::is_integral<Type>(), "Type must be integral.");

 public:
  /*!
   * \brief Default constructor.
   */
  NumericalType() noexcept = default;

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit NumericalType(ara::core::Vector<Type> const& other) noexcept : data_(other) {}

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit NumericalType(amsr::core::Span<Type> const& other) noexcept
      : NumericalType(ara::core::Vector<Type>{other.cbegin(), other.cend()}) {}

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit NumericalType(amsr::core::Span<Type const> const& other) noexcept
      : NumericalType(ara::core::Vector<Type>{other.cbegin(), other.cend()}) {}

  /*!
   * \brief Constructor from single value.
   * \param[in] other Other value.
   */
  explicit NumericalType(Type const& other) noexcept : NumericalType(ara::core::Vector<Type>{other}) {}

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value.
   * \return \c true if same, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator==(NumericalType<Type> const& other) const noexcept -> bool { return Data() == other.Data(); }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value.
   * \return \c true if not same, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator!=(NumericalType<Type> const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value to compare.
   * \return \c true if this is less than \p other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator<(NumericalType<Type> const& other) const noexcept -> bool {
    bool is_lesser{false};
    if (data_.size() < other.data_.size()) {
      is_lesser = true;
    } else if (data_.size() == other.data_.size()) {
      is_lesser = std::memcmp(data_.data(), other.data_.data(), (data_.size() * sizeof(Type))) < 0;
    } else {
      // Nothing
    }
    return is_lesser;
  }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value to compare.
   * \return \c true if this is less than or equlal to \p other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator<=(NumericalType<Type> const& other) const noexcept -> bool {
    return (*this < other) || (*this == other);
  }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value to compare.
   * \return \c true if this is greater than or equal to \p other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator>=(NumericalType<Type> const& other) const noexcept -> bool { return !(*this < other); }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value to compare.
   * \return \c true if this is greater than \p other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator>(NumericalType<Type> const& other) const noexcept -> bool {
    return (*this >= other) && (*this != other);
  }

  /*!
   * \brief  Read-only access interface for date member.
   * \return Returns reference to stored data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Data() const noexcept -> ara::core::Vector<Type> const& { return data_; }

  /*!
   * \brief Access interface for date member.
   * \return Returns stored data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Data() noexcept -> ara::core::Vector<Type>& { return data_; }

 private:
  /*! \brief Internal representation of contained type. */
  ara::core::Vector<Type> data_{};
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NUMERICAL_TYPE_H_
