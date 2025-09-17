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
/*!        \file  amsr/asn1/structure/general_type.h
 *        \brief  Representation of general type.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_TYPE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <cstring>
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Represents general type.
 * \tparam Type Element type.
 */
template <typename Type>
class GeneralType {
 public:
  /*!
   * \brief Default constructor.
   */
  GeneralType() = default;

  /*!
   * \brief Default destructor.
   */
  virtual ~GeneralType() = default;

  /*!
   * \brief Copy constructor.
   * \param other The instance to copy.
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_raw_data_adds_nothing
  GeneralType(GeneralType const& other) = default;

  /*!
   * \brief Default move constructor.
   * \param other The instance to move.
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_raw_data_adds_nothing
  GeneralType(GeneralType&& other) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \param other The instance to copy.
   * \return \c *this
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_raw_data_adds_nothing
  auto operator=(GeneralType const& other) & -> GeneralType& = default;

  /*!
   * \brief Default move assignment operator.
   * \param other The instance to move.
   * \return \c *this
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_raw_data_adds_nothing
  auto operator=(GeneralType&& other) & noexcept -> GeneralType& = default;

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit GeneralType(ara::core::Vector<Type> const& other) : data_(other) {}

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit GeneralType(amsr::core::Span<Type> const& other) noexcept
      : GeneralType(ara::core::Vector<Type>{other.cbegin(), other.cend()}) {}

  /*!
   * \brief Constructor.
   * \param[in] other Other value.
   */
  explicit GeneralType(amsr::core::Span<Type const> const& other) noexcept
      : GeneralType(ara::core::Vector<Type>{other.cbegin(), other.cend()}) {}

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
  auto operator==(GeneralType<Type> const& other) const -> bool { return Data() == other.Data(); }

  /*!
   * \brief  Gets size of stored data.
   * \return Returns size of stored data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto size() const noexcept -> std::uint64_t { return data_.size(); }

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
  ara::core::Vector<Type>& Data() noexcept { return data_; }

 private:
  /*! \brief Internal representation of contained type. */
  ara::core::Vector<Type> data_{};
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_TYPE_H_
