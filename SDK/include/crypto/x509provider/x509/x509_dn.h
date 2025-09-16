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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  crypto/x509provider/x509/x509_dn.h
 *        \brief  Provides X.509 distinguished names.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_DN_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_DN_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/name.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/x509/x509_dn.h"
#include "crypto/common/util/logger.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace x509provider {
namespace x509 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
using vac::container::literals::operator"" _sv;

/*! Wildcard character for attribute lookup */
static constexpr vac::container::CStringView kWildcardCharacter{"*"_sv};

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief           X509DN implementation.
 * \vprivate        Component Private
 */
class X509DN : public ara::crypto::x509::X509DN {
 public:
  /*!
   * \brief         Digest size.
   * \vprivate      Component Private
   */
  static constexpr std::size_t kDigestSize{32};

  /*!
   * \brief         Digest container type.
   * \vprivate      Component Private
   */
  using DigestContainer = std::array<std::uint8_t, kDigestSize>;

  /*!
   * \brief         Unique smart pointer of the interface.
   * \vprivate      Component Private
   */
  using Uptr = std::unique_ptr<X509DN, ara::crypto::CustomDeleter>;

  /*!
   * \brief         Default constructor
   * \vprivate      Component Private
   */
  ~X509DN() noexcept override = default;

  /*!
   * \brief         Copy construct a new X509DN instance.
   * \param[in]     other The X509DN instance to copy from
   * \vprivate      Component Private
   */
  X509DN(X509DN const& other) noexcept;

  /*!
   * \brief         Move construct a X509DN.
   * \vprivate      Component Private
   */
  X509DN(X509DN&&) noexcept = default;

  /*!
   * \brief         Deleted copy assignment operator
   * \return        -
   * \vprivate      Component Private
   */
  auto operator=(X509DN const&) & -> X509DN& = delete;

  /*!
   * \brief         Default move assignment operator
   * \return        reference to this
   * \vprivate      Component Private
   */
  auto operator=(X509DN&&) & noexcept -> X509DN& = default;

  /*!
   * \brief         Release allocated memory and other resources.
   * \details       May be used only if the object's memory was allocated using the new operator.
                    The object must not be used once the method is called.
   * \context       ANY
   * \pre           The object's memory is allocated with the new operator.
                    The memory is not released.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void Release() noexcept override { delete this; };

  /*!
   * \brief         Create an X509DN.
   * \param[in]     crypto_provider Crypto provider to be used for internal computations
   * \return        A unique pointer to the created X509DN object
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if the \c crypto_provider is nullptr
   * \vprivate      Component Private
   */
  static auto Create(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider) noexcept -> Uptr {
    return Create(std::move(crypto_provider), amsr::asn1::structure::Name{}).Value();
  }

  // VECTOR NC AutosarC++17_10-M3.2.4: MD_CRYPTO_AutosarC++17_10-M3.2.4_type_without_definition_false_positive
  /*!
   * \brief         Create an X509DN.
   * \param[in]     crypto_provider Crypto provider to be used for internal computations
   * \param[in]     x509_dn_asn1 X509 DN ASN1 object
   * \return        A unique pointer to the created X509DN object
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if the parsed names invalid or \c crypto_provider is
   *                nullptr
   * \vprivate      Component Private
   */
  static auto Create(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                     amsr::asn1::structure::Name x509_dn_asn1) noexcept -> ara::core::Result<Uptr>;

  /*!
   * \brief         Create an X509DN.
   * \param[in]     crypto_provider Crypto provider to be used for internal computations
   * \param[in]     distinguished_name_der DER encoded Distinguished name to be parsed
   * \return        A unique pointer to the created X509DN object
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if the given data cannot be parsed, the parsed name is
   *                invalid or if the \c crypto_provider is nullptr
   * \vprivate      Component Private
   */
  static auto Create(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider,
                     ara::crypto::ReadOnlyMemRegion distinguished_name_der) noexcept -> ara::core::Result<Uptr>;

  /*!
   * \brief         Get the whole Distinguished Name (DN) as a single string.
   * \details       Capacity of the output string must be enough for storing the output value!
   * \details       If (dn == nullptr) then method only returns required buffer capacity
   * \details       The DN will be truncated after 1024 characters.
   * \param[out]    dn A pointer to a string for storing whole DN value as a single string
   * \return        length of the whole DN string
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetDnString(ara::core::String* dn) const noexcept -> ara::core::Result<std::size_t> override;

  /*!
   * \brief         Set whole Distinguished Name (DN) from a single string.
   * \details       Method not supported.
   * \param[in]     dn the single string containing the whole DN value in text format.
   * \return        A result that contains either a value or an error.
   * \error         ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \vprivate      Component Private
   */
  auto SetDn(ara::core::StringView dn) noexcept -> ara::core::Result<void> override;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief         Get DN attribute by its ID
   *                (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \details       Capacity of the output string must be enough for storing the output value!
   * \details       If (attribute == nullptr) then method only returns required buffer capacity.
   * \param[in]     id  the identifier of required attribute.
   * \param[out]    attribute  the pointer to a string for storing attribute value.
   * \return        length of the attribute value (0 for empty attributes).
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kUnknownIdentifier if the id argument has unsupported value.
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity if (attribute != nullptr),
   *                   but attribute->capacity() is less than required for storing of the output.
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0)
   * \error         ara::crypto::SecurityErrc::kAboveBoundary if
   *                   ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than or equal to the actual
   *                   number of components in the specified attribute.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault Unforeseen failure.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetAttribute(X509DN::AttributeId id, ara::core::String* attribute) const noexcept
      -> ara::core::Result<std::size_t> override;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief         Set DN attribute by its ID
   *                (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \param[in]     id  the identifier of required attribute.
   * \param[in]     attribute  the attribute value.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kUnknownIdentifier if the id argument has unsupported value.
   * \error         ara::crypto::SecurityErrc::kUnexpectedValue  if the attribute string contains incorrect
   *                   characters or it has unsupported length.
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0).
   * \error         ara::crypto::SecurityErrc::kAboveBoundary if ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than the current number of components in the specified attribute.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SetAttribute(X509DN::AttributeId id, ara::core::StringView attribute) const noexcept
      -> ara::core::Result<void> override;

  /*!
   * \brief         Return DN attribute by its ID and sequential index
   *                (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \details       Capacity of the output string must be enough for storing the output value!
   * \details       If (attribute == nullptr) then method only returns required buffer capacity.
   * \param[in]     id the identifier of required attribute.
   * \param[in]     index the zero-based index of required component of the attribute.
   * \param[out]    attribute the pointer to a string for storing attribute value.
   * \return        length of the attribute value (0 for empty attributes).
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kUnknownIdentifier if the id argument has unsupported value.
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity if (attribute != nullptr),
   *                   but attribute->capacity() is less than required for storing of the output.
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0)
   * \error         ara::crypto::SecurityErrc::kAboveBoundary if
   *                   ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than or equal to the actual
   *                   number of components in the specified attribute.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault Unforseen failure.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetAttribute(X509DN::AttributeId id, std::uint32_t index, ara::core::String* attribute) const noexcept
      -> ara::core::Result<std::size_t> override;

  /*!
   * \brief         Set DN attribute by its ID and sequential index
   *                (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \param[in]     id  the identifier of required attribute.
   * \param[in]     index  the zero-based index of required component of the attribute.
   * \param[in]     attribute  the attribute value.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kUnknownIdentifier if the id argument has unsupported value.
   * \error         ara::crypto::SecurityErrc::kUnexpectedValue  if the attribute string contains incorrect
   *                   characters or it has unsupported length.
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0).
   * \error         ara::crypto::SecurityErrc::kAboveBoundary if ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than the current number of components in the specified attribute.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto SetAttribute(X509DN::AttributeId id, std::uint32_t index, ara::core::StringView attribute) const noexcept
      -> ara::core::Result<void> override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief         Serialize itself publicly.
   * \param[out]    output The preallocated output buffer (it can be empty if only the required
   *                size of the output buffer is interested).
   * \param[in]     format_id The Crypto Provider specific identifier of the output format.
   * \return        Size required for storing of the output object.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kRuntimeFault Failed to encode the X.509 name
   * \error         ara::crypto::SecurityErrc::kInsufficientCapacity
   *                if (output.empty() == false), but its capacity is less than required
   * \error         ara::crypto::SecurityErrc::kUnsupportedFormat
   *                if the specified format ID is not supported for this object type
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<std::size_t> override;

  /*!
   * \brief         Compares two X509DN attribute values for equality.
   * \param[in]     lhs Left hand side of the comparison
   * \param[in]     rhs Right hand side of the comparison
   * \return        True iff the X509DN attribute strings are equal, other
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto CompareAttributeValue(amsr::core::StringView lhs, amsr::core::StringView rhs) noexcept -> bool;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief         Compares the given attribute of two X509DN objects under consideration of possible wildcards.
   * \param[in]     other Other X509DN object for the comparison.
   * \param[in]     id AttributeId of the attribute to compare.
   * \param[in]     index the zero-based index of required component of the attribute.
   *                (Relevant only for OrgUnit and DomainComponent attributes)
   * \return        True iff the attributes match, false otherwise.
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CompareAttributeWithWildcards(X509DN const& other, AttributeId id, std::uint32_t index = 0) const noexcept
      -> bool;

  /*!
   * \brief         Compares two X509DN objects under consideration of possible wildcards.
   * \param[in]     other Other X509DN object for the comparison.
   * \return        True iff the X509DN objects match, false otherwise.
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto CompareWithWildcards(X509DN const& other) const noexcept -> bool;

  /*!
   * \brief         Gets the digest.
   * \return        The digest for this instance.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto GetDigest() const noexcept -> DigestContainer const&;

  /*!
   * \brief         Equality operator for X509DN objects.
   * \details       Two X509DN are considered equal if they consist of equal RelativeDistinguishedName sequences, i.e.
   *                each RelativeDistinguishedName in the RDNSequence is equal to the RelativeDistinguishedName in the
   *                RDNSequence in \c rhs at the same index. Two RelativeDistinguishedNames are considered equal if both
   *                contain the same AttributeTypeAndValue elements, regardless of the order.
   * \param[in]     rhs Right hand side object for the comparison.
   * \return        true if both objects are considered equal, otherwise false.
   * \vprivate      Component Private
   */
  auto operator==(X509DN const& rhs) const noexcept -> bool;

  /*!
   * \brief         Inequality operator for X509DN objects.
   * \param[in]     rhs Right hand side object for the comparison.
   * \return        true if both objects are not equal, otherwise false.
   * \vprivate      Component Private
   */
  auto operator!=(X509DN const& rhs) const noexcept -> bool;

 private:
  /*!
   * \brief         POD representing a DN attribute and value.
   */
  struct AttributeTypeAndValue {
    AttributeId type{};         /*!< Attribute identifier (type) */
    amsr::core::String value{}; /*!< Attribute value */
  };

  /*! Alias for the internal structure for holding attributes */
  using AttributeContainer = std::vector<AttributeTypeAndValue>;

  /*!
   * \brief         Construct a new X509DN instance.
   * \param[in]     crypto_provider CryptoProvider object
   * \param[in]     x509_dn_asn1 X509 DN ASN1 object
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  X509DN(ara::crypto::cryp::CryptoProvider::Sptr crypto_provider, amsr::asn1::structure::Name x509_dn_asn1) noexcept;

  /*!
   * \brief         Initialize attribute container from ASN.1 name structure.
   * \param[in]     x509_dn_asn1 ASN.1 name structure
   * \param[in]     attribute_container Target container to write attribute values to
   * \return        -
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kInvalidArgument if the given X509 DN is invalid
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  static auto InitAttributes(amsr::asn1::structure::Name const& x509_dn_asn1,
                             AttributeContainer& attribute_container) noexcept -> ara::core::Result<void>;

  /*!
   * \brief Gets a vector of references to all attribute strings for given attribute id taken from given attribute
   *        container.
   * \param[in] attributes The source attribute container
   * \param[in] attribute_id The attribute id to look for
   * \return A vector containing references to all attribute string elements in given attribute container that match the
   *         given attribute id.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  static auto GetElementsByAttributeId(AttributeContainer& attributes, X509DN::AttributeId attribute_id) noexcept
      -> ara::core::Vector<std::reference_wrapper<ara::core::String>>;

  /*!
   * \brief         Handle changes to the attributes.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  void OnAttributesChanged() const noexcept;

  /*!
   * \brief         Update the digest internally, e.g. after the attributes have changed.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  void UpdateDigest() const noexcept;

  /*!
   * \brief         Escape the given string according to RFC2253.
   * \param[in]     string_to_escape The input string
   * \return        The escaped string
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto EscapeString(amsr::core::StringView string_to_escape) noexcept -> amsr::core::String;

  /*!
   * \brief         Extract the value as string
   * \param[in]     single_attribute The ASN.1 structure to extract the value from
   * \return        The value string
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \error         ara::crypto::SecurityErrc::kInvalidArgument
   *                If the value could not be extracted
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto ExtractAttributeValue(amsr::asn1::structure::SingleAttribute const& single_attribute) noexcept
      -> amsr::core::Result<amsr::core::String>;

  // VECTOR NC AutosarC++17_10-M3.2.4: MD_CRYPTO_AutosarC++17_10-M3.2.4_type_without_definition_false_positive
  /*!
   * \brief         Create DN string representation of given ASN1 object according to RFC2253.
   * \details        The string is truncated after 1024 characters.
   * \param[in]     x509_dn_asn1 An X509Dn ASN1 object
   * \return        A DN string
   * \context       ANY
   * \pre           \c x509_dn_asn1 contains only UTF8String or PrintableString attributes
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  static auto CreateDnString(amsr::asn1::structure::Name const& x509_dn_asn1) noexcept -> amsr::core::String;

  /*!
   * \brief       Convert an OID string to the corresponding AttributeId enumeration value.
   * \param[in]   oid_string OID string to map
   * \return      The corresponding AttributeId enumeration value
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \error       ara::crypto::SecurityErrc::kUnknownIdentifier
   *              If there is no matching enumeration value for given \c oid_string
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  static auto ToAttributeId(amsr::core::StringView oid_string) noexcept -> amsr::core::Result<AttributeId>;

  /*!
   * \brief       Convert an AttributeId enumeration value to the corresponding object identifier string.
   * \param[in]   attribute_id AttributeId to map
   * \return      The corresponding object identifier
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  static auto ToOidString(AttributeId attribute_id) noexcept -> amsr::core::StringView;

  /*!
   * \brief       Convert an OID string to the corresponding short name if applicable.
   * \param[in]   oid_string OID string to map
   * \return      The corresponding short name if applicable, otherwise \c oid_string itself
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  static auto ToShortName(amsr::core::StringView oid_string) noexcept -> amsr::core::StringView;

  /*! X509Dn ASN1 object */
  mutable amsr::asn1::structure::Name asn1_x509_dn_;
  /*! Memory for the digest of this DN object */
  mutable DigestContainer digest_;
  /*! Hash function used to compute the digest */
  ara::crypto::cryp::CryptoProvider::Sptr crypto_provider_;
  /*! Container containing all attributes of this DN */
  mutable AttributeContainer attributes_;
  /*! Logger instance */
  crypto::common::util::Logger logger_;

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, EscapeString);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, CreateDnString);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, CreateDnString__Truncated);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, ToAttributeId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, ToOidString);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__X509DN, ToShortName);
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_X509_DN_H_
