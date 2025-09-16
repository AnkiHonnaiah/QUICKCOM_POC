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
/*!        \file  ara/crypto/common/crypto_object_uid.h
 *        \brief  Definition of Crypto Object Unique Identifier (COUID) type.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \brief Definition of Crypto Object Unique Identifier (COUID) type.
 * \vpublic
 */
struct CryptoObjectUid final {
  // VECTOR NC AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  /*!
   * \brief UUID of a generator that has produced this COUID. This UUID can be associated with HSM, physical host/ECU or
   * VM.
   * \vpublic
   */
  Guid mGeneratorUid{};
  // VECTOR NC AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_in_standard
  /*!
   * \brief Sequential value of a steady timer or simple counter, representing version of correspondent Crypto Object.
   * \vpublic
   */
  std::uint64_t mVersionStamp{};

  /*!
   * \brief Initializes the identifier by zeros.
   * \vpublic
   */
  constexpr CryptoObjectUid() = default;

  /*!
   * \brief Default Destructor
   * \vprivate Component Private
   */
  ~CryptoObjectUid() noexcept = default;

  /*!
   * \brief Constructs object as copy of another object.
   * \vpublic
   */
  constexpr CryptoObjectUid(CryptoObjectUid const&) = default;

  /*!
   * \brief Creates object by moving another object.
   * \vpublic
   */
  constexpr CryptoObjectUid(CryptoObjectUid&&) noexcept = default;

  /*!
   * \brief Copies other object.
   * \return The copy.
   * \vpublic
   */
  constexpr CryptoObjectUid& operator=(CryptoObjectUid const&) & = default;

  /*!
   * \brief Moves other object.
   * \return The copy.
   * \vpublic
   */
  constexpr CryptoObjectUid& operator=(CryptoObjectUid&&) & noexcept = default;

  /*!
   * \brief Checks whether this identifier has a common source with the one provided by the argument.
   * \param[in] another_id Another identifier for comparison.
   * \return true if both identifiers has common source (identical value of the mGeneratorUid field).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr bool HasSameSourceAs(CryptoObjectUid const& another_id) const noexcept;

  /*!
   * \brief Checks whether this identifier was generated earlier than the one provided by the argument.
   * \param[in] another_id Another identifier for comparison.
   * \return true if this identifier was generated earlier than the another_id.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr bool HasEarlierVersionThan(CryptoObjectUid const& another_id) const noexcept;

  /*!
   * \brief Checks whether this identifier was generated later than the one provided by the argument.
   * \param[in] another_id Another identifier for comparison.
   * \return true if this identifier was generated later than the another_id.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr bool HasLaterVersionThan(CryptoObjectUid const& another_id) const noexcept;

  /*!
   * \brief check whether the generator's identifier is the "Nil UUID" (according to RFC4122).
   * \return true if this identifier is the "Nil UUID"
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr bool IsNil() const noexcept;
};

inline constexpr bool CryptoObjectUid::HasSameSourceAs(CryptoObjectUid const& another_id) const noexcept {
  return mGeneratorUid == another_id.mGeneratorUid;
}

inline constexpr bool CryptoObjectUid::HasEarlierVersionThan(CryptoObjectUid const& another_id) const noexcept {
  return HasSameSourceAs(another_id) && (mVersionStamp < another_id.mVersionStamp);
}

inline constexpr bool CryptoObjectUid::HasLaterVersionThan(CryptoObjectUid const& another_id) const noexcept {
  return HasSameSourceAs(another_id) && (mVersionStamp > another_id.mVersionStamp);
}

inline constexpr bool CryptoObjectUid::IsNil() const noexcept { return mGeneratorUid.IsNil(); }

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs == rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator==(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) {
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp == rhs.mVersionStamp);
}

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs < rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator<(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) {
  return lhs.HasEarlierVersionThan(rhs);
}

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs > rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator>(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) {
  return lhs.HasLaterVersionThan(rhs);
}

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs != rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator!=(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) { return !(lhs == rhs); }

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs <= rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator<=(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) {
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp <= rhs.mVersionStamp);
}

/*!
 * \brief Standard comparison operator with default behavior.
 * \param[in] lhs left hand side.
 * \param[in] rhs right hand side.
 * \return true if (lhs >= rhs).
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr bool operator>=(CryptoObjectUid const& lhs, CryptoObjectUid const& rhs) {
  return lhs.HasSameSourceAs(rhs) && (lhs.mVersionStamp >= rhs.mVersionStamp);
}

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H_
