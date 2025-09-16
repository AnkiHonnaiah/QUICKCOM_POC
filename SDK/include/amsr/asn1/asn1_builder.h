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
/*!       \file   amsr/asn1/asn1_builder.h
 *        \brief  ASN.1 builder interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BUILDER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "asn1_error_domain.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief ASN.1 builder interface.
 * \trace DSGN-VaAsn1-Builder
 * \vprivate Product Private
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_builder_must_support_all_asn1_types
class Asn1Builder {
 public:
  /*!
   * \brief Constructor instance of class.
   * \param [in] parent Parent builder, or null for root-level builders.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Product Private
   */
  explicit Asn1Builder(Asn1Builder* parent = nullptr) : parent_(parent) {}

  /*!
   * \brief Explicitly deleted constructor.
   * \vprivate Product Private
   */
  Asn1Builder(Asn1Builder const&) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \vprivate Product Private
   */
  Asn1Builder(Asn1Builder&&) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1Builder& operator=(Asn1Builder&) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1Builder& operator=(Asn1Builder&&) = delete;

  /*!
   * \brief Destructs instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Product Private
   */
  virtual ~Asn1Builder() = default;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Integer = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using NumberOfUnusedBits = std::uint8_t;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using BitString = std::pair<amsr::core::Span<std::uint8_t const>, NumberOfUnusedBits>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using OctetString = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Oid = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Real = double;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using ObjectDescriptor = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Utf8String = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using RelativeOid = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Time = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using NumericString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using PrintableString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using T61String = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using VideotexString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using VisibleString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Ia5String = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using GraphicString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using GeneralizedTime = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using UtcTime = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Date = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using TimeOfDay = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using DateTime = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Enumerated = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using GeneralString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using UniversalString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using BMPString = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using Duration = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using ContextSpecificData = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using ApplicationData = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using PrivateData = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using RawData = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using OIDInternationalized = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using RelativeOIDInternationalized = amsr::core::StringView;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using ContextSpecificTag = std::uint8_t;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using ApplicationTag = std::uint8_t;

  /*!
   * \brief Type alias.
   * \vprivate Product Private
   */
  using PrivateTag = std::uint8_t;

  /*!
   * \brief Type alias.
   * \tparam T Type to be checked
   * \vprivate Product Private
   */
  template <typename T>
  using Uptr = std::unique_ptr<T>;

  /*!
   * \brief Type alias.
   * \tparam T Type to be checked
   * \vprivate Product Private
   */
  template <typename T>
  using Sptr = std::shared_ptr<T>;

  /*!
   * \brief Called when a sequence starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a sequence is not expected.
   * \vprivate Product Private
   */
  virtual auto OnSequenceStart() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a sequence ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a sequence is not expected.
   * \vprivate Product Private
   */
  virtual auto OnSequenceEnd() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a set starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a set is not expected.
   * \vprivate Product Private
   */
  virtual auto OnSetStart() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a set ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a set is not expected.
   * \vprivate Product Private
   */
  virtual auto OnSetEnd() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an External type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a external type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnExternalStart() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an External type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an External type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnExternalEnd() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a context specific constructed type starts.
   * \param[in] context_specific_tag Context specific tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific constructed type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnContextSpecificConstructedTypeStart(ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a context specific constructed type ends.
   * \param[in] context_specific_tag Context specific tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific constructed type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnContextSpecificConstructedTypeEnd(ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a context specific type is encountered.
   * \param[in] context_specific_tag Context specific tag.
   * \param[in] parsed_context_specific_data Context specific data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnContextSpecificType(ContextSpecificTag context_specific_tag,
                                     ContextSpecificData parsed_context_specific_data) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Application type starts.
   * \param[in] application_tag Application tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Application type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnApplicationConstructedTypeStart(ApplicationTag application_tag) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Application type ends.
   * \param[in] application_tag Application tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Application type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnApplicationConstructedTypeEnd(ApplicationTag application_tag) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Application type is encountered.
   * \param[in] application_tag Application tag.
   * \param[in] parsed_application_data Application type data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application class type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnApplicationType(ApplicationTag application_tag, ApplicationData parsed_application_data) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a private constructed type starts.
   * \param[in] private_tag Private tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private constructed type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnPrivateConstructedTypeStart(PrivateTag private_tag) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a private constructed type ends.
   * \param[in] private_tag Private tag.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private constructed type is not expected.
   * \vprivate Product Private
   */
  virtual auto OnPrivateConstructedTypeEnd(PrivateTag private_tag) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a private type is encountered.
   * \param[in] private_tag Private tag.
   * \param[in] parsed_private_data Private data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnPrivateType(PrivateTag private_tag, PrivateData parsed_private_data) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Embedded-PDV type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Embedded-PDV type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnEmbeddedPDVStart() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Embedded-PDV type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Embedded-PDV type has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnEmbeddedPDVEnd() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Character String type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Character String is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnCharacterStringStart() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Character String type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Character String has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnCharacterStringEnd() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a boolean is encountered.
   * \param[in] parsed_bool Parsed boolean value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a boolean is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnBool(bool parsed_bool) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an integer is encountered.
   * \param[in] parsed_integer Parsed integer value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an integer is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnInteger(Integer parsed_integer) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a bit string is encountered.
   * \param[in] parsed_bit_string Parsed bit string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a bit string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnBitString(BitString parsed_bit_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an octet string is encountered.
   * \param[in] parsed_octet_string Parsed octet string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an octet string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnOctetString(OctetString parsed_octet_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a NULL is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a NULL is not expected.
   * \vprivate Product Private
   */
  virtual auto OnNull() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an OID is encountered.
   * \param[in] parsed_oid Parsed OID value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an OID is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnOid(Oid parsed_oid) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a real value is encountered.
   * \param[in] parsed_real Parsed real value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a real is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnReal(Real parsed_real) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Object Descriptor is encountered.
   * \param[in] parsed_object_descriptor Parsed Object Descriptor value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Object Descriptor is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnObjectDescriptor(ObjectDescriptor parsed_object_descriptor) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an UTF8 string is encountered.
   * \param[in] parsed_utf8_string Parsed UTF8 string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an UTF8 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnUtf8String(Utf8String parsed_utf8_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Relative oid is encountered.
   * \param[in] parsed_relative_oid Parsed relative oid value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a relative oid is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnROid(RelativeOid parsed_relative_oid) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a time type is encountered.
   * \param[in] parsed_time Parsed numeric string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a time type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnTime(Time parsed_time) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a numeric string is encountered.
   * \param[in] parsed_numeric_string Parsed numeric string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a numeric string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnNumericString(NumericString parsed_numeric_string) noexcept -> amsr::core::Result<void> = 0;
  /*!
   * \brief Called when a printable string is encountered.
   * \param[in] parsed_printable_string Parsed printable string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a printable string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnPrintableString(PrintableString parsed_printable_string) noexcept -> amsr::core::Result<void> = 0;
  /*!
   * \brief Called when a T61 string is encountered.
   * \param[in] parsed_t61_string Parsed t61 string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a T61 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnT61String(T61String parsed_t61_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Videotex string is encountered.
   * \param[in] parsed_videotex_string Parsed VideoTex string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Videotex string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnVideotexString(VideotexString parsed_videotex_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Visible string is encountered.
   * \param[in] parsed_visible_string Parsed Visible string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Visible string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnVisibleString(VisibleString parsed_visible_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an IA5 string is encountered.
   * \param[in] parsed_ia5_string Parsed IA5 string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an IA5 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnIa5String(Ia5String parsed_ia5_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Graphic string is encountered.
   * \param[in] parsed_graphic_string Parsed Graphic string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Graphic string is not expected or has an invalid value.
   * \vprivate Product Private
   */

  virtual auto OnGraphicString(GraphicString parsed_graphic_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a generalized time is encountered.
   * \param[in] parsed_generalized_time Parsed generalized time value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a generalized time is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnGeneralizedTime(GeneralizedTime parsed_generalized_time) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a UTC time is encountered.
   * \param[in] parsed_utc_time Parsed UTC time value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a UTC time is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnUtcTime(UtcTime parsed_utc_time) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Date is encountered.
   * \param[in] parsed_date Parsed date value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Date is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnDate(Date parsed_date) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an TimeOfDay type is encountered.
   * \param[in] parsed_time_of_day Parsed time of the day value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an TimeOfDay type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnTimeOfDay(TimeOfDay parsed_time_of_day) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an DateTime type is encountered.
   * \param[in] parsed_date_time Parsed enumerated value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an DateTime type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnDateTime(Date parsed_date_time) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an Enumerated is encountered.
   * \param[in] parsed_enumerated Parsed enumerated value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Enumerated is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnEnumerated(Enumerated parsed_enumerated) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a GeneralString is encountered.
   * \param[in] parsed_general_string Parsed GeneralString value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a GeneralString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnGeneralString(GeneralString parsed_general_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a UniversalString is encountered.
   * \param[in] parsed_universal_string Parsed UniversalString value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a UniversalString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnUniversalString(UniversalString parsed_universal_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a BMPString is encountered.
   * \param[in] parsed_bmp_string Parsed BMPString value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a BMPString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnBMPString(BMPString parsed_bmp_string) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Duration is encountered.
   * \param[in] parsed_duration Parsed Duration value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Duration is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnDuration(Duration parsed_duration) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when an OID Internationalized Resource Identifier is encountered.
   * \param[in] parsed_oid_internationalized Parsed OID Internationalized Resource Identifier value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an OID Internationalized Resource Identifier is not expected or has an invalid value.
   * \vprivate Product Private
   */
  virtual auto OnOIDInternationalized(OIDInternationalized parsed_oid_internationalized) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when a Relative OID Internationalized Resource Identifier is encountered.
   * \param[in] parsed_relative_oid_internationalized Parsed Relative OID Internationalized Resource Identifier value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Relative OID Internationalized Resource Identifier is not expected or has an invalid
   *        value.
   * \vprivate Product Private
   */
  virtual auto OnRelativeOIDInternationalized(
      RelativeOIDInternationalized parsed_relative_oid_internationalized) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Called when the type of the parsed data is unknown, as a last resort to save raw data.
   * \param[in] tlv The raw TLV (Tag, Length, Value) data of the unknown ASN.1 structure.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If the builder does not expect raw data at this point.
   * \vprivate Product Private
   */
  virtual auto OnRawData(amsr::core::Span<std::uint8_t const> tlv) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Access interface for parent.
   * \return The parent builder, or nullptr for root-level builders.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto GetParent() const noexcept -> Asn1Builder* { return parent_; }

  /*!
   * \brief Receives notification from child upon exiting final state.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If Accept is not expected.
   * \vprivate Product Private
   */
  virtual auto Accept() noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Resets the builder to its initial state.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto Reset() noexcept -> void = 0;

  /*!
   * \brief Check if current state is accepted state.
   * \return \c true if this builder finished all building, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto IsCompleted() const noexcept -> bool = 0;

 private:
  Asn1Builder* parent_; /*!< Parent handle */
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BUILDER_H_
