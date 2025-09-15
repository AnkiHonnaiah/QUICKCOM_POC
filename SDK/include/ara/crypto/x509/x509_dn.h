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
/*!        \file  ara/crypto/x509/x509_dn.h
 *        \brief  Interface of X.509 Distinguished Name (DN).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_DN_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_DN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Interface of X.509 Distinguished Name (DN).
 * \vpublic
 */
class X509DN : public ara::crypto::Serializable,
               public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface CustomDisposable
                                                       // (CustomDeleter requires interface)
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<X509DN, ara::crypto::CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the constant interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<X509DN const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Enumeration of DN attributes' identifiers.
   * \details Storage type: 8 bit unsigned integer.
   * \vpublic
   */
  enum class AttributeId : std::uint8_t {
    kCommonName = 0,           /*!< Common Name */
    kCountry = 1,              /*!< Country */
    kState = 2,                /*!< State */
    kLocality = 3,             /*!< Locality */
    kOrganization = 4,         /*!< Organization */
    kOrgUnit = 5,              /*!< Organization Unit */
    kStreet = 6,               /*!< Street */
    kPostalCode = 7,           /*!< Postal Code */
    kTitle = 8,                /*!< Title */
    kSurname = 9,              /*!< Surname */
    kGivenName = 10,           /*!< Given Name */
    kInitials = 11,            /*!< Initials */
    kPseudonym = 12,           /*!< Pseudonym */
    kGenerationQualifier = 13, /*!< Generation Qualifier */
    kDomainComponent = 14,     /*!< Domain Component */
    kDnQualifier = 15,         /*!< Distinguished Name Qualifier */
    kEmail = 16,               /*!< E-mail */
    kUri = 17,                 /*!< URI */
    kDns = 18,                 /*!< DNS */
    kHostName = 19,            /*!< Host Name (UNSTRUCTUREDNAME) */
    kIpAddress = 20,           /*!< IP Address (UNSTRUCTUREDADDRESS) */
    kSerialNumbers = 21,       /*!< Serial Numbers */
    kUserId = 22,              /*!< User ID */
    kHouseIdentifier = 23      /*!< House Identifier */
  };

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get the whole Distinguished Name (DN) as a single string.
   * \details Capacity of the output string must be enough for storing the output value!
   * \details If (dn == nullptr) then method only returns required buffer capacity.
   * \details       The DN will be truncated after 1024 characters.
   * \param[out] dn  the pointer to a string for storing whole DN value as a single string.
   * \return length of the whole DN string.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity if (dn != nullptr),
   *                   but dn->capacity() is less than required for the output value storing.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetDnString(ara::core::String* dn = nullptr) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set whole Distinguished Name (DN) from a single string.
   * \param[in] dn the single string containing the whole DN value in text format.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnexpectedValue if the dn string has incorrect syntax.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get DN attribute by its ID
   *        (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \details Capacity of the output string must be enough for storing the output value!
   * \details If (attribute == nullptr) then method only returns required buffer capacity.
   * \param[in] id  the identifier of required attribute.
   * \param[out] attribute  the pointer to a string for storing attribute value.
   * \return length of the attribute value (0 for empty attributes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier if the id argument has unsupported value.
   * \error SecurityErrorDomain::kInsufficientCapacity  if (attribute != nullptr),
   *                   but attribute->capacity() is less than required for storing of the output
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetAttribute(AttributeId id,
                                                      ara::core::String* attribute = nullptr) const noexcept = 0;

  /*!
   * \brief Set DN attribute by its ID
   *        (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \param[in] id  the identifier of required attribute.
   * \param[in] attribute  the attribute value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the id argument has unsupported value.
   * \error SecurityErrorDomain::kUnexpectedValue if the attribute string contains incorrect
   *                   characters or it has unsupported length.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Return DN attribute by its ID and sequential index
   *        (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \details Capacity of the output string must be enough for storing the output value!
   * \details If (attribute == nullptr) then method only returns required buffer capacity.
   * \param[in] id the identifier of required attribute.
   * \param[in] index the zero-based index of required component of the attribute.
   * \param[out] attribute the pointer to a string for storing attribute value.
   * \return length of the attribute value (0 for empty attributes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier if the id argument has unsupported value.
   * \error SecurityErrorDomain::kInsufficientCapacity if (attribute != nullptr),
   *                   but attribute->capacity() is less than required for storing of the output.
   * \error SecurityErrorDomain::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0)
   * \error SecurityErrorDomain::kAboveBoundary if
   *                   ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than or equal to the actual
   *                   number of components in the specified attribute.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetAttribute(AttributeId id, std::uint32_t index,
                                                      ara::core::String* attribute = nullptr) const noexcept = 0;

  /*!
   * \brief Set DN attribute by its ID and sequential index
   *        (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \param[in] id  the identifier of required attribute.
   * \param[in] index  the zero-based index of required component of the attribute.
   * \param[in] attribute  the attribute value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier if the id argument has unsupported value.
   * \error SecurityErrorDomain::kUnexpectedValue  if the attribute string contains incorrect
   *                   characters or it has unsupported length.
   * \error SecurityErrorDomain::kInvalidArgument if
   *                   (id != kOrgUnit) && (id != kDomainComponent) && (index > 0).
   * \error SecurityErrorDomain::kAboveBoundary if ((id == kOrgUnit) || (id == kDomainComponent)) and
   *                   the index value is greater than the current number of components in the specified attribute.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetAttribute(AttributeId id, std::uint32_t index,
                                               ara::core::StringView attribute) const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_DN_H_
