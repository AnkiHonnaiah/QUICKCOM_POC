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
/*!        \file  response_data.h
 *        \brief  Representation of parsed ResponseData
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_H_
#define AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_H_

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/extension.h"
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/responder_id.h"
#include "amsr/asn1/structure/single_response.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief ResponseData Structure (RFC 6960).
 */
class ResponseData {
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
  auto operator==(ResponseData const& other) const -> bool {
    return std::tie(Version(), ResponderId(), ProducedAt(), Responses(), ResponseExtensions()) ==
           std::tie(other.Version(), other.ResponderId(), other.ProducedAt(), other.Responses(),
                    other.ResponseExtensions());
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
  auto operator!=(ResponseData const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property version.
   * \return Value of version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() const noexcept -> Integer const& { return version_; }
  /*!
   * \brief Access interface for property version.
   * \return Value of version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Version() noexcept -> Integer& { return version_; }
  /*!
   * \brief Read-only access interface for property responderID.
   * \return Value of responderID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponderId() const noexcept -> amsr::core::Variant<Name, OctetString> const& { return responder_id_; }
  /*!
   * \brief Access interface for property responderID.
   * \return Value of responderID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponderId() noexcept -> amsr::core::Variant<Name, OctetString>& { return responder_id_; }
  /*!
   * \brief Read-only access interface for property producedAt.
   * \return Value of producedAt.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ProducedAt() const noexcept -> GeneralizedTime const& { return produced_at_; }
  /*!
   * \brief Access interface for property producedAt.
   * \return Value of producedAt.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ProducedAt() noexcept -> GeneralizedTime& { return produced_at_; }
  /*!
   * \brief Read-only access interface for property responses.
   * \return Value of responses.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Responses() const noexcept -> ara::core::Vector<SingleResponse> const& { return responses_; }
  /*!
   * \brief Access interface for property responses.
   * \return Value of responses.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Responses() noexcept -> ara::core::Vector<SingleResponse>& { return responses_; }
  /*!
   * \brief Read-only access interface for property responseExtensions.
   * \return Value of responseExtensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponseExtensions() const noexcept -> amsr::core::Optional<ara::core::Vector<Extension>> const& {
    return response_extensions_;
  }
  /*!
   * \brief Access interface for property responseExtensions.
   * \return Value of responseExtensions.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ResponseExtensions() noexcept -> amsr::core::Optional<ara::core::Vector<Extension>>& {
    return response_extensions_;
  }
  /*!
   * \brief Method to encode ResponseData class.
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
  Integer version_{0x00};                                                  /*!< version */
  structure::ResponderId responder_id_;                                    /*!< responderId */
  GeneralizedTime produced_at_;                                            /*!< producetAt */
  ara::core::Vector<SingleResponse> responses_;                            /*!< responses */
  amsr::core::Optional<ara::core::Vector<Extension>> response_extensions_; /*!< responseExtensions */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // AMSR_VECTOR_FS_VAASN1_RESPONSE_DATA_H_
