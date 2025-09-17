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
/*!        \file  amsr/asn1/structure/rsaes_oaep_params.h
 *        \brief  Representation of parsed Field ID for EC function.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSAES_OAEP_PARAMS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSAES_OAEP_PARAMS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Forward declaration.
 */
class AlgorithmIdentifier;

/*!
 * \brief RSAES OAEP parameter structure.
 */
class RSAEsOaepParams final {
  /*!
   * \brief Alias declaration.
   * \tparam T The type of data to point to
   */
  template <typename T>
  using Uptr = std::unique_ptr<T>;

 public:
  /*!
   * \brief Constructor
   */
  RSAEsOaepParams() noexcept;

  /*!
   * \brief Default destructor.
   */
  ~RSAEsOaepParams();

  /*!
   * \brief Copy constructor.
   * \param other The instance to copy.
   */
  RSAEsOaepParams(RSAEsOaepParams const& other);

  /*!
   * \brief Default move constructor.
   * \param other The instance to move.
   */
  RSAEsOaepParams(RSAEsOaepParams&& other) noexcept = default;

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
  auto operator=(RSAEsOaepParams const& other) & noexcept -> RSAEsOaepParams&;

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
  auto operator=(RSAEsOaepParams&& other) & noexcept -> RSAEsOaepParams& = default;

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
  auto operator==(RSAEsOaepParams const& other) const -> bool;

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
  auto operator!=(RSAEsOaepParams const& other) const -> bool;

  /*!
   * \brief Read-only access interface for property hash function.
   * \return Value of hash function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto HashFunction() const noexcept -> AlgorithmIdentifier const&;

  /*!
   * \brief Access interface for property hash function.
   * \return Value of hash function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto MaskGenFunction() noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Read-only access interface for property mask generation function.
   * \return Value of mask generation function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto MaskGenFunction() const noexcept -> AlgorithmIdentifier const&;

  /*!
   * \brief Access interface for property mask generation function.
   * \return Value of mask generation function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto HashFunction() noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Read-only access interface for property P Source Function.
   * \return Value of P Source Function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PSourceFunction() const noexcept -> AlgorithmIdentifier const&;

  /*!
   * \brief Access interface for property p source function.
   * \return Value of p source function.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto PSourceFunction() noexcept -> AlgorithmIdentifier&;

  /*!
   * \brief Method to encode RSAEsOaepParams class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

 private:
  Uptr<AlgorithmIdentifier> hash_func_;     /*!< Hash Function a. */
  Uptr<AlgorithmIdentifier> mask_gen_func_; /*!< Mask Generation Function. */
  Uptr<AlgorithmIdentifier> p_source_func_; /*!< Source Function. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_RSAES_OAEP_PARAMS_H_
