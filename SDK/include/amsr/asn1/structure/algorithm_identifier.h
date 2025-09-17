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
/*!        \file  amsr/asn1/structure/algorithm_identifier.h
 *        \brief  Representation of parsed Algorithm identifier.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ALGORITHM_IDENTIFIER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ALGORITHM_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"
#include "asn1_primitive_types.h"
#include "domain_parameters.h"
#include "dss_parms.h"
#include "ec_parameters.h"
#include "rsaes_oaep_params.h"
#include "rsassa_pss_params.h"

namespace amsr {
namespace asn1 {
namespace structure {

class AlgorithmIdentifier;

/*!
 * \brief Factory method for standard SHA-1 <tt>AlgorithmIdentifier</tt>s.
 * \return A default SHA1 \c AlgorithmIdentifier with Null parameters.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto ConstructSha1AlgorithmId() noexcept -> AlgorithmIdentifier;

/*!
 * \brief Factory method for MGF1 <tt>AlgorithmIdentifier</tt>s.
 * \return A default MGF1 \c AlgorithmIdentifier with SHA-1 for the hash function.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto ConstructMgf1Sha1AlgorithmId() noexcept -> AlgorithmIdentifier;

/*!
 * \brief Alias for algorithm parameter.
 */
using AIParameters = amsr::core::Variant<Null, OctetString, DomainParameters, EcParameters, RSASsaPssParams,
                                         RSAEsOaepParams, AlgorithmIdentifier, DssParms, RawData>;

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Algorithm identifier Structure.
 * \vprivate Product Private
 */
class AlgorithmIdentifier final {
 public:
  /*!
   * \brief Default constructor.
   * \vprivate Product Private
   */
  AlgorithmIdentifier();

  /*!
   * \brief Default destructor.
   * \vprivate Product Private
   */
  ~AlgorithmIdentifier();

  /*!
   * \brief Copy constructor.
   * \param other The instance to copy.
   * \vprivate Product Private
   */
  AlgorithmIdentifier(AlgorithmIdentifier const& other);

  /*!
   * \brief Default move constructor.
   * \param other The instance to move.
   * \vprivate Product Private
   */
  AlgorithmIdentifier(AlgorithmIdentifier&& other) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \param other The instance to copy.
   * \return \c *this
   * \vprivate Product Private
   */
  auto operator=(AlgorithmIdentifier const& other) & noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Default move assignment operator.
   * \param other The instance to move.
   * \return \c *this
   * \vprivate Product Private
   */
  auto operator=(AlgorithmIdentifier&& other) & noexcept -> AlgorithmIdentifier& = default;

  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   * \vprivate Product Private
   */
  auto operator==(AlgorithmIdentifier const& other) const -> bool;

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(AlgorithmIdentifier const& other) const -> bool;

  /*!
   * \brief Read-only access interface for property algorithm.
   * \return Value of algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Algorithm() const noexcept -> Oid const& { return algorithm_; }

  /*!
   * \brief Access interface for property algorithm.
   * \return Value of algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Algorithm() noexcept -> Oid& { return algorithm_; }

  /*!
   * \brief Read-only access interface for property parameters.
   * \return Value of parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Parameters() const noexcept -> amsr::core::Optional<AIParameters> const&;

  /*!
   * \brief Access interface for property parameters.
   * \return Value of parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Parameters() noexcept -> amsr::core::Optional<AIParameters>&;

  /*!
   * \brief Method to encode AlgorithmIdentifier class with a universal header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   *  OID for RSASsaPss.
   */
  static constexpr amsr::core::StringView kIdRsassaPss{"1.2.840.113549.1.1.10"_sv};

  /*!
   *  OID for RSAESOaep.
   */
  static constexpr amsr::core::StringView kIdRsaesOaep{"1.2.840.113549.1.1.7"_sv};

  /*!
   *  OID for DHPublicNumber.
   */
  static constexpr amsr::core::StringView kDhPublicNumber{"1.2.840.10046.2.1"_sv};

  /*!
   *  OID for KeyExchangeAlgorithm.
   */
  static constexpr amsr::core::StringView kIdKeyExchangeAlgorithm{"2.16.840.1.101.2.1.1.22"_sv};

  /*!
   *  OID for ECPublicKey.
   */
  static constexpr amsr::core::StringView kIdEcPublicKey{"1.2.840.10045.2.1"_sv};

  /*!
   *  OID for rsaOAEP-pSpecified
   */
  static constexpr amsr::core::StringView kIdpSpecified{"1.2.840.113549.1.1.9"_sv};

  /*!
   *  OID for MGF1
   */
  static constexpr amsr::core::StringView kIdMgf1{"1.2.840.113549.1.1.8"_sv};

  /*!
   *  OID for Dsa
   */
  static constexpr amsr::core::StringView kIdDsa{"1.2.840.10040.4.1"_sv};

  /*!
   *  OID for SHA-1
   */
  static constexpr amsr::core::StringView kIdSha1{"1.3.14.3.2.26"_sv};

  /*!
   *  OID for SHA-224
   */
  static constexpr amsr::core::StringView kIdSha224{"2.16.840.1.101.3.4.2.4"_sv};

  /*!
   *  OID for SHA-256
   */
  static constexpr amsr::core::StringView kIdSha256{"2.16.840.1.101.3.4.2.1"_sv};

  /*!
   *  OID for SHA-384
   */
  static constexpr amsr::core::StringView kIdSha384{"2.16.840.1.101.3.4.2.2"_sv};

  /*!
   *  OID for SHA-512
   */
  static constexpr amsr::core::StringView kIdSha512{"2.16.840.1.101.3.4.2.3"_sv};

  /*!
   *  OID for SHA-512/224
   */
  static constexpr amsr::core::StringView kIdSha512_224{"2.16.840.1.101.3.4.2.5"_sv};

  /*!
   *  OID for SHA-512/256
   */
  static constexpr amsr::core::StringView kIdSha512_256{"2.16.840.1.101.3.4.2.6"_sv};

  /*!
   *  OID for RSA encryption
   */
  static constexpr amsr::core::StringView kIdRsa{"1.2.840.113549.1.1.1"_sv};

  /*!
   *  OID for MD2
   */
  static constexpr amsr::core::StringView kIdMd2{"1.2.840.113549.2.2"_sv};

  /*!
   *  OID for MD5
   */
  static constexpr amsr::core::StringView kIdMd5{"1.2.840.113549.2.5"_sv};

  /*!
   *  OID for MD2 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdMd2Rsa{"1.2.840.113549.1.1.2"_sv};

  /*!
   *  OID for MD5 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdMd5Rsa{"1.2.840.113549.1.1.4"_sv};

  /*!
   *  OID for SHA-1 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha1Rsa{"1.2.840.113549.1.1.5"_sv};

  /*!
   *  OID for SHA-224 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha224Rsa{"1.2.840.113549.1.1.14"_sv};

  /*!
   *  OID for SHA-256 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha256Rsa{"1.2.840.113549.1.1.11"_sv};

  /*!
   *  OID for SHA-384 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha384Rsa{"1.2.840.113549.1.1.12"_sv};

  /*!
   *  OID for SHA-512 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha512Rsa{"1.2.840.113549.1.1.13"_sv};

  /*!
   *  OID for SHA-512/224 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha512_224Rsa{"1.2.840.113549.1.1.15"_sv};

  /*!
   *  OID for SHA-512/256 with RSA encryption
   */
  static constexpr amsr::core::StringView kIdSha512_256Rsa{"1.2.840.113549.1.1.16"_sv};

 private:
  /*!
   * \brief Method to encode AlgorithmIdentifier class without a header.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto EncodeWithoutHeader() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  Oid algorithm_{};                                                  /*!< OID for algorithm. */
  std::unique_ptr<amsr::core::Optional<AIParameters>> parameters_{}; /*!< Parameter required for algorithm. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ALGORITHM_IDENTIFIER_H_
