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
/*!       \file   amsr/asn1/builder/primitive_type_builder_state.h
 *        \brief  ASN.1 Builder for primitive base.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_PRIMITIVE_TYPE_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_PRIMITIVE_TYPE_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/builder/asn1_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief State of the builder's last result.
 * \details All builders are to implement \c Yield(). This method's core requirement is that it must never return
 *    incomplete data. For primitive values, a given value is valid exactly when it has been processed successfully.
 *    This enum allows for the primitive builder to track the state of the currently held data and to yield only if
 *    allowed to do so.
 */
enum class ResultStatus : std::uint8_t { kUninitialized, kValid, kInvalid };

/*!
 * \brief Builder state for primitive type base.
 */
template <class YieldedType>
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_builder_must_support_all_asn1_types
class PrimitiveTypeBuilderState : public Asn1BuilderState<YieldedType> {
 public:
  /*!
   * \brief Inheriting parent constructor.
   */
  using Asn1BuilderState<YieldedType>::Asn1BuilderState;

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
  auto OnSequenceStart() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnSequenceStart callback."_sv);
  }

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
  auto OnSequenceEnd() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnSequenceEnd callback."_sv);
  }

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
  auto OnSetStart() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnSetStart callback."_sv);
  }

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
  auto OnSetEnd() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnSetEnd callback."_sv);
  }

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
  auto OnExternalStart() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnExternalStart callback."_sv);
  }

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
  auto OnExternalEnd() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnExternalEnd callback."_sv);
  }

  /*!
   * \brief Called when a context specific constructed type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific constructed type is not expected.
   * \vprivate Product Private
   */
  auto OnContextSpecificConstructedTypeStart(Asn1Builder::ContextSpecificTag) noexcept
      -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnContextSpecificConstructedTypeStart callback."_sv);
  }

  /*!
   * \brief Called when a context specific constructed type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific constructed type is not expected.
   * \vprivate Product Private
   */
  auto OnContextSpecificConstructedTypeEnd(Asn1Builder::ContextSpecificTag) noexcept
      -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnContextSpecificConstructedTypeEnd callback."_sv);
  }

  /*!
   * \brief Called when a context specific type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnContextSpecificType(Asn1Builder::ContextSpecificTag, Asn1Builder::ContextSpecificData) noexcept
      -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnContextSpecificType callback."_sv);
  }

  /*!
   * \brief Called when an Application type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Application type is not expected.
   * \vprivate Product Private
   */
  auto OnApplicationConstructedTypeStart(Asn1Builder::ApplicationTag) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnApplicationConstructedTypeStart callback."_sv);
  }

  /*!
   * \brief Called when an Application type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Application type is not expected.
   * \vprivate Product Private
   */
  auto OnApplicationConstructedTypeEnd(Asn1Builder::ApplicationTag) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnApplicationConstructedTypeEnd callback."_sv);
  }

  /*!
   * \brief Called when an Application type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application class type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnApplicationType(Asn1Builder::ApplicationTag, Asn1Builder::ApplicationData) noexcept
      -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnApplicationType callback."_sv);
  }

  /*!
   * \brief Called when a private constructed type starts.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private constructed type is not expected.
   * \vprivate Product Private
   */
  auto OnPrivateConstructedTypeStart(Asn1Builder::PrivateTag) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnPrivateConstructedTypeStart callback."_sv);
  }

  /*!
   * \brief Called when a private constructed type ends.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private constructed type is not expected.
   * \vprivate Product Private
   */
  auto OnPrivateConstructedTypeEnd(Asn1Builder::PrivateTag) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnPrivateConstructedTypeEnd callback."_sv);
  }

  /*!
   * \brief Called when a private type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a private type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnPrivateType(Asn1Builder::PrivateTag, Asn1Builder::PrivateData) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnPrivateType callback."_sv);
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
   * \vprivate Product Private
   */
  auto OnEmbeddedPDVStart() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnEmbeddedPDVStart callback."_sv);
  }

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
  auto OnEmbeddedPDVEnd() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnEmbeddedPDVEnd callback."_sv);
  }

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
  auto OnCharacterStringStart() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnCharacterStringStart callback."_sv);
  }

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
  auto OnCharacterStringEnd() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnCharacterStringEnd callback."_sv);
  }

  /*!
   * \brief Called when a boolean is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a boolean is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnBool(bool) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnBool callback."_sv);
  }

  /*!
   * \brief Called when an integer is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an integer is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnInteger(Asn1Builder::Integer) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnInteger callback."_sv);
  }

  /*!
   * \brief Called when a bit string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a bit string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnBitString(Asn1Builder::BitString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnBitString callback."_sv);
  }

  /*!
   * \brief Called when an octet string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an octet string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnOctetString(Asn1Builder::OctetString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnOctetString callback."_sv);
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
   * \vprivate Product Private
   */
  auto OnNull() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnNull callback."_sv);
  }

  /*!
   * \brief Called when an OID is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an OID is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnOid(Asn1Builder::Oid) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnOid callback."_sv);
  }

  /*!
   * \brief Called when a real value is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a real is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnReal(Asn1Builder::Real) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnReal callback."_sv);
  }

  /*!
   * \brief Called when an Object Descriptor is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Object Descriptor is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnObjectDescriptor(Asn1Builder::ObjectDescriptor) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnObjectDescriptor callback."_sv);
  }

  /*!
   * \brief Called when an UTF8 string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an UTF8 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnUtf8String(Asn1Builder::Utf8String) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnUtf8String callback."_sv);
  }

  /*!
   * \brief Called when a Relative oid is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a relative oid is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnROid(Asn1Builder::RelativeOid) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnROid callback."_sv);
  }

  /*!
   * \brief Called when a time type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a time type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnTime(Asn1Builder::Time) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnTime callback."_sv);
  }

  /*!
   * \brief Called when a numeric string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a numeric string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnNumericString(Asn1Builder::NumericString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnNumericString callback."_sv);
  }
  /*!
   * \brief Called when a printable string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a printable string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnPrintableString(Asn1Builder::PrintableString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnPrintableString callback."_sv);
  }
  /*!
   * \brief Called when a T61 string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a T61 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnT61String(Asn1Builder::T61String) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnT61String callback."_sv);
  }

  /*!
   * \brief Called when an Videotex string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Videotex string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnVideotexString(Asn1Builder::VideotexString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnVideotexString callback."_sv);
  }

  /*!
   * \brief Called when an Visible string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Visible string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnVisibleString(Asn1Builder::VisibleString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnVisibleString callback."_sv);
  }

  /*!
   * \brief Called when an IA5 string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an IA5 string is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnIa5String(Asn1Builder::Ia5String) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnIa5String callback."_sv);
  }

  /*!
   * \brief Called when an Graphic string is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Graphic string is not expected or has an invalid value.
   * \vprivate Product Private
   */

  auto OnGraphicString(Asn1Builder::GraphicString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnGraphicString callback."_sv);
  }

  /*!
   * \brief Called when a generalized time is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a generalized time is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnGeneralizedTime(Asn1Builder::GeneralizedTime) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnGeneralizedTime callback."_sv);
  }

  /*!
   * \brief Called when a UTC time is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a UTC time is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnUtcTime(Asn1Builder::UtcTime) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnUtcTime callback."_sv);
  }

  /*!
   * \brief Called when an Date is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Date is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnDate(Asn1Builder::Date) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnDate callback."_sv);
  }

  /*!
   * \brief Called when an TimeOfDay type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an TimeOfDay type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnTimeOfDay(Asn1Builder::TimeOfDay) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnTimeOfDay callback."_sv);
  }

  /*!
   * \brief Called when an DateTime type is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an DateTime type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnDateTime(Asn1Builder::Date) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnDateTime callback."_sv);
  }

  /*!
   * \brief Called when an Enumerated is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an Enumerated is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnEnumerated(Asn1Builder::Enumerated) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnEnumerated callback."_sv);
  }

  /*!
   * \brief Called when a GeneralString is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a GeneralString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnGeneralString(Asn1Builder::GeneralString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnGeneralString callback."_sv);
  }
  /*!
   * \brief Called when a UniversalString is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a UniversalString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnUniversalString(Asn1Builder::UniversalString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnUniversalString callback."_sv);
  }

  /*!
   * \brief Called when a BMPString is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a BMPString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnBMPString(Asn1Builder::BMPString) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnBMPString callback."_sv);
  }

  /*!
   * \brief Called when a Duration is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Duration is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnDuration(Asn1Builder::Duration) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnDuration callback."_sv);
  }

  /*!
   * \brief Called when an OID Internationalized Resource Identifier is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an OID Internationalized Resource Identifier is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnOIDInternationalized(Asn1Builder::OIDInternationalized) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A primitive builder received an unexpected OnOIDInternationalized callback."_sv);
  }

  /*!
   * \brief Called when a Relative OID Internationalized Resource Identifier is encountered.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Relative OID Internationalized Resource Identifier is not expected or has an invalid
   *  value.
   * \vprivate Product Private
   */
  auto OnRelativeOIDInternationalized(Asn1Builder::RelativeOIDInternationalized) noexcept
      -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder received an unexpected OnRelativeOIDInternationalized callback."_sv);
  }

  /*!
   * \brief Called when raw data is encountered.
   * \error kRuntimeFault If a raw data is not expected or has an invalid
   *    value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto OnRawData(Asn1Builder::RawData) noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                               "A primitive builder received an unexpected OnRawData callback."_sv);
  }

  /*!
   * \brief Receives when child reaches accepted state.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If Accept is not expected.
   * \vprivate Product Private
   */
  auto Accept() noexcept -> amsr::core::Result<void> override {
    return amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A primitive builder does not handle its Accept call. This indicates incomplete code."_sv);
    // FIXME: Which brings up the question: Why is this not abstract/purely virtual?
  }
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_PRIMITIVE_TYPE_BUILDER_STATE_H_
