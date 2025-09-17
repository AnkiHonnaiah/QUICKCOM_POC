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
/*!        \file  amsr/asn1/structure/edi_party_name.h
 *        \brief  Representation of parsed EDIPartyName(RFC5280).
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EDI_PARTY_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EDI_PARTY_NAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/directory_string.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief EDIPartyName Structure.
 */
class EDIPartyName {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(EDIPartyName const& other) const -> bool {
    return std::tie(NameAssigner(), PartyName()) == std::tie(other.NameAssigner(), other.PartyName());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(EDIPartyName const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property NameAssigner.
   * \return Value of NameAssigner.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NameAssigner() const noexcept -> amsr::core::Optional<structure::DirectoryString> const& {
    return name_assigner_;
  }

  /*!
   * \brief Access interface for property NameAssigner.
   * \return Value of NameAssigner.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NameAssigner() noexcept -> amsr::core::Optional<structure::DirectoryString>& { return name_assigner_; }

  /*!
   * \brief Read-only access interface for property partyName.
   * \return Value of partyName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PartyName() const noexcept -> structure::DirectoryString const& { return party_name_; }

  /*!
   * \brief Access interface for property partyName.
   * \return Value of partyName.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PartyName() noexcept -> structure::DirectoryString& { return party_name_; }

  /*!
   * \brief Method to encode EDIPartyName class.
   * \param[in] context_specific bool to check context specific or not
   * \return encoded data with header if it is not context specific, else return the encoded data without header.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidContent  If the content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  auto Encode(bool context_specific = false) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   *  ubMax from RFC 5912, used for the members of EDIPartyName.
   */
  static constexpr std::size_t kUbMax{32768u};

 private:
  // TODO(lwech): Same function is in AttributeEncoder -> move to a utility namespace.
  /*!
   * \brief Encodes DirectoryString class to encoded ASN.1 data.
   * \param[in] context_tag  context-specific tag
   * \param[in] directory_string  Directory string structure reference
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeDirectoryString(std::uint64_t context_tag,
                                    structure::DirectoryString const& directory_string) noexcept
      -> ara::core::Vector<std::uint8_t>;

  amsr::core::Optional<structure::DirectoryString> name_assigner_{}; /*!< NameAssigner Identifier. */
  structure::DirectoryString party_name_{};                          /*!< PartyName Identifier. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_EDI_PARTY_NAME_H_
