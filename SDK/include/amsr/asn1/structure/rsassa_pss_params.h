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
/*!        \file  amsr/asn1/structure/rsassa_pss_params.h
 *        \brief  Representation of parsed RSASSA-PSS-params as per RFC 5912 (imported from RFC 4055)
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSASSA_PSS_PARAMS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSASSA_PSS_PARAMS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Forward declaration.
 */
class AlgorithmIdentifier;

/*!
 * \brief RSASSA-PSS-params structure as per RFC 5912.
 */
class RSASsaPssParams final {
 public:
  /*!
   * \brief Constructor
   */
  RSASsaPssParams() noexcept;

  /*!
   * \brief Default destructor.
   */
  ~RSASsaPssParams();

  /*!
   * \brief Copy constructor.
   * \param other The instance to copy.
   */
  RSASsaPssParams(RSASsaPssParams const& other);

  /*!
   * \brief Default move constructor.
   * \param other The instance to move.
   */
  RSASsaPssParams(RSASsaPssParams&& other) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \param other The instance to copy.
   * \return \c *this
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator=(RSASsaPssParams const& other) & noexcept -> RSASsaPssParams&;

  /*!
   * \brief Default move assignment operator.
   * \param other The instance to move.
   * \return \c *this
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator=(RSASsaPssParams&& other) & noexcept -> RSASsaPssParams& = default;

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
  auto operator==(RSASsaPssParams const& other) const -> bool;

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
  auto operator!=(RSASsaPssParams const& other) const -> bool;

  /*!
   * \brief Read-only access interface for property hash algorithm.
   * \return Value of hash algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto HashAlgorithm() const noexcept -> AlgorithmIdentifier const&;

  /*!
   * \brief Access interface for property hash algorithm.
   * \return Value of hash algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto MaskGenAlgorithm() noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Read-only access interface for property mask generation algorithm.
   * \return Value of mask generation algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto MaskGenAlgorithm() const noexcept -> AlgorithmIdentifier const&;

  /*!
   * \brief Access interface for property mask generation algorithm.
   * \return Value of mask generation algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto HashAlgorithm() noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Read-only access interface for property salt length.
   * \return Value of salt length.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SaltLength() const noexcept -> Integer const&;

  /*!
   * \brief Access interface for property salt length.
   * \return Value of salt length.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto SaltLength() noexcept -> Integer&;

  /*!
   * \brief Read-only access interface for property trailer field.
   * \return Value of trailer field.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TrailerField() const noexcept -> Integer const&;

  /*!
   * \brief Access interface for property trailer field.
   * \return Value of trailer field.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto TrailerField() noexcept -> Integer&;

  /*!
   * \brief Method to encode RSASsaPssParams class.
   * \return encoded data
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  std::unique_ptr<AlgorithmIdentifier> hash_algorithm_;     /*!< Hash Algorithm. */
  std::unique_ptr<AlgorithmIdentifier> mask_gen_algorithm_; /*!< Mask Generation Algorithm. */
  Integer salt_length_;                                     /*!< Salt length. */
  Integer trailer_field_;                                   /*!< Trailer field. */
};

}  // namespace structure

namespace context {
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::RSASsaPssParams>;
}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSASSA_PSS_PARAMS_H_
