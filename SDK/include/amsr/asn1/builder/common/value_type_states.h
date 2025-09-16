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
/*!       \file   amsr/asn1/builder/common/value_type_states.h
 *        \brief  ASN.1 Builder for primitive type with stored value.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_VALUE_TYPE_STATES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_VALUE_TYPE_STATES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_der_parser.h"
#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/builder/constructed_type_builder_state.h"
#include "amsr/asn1/builder/primitive_type_builder_state.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Represents primitive type with stored value.
 * \tparam YieldedType The yielded data type, an ASN.1 primitive
 */
template <typename YieldedType>
class ValueTypeBuilderState : public PrimitiveTypeBuilderState<YieldedType> {
 public:
  /*!
   * \brief Alias for constraint type.
   */
  using CPType = YieldedType;

  using PrimitiveTypeBuilderState<YieldedType>::PrimitiveTypeBuilderState;

  /*!
   * \brief Sets constraint.
   * \param[in] constraint Constraint to be applied.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void SetConstraint(Asn1Builder::Sptr<Constraint<YieldedType>> constraint) noexcept { constraint_ = constraint; }

  /*!
   * \brief Checks if constraints are satisfied.
   * \return \c true if valid, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CheckConstraints() const noexcept -> bool {
    bool accepted{true};

    if (constraint_) {
      accepted = constraint_->IsAccepted(data_);
    }
    return accepted;
  }

  /*!
   * \brief Checks if constraints are satisfied.
   * \param[in] value Data received from parser.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If value is not accepted by constraint.
   * \vprivate Product Private
   */
  auto OnValue(YieldedType const& value) noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Internal unexpected failure during parsing of a primitive value."_sv)};
    data_ = value;
    if (CheckConstraints()) {
      result_status_ = ResultStatus::kValid;
      // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAASN1_AutosarC++17_10-M0.1.2_dead_branch_false_positive
      if (Asn1Builder* const parent{this->GetParent()}) {
        result = parent->Accept();
      } else {
        result = amsr::core::Result<void>::FromValue();
      }
    } else {
      result_status_ = ResultStatus::kInvalid;
      result.template EmplaceError(Asn1Errc::kConstraintCheckFail, "A primitive value violates its constraints.");
    }
    return result;
  }

  /*!
   * \brief Checks whether the builder has produced a valid primitive value.
   * \return \c true if this builder successfully built a primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsCompleted() const noexcept -> bool override { return result_status_ == ResultStatus::kValid; }

  /*!
   * \brief Returns the built data.
   * \return The built data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto DoYield() const noexcept -> amsr::core::Result<YieldedType> override { return data_; }

  /*!
   * \brief Erases primitive data and resets to uninitialized state.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Reset() noexcept -> void override {
    result_status_ = ResultStatus::kUninitialized;
    data_ = {};
  }

 private:
  Asn1Builder::Sptr<Constraint<YieldedType>> constraint_{};  /*!< Subtype constraint */
  ResultStatus result_status_{ResultStatus::kUninitialized}; /*!< Status of last builder call */
  YieldedType data_{};                                       /*!< Data to be built */
};

/*!
 * \brief Builder for Boolean type.
 */
class BooleanBuilderState : public ValueTypeBuilderState<structure::Boolean> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState<structure::Boolean>::ValueTypeBuilderState;

  /*!
   * \brief Handles boolean input from parser.
   * \param[in] parsed_bool Input value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If value is not accepted by constraint.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnBool(bool parsed_bool) noexcept override { return OnValue(parsed_bool); }

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(ContextSpecificTag,
                                                 ContextSpecificData parsed_context_specific_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A context-specifically tagged BOOLEAN contains more than one octet of payload."_sv)};
    if (parsed_context_specific_data.size() == 1) {
      result =
          OnValue(static_cast<structure::Boolean>(static_cast<structure::Boolean>(parsed_context_specific_data[0])));
    }
    return result;
  }

  /*!
   * \brief Called when an application type is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(ApplicationTag,
                                             ApplicationData parsed_application_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "An application-specifically tagged BOOLEAN contains more than one octet of payload."_sv)};
    if (parsed_application_data.size() == 1) {
      result = OnValue(static_cast<structure::Boolean>(static_cast<structure::Boolean>(parsed_application_data[0])));
    }
    return result;
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(PrivateTag, PrivateData parsed_private_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A privately tagged BOOLEAN contains more than one octet of payload."_sv)};
    if (parsed_private_data.size() == 1) {
      result = OnValue(static_cast<structure::Boolean>(static_cast<structure::Boolean>(parsed_private_data[0])));
    }
    return result;
  }
};

/*!
 * \brief Builder for NumericalType<uint8_t> type.
 */
class NumericalBuilderState : public ValueTypeBuilderState<structure::Integer> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState<structure::Integer>::ValueTypeBuilderState;

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(ContextSpecificTag,
                                                 ContextSpecificData parsed_context_specific_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "A context-specifically tagged numerical value is empty."_sv)};
    if (!parsed_context_specific_data.empty()) {
      result = OnValue(static_cast<structure::Integer>(
          ara::core::Vector<std::uint8_t>(parsed_context_specific_data.cbegin(), parsed_context_specific_data.cend())));
    }
    return result;
  }

  /*!
   * \brief Called when an application type is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(ApplicationTag,
                                             ApplicationData parsed_application_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "An application-specifically tagged numerical value is empty."_sv)};
    if (!parsed_application_data.empty()) {
      result = OnValue(static_cast<structure::Integer>(
          ara::core::Vector<std::uint8_t>(parsed_application_data.cbegin(), parsed_application_data.cend())));
    }
    return result;
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(PrivateTag, PrivateData parsed_private_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "A privately tagged numerical value is empty."_sv)};
    if (!parsed_private_data.empty()) {
      result = OnValue(static_cast<structure::Integer>(
          ara::core::Vector<std::uint8_t>(parsed_private_data.cbegin(), parsed_private_data.cend())));
    }
    return result;
  }
};

/*!
 * \brief Builder for Integer type.
 */
class IntegerBuilderState : public NumericalBuilderState {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using NumericalBuilderState::NumericalBuilderState;

  /*!
   * \brief Handles integer input from parser.
   * \param[in] parsed_integer Input value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If value is not accepted by constraint.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnInteger(Integer parsed_integer) noexcept override {
    return OnValue(structure::Integer{parsed_integer});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kInteger)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Enumerated type.
 */
class EnumeratedBuilderState : public NumericalBuilderState {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using NumericalBuilderState::NumericalBuilderState;

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
  amsr::core::Result<void> OnEnumerated(Enumerated parsed_enumerated) noexcept override {
    return OnValue(structure::Enumerated{parsed_enumerated});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kEnumerated)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for BitString type.
 */
class BitStringBuilderState : public ValueTypeBuilderState<structure::BitString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState<structure::BitString>::ValueTypeBuilderState;

  /*!
   * \brief Handles bit string input from parser.
   * \param[in] parsed_bit_string Input value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If value is not accepted by constraint.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnBitString(BitString parsed_bit_string) noexcept override {
    return OnValue(structure::BitString{std::make_pair(
        amsr::asn1::structure::GeneralType<std::uint8_t>{parsed_bit_string.first}, parsed_bit_string.second)});
  }

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(ContextSpecificTag,
                                                 ContextSpecificData parsed_context_specific_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "A context-specifically tagged BIT STRING contains less than one octet of payload."_sv)};
    if (!parsed_context_specific_data.empty()) {
      result = OnValue(structure::BitString{
          std::make_pair(structure::GeneralType<std::uint8_t>{parsed_context_specific_data.subspan(1)},
                         parsed_context_specific_data[0])});
    } else {
      // Return error by default.
    }
    return result;
  }

  /*!
   * \brief Called when a Application type is encountered.
   * \param[in] parsed_application_data Private data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(ApplicationTag,
                                             ApplicationData parsed_application_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "An application-specifically tagged BIT STRING contains less than one octet of payload."_sv)};
    if (!parsed_application_data.empty()) {
      result =
          OnValue(structure::BitString{std::make_pair(parsed_application_data.subspan(1), parsed_application_data[0])});
    } else {
      // Return error by default.
    }
    return result;
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(PrivateTag, PrivateData parsed_private_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "A privately tagged BIT STRING contains less than one octet of payload."_sv)};
    if (!parsed_private_data.empty()) {
      result = OnValue(structure::BitString{std::make_pair(parsed_private_data.subspan(1), parsed_private_data[0])});
    } else {
      // Return error by default.
    }
    return result;
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kBitString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Null types.
 * \details Since there can be CHOICEs of differently tagged NULL values, Null objects may have to be distinguished by
 *      type. This class template allows for builders that utilize said null types.
 * \tparam NullType A more specific null type.
 */
template <class NullType>
class SpecializedNullBuilderState : public ValueTypeBuilderState<NullType> {
 public:
  /*!
   * \brief Explicitly constrained constructor. This makes sure \c NullType inherits from \c structure::Null.
   * \param parent See \c ValueTypeBuilderState.
   */
  explicit SpecializedNullBuilderState(Asn1Builder* parent = nullptr) : ValueTypeBuilderState<NullType>(parent) {
    static_assert(std::is_base_of<structure::Null, NullType>::value, "NullType must inherit from Null");
  }

  /*!
   * \brief Handles null input from parser.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If value is not accepted by constraint.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnNull() noexcept override { return this->OnValue(NullType{}); }

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(
      Asn1Builder::ContextSpecificTag,
      Asn1Builder::ContextSpecificData parsed_context_specific_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "A context-specifically tagged NULL type is not empty."_sv)};
    if (parsed_context_specific_data.empty()) {
      result = this->OnValue(NullType{});
    }
    return result;
  }

  /*!
   * \brief Called when an application type is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(Asn1Builder::ApplicationTag,
                                             Asn1Builder::ApplicationData parsed_application_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "An application-specifically tagged NULL type is not empty."_sv)};
    if (parsed_application_data.empty()) {
      result = this->OnValue(NullType{});
    }
    return result;
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(Asn1Builder::PrivateTag,
                                         Asn1Builder::PrivateData parsed_private_data) noexcept override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "A privately tagged NULL type is not empty."_sv)};
    if (parsed_private_data.empty()) {
      result = this->OnValue(NullType{});
    }
    return result;
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Parser::Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kNull)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Null type.
 */
using NullBuilderState = SpecializedNullBuilderState<structure::Null>;

/*!
 * \brief Builder for GeneralType type.
 */
class GeneralTypeBuilderState : public ValueTypeBuilderState<structure::GeneralType<std::uint8_t>> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState<structure::GeneralType<std::uint8_t>>::ValueTypeBuilderState;

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(ContextSpecificTag,
                                                 ContextSpecificData parsed_context_specific_data) noexcept override {
    return OnValue(structure::GeneralType<std::uint8_t>{parsed_context_specific_data});
  }

  /*!
   * \brief Called when an application type is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(ApplicationTag,
                                             ApplicationData parsed_application_data) noexcept override {
    return OnValue(structure::GeneralType<std::uint8_t>{parsed_application_data});
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(PrivateTag, PrivateData parsed_private_data) noexcept override {
    return OnValue(structure::GeneralType<std::uint8_t>{parsed_private_data});
  }
};

/*!
 * \brief Builder for OctetString type.
 */
class OctetStringBuilderState : public GeneralTypeBuilderState {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using GeneralTypeBuilderState::GeneralTypeBuilderState;

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
  amsr::core::Result<void> OnOctetString(OctetString parsed_octet_string) noexcept override {
    return OnValue(structure::OctetString{parsed_octet_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kOctetString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for raw data (as fallback for ANY types).
 */
class RawDataBuilderState : public ValueTypeBuilderState<structure::RawData> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState::ValueTypeBuilderState;

  /*!
   * \brief Called when the type of the parsed data is unknown, as a last resort to save raw data.
   * \param[in] tlv The raw TLV (Tag, Length, Value) data of the unknown ASN.1 structure.
   * \error kRuntimeFault If the builder does not expect raw data at this point.
   * \vpublic
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnRawData(amsr::core::Span<std::uint8_t const> tlv) noexcept override {
    return OnValue(structure::RawData{tlv});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{ConstructedTypeBuilderState<structure::RawData>::kRawDataInput,
             ConstructedTypeBuilderState<structure::RawData>::kRawDataId}};
  }
};

/*!
 * \brief Builder for String type.
 */
template <typename YieldedType = amsr::core::String>
class StringBuilderState : public ValueTypeBuilderState<YieldedType> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState<YieldedType>::ValueTypeBuilderState;

  /*!
   * \brief Called when a context specific type is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(
      Asn1Builder::ContextSpecificTag,
      Asn1Builder::ContextSpecificData parsed_context_specific_data) noexcept override {
    return this->OnValue(
        YieldedType{amsr::core::String(parsed_context_specific_data.cbegin(), parsed_context_specific_data.cend())});
  }

  /*!
   * \brief Called when an application type is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If an application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(Asn1Builder::ApplicationTag,
                                             Asn1Builder::ApplicationData parsed_application_data) noexcept override {
    return this->OnValue(
        YieldedType{amsr::core::String(parsed_application_data.cbegin(), parsed_application_data.cend())});
  }

  /*!
   * \brief Called when a private type is encountered.
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
  amsr::core::Result<void> OnPrivateType(Asn1Builder::PrivateTag,
                                         Asn1Builder::PrivateData parsed_private_data) noexcept override {
    return this->OnValue(YieldedType{amsr::core::String(parsed_private_data.cbegin(), parsed_private_data.cend())});
  }
};

/*!
 * \brief Builder for T61String type.
 */
class T61StringBuilderState : public StringBuilderState<structure::T61String> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState<structure::T61String>::StringBuilderState;

  /*!
   * \brief Called when an T61String is encountered.
   * \param[in] parsed_t61_string Parsed T61String value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a T61String is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnT61String(Asn1Builder::T61String parsed_t61_string) noexcept override {
    return OnValue(structure::T61String{parsed_t61_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kT61String)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for NumericString type.
 */
class NumericStringBuilderState : public StringBuilderState<structure::NumericString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an NumericString is encountered.
   * \param[in] parsed_numeric_string Parsed NumericString value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a NumericString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnNumericString(NumericString parsed_numeric_string) noexcept override {
    return OnValue(structure::NumericString{parsed_numeric_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kNumericString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for PrintableString type.
 */
class PrintableStringBuilderState : public StringBuilderState<structure::PrintableString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an PrintableString is encountered.
   * \param[in] parsed_printable_string Parsed PrintableString value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a PrintableString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnPrintableString(PrintableString parsed_printable_string) noexcept override {
    return OnValue(structure::PrintableString{parsed_printable_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kPrintableString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for UniversalString type.
 */
class UniversalStringBuilderState : public StringBuilderState<structure::UniversalString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an UniversalString is encountered.
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
  amsr::core::Result<void> OnUniversalString(UniversalString parsed_universal_string) noexcept override {
    return OnValue(structure::UniversalString{parsed_universal_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kUniversalString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for BMPString type.
 */
class BMPStringBuilderState : public StringBuilderState<structure::BMPString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an BMPString is encountered.
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
  amsr::core::Result<void> OnBMPString(BMPString parsed_bmp_string) noexcept override {
    return OnValue(structure::BMPString{parsed_bmp_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kBMPString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Utf8String type.
 */
class Utf8StringBuilderState : public GeneralTypeBuilderState {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using GeneralTypeBuilderState::GeneralTypeBuilderState;

  /*!
   * \brief Called when an Utf8String is encountered.
   * \param[in] parsed_utf8_string Parsed Utf8String value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a Utf8String is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnUtf8String(Utf8String parsed_utf8_string) noexcept override {
    return OnValue(structure::Utf8String{parsed_utf8_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kUTF8String)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Oid type.
 */
class OidBuilderState : public ValueTypeBuilderState<structure::Oid> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using ValueTypeBuilderState::ValueTypeBuilderState;

  /*!
   * \brief Called when a universal OID is encountered.
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
  amsr::core::Result<void> OnOid(Oid parsed_oid) noexcept override { return OnValue(structure::Oid{parsed_oid}); }

  /*!
   * \brief Called when a context specific OID is encountered.
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
  amsr::core::Result<void> OnContextSpecificType(
      Asn1Builder::ContextSpecificTag,
      Asn1Builder::ContextSpecificData parsed_context_specific_data) noexcept override {
    return Asn1DERParser::OidToDotNotationString(parsed_context_specific_data)
        .AndThen([this](amsr::core::String const& oid_dot_notation) {
          return this->OnValue(structure::Oid{oid_dot_notation});
        });
  }

  /*!
   * \brief Called when an application OID is encountered.
   * \param[in] parsed_application_data Application data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a application type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnApplicationType(Asn1Builder::ApplicationTag,
                                             Asn1Builder::ApplicationData parsed_application_data) noexcept override {
    return Asn1DERParser::OidToDotNotationString(parsed_application_data)
        .AndThen([this](amsr::core::String const& oid_dot_notation) {
          return this->OnValue(structure::Oid{oid_dot_notation});
        });
  }

  /*!
   * \brief Called when a private OID is encountered.
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
  amsr::core::Result<void> OnPrivateType(Asn1Builder::PrivateTag,
                                         Asn1Builder::PrivateData parsed_private_data) noexcept override {
    return Asn1DERParser::OidToDotNotationString(parsed_private_data)
        .AndThen([this](amsr::core::String const& oid_dot_notation) {
          return this->OnValue(structure::Oid{oid_dot_notation});
        });
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kObjectIdentifier)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for UTCTime type.
 */
class UtcTimeBuilderState : public StringBuilderState<structure::UtcTime> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an UTCTime is encountered.
   * \param[in] parsed_utc_time Parsed TIME value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a TIME is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnUtcTime(UtcTime parsed_utc_time) noexcept override {
    return OnValue(structure::UtcTime{parsed_utc_time});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kUTCTime)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for GeneralizedTime type.
 */
class GeneralizedTimeBuilderState : public StringBuilderState<structure::GeneralizedTime> {
 public:
  /*!
   *\brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when an GeneralizedTime is encountered.
   * \param[in] parsed_generalized_time Parsed TIME value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kInvalidContent If the generalized time contains an invalid value.
   * \error kRuntimeFault If a generalized time is not expected at this location.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnGeneralizedTime(GeneralizedTime parsed_generalized_time) noexcept override {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
    structure::GeneralizedTime const converted_generalized_time{parsed_generalized_time};
    if (converted_generalized_time.IsValid()) {
      retval = OnValue(converted_generalized_time);
    } else {
      retval.EmplaceError(Asn1Errc::kInvalidContent, "A GeneralizedTime has an invalid value.");
    }
    return retval;
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kGeneralizedTime)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for VisibleString type.
 */
class VisibleStringBuilderState : public StringBuilderState<structure::VisibleString> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState::StringBuilderState;

  /*!
   * \brief Called when a VisibleString type is encountered.
   * \param[in] parsed_visible_string VisibleString data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnVisibleString(VisibleString parsed_visible_string) noexcept override {
    return OnValue(structure::VisibleString{parsed_visible_string});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kVisibleString)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};

/*!
 * \brief Builder for Ia5String subtypes
 * \details Since there can be CHOICEs of differently tagged Ia5String values, such objects may have to be distinguished
 *      by type. This class template allows for builders that utilize said types.
 * \tparam EmitType type to emit
 */
template <typename EmitType>
class SpecializedIa5StringBuilder : public StringBuilderState<EmitType> {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using StringBuilderState<EmitType>::StringBuilderState;

  /*!
   * \brief Called when a Ia5String type is encountered.
   * \param[in] parsed_ia5_string Ia5String data.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a context specific type is not expected or has an invalid value.
   * \vprivate Product Private
   */
  amsr::core::Result<void> OnIa5String(Asn1Builder::Ia5String parsed_ia5_string) noexcept override {
    return this->OnValue(EmitType{static_cast<amsr::core::String>(parsed_ia5_string)});
  }

  /*!
   * \brief Returns transitions for constructed contexts as a convenience for e.g. SEQUENCE OF builders.
   * \return The singular transition for the configured primitive value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetTransitions() noexcept
      -> ara::core::Vector<std::pair<builder::ElementInput, builder::ElementIdentifier>> {
    return {{{Asn1Parser::Asn1Class::kUniversal, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kIA5String)},
             {1, builder::ElementPresence::kPresent, true}}};
  }
};
/*!
 * \brief Builder for Ia5String type.
 */
using Ia5StringBuilderState = SpecializedIa5StringBuilder<structure::Ia5String>;

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_VALUE_TYPE_STATES_H_
