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
/*!        \file  ocsp_response.h
 *        \brief  Representation of parsed OCSPResponse
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_H
#define AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_H

#include "amsr/core/optional.h"
#include "asn1_primitive_types.h"
#include "response_bytes.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief OCSPResponse Structure (RFC 6960).
 */
class OcspResponse {
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
  auto operator==(OcspResponse const& other) const -> bool {
    return std::tie(ResponseStatus(), RespBytes()) == std::tie(other.ResponseStatus(), other.RespBytes());
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
  auto operator!=(OcspResponse const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property responseStatus.
   * \return Value of responseStatus.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponseStatus() const noexcept -> Enumerated const& { return response_status_; }

  /*!
   * \brief Access interface for property responseStatus.
   * \return Value of responseStatus.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponseStatus() noexcept -> Enumerated& { return response_status_; }

  /*!
   * \brief Read-only access interface for property responseBytes.
   * \return Value of responseBytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RespBytes() const noexcept -> amsr::core::Optional<ResponseBytes> const& { return response_bytes_; }

  /*!
   * \brief Access interface for property responseBytes.
   * \return Value of responseBytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto RespBytes() noexcept -> amsr::core::Optional<ResponseBytes>& { return response_bytes_; }

  /*!
   * \brief Method to encode OcspResponse class.
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
  Enumerated response_status_;                         /*!< Enumerated data */
  amsr::core::Optional<ResponseBytes> response_bytes_; /*!< ResponseBytes data */
};
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_OCSP_RESPONSE_H
