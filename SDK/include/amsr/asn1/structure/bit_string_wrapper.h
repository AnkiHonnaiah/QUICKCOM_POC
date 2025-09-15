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
/*!        \file  amsr/asn1/structure/bit_string_wrapper.h
 *        \brief  Representation of parsed bit string.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BIT_STRING_WRAPPER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BIT_STRING_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/array.h"
#include "amsr/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Wrapper for BitString structure
 */
class BitStringWrapper {
 public:
  /*!
   * \brief Default destructor
   */
  virtual ~BitStringWrapper() = default;

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
  auto operator==(BitStringWrapper const& other) const -> bool { return Data() == other.Data(); }

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
  auto operator!=(BitStringWrapper const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read only access API for stored data.
   * \return Stored data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Data() const noexcept -> BitString const& { return data_; }

  /*!
   * \brief Read-only access API for stored data.
   * \return Stored data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Data() noexcept -> BitString& { return data_; }

  /*!
   * \brief Helper function for data extration.
   * \param[in] position Bitstring value received from Parser.
   * \return Stored bit value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean BitAt(std::uint8_t const position) const noexcept {
    Boolean result{false};
    if ((data_.first.size() * 8U) > position) {
      constexpr amsr::core::Array<std::uint8_t, 8> const kMask{0x80U, 0x40U, 0x20U, 0x10U, 0x08U, 0x04U, 0x02U, 0x01U};
      std::uint8_t const index{static_cast<std::uint8_t>(position >> 3U)};
      std::uint8_t const offset{static_cast<std::uint8_t>(position & 0x07U)};
      result = (data_.first.Data()[index] & kMask[offset]) == kMask[offset];
    }
    return result;
  }

  /*!
   * \brief Encodes BitStringWrapper class to encoded ASN.1 data.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode() const noexcept;

  /*!
   * \brief Encodes BitStringWrapper class to encoded ASN.1 data.
   * \param[in] context_specific If context-specific \c true, otherwise \c false.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  amsr::core::Result<ara::core::Vector<std::uint8_t>> Encode(bool context_specific) const noexcept;

 protected:
  /*!
   * \brief Default no-args constructor
   */
  BitStringWrapper() = default;
  /*!
   * \brief Default copy constructor
   */
  BitStringWrapper(BitStringWrapper const&) = default;
  /*!
   * \brief Default move constructor
   */
  BitStringWrapper(BitStringWrapper&&) noexcept = default;
  /*!
   * \brief Default copy assignment
   */
  auto operator=(BitStringWrapper const&) & -> BitStringWrapper& = default;
  /*!
   * \brief Default move assignment
   */
  auto operator=(BitStringWrapper&&) & noexcept -> BitStringWrapper& = default;

 private:
  BitString data_{}; /*!< Wrapped data. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BIT_STRING_WRAPPER_H_
