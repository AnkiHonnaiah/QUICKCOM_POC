/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights Reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license Conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/asn1/structure/key_usage.h
 *        \brief  Representation of Key Usage extension Structure.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_KEY_USAGE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_KEY_USAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/bit_string_wrapper.h"
#include "ara/core/result.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Key Usage extension Structure.
 */
class KeyUsage final : public BitStringWrapper {
 public:
  /*!
   * \brief Default no-args constructor
   */
  KeyUsage() = default;
  /*!
   * \brief Default copy constructor
   */
  KeyUsage(KeyUsage const&) = default;
  /*!
   * \brief Default move constructor
   */
  KeyUsage(KeyUsage&&) noexcept = default;
  /*!
   * \brief Default copy assignment
   */
  auto operator=(KeyUsage const&) & -> KeyUsage& = default;
  /*!
   * \brief Default move assignment
   */
  auto operator=(KeyUsage&&) & noexcept -> KeyUsage& = default;
  /*!
   * \brief Default destructor
   */
  ~KeyUsage() final = default;

  /*!
   * \brief Read-only access API for DigitalSignature.
   * \return Stored DigitalSignature.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean DigitalSignature() const noexcept { return BitAt(0); }

  /*!
   * \brief Read-only access API for NonRepudiation.
   * \return Stored NonRepudiation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean NonRepudiation() const noexcept { return BitAt(1); }

  /*!
   * \brief Read-only access API for KeyEncipherment.
   * \return Stored KeyEncipherment.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean KeyEncipherment() const noexcept { return BitAt(2); }

  /*!
   * \brief Read-only access API for DataEncipherment.
   * \return Stored DataEncipherment.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean DataEncipherment() const noexcept { return BitAt(3); }

  /*!
   * \brief Read-only access API for KeyAgreement.
   * \return Stored KeyAgreement.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean KeyAgreement() const noexcept { return BitAt(4); }

  /*!
   * \brief Read-only access API for KeyCertSign.
   * \return Stored KeyCertSign.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean KeyCertSign() const noexcept { return BitAt(5); }

  /*!
   * \brief Read-only access API for CRLSign.
   * \return Stored CRLSign.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean CRLSign() const noexcept { return BitAt(6); }

  /*!
   * \brief Read-only access API for EncipherOnly.
   * \return Stored EncipherOnly.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean EncipherOnly() const noexcept { return BitAt(7); }

  /*!
   * \brief Read-only access API for DecipherOnly.
   * \return Stored DecipherOnly.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  Boolean DecipherOnly() const noexcept { return BitAt(8); }
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_KEY_USAGE_H_
