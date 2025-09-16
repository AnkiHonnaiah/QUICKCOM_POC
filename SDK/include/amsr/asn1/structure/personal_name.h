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
/*!        \file  amsr/asn1/structure/personal_name.h
 *        \brief  Representation of PersonalName.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_PERSONAL_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_PERSONAL_NAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

namespace ub {

/*!
 * \brief Upper bound for surname
 */
constexpr std::uint64_t const kSurname{40};

/*!
 * \brief Upper bound for givenName
 */
constexpr std::uint64_t const kGivenName{16};

/*!
 * \brief Upper bound for initials
 */
constexpr std::uint64_t const kInitials{5};

/*!
 * \brief Upper bound for generationQualifier
 */
constexpr std::uint64_t const kGenerationQualifier{3};
}  // namespace ub

/*!
 * \brief Personal Name Structure.
 */
class PersonalName {
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
  auto operator==(PersonalName const& other) const -> bool {
    return std::tie(Surname(), GivenName(), Initials(), GenerationQualifiers()) ==
           std::tie(other.Surname(), other.GivenName(), other.Initials(), other.GenerationQualifiers());
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
  auto operator!=(PersonalName const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access API for Surname.
   * \return Value of Surname.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Surname() const noexcept -> PrintableString const& { return surname_; };

  /*!
   * \brief Access API for Surname.
   * \return Value of Surname.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Surname() noexcept -> PrintableString& { return surname_; };

  /*!
   * \brief Read-only access API for Given Name.
   * \return Value of Given Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GivenName() const noexcept -> amsr::core::Optional<PrintableString> const& { return given_name_; };

  /*!
   * \brief Access API for Given Name.
   * \return Value of Given Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GivenName() noexcept -> amsr::core::Optional<PrintableString>& { return given_name_; };

  /*!
   * \brief Read-only access API for Initials.
   * \return Value of Initials.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Initials() const noexcept -> amsr::core::Optional<PrintableString> const& { return initials_; };

  /*!
   * \brief Access API for Initials.
   * \return Value of Initials.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Initials() noexcept -> amsr::core::Optional<PrintableString>& { return initials_; };

  /*!
   * \brief Read-only access API for Generation Qualifiers.
   * \return Value of Generation Qualifiers.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GenerationQualifiers() const noexcept -> amsr::core::Optional<PrintableString> const& {
    return generation_qualifier_;
  };

  /*!
   * \brief Access API for Generation Qualifiers.
   * \return Value of Generation Qualifiers.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GenerationQualifiers() noexcept -> amsr::core::Optional<PrintableString>& { return generation_qualifier_; };

  /*!
   * \brief Method to encode PersonalName class.
   * \param[in] context_specific If context-specific \c true, otherwise \c false.
   * \return Encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode(bool context_specific) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode PersonalName class with a universal header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>> { return Encode(false); }

 private:
  PrintableString surname_{};                                    /*!< Surname. */
  amsr::core::Optional<PrintableString> given_name_{};           /*!< Given Name. */
  amsr::core::Optional<PrintableString> initials_{};             /*!< Initials. */
  amsr::core::Optional<PrintableString> generation_qualifier_{}; /*!< Generation Qualifiers. */
};

}  // namespace structure

namespace context {
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::PersonalName>;
}  // namespace context

namespace validator {
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename PN>
class Validator<PN, context::EnableFor<PN, structure::PersonalName>>;
}  // namespace validator
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_PERSONAL_NAME_H_
