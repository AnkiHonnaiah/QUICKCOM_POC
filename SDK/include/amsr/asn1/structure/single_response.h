/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  single_response.h
 *        \brief  Representation of parsed SingleResponse
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_SINGLE_RESPONSE_H
#define AMSR_VECTOR_FS_VAASN1_SINGLE_RESPONSE_H

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "cert_id.h"
#include "cert_status.h"
#include "extension.h"
#include "revoked_info.h"
#include "unknown_info.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief SingleResponse Structure (RFC 6960).
 */
class SingleResponse {
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
  auto operator==(SingleResponse const& other) const -> bool {
    return std::tie(CertId(), CertStatus(), ThisUpdate(), NextUpdate(), SingleExtensions()) ==
           std::tie(other.CertId(), other.CertStatus(), other.ThisUpdate(), other.NextUpdate(),
                    other.SingleExtensions());
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
  auto operator!=(SingleResponse const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property certID.
   * \return Value of certID.
   */
  auto CertId() const noexcept -> structure::CertId const& { return cert_id_; }
  /*!
   * \brief Access interface for property certID.
   * \return Value of certID.
   */
  auto CertId() noexcept -> structure::CertId& { return cert_id_; }
  /*!
   * \brief Read-only access interface for property certStatus.
   * \return Value of certStatus.
   */
  auto CertStatus() const noexcept -> structure::CertStatus const& { return cert_status_; }
  /*!
   * \brief Access interface for property certStatus.
   * \return Value of certStatus.
   */
  auto CertStatus() noexcept -> structure::CertStatus& { return cert_status_; }
  /*!
   * \brief Read-only access interface for property thisUpdate.
   * \return Value of tbsResponseData.
   */
  auto ThisUpdate() const noexcept -> GeneralizedTime const& { return this_update_; }
  /*!
   * \brief Access interface for property thisUpdate.
   * \return Value of tbsResponseData.
   */
  auto ThisUpdate() noexcept -> GeneralizedTime& { return this_update_; }
  /*!
   * \brief Read-only access interface for property nextUpdate.
   * \return Value of tbsResponseData.
   */
  auto NextUpdate() const noexcept -> amsr::core::Optional<GeneralizedTime> const& { return next_update_; }
  /*!
   * \brief Access interface for property nextUpdate.
   * \return Value of tbsResponseData.
   */
  auto NextUpdate() noexcept -> amsr::core::Optional<GeneralizedTime>& { return next_update_; }
  /*!
   * \brief Read-only access interface for property singleExtensions.
   * \return Value of tbsResponseData.
   */
  auto SingleExtensions() const noexcept -> amsr::core::Optional<ara::core::Vector<Extension>> const& {
    return single_extensions_;
  }
  /*!
   * \brief Access interface for property singleExtensions.
   * \return Value of tbsResponseData.
   */
  auto SingleExtensions() noexcept -> amsr::core::Optional<ara::core::Vector<Extension>>& { return single_extensions_; }
  /*!
   * \brief Method to encode SingleResponse class.
   * \return encoded data.
   */
  auto Encode() const noexcept -> amsr::core::Result<ara::core::Vector<uint8_t>>;

 private:
  structure::CertId cert_id_;                                            /*!< certId */
  structure::CertStatus cert_status_;                                    /*!< certStatus */
  GeneralizedTime this_update_;                                          /*!< thisUpdate */
  amsr::core::Optional<GeneralizedTime> next_update_;                    /*!< nextUpdate */
  amsr::core::Optional<ara::core::Vector<Extension>> single_extensions_; /*!< singleExtensions */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_SINGLE_RESPONSE_H
