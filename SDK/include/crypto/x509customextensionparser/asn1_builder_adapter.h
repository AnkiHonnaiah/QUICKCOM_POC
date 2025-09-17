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
/*!        \file  asn1_builder_adapter.h
 *        \brief  Adapter from VaAsn1 builder interface to ASN.1 callback interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_BUILDER_ADAPTER_H_
#define LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_BUILDER_ADAPTER_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/exceptions.h"

#include "amsr/asn1/asn1_builder.h"
#include "amsr/crypto/common/asn1_callback_interface.h"

namespace crypto {
namespace x509customextensionparser {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Adapter from VaAsn1 builder interface to ASN.1 callback interface.
 */
class Asn1BuilderAdapter final : public amsr::asn1::Asn1Builder {
 public:
  /*!
   * \brief Construct a new Asn1BuilderAdapter
   * \param[in] callback_interface A reference to the callback interface to adapt to
   */
  explicit Asn1BuilderAdapter(amsr::crypto::common::Asn1CallbackInterface& callback_interface) noexcept
      : amsr::asn1::Asn1Builder(), callback_interface_{callback_interface} {}

  /*!
   * \brief Destroy this Asn1BuilderAdapter instance.
   */
  ~Asn1BuilderAdapter() noexcept final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  Asn1BuilderAdapter(Asn1BuilderAdapter const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor.
   */
  Asn1BuilderAdapter(Asn1BuilderAdapter&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator for Asn1BuilderAdapter.
   */
  auto operator=(Asn1BuilderAdapter const&) & noexcept -> Asn1BuilderAdapter& = delete;

  /*!
   * \brief Deleted move assignment operator for Asn1BuilderAdapter.
   */
  auto operator=(Asn1BuilderAdapter&&) & noexcept -> Asn1BuilderAdapter& = delete;

  /*!
   * \brief Called when a sequence starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a sequence is not expected.
   * \vprivate Component Private
   */
  auto OnSequenceStart() noexcept -> amsr::core::Result<void> final { return callback_interface_.OnSequenceStart(); }

  /*!
   * \brief Called when a sequence ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a sequence is not expected.
   * \vprivate Component Private
   */
  auto OnSequenceEnd() noexcept -> amsr::core::Result<void> final { return callback_interface_.OnSequenceEnd(); }

  /*!
   * \brief Called when a set starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a set is not expected.
   * \vprivate Component Private
   */
  auto OnSetStart() noexcept -> amsr::core::Result<void> final { return callback_interface_.OnSetStart(); }

  /*!
   * \brief Called when a set ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a set is not expected.
   * \vprivate Component Private
   */
  auto OnSetEnd() noexcept -> amsr::core::Result<void> final { return callback_interface_.OnSetEnd(); }

  /*!
   * \brief Called when an External type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a external type is not expected.
   * \vprivate Component Private
   */
  auto OnExternalStart() noexcept -> amsr::core::Result<void> final { return Ignore(); }

  /*!
   * \brief Called when an External type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an External type is not expected.
   * \vprivate Component Private
   */
  auto OnExternalEnd() noexcept -> amsr::core::Result<void> final { return Ignore(); }

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
   * \vprivate Component Private
   */
  auto OnContextSpecificConstructedTypeStart(ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> final {
    return callback_interface_.OnContextSpecificConstructedTypeStart(context_specific_tag);
  }

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
   * \vprivate Component Private
   */
  auto OnContextSpecificConstructedTypeEnd(ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> final {
    return callback_interface_.OnContextSpecificConstructedTypeEnd(context_specific_tag);
  }

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
   * \vprivate Component Private
   */
  auto OnContextSpecificType(ContextSpecificTag context_specific_tag,
                             ContextSpecificData parsed_context_specific_data) noexcept
      -> amsr::core::Result<void> final {
    return callback_interface_.OnContextSpecificType(context_specific_tag, parsed_context_specific_data);
  }

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
   * \vprivate Component Private
   */
  auto OnApplicationConstructedTypeStart(ApplicationTag application_tag) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(application_tag);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnApplicationConstructedTypeEnd(ApplicationTag application_tag) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(application_tag);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnApplicationType(ApplicationTag application_tag, ApplicationData parsed_application_data) noexcept
      -> amsr::core::Result<void> final {
    static_cast<void>(application_tag);
    static_cast<void>(parsed_application_data);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnPrivateConstructedTypeStart(PrivateTag private_tag) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(private_tag);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnPrivateConstructedTypeEnd(PrivateTag private_tag) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(private_tag);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnPrivateType(PrivateTag private_tag, PrivateData parsed_private_data) noexcept
      -> amsr::core::Result<void> final {
    static_cast<void>(private_tag);
    static_cast<void>(parsed_private_data);
    return Ignore();
  }

  /*!
   * \brief Called when a Embedded-PDV type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Embedded-PDV type is not expected or has an invalid value.
   * \vprivate Component Private
   */
  auto OnEmbeddedPDVStart() noexcept -> amsr::core::Result<void> final { return Ignore(); }

  /*!
   * \brief Called when a Embedded-PDV type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Embedded-PDV type has an invalid value.
   * \vprivate Component Private
   */
  auto OnEmbeddedPDVEnd() noexcept -> amsr::core::Result<void> final { return Ignore(); }

  /*!
   * \brief Called when a Character String type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Character String is not expected or has an invalid value.
   * \vprivate Component Private
   */
  auto OnCharacterStringStart() noexcept -> amsr::core::Result<void> final { return Ignore(); }

  /*!
   * \brief Called when a Character String type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Character String has an invalid value.
   * \vprivate Component Private
   */
  auto OnCharacterStringEnd() noexcept -> amsr::core::Result<void> final { return Ignore(); }

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
   * \vprivate Component Private
   */
  auto OnBool(bool parsed_bool) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnBool(parsed_bool);
  }

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
   * \vprivate Component Private
   */
  auto OnInteger(Integer parsed_integer) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnInteger(parsed_integer);
  }

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
   * \vprivate Component Private
   */
  auto OnBitString(BitString parsed_bit_string) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnBitString(parsed_bit_string);
  }

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
   * \vprivate Component Private
   */
  auto OnOctetString(OctetString parsed_octet_string) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnOctetString(parsed_octet_string);
  }

  /*!
   * \brief Called when a NULL is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a NULL is not expected.
   * \vprivate Component Private
   */
  auto OnNull() noexcept -> amsr::core::Result<void> final { return callback_interface_.OnNull(); }

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
   * \vprivate Component Private
   */
  auto OnOid(Oid parsed_oid) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnOid(parsed_oid);
  }

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
   * \vprivate Component Private
   */
  auto OnReal(Real parsed_real) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_real);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnObjectDescriptor(ObjectDescriptor parsed_object_descriptor) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_object_descriptor);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnUtf8String(Utf8String parsed_utf8_string) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnUtf8String(parsed_utf8_string);
  }

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
   * \vprivate Component Private
   */
  auto OnROid(RelativeOid parsed_relative_oid) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_relative_oid);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnTime(Time parsed_time) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_time);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnNumericString(NumericString parsed_numeric_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_numeric_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnPrintableString(PrintableString parsed_printable_string) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnPrintableString(parsed_printable_string);
  }

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
   * \vprivate Component Private
   */
  auto OnT61String(T61String parsed_t61_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_t61_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnVideotexString(VideotexString parsed_videotex_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_videotex_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnVisibleString(VisibleString parsed_visible_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_visible_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnIa5String(Ia5String parsed_ia5_string) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnIa5String(parsed_ia5_string);
  }

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
   * \vprivate Component Private
   */
  auto OnGraphicString(GraphicString parsed_graphic_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_graphic_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnGeneralizedTime(GeneralizedTime parsed_generalized_time) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnGeneralizedTime(parsed_generalized_time);
  }

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
   * \vprivate Component Private
   */
  auto OnUtcTime(UtcTime parsed_utc_time) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnUtcTime(parsed_utc_time);
  }

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
   * \vprivate Component Private
   */
  auto OnDate(Date parsed_date) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_date);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnTimeOfDay(TimeOfDay parsed_time_of_day) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_time_of_day);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnDateTime(Date parsed_date_time) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_date_time);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnEnumerated(Enumerated parsed_enumerated) noexcept -> amsr::core::Result<void> final {
    return callback_interface_.OnEnumerated(parsed_enumerated);
  }

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
   * \vprivate Component Private
   */
  auto OnGeneralString(GeneralString parsed_general_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_general_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnUniversalString(UniversalString parsed_universal_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_universal_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnBMPString(BMPString parsed_bmp_string) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_bmp_string);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnDuration(Duration parsed_duration) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(parsed_duration);
    return Ignore();
  }

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
   * \vprivate Component Private
   */
  auto OnOIDInternationalized(OIDInternationalized parsed_oid_internationalized) noexcept
      -> amsr::core::Result<void> final {
    static_cast<void>(parsed_oid_internationalized);
    return Ignore();
  }

  /*!
   * \brief Called when n Relative OID Internationalized Resource Identifier is encountered.
   * \param[in] parsed_relative_oid_internationalized Parsed Relative OID Internationalized Resource Identifier value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Relative OID Internationalized Resource Identifier is not expected or has an invalid
   *        value.
   * \vprivate Component Private
   */
  auto OnRelativeOIDInternationalized(RelativeOIDInternationalized parsed_relative_oid_internationalized) noexcept
      -> amsr::core::Result<void> final {
    static_cast<void>(parsed_relative_oid_internationalized);
    return Ignore();
  }

  /*!
   * \brief Called when the type of the parsed data is unknown, as a last resort to save raw data.
   * \param[in] tlv The raw TLV (Tag, Length, Value) data of the unknown ASN.1 structure.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If the builder does not expect raw data at this point.
   * \vprivate Component Private
   */
  auto OnRawData(amsr::core::Span<std::uint8_t const> tlv) noexcept -> amsr::core::Result<void> final {
    static_cast<void>(tlv);
    return amsr::core::Result<void>::FromError(amsr::asn1::Asn1Errc::kRuntimeFault);
  }

  /*!
   * \brief Receives notification from child upon exiting final state.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If Accept is not expected.
   * \vprivate Component Private
   */
  auto Accept() noexcept -> amsr::core::Result<void> final { return Ignore(); }

  /*!
   * \brief Resets the builder to its initial state.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_external_side_effect
  auto Reset() noexcept -> void final {}

  /*!
   * \brief Check if current state is accepted state.
   * \return \c true if this builder finished all building, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsCompleted() const noexcept -> bool final { return true; }

 private:
  /*!
   * \brief Ignore a callback function.
   */
  static auto Ignore() noexcept -> amsr::core::Result<void> { return amsr::core::Result<void>::FromValue(); }

  /*! A reference to an instance of Asn1CallbackInterface */
  amsr::crypto::common::Asn1CallbackInterface& callback_interface_;
};

}  // namespace x509customextensionparser
}  // namespace crypto

#endif  //  LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_BUILDER_ADAPTER_H_
