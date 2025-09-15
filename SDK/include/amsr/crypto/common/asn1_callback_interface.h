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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  amsr/crypto/common/asn1_callback_interface.h
 *        \brief Allows parsing DER encoded ASN.1 data
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_CALLBACK_INTERFACE_H_
#define LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_CALLBACK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>  // for signed and unsigned integer types
#include <utility>  // for utilities

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/mem_region.h"

namespace amsr {
namespace crypto {
namespace common {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief ASN.1 callback interface
 * \vpublic
 */
class Asn1CallbackInterface {
 public:
  /*!
   * \brief Default constructor.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Component Private
   */
  Asn1CallbackInterface() = default;

  /*!
   * \brief Destructs instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vpublic
   */
  virtual ~Asn1CallbackInterface() noexcept = default;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using Integer = ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Type alias used in bit string
   * \vpublic
   */
  using NumberOfUnusedBits = std::uint8_t;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using BitString = std::pair<ara::crypto::ReadOnlyMemRegion, NumberOfUnusedBits>;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using OctetString = ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using Oid = ara::core::StringView;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using Utf8String = ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using PrintableString = ara::core::StringView;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using Ia5String = ara::core::StringView;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using GeneralizedTime = ara::core::StringView;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using UtcTime = ara::core::StringView;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using Enumerated = ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using ContextSpecificData = ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Type alias
   * \vpublic
   */
  using ContextSpecificTag = std::uint8_t;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a sequence starts
   * \error SecurityErrorDomain::kRuntimeFault If a sequence is not expected
   * \vpublic
   */
  virtual auto OnSequenceStart() noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a sequence ends
   * \error SecurityErrorDomain::kRuntimeFault If a sequence is not expected
   * \vpublic
   */
  virtual auto OnSequenceEnd() noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a set starts
   * \error SecurityErrorDomain::kRuntimeFault If a set is not expected
   * \vpublic
   */
  virtual auto OnSetStart() noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a set ends
   * \error SecurityErrorDomain::kRuntimeFault If a set is not expected
   * \vpublic
   */
  virtual auto OnSetEnd() noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a context specific constructed type starts
   * \param[in] context_specific_tag Parsed tag
   * \error SecurityErrorDomain::kRuntimeFault If a context specific constructed type is not expected
   * \vpublic
   */
  virtual auto OnContextSpecificConstructedTypeStart(ContextSpecificTag context_specific_tag) noexcept
      -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a context specific constructed type ends
   * \param[in] context_specific_tag Parsed tag
   * \error SecurityErrorDomain::kRuntimeFault If a context specific constructed type is not expected
   * \vpublic
   */
  virtual auto OnContextSpecificConstructedTypeEnd(ContextSpecificTag context_specific_tag) noexcept
      -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a context specific type is encountered
   * \param[in] context_specific_tag Parsed tag
   * \param[in] parsed_context_specific_data Parsed data
   * \error SecurityErrorDomain::kRuntimeFaultIf a context specific type is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnContextSpecificType(ContextSpecificTag context_specific_tag,
                                     ContextSpecificData parsed_context_specific_data) noexcept
      -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a boolean is encountered
   * \param[in] parsed_bool Parsed boolean value
   * \error SecurityErrorDomain::kRuntimeFault If a boolean is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnBool(bool parsed_bool) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an integer is encountered
   * \param[in] parsed_integer Parsed integer value
   * \error SecurityErrorDomain::kRuntimeFault If an integer is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnInteger(Integer parsed_integer) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a bit string is encountered
   * \param[in] parsed_bit_string Parsed bit string value
   * \error SecurityErrorDomain::kRuntimeFault If a bit string is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnBitString(BitString parsed_bit_string) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an octet string is encountered
   * \param[in] parsed_octet_string Parsed octet string value
   * \error SecurityErrorDomain::kRuntimeFault If an octet string is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnOctetString(OctetString parsed_octet_string) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a NULL is encountered
   * \error SecurityErrorDomain::kRuntimeFault If a NULL is not expected
   * \vpublic
   */
  virtual auto OnNull() noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an oid is encountered
   * \param[in] parsed_oid Parsed oid value
   * \error SecurityErrorDomain::kRuntimeFault If an oid is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnOid(Oid parsed_oid) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an UTF8 string is encountered
   * \param[in] parsed_utf8_string Parsed UTF8 string value
   * \error SecurityErrorDomain::kRuntimeFault If an UTF8 string is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnUtf8String(Utf8String parsed_utf8_string) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a printable string is encountered
   * \param[in] parsed_printable_string Parsed printable string value
   * \error SecurityErrorDomain::kRuntimeFault If a printable string is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnPrintableString(PrintableString parsed_printable_string) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an IA5 string is encountered
   * \param[in] parsed_ia5_string Parsed IA5 string value
   * \error SecurityErrorDomain::kRuntimeFault If an IA5 string is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnIa5String(Ia5String parsed_ia5_string) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a generalized time is encountered
   * \param[in] parsed_generalized_time Parsed generalized time value
   * \error SecurityErrorDomain::kRuntimeFault If a generalized time is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnGeneralizedTime(GeneralizedTime parsed_generalized_time) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when a UTC time is encountered
   * \param[in] parsed_utc_time Parsed UTC time value
   * \error SecurityErrorDomain::kRuntimeFault If a UTC time is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnUtcTime(UtcTime parsed_utc_time) noexcept -> ara::core::Result<void> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Called when an Enumerated is encountered
   * \param[in] parsed_enumerated Parsed Enumerated value
   * \error SecurityErrorDomain::kRuntimeFault If an Enumerated is not expected or has an invalid value
   * \vpublic
   */
  virtual auto OnEnumerated(Enumerated parsed_enumerated) noexcept -> ara::core::Result<void> = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  Asn1CallbackInterface(Asn1CallbackInterface const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  Asn1CallbackInterface(Asn1CallbackInterface&& /*other*/) noexcept = default;

  /*!
   * \brief Default move assignment constructor
   * \vprivate Component Private
   */
  auto operator=(Asn1CallbackInterface&& /*other*/) & noexcept -> Asn1CallbackInterface& = default;

  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  auto operator=(Asn1CallbackInterface const& /*other*/) & -> Asn1CallbackInterface& = default;
};

}  // namespace common
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ASN1_CALLBACK_INTERFACE_H_
