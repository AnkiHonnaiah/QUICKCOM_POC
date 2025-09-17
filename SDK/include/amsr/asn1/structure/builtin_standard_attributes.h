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
/*!        \file  amsr/asn1/structure/builtin_standard_attributes.h
 *        \brief  Representation of parsed builtin standard attributes.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_STANDARD_ATTRIBUTES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_STANDARD_ATTRIBUTES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/personal_name.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {
namespace ub {

/*!
 * \brief Upper bound for name
 */
constexpr std::uint64_t const kCountryNameAlpha{2};
/*!
 * \brief Upper bound for country
 */
constexpr std::uint64_t const kCountryNameNumeric{3};
/*!
 * \brief Upper bound for domain name
 */
constexpr std::uint64_t const kDomainName{16};
/*!
 * \brief Upper bound for X.121 address
 */
constexpr std::uint64_t const kX121Address{16};
/*!
 * \brief Upper bound for terminal ID
 */
constexpr std::uint64_t const kTerminalId{24};
/*!
 * \brief Upper bound for organization name
 */
constexpr std::uint64_t const kOrganizationName{64};
/*!
 * \brief Upper bound for numeric user ID
 */
constexpr std::uint64_t const kNumericUserId{32};
/*!
 * \brief Upper bound for organizational units
 */
constexpr std::uint64_t const kOrganizationalUnits{4};
/*!
 * \brief Upper bound for organizational unit name
 */
constexpr std::uint64_t const kOrganizationalUnitName{32};

}  // namespace ub
/*!
 * \brief Alias declaration.
 */
using CountryName = amsr::core::Variant<structure::PrintableString, structure::NumericString>;

/*!
 * \brief Alias declaration.
 */
using AdministrationDomainName = amsr::core::Variant<structure::NumericString, structure::PrintableString>;

/*!
 * \brief Alias declaration.
 */
using PrivateDomainName = amsr::core::Variant<structure::NumericString, structure::PrintableString>;

/*!
 * \brief BuiltIn Standard Attributes Structure.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_structure_has_many_members
class BuiltinStandardAttributes {
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
  auto operator==(BuiltinStandardAttributes const& other) const -> bool {
    return std::tie(CountryName(), AdministrationDomainName(), NetworkAddress(), TerminalIdentifier(),
                    PrivateDomainName(), OrganizationName(), NumericUserIdentifier(), PersonalName(),
                    OrganizationalUnitNames()) ==
           std::tie(other.CountryName(), other.AdministrationDomainName(), other.NetworkAddress(),
                    other.TerminalIdentifier(), other.PrivateDomainName(), other.OrganizationName(),
                    other.NumericUserIdentifier(), other.PersonalName(), other.OrganizationalUnitNames());
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
  auto operator!=(BuiltinStandardAttributes const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access API for Country Name.
   * \return Value of property Country Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CountryName() const noexcept -> amsr::core::Optional<structure::CountryName> const& { return country_name_; }

  /*!
   * \brief Access API for Country Name.
   * \return Value of property Country Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CountryName() noexcept -> amsr::core::Optional<structure::CountryName>& { return country_name_; }

  /*!
   * \brief Read-Only access API for Administration Domain Name.
   * \return Value of Administration Domain Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AdministrationDomainName() const noexcept -> amsr::core::Optional<structure::AdministrationDomainName> const& {
    return administration_domain_name_;
  }

  /*!
   * \brief Access API for Administration Domain Name.
   * \return Value of Administration Domain Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto AdministrationDomainName() noexcept -> amsr::core::Optional<structure::AdministrationDomainName>& {
    return administration_domain_name_;
  }

  /*!
   * \brief Read-Only access API for Network Address.
   * \return Value of Network Address.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NetworkAddress() const noexcept -> amsr::core::Optional<NumericString> const& { return network_address_; }

  /*!
   * \brief Access API for Network Address.
   * \return Value of Network Address.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NetworkAddress() noexcept -> amsr::core::Optional<NumericString>& { return network_address_; }

  /*!
   * \brief Read-Only access API for Terminal Identifier.
   * \return Value of Terminal Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TerminalIdentifier() const noexcept -> amsr::core::Optional<PrintableString> const& {
    return terminal_identifier_;
  }

  /*!
   * \brief Access API for Terminal Identifier.
   * \return Value of Terminal Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TerminalIdentifier() noexcept -> amsr::core::Optional<PrintableString>& { return terminal_identifier_; }

  /*!
   * \brief Read-Only access API for Private Domain Name.
   * \return Value of Private Domain Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PrivateDomainName() const noexcept -> amsr::core::Optional<structure::PrivateDomainName> const& {
    return private_domain_name_;
  }

  /*!
   * \brief Access API for Private Domain Name.
   * \return Value of Private Domain Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PrivateDomainName() noexcept -> amsr::core::Optional<structure::PrivateDomainName>& {
    return private_domain_name_;
  }

  /*!
   * \brief Read-Only access API for Organization Name.
   * \return Value of Organization Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OrganizationName() const noexcept -> amsr::core::Optional<PrintableString> const& { return organization_name_; }

  /*!
   * \brief Access API for Organization Name.
   * \return Value of Organization Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OrganizationName() noexcept -> amsr::core::Optional<PrintableString>& { return organization_name_; }

  /*!
   * \brief Read-Only access API for Numeric User Identifier.
   * \return Value of Numeric User Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NumericUserIdentifier() const noexcept -> amsr::core::Optional<NumericString> const& {
    return numeric_user_identifier_;
  }

  /*!
   * \brief Access API for Numeric User Identifier.
   * \return Value of Numeric User Identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NumericUserIdentifier() noexcept -> amsr::core::Optional<NumericString>& { return numeric_user_identifier_; }

  /*!
   * \brief Read-Only access API for Personal Name.
   * \return Value of Personal Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PersonalName() const noexcept -> amsr::core::Optional<structure::PersonalName> const& { return personal_name_; }

  /*!
   * \brief Access API for Personal Name.
   * \return Value of Personal Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PersonalName() noexcept -> amsr::core::Optional<structure::PersonalName>& { return personal_name_; }

  /*!
   * \brief Read-Only access API for Organizational Unit Name.
   * \return Value of Organizational Unit Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OrganizationalUnitNames() const noexcept -> amsr::core::Optional<ara::core::Vector<PrintableString>> const& {
    return organizational_unit_names_;
  }

  /*!
   * \brief Access API for Organizational Unit Name.
   * \return Value of Organizational Unit Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OrganizationalUnitNames() noexcept -> amsr::core::Optional<ara::core::Vector<PrintableString>>& {
    return organizational_unit_names_;
  }

  /*!
   * \brief Method to encode BuiltinStandardAttributes class.
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
  auto Encode(bool context_specific = false) const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  amsr::core::Optional<structure::CountryName> country_name_{}; /*!< Country Name. */
  amsr::core::Optional<structure::AdministrationDomainName>
      administration_domain_name_{};                                         /*!< Administration Domain Name. */
  amsr::core::Optional<NumericString> network_address_{};                    /*!< Network Address. */
  amsr::core::Optional<PrintableString> terminal_identifier_{};              /*!< Terminal Identifier. */
  amsr::core::Optional<structure::PrivateDomainName> private_domain_name_{}; /*!< Private Domain Name. */
  amsr::core::Optional<PrintableString> organization_name_{};                /*!< Organization Name. */
  amsr::core::Optional<NumericString> numeric_user_identifier_{};            /*!< Numeric User Identifier. */
  amsr::core::Optional<structure::PersonalName> personal_name_{};            /*!< Personal Name. */
  amsr::core::Optional<ara::core::Vector<PrintableString>>
      organizational_unit_names_{}; /*!< Organizational Unit Name. */

  /*!
   * \brief Method to encode CountryName.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeCountryName(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode AdministrationDomainName.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeAdministrationDomainName(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode PrivateDomainName.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodePrivateDomainName(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode organizational-unit-names.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeOrganizationalUnitNames(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode network-address.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeNetworkAddress(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode terminal-identifier.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeTerminalIdentifier(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode organization-name.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeOrganizationName(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode numeric-user-identifier.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodeNumericUserIdentifier(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Method to encode personal-name.
   * \param[out] encoded_data The place to insert the encoded data
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
  auto EncodePersonalName(ara::core::Vector<std::uint8_t>& encoded_data) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_BUILTIN_STANDARD_ATTRIBUTES_H_
