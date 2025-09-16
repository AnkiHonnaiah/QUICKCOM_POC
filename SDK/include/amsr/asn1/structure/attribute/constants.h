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
/*!        \file  constants.h
 *        \brief  Constants concerning Attributes
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_CONSTANTS_H
#define AMSR_VECTOR_FS_VAASN1_CONSTANTS_H

#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {
namespace structure {
namespace attribute {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;
/*!
 *  OID for at-authenticationInfo.
 */
constexpr amsr::core::StringView kIdAcaAuthenticationInfo{"1.3.6.1.5.5.7.10.1"_sv};
/*!
 *  OID for at-name.
 */
constexpr amsr::core::StringView kIdAtName{"2.5.4.41"_sv};

/*!
 *  OID for at-surname.
 */
constexpr amsr::core::StringView kIdAtSurname{"2.5.4.4"_sv};

/*!
 *  OID for at-givenName.
 */
constexpr amsr::core::StringView kIdAtGivenName{"2.5.4.42"_sv};

/*!
 *  OID for at-initials.
 */
constexpr amsr::core::StringView kIdAtInitials{"2.5.4.43"_sv};

/*!
 *  OID for at-generationQualifier.
 */
constexpr amsr::core::StringView kIdAtGenerationQualifier{"2.5.4.44"_sv};

/*!
 *  OID for at-commonName.
 */
constexpr amsr::core::StringView kIdAtCommonName{"2.5.4.3"_sv};

/*!
 *  OID for at-x520LocalityName.
 */
constexpr amsr::core::StringView kIdAtX520LocalityName{"2.5.4.7"_sv};

/*!
 *  OID for at-x520StateOrProvinceName.
 */
constexpr amsr::core::StringView kIdAtX520StateOrProvinceName{"2.5.4.8"_sv};

/*!
 *  OID for at-x520OrganizationName.
 */
constexpr amsr::core::StringView kIdAtX520OrganizationName{"2.5.4.10"_sv};

/*!
 *  OID for at-x520OrganizationalUnitName.
 */
constexpr amsr::core::StringView kIdAtX520OrganizationalUnitName{"2.5.4.11"_sv};

/*!
 *  OID for at-title.
 */
constexpr amsr::core::StringView kIdAtTitle{"2.5.4.12"_sv};

/*!
 *  OID for at-dnQualifier.
 */
constexpr amsr::core::StringView kIdAtDnQualifier{"2.5.4.46"_sv};

/*!
 *  OID for at-countryName.
 */
constexpr amsr::core::StringView kIdAtCountryName{"2.5.4.6"_sv};

/*!
 *  OID for at-serialNumber.
 */
constexpr amsr::core::StringView kIdAtSerialNumber{"2.5.4.5"_sv};

/*!
 *  OID for at-pseudonym.
 */
constexpr amsr::core::StringView kIdAtPseudonym{"2.5.4.65"_sv};

/*!
 *  OID for at-houseIdentifier.
 */
constexpr amsr::core::StringView kIdAtHouseIdentifier{"2.5.4.51"_sv};

/*!
 *  OID for at-domainComponent.
 */
constexpr amsr::core::StringView kIdAtDomainComponent{"0.9.2342.19200300.100.1.25"_sv};

/*!
 *  OID for at-emailAddress.
 */
constexpr amsr::core::StringView kIdAtEmailAddress{"1.2.840.113549.1.9.1"_sv};

/*!
 *  OID for uid as per RFC 4519.
 */
constexpr amsr::core::StringView kIdRfc4519UserId{"0.9.2342.19200300.100.1.1"_sv};

/*!
 *  Upper bound <tt>ub-state-name</tt>.
 */
constexpr std::size_t kUbStateName{128u};

/*!
 *  Upper bound <tt>ub-organization-name</tt>.
 */
constexpr std::size_t kUbOrganizationName{64u};

/*!
 *  Upper bound <tt>ub-organizational-unit-name</tt>.
 */
constexpr std::size_t kUbOrganizationalUnitName{64u};

/*!
 *  Upper bound <tt>ub-title</tt>.
 */
constexpr std::size_t kUbTitle{64u};

/*!
 *  Upper bound <tt>ub-serial-number</tt>.
 */
constexpr std::size_t kUbSerialNumber{64u};

/*!
 *  Upper bound <tt>ub-pseudonym</tt>.
 */
constexpr std::size_t kUbPseudonym{128u};

/*!
 *  Upper bound <tt>ub-emailaddress-length</tt>.
 */
constexpr std::size_t kUbEmailaddressLength{255u};

/*!
 *  Upper bound <tt>ub-locality-name</tt>.
 */
constexpr std::size_t kUbLocalityName{128u};

/*!
 *  Upper bound <tt>ub-common-name</tt>.
 */
constexpr std::size_t kUbCommonName{64u};

/*!
 *  Upper bound <tt>ub-name</tt>.
 */
constexpr std::size_t kUbName{32768u};

/*!
 *  Upper bound <tt>uid</tt>. (Not specified, but synthetic. The RFCs are not clear on whether this is unbounded.
 */
constexpr std::size_t kUbRfc4519UserId{255u};

/*!
 * \brief Helper function for evaluating attribute type
 * \param oid The \c OBJECT \c IDENTIFIER of the attribute to check
 * \return \c true iff \c OBJECT \c IDENTIFIER maps to \c DirectoryString
 */
auto MapsToDirectoryString(amsr::core::StringView oid) -> bool;

/*!
 * \brief Helper function for evaluating attribute type
 * \param oid The \c OBJECT \c IDENTIFIER of the attribute to check
 * \return \c true iff \c OBJECT \c IDENTIFIER maps to \c PrintableString
 */
auto MapsToPrintableString(amsr::core::StringView oid) -> bool;

/*!
 * \brief Helper function for evaluating attribute type
 * \param oid The \c OBJECT \c IDENTIFIER of the attribute to check
 * \return \c true iff \c OBJECT \c IDENTIFIER maps to \c Ia5String
 */
auto MapsToIa5String(amsr::core::StringView oid) -> bool;

}  // namespace attribute
}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_CONSTANTS_H