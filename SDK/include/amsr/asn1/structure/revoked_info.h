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
/*!        \file  revoked_info.h
 *        \brief  Representation of parsed UnknownInfo
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_REVOKED_INFO_H
#define AMSR_VECTOR_FS_VAASN1_REVOKED_INFO_H
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief RevokedInfo Structure (RFC 6960).
 */
class RevokedInfo {
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
  auto operator==(RevokedInfo const& other) const -> bool {
    return std::tie(RevocationTime(), RevocationReason()) == std::tie(other.RevocationTime(), other.RevocationReason());
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
  auto operator!=(RevokedInfo const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property revocationTime.
   * \return Value of revocationTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RevocationTime() const noexcept -> GeneralizedTime const& { return revocation_time_; }
  /*!
   * \brief Access interface for property revocationTime.
   * \return Value of revocationTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RevocationTime() noexcept -> GeneralizedTime& { return revocation_time_; }
  /*!
   * \brief Read-only access interface for property revocationReason.
   * \return Value of revocationReason.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RevocationReason() const noexcept -> amsr::core::Optional<Enumerated> const& { return revocation_reason_; }
  /*!
   * \brief Access interface for property revocationReason.
   * \return Value of revocationReason.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RevocationReason() noexcept -> amsr::core::Optional<Enumerated>& { return revocation_reason_; }
  /*!
   * \brief Method to encode RevokedInfo class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  GeneralizedTime revocation_time_;                    /*!< revocationTime */
  amsr::core::Optional<Enumerated> revocation_reason_; /*!< revocationReason */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // AMSR_VECTOR_FS_VAASN1_REVOKED_INFO_H
