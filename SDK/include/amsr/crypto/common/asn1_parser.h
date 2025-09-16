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
/*!       \file  amsr/crypto/common/asn1_parser.h
 *       \brief  ASN.1 parser interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_PARSER_H_
#define LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/crypto/common/asn1_callback_interface.h"
#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"

namespace amsr {
namespace crypto {
namespace common {

/*!
 * \brief ASN.1 Parser.
 * \vpublic
 */
class Asn1Parser {
 public:
  /*!
   * \brief Default constructor.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Component Private
   */
  Asn1Parser() = default;

  /*!
   * \brief Destructs instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vpublic
   */
  virtual ~Asn1Parser() noexcept = default;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<Asn1Parser>;

  /*!
   * \brief Parse DER encoded ASN.1 structure
   * \details For each parsed ASN.1 element, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] der_encoded_asn1_structure DER encoded ASN.1 structure to be parsed.
   * \param[in] asn1_callback_interface Callback interface.
   * \pre asn1_callback_interface != nullptr
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kInvalidArgument If \c asn1_callback_interface == nullptr
   * \error SecurityErrorDomain::kRuntimeFault If parsing fails or calling one of the callback fails.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Asn1Parse(
      ara::crypto::ReadOnlyMemRegion der_encoded_asn1_structure,
      std::unique_ptr<Asn1CallbackInterface> asn1_callback_interface) const noexcept = 0;

  /*!
   * \brief Parse DER encoded ASN.1 structure
   * \details For each parsed ASN.1 element, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] der_encoded_asn1_structure DER encoded ASN.1 structure to be parsed.
   * \param[in] asn1_callback_interface Callback interface.
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kRuntimeFault If parsing fails or calling one of the callback fails.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Asn1Parse(ara::crypto::ReadOnlyMemRegion der_encoded_asn1_structure,
                                            Asn1CallbackInterface& asn1_callback_interface) const noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  Asn1Parser(Asn1Parser const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  Asn1Parser(Asn1Parser&& /*other*/) noexcept = default;

  /*!
   * \brief Default move assignment constructor
   * \vprivate Component Private
   */
  auto operator=(Asn1Parser&& /*other*/) & noexcept -> Asn1Parser& = default;

  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  auto operator=(Asn1Parser const& /*other*/) & -> Asn1Parser& = default;
};

}  // namespace common
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_PARSER_H_
