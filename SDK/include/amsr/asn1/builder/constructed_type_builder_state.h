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
/*!       \file   amsr/asn1/builder/constructed_type_builder_state.h
 *        \brief  ASN.1 Builder for composite types (i.e. constructed types and CHOICEs).
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRUCTED_TYPE_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRUCTED_TYPE_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include "amsr/asn1/asn1_parser.h"
#include "amsr/asn1/builder/asn1_builder_state.h"
#include "amsr/asn1/builder/transition.h"
#include "amsr/core/optional.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Builder state for constructed type base.
 * \tparam DerivedThis The concrete subclass of this builder. Used for fetching transitions.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_builder_must_support_all_asn1_types
template <class YieldedType>
// TODO: Consider renaming to "CompositeBuilder" as opposed to "ValueBuilder". CHOICEs are neither primitive nor
//  constructed, but their builders require other builders in a parent-child relationship.
class ConstructedTypeBuilderState : public Asn1BuilderState<YieldedType> {
 public:
  /*!
   * \brief Local type alias.
   */
  using Asn1Class = ::amsr::asn1::Asn1Parser::Asn1Class;
  /*!
   * \brief Local type alias.
   */
  using UniversalTag = ::amsr::asn1::Asn1Parser::UniversalElementTag;

  /*!
   * \brief Constructor for state.
   * \param[in] transitions List of transitions.
   * \param[in] parent Parent state.
   * \param[in] init_id Initial element id.
   */
  explicit ConstructedTypeBuilderState(std::set<Transition> const& transitions, Asn1Builder* parent = nullptr,
                                       ElementIdentifier const init_id = kInitId)
      : Asn1BuilderState<YieldedType>(parent),
        current_element_{init_id},
        init_element_{init_id},
        transitions_{transitions} {
    std::set<std::uint64_t> ids;
    for (Transition const& it : transitions) {
      ElementIdentifier const& current{it.CurentElement()};
      ElementIdentifier const& next{it.NextElement()};
      if (current.presence_flag == ElementPresence::kPresent) {
        static_cast<void>(ids.insert(current.id));
      }
      if (next.presence_flag == ElementPresence::kPresent) {
        static_cast<void>(ids.insert(next.id));
      }
    }
    required_ = ids.size();
  }

  /*!
   * \brief Subclass accessor for current state
   * \return Reference to current state
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CurrentState() noexcept -> Asn1Builder*& { return current_state_; }

  /*!
   * \brief Read-only subclass accessor for current state
   * \return Reference to current state
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CurrentState() const noexcept -> Asn1Builder* const& { return current_state_; }

  /*!
   * \brief Subclass accessor for current element
   * \return Reference to current element
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CurrentElement() noexcept -> ElementIdentifier& { return current_element_; }

  /*!
   * \brief Read-only Subclass accessor for current element
   * \return Reference to current element
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CurrentElement() const noexcept -> ElementIdentifier const& { return current_element_; }

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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of UNIVERSAL SEQUENCE."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSequence));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnSequenceStart();
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of UNIVERSAL SEQUENCE."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnSequenceEnd();
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail, "A UNIVERSAL SEQUENCE violates its constraints.");
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of UNIVERSAL SET."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSet));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnSetStart();
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of UNIVERSAL SET."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnSetEnd();
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail, "A UNIVERSAL SET violates its constraints.");
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of EXTERNAL."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kExternal));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnExternalStart();
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of EXTERNAL."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnExternalEnd();
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail, "An EXTERNAL value violates its constraints.");
    }

    return result;
  }

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
  auto OnContextSpecificConstructedTypeStart(Asn1Builder::ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "Builder did not expect parser call for: Start of context-specific constructed type."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kContextSpecific, context_specific_tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnContextSpecificConstructedTypeStart(context_specific_tag);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnContextSpecificConstructedTypeEnd(Asn1Builder::ContextSpecificTag context_specific_tag) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "Builder did not expect parser call for: End of context-specific constructed type."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnContextSpecificConstructedTypeEnd(context_specific_tag);
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail,
                          "A context-specific constructed value violates its constraints.");
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnContextSpecificType(Asn1Builder::ContextSpecificTag context_specific_tag,
                             Asn1Builder::ContextSpecificData parsed_context_specific_data) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Context-specific primitive type."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kContextSpecific, context_specific_tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnContextSpecificType(context_specific_tag, parsed_context_specific_data);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "A context-specific primitive value violates its constraints."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnApplicationConstructedTypeStart(Asn1Builder::ApplicationTag application_tag) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "Builder did not expect parser call for: Start of application-specific constructed type."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kApplication, application_tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnApplicationConstructedTypeStart(application_tag);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnApplicationConstructedTypeEnd(Asn1Builder::ApplicationTag application_tag) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault,
        "Builder did not expect parser call for: End of application-specific constructed type."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnApplicationConstructedTypeEnd(application_tag);
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail,
                          "A application-specific constructed value violates its constraints.");
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnApplicationType(Asn1Builder::ApplicationTag application_tag,
                         Asn1Builder::ApplicationData parsed_application_data) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kApplication, application_tag);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Application-specific primitive type."_sv);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnApplicationType(application_tag, parsed_application_data);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnPrivateConstructedTypeStart(Asn1Builder::PrivateTag private_tag) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of private constructed type."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kPrivate, private_tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnPrivateConstructedTypeStart(private_tag);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnPrivateConstructedTypeEnd(Asn1Builder::PrivateTag private_tag) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of private constructed type."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnPrivateConstructedTypeEnd(private_tag);
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail, "A private constructed value violates its constraints.");
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnPrivateType(Asn1Builder::PrivateTag private_tag, Asn1Builder::PrivateData parsed_private_data) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Private primitive type."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kPrivate, private_tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnApplicationType(private_tag, parsed_private_data);
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of EMBEDDED PDV."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnEmbeddedPDVStart();
    } else {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSet));
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of EMBEDDED PDV."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnEmbeddedPDVEnd();
    } else if (CheckConstraints()) {
      result = AcceptOnFinalState();
    } else {
      result.EmplaceError(Asn1Errc::kConstraintCheckFail, "An EMBEDDED PDV violates its constraints.");
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: Start of CHARACTER STRING."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnCharacterStringStart();
    } else {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kSet));
    }

    return result;
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
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: End of CHARACTER STRING."_sv)};
    if (current_state_ != nullptr) {
      result = current_state_->OnCharacterStringEnd();
    } else {
      result = AcceptOnFinalState();
    }

    return result;
  }

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
  auto OnBool(bool parsed_bool) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kBoolean));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnBool(parsed_bool);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL BOOLEAN."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnInteger(Asn1Builder::Integer parsed_integer) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kInteger));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnInteger(parsed_integer);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL INTEGER."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnBitString(Asn1Builder::BitString parsed_bit_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kBitString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnBitString(parsed_bit_string);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL BIT STRING."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnOctetString(Asn1Builder::OctetString parsed_octet_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kOctetString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnOctetString(parsed_octet_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL OCTET STRING."_sv);
    }

    return result;
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
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kNull));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnNull();
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL NULL."_sv);
    }

    return result;
  }

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
  auto OnOid(Asn1Builder::Oid parsed_oid) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kObjectIdentifier));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnOid(parsed_oid);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL OBJECT IDENTIFIER."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnReal(Asn1Builder::Real parsed_real) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kReal));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnReal(parsed_real);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL REAL."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnObjectDescriptor(Asn1Builder::ObjectDescriptor parsed_object_descriptor) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kObjectDescriptor));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnObjectDescriptor(parsed_object_descriptor);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL ObjectDescriptor."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnUtf8String(Asn1Builder::Utf8String parsed_utf8_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kUTF8String));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnUtf8String(parsed_utf8_string);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL UTF8String."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnROid(Asn1Builder::RelativeOid parsed_relative_oid) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kRelativeOID));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnROid(parsed_relative_oid);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL RELATIVE-OID."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnTime(Asn1Builder::Time parsed_time) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kTime));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnTime(parsed_time);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL TIME."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnNumericString(Asn1Builder::NumericString parsed_numeric_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kNumericString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnNumericString(parsed_numeric_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL NumericString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnPrintableString(Asn1Builder::PrintableString parsed_printable_string) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kPrintableString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnPrintableString(parsed_printable_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL PrintableString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnT61String(Asn1Builder::T61String parsed_t61_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kT61String));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnT61String(parsed_t61_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL T61String/TeletexString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnVideotexString(Asn1Builder::VideotexString parsed_videotex_string) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kVideotexString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnVideotexString(parsed_videotex_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL VideotexString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnVisibleString(Asn1Builder::VisibleString parsed_visible_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kVisibleString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnVisibleString(parsed_visible_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL VisibleString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnIa5String(Asn1Builder::Ia5String parsed_ia5_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kIA5String));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnIa5String(parsed_ia5_string);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL IA5String."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */

  auto OnGraphicString(Asn1Builder::GraphicString parsed_graphic_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kGraphicString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnGraphicString(parsed_graphic_string);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL GraphicString."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnGeneralizedTime(Asn1Builder::GeneralizedTime parsed_generalized_time) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kGeneralizedTime));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnGeneralizedTime(parsed_generalized_time);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL GeneralizedTime."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnUtcTime(Asn1Builder::UtcTime parsed_utc_time) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kUTCTime));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnUtcTime(parsed_utc_time);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL UTCTime."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnDate(Asn1Builder::Date parsed_date) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kDate));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnDate(parsed_date);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL DATE."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnTimeOfDay(Asn1Builder::TimeOfDay parsed_time_of_day) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kTimeOfDay));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnTimeOfDay(parsed_time_of_day);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL TIME-OF-DAY."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnDateTime(Asn1Builder::Date parsed_date_time) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kDateTime));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnDateTime(parsed_date_time);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL DATE-TIME."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnEnumerated(Asn1Builder::Enumerated parsed_enumerated) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kEnumerated));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnEnumerated(parsed_enumerated);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL ENUMERATED."_sv);
    }

    return result;
  }

  /*!
   * \brief Called when a GeneralString is encountered.
   * \param[in] parsed_general_string Parsed general string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a GeneralString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnGeneralString(Asn1Builder::GeneralString parsed_general_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL GeneralString."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kGeneralString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnGeneralString(parsed_general_string);
    }

    return result;
  }

  /*!
   * \brief Called when a UniversalString is encountered.
   * \param[in] parsed_universal_string Parsed universal string value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If a UniversalString is not expected or has an invalid value.
   * \vprivate Product Private
   */
  auto OnUniversalString(Asn1Builder::UniversalString parsed_universal_string) noexcept
      -> amsr::core::Result<void> override {
    // NOTE: "UNIVERSAL" refers to tagging mode and is not the same as the "Universal" in "UniversalString".
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL UniversalString."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kUniversalString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnUniversalString(parsed_universal_string);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnBMPString(Asn1Builder::BMPString parsed_bmp_string) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL BMPString."_sv)};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kBMPString));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnBMPString(parsed_bmp_string);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnDuration(Asn1Builder::Duration parsed_duration) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kDuration));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnDuration(parsed_duration);
    } else {
      result = amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault,
                                                   "Builder did not expect parser call for: UNIVERSAL DURATION."_sv);
    }

    return result;
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
   * \vprivate Product Private
   */
  auto OnOIDInternationalized(Asn1Builder::OIDInternationalized parsed_oid_internationalized) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal, static_cast<std::uint64_t>(UniversalTag::kOIDInternationalized));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnOIDInternationalized(parsed_oid_internationalized);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault, "Builder did not expect parser call for: UNIVERSAL OID-IRI (international OID)."_sv);
    }

    return result;
  }

  /*!
   * \brief Called when a Relative OID Internationalized Resource Identifier is encountered.
   * \param[in] parsed_relative_oid_internationalized Parsed Relative OID Internationalized Resource Identifier value.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If a Relative OID Internationalized Resource Identifier is not expected or has an invalid
   * value.
   * \vprivate Product Private
   */
  auto OnRelativeOIDInternationalized(
      Asn1Builder::RelativeOIDInternationalized parsed_relative_oid_internationalized) noexcept
      -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(Asn1Class::kUniversal,
                               static_cast<std::uint64_t>(UniversalTag::kRelativeOIDInternationalized));
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnRelativeOIDInternationalized(parsed_relative_oid_internationalized);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault,
          "Builder did not expect parser call for: UNIVERSAL Relative OID-IRI (international OID)."_sv);
    }

    return result;
  }

  /*!
   * \brief Called when some data cannot be parsed with any known schema and is retried as a raw byte stream.
   * \param[in] tlv The raw TLV (Tag, Length, Value) data of the unknown ASN.1 structure.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault If the builder does not expect raw data at this point
   * \vprivate Product Private
   */
  auto OnRawData(Asn1Builder::RawData tlv) noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> result{Asn1Errc::kRuntimeFault};
    if (current_state_ == nullptr) {
      result = MoveToNextState(kRawDataInput.class_type, kRawDataInput.tag);
    }
    if (current_state_ != nullptr) {
      result = current_state_->OnRawData(tlv);
    } else {
      result = amsr::core::Result<void>::FromError(
          Asn1Errc::kRuntimeFault,
          "Builder did not expect parser call for raw data of unknown/extensible schema ('...', ANY, etc.)"_sv);
    }

    return result;
  }

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
  auto IsCompleted() const noexcept -> bool override {
    return (current_element_.is_final) && (current_element_.id == kCompletedId.id);
  }

  /*!
   * \brief Resets the builder.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Reset() noexcept -> void override {
    if (current_state_ != nullptr) {
      current_state_ = nullptr;
    }
    current_element_ = init_element_;
    count_ = 0;
    DoReset();
  }

  /*! Special input for raw data type. */
  static constexpr ElementInput const kRawDataInput{Asn1BuilderState<YieldedType>::Asn1Class::kUniversal, 1001U};
  /*! Indicator of raw data. */
  static constexpr ElementIdentifier const kRawDataId{std::numeric_limits<std::uint64_t>::max() - 1,
                                                      ElementPresence::kOptional, true};
  /*! Special input for choice type. */
  static constexpr ElementInput const kChoiceInput{Asn1BuilderState<YieldedType>::Asn1Class::kUniversal, 1000U};

 protected:
  /*!
   * \brief Creates child states based on current state.
   * \param[in] element Identifier for state.
   * \return Pointer to next state.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto CreateState(ElementIdentifier const& element) noexcept -> Asn1Builder* = 0;

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
  virtual auto CheckConstraints() const noexcept -> bool = 0;

  /*!
   * \brief Specific reset logic for concrete builders, calls reset for all subbuilders down to primitives.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto DoReset() noexcept -> void = 0;

  /*!
   * \brief Changes state handler based on current state and input.
   * \param[in] class_id Class of current input.
   * \param[in] tag Tag value of current input.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If No transition found or constraint failed.
   * \vprivate Product Private
   */
  virtual auto MoveToNextState(Asn1Parser::Asn1Class const class_id, std::uint64_t const tag) noexcept
      -> amsr::core::Result<void> {
    // NOTE: This error is intended to be overwritten with a more specific text.
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Builder could not find a state for the passed tag."_sv)};
    if ((current_element_.id == kInitId.id) || (current_element_.id == kCompletedId.id)) {
      // Either this builder was just initialized and the passed tag is thus consumed,
      // or it has completed and returns control to its parent.
      // Note that CHOICEs generally don't do this since there isn't necessarily a tag around them.
      current_element_ = kAnyElementId;
      current_element_.is_final = true;
      result = amsr::core::Result<void>::FromValue();
    } else {
      Transition source{current_element_, {class_id, tag}};
      std::set<Transition>::iterator it{transitions_.find(source)};
      bool found{it != transitions_.end()};
      if (!found) {
        source = Transition{current_element_, kChoiceInput};
        it = transitions_.find(source);
        found = it != transitions_.end();
      }
      if (found && (*it == source)) {
        current_element_ = it->NextElement();
        if (current_element_.presence_flag == ElementPresence::kPresent) {
          ++count_;
        }
        current_state_ = CreateState(current_element_);
        result = amsr::core::Result<void>::FromValue();
      }
    }
    return result;
  }

  /*!
   * \brief Check if current state is accepted state.
   * \return \c true if current state is one of final, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsFinalState() const noexcept -> bool { return (current_element_.is_final) && (count_ >= required_); }

  /*!
   * \brief Check if current state is accepted state and notifies parent.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If the internal structure is not consistent with the corresponding ASN.1 schema.
   * \vprivate Product Private
   */
  auto AcceptOnFinalState() noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kRuntimeFault, "Internal error: Call to AcceptOnFinalState on unfinished builder"_sv)};
    if ((IsFinalState()) && (current_state_ == nullptr)) {
      current_element_ = kCompletedId;
      // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAASN1_AutosarC++17_10-M0.1.2_dead_branch_false_positive
      if (Asn1Builder* const parent{this->GetParent()}) {
        result = parent->Accept();
      } else {
        result = amsr::core::Result<void>();
      }
    }
    return result;
  }

  /*!
   * \brief     Pre-Accept hook for special constructed types, i.e. CHOICEs.
   * \details   Since there is no OnChoiceEnd callback and we want to avoid calls to base class implementations in
   *            DoAccept, we provide a pre-accept hook where CHOICEs can change their state so that IsCompleted returns
   *            true when the CHOICE has been evaluated. For other constructed types, this would usually be done
   *            in their On...End hooks.
   * \return    An error depending on the subclass implementation; success per default.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto PreAccept() noexcept -> amsr::core::Result<void> { return amsr::core::Result<void>::FromValue(); }

  /*!
   * \brief Read-only access interface for init element.
   * \return Value of Init Element.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto InitElement() const -> ElementIdentifier const& { return init_element_; }

  /*!
   * \brief Type list of builders with more than one element.
   *    Builders must have a yielded type. Intended for use with <tt>YieldToVariant</tt>.
   * \tparam THead List head, or the "current element" for iterative purposes.
   * \tparam TTail List tail, or the "remaining elements" for iterative purposes.
   */
  template <typename THead, typename... TTail>
  class TypeList {
   public:
    /*!
     * \brief List recursion for YieldToVariant.
     * \details This method tries to yield with the current \c THead type. If neither an error nor a valid result
     *      is returned, it tries the same with the next type.
     * \tparam BT Builder variant type: A variant containing builders to yield into <tt>structure_variant</tt>.
     * \tparam ST Structure variant type: A variant containing possible types for \c builder_variant to yield. At this
     *      level, this need not necessarily be a variant but may also be an optional variant. See overloads of
     *      \c YieldToVariant for details.
     * \param[in] builder_variant The source of yielded data, as a variant.
     * \param[out] structure_variant The destination of yielded data, as a variant.
     * \return A boolean result indicating if one of the variant alternatives has been yielded.
     *      Ignore for optional choices.
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \synchronous TRUE
     * \vprivate Product Private
     */
    template <class BT, class ST>
    static auto GuessYield(BT& builder_variant, ST& structure_variant) noexcept -> amsr::core::Result<bool> {
      // Treat the current element as a single-element list (TypeList<THead>) and call that list's final case.
      amsr::core::Result<bool> retval{TypeList<THead>::GuessYield(builder_variant, structure_variant)};

      if ((retval.HasValue()) && (!retval.Value())) {
        retval = TypeList<TTail...>::GuessYield(builder_variant, structure_variant);
      }

      return retval;
    }

    /*!
     * \brief Resets the passed builder regardless of what type it contains.
     * \tparam BT A builder variant type.
     * \param[in] builder_variant The builder to reset.
     * \return void
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \synchronous TRUE
     * \vprivate Product Private
     */
    template <class BT>
    static auto GuessReset(BT& builder_variant) noexcept -> void {
      TypeList<THead>::GuessReset(builder_variant);
      TypeList<TTail...>::GuessReset(builder_variant);
    }
  };

  /*!
   * \brief Type list of builders with one element.
   *    Builders must have a yielded type. Intended for use with <tt>YieldToVariant</tt>.
   * \tparam Head List head, or the "only element" for iterative purposes.
   */
  template <typename Head>
  class TypeList<Head> {
   public:
    /*!
     * \brief List recursion final case for YieldToVariant.
     * \details If the builder is of \c Head type, yields to <tt>structure_variant</tt>.
     * \tparam BT Builder variant type: A variant containing builders to yield into <tt>structure_variant</tt>.
     * \tparam ST Structure variant type: A variant containing possible types for \c builder_variant to yield.
     * \param[in] builder_variant The source of yielded data, as a variant.
     * \param[out] structure_variant The destination of yielded data, as a variant.
     * \return A boolean result indicating if data of type \c Head::YieldedType have been yielded.
     *      Ignore for optional choices.
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \synchronous TRUE
     * \vprivate Product Private
     */
    template <class BT, class ST>
    static auto GuessYield(BT& builder_variant, ST& structure_variant) noexcept -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{Asn1Errc::kRuntimeFault};

      if (Head const* const builder_guess_ptr{builder_variant.template get_if<Head>()}) {
        retval = builder_guess_ptr->Yield()
                     .Inspect([&structure_variant](typename Head::YieldedType const& yielded_value) {
                       structure_variant = yielded_value;
                     })
                     .template Replace<bool>(true);
      } else {
        retval.EmplaceValue(false);
      }

      return retval;
    }

    /*!
     * \brief List recursion final case for YieldToVariant for optionals.
     * \details If the builder is of \c Head type, yields to \c structure_variant.
     * \tparam BT Builder variant type: A variant containing builders to yield into <tt>structure_variant</tt>.
     * \tparam ST Structure variant type: A variant containing possible types for \c builder_variant to yield.
     * \param[in] builder_variant The source of yielded data, as a variant.
     * \param[out] structure_variant The destination of yielded data, as an optional variant.
     * \return A boolean result indicating if data of type \c Head::YieldedType have been yielded.
     *      Ignore for optional choices.
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \synchronous TRUE
     * \vprivate Product Private
     */
    template <class BT, class ST>
    static auto GuessYield(BT& builder_variant, amsr::core::Optional<ST>& structure_variant) noexcept
        -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{Asn1Errc::kRuntimeFault};

      if (Head const* const builder_guess_ptr{builder_variant.template get_if<Head>()}) {
        if (builder_guess_ptr->IsCompleted()) {
          retval = builder_guess_ptr->Yield()
                       .Inspect([&structure_variant](typename Head::YieldedType const& yielded_value) {
                         structure_variant.emplace(yielded_value);
                       })
                       .template Replace<bool>(true);
        } else {
          retval.EmplaceValue(false);
        }
      } else {
        retval.EmplaceValue(false);
      }

      return retval;
    }

    /*!
     * \brief Tries to reset the passed builder if it is of type \c Head.
     * \tparam BT A builder variant type.
     * \param[in] builder_variant The builder to reset.
     * \return void
     */
    template <class BT>
    static auto GuessReset(BT& builder_variant) noexcept -> void {
      if (Head* const builder_guess_ptr{builder_variant.template get_if<Head>()}) {
        builder_guess_ptr->Reset();
      }
    }
  };

  /*!
   * \brief For a choice builder, this checks which concrete builder is present and yields into the data variant
   *        accordingly.
   * \tparam YTs Yielded Types: A list of the types that the passed builder types can build.
   * \tparam SBTs Specific Builder Types: A list of possible types of builders. This is the representation of a
   *        choice builder.
   * \param[in] builder_variant The builder variant containing the builder that is to yield.
   * \param[out] structure_variant The data variant to yield into.
   * \return A boolean result indicating if any data was yielded. This can usually be ignored for optional choices.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class... YTs, class... SBTs>
  static auto YieldToVariant(amsr::core::Variant<SBTs...> const& builder_variant,
                             amsr::core::Variant<YTs...>& structure_variant) noexcept -> amsr::core::Result<bool> {
    return TypeList<SBTs...>::GuessYield(builder_variant, structure_variant);
  }

  /*!
   * \brief A version of \c YieldToVariant for optional data structures.
   * \tparam YTs Yielded Types: A list of the types that the passed builder types can build.
   * \tparam SBTs Specific Builder Types: A list of possible types of builders. This is the representation of a
   *        choice builder.
   * \param[in] builder_variant The builder variant containing the builder that is to yield.
   * \param[out] structure_variant The optional data variant to yield into.
   * \return A boolean result indicating if any data was yielded. This can usually be ignored for optional choices.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class... YTs, class... SBTs>
  static auto YieldToVariant(amsr::core::Variant<SBTs...> const& builder_variant,
                             amsr::core::Optional<amsr::core::Variant<YTs...>>& structure_variant) noexcept
      -> amsr::core::Result<bool> {
    return TypeList<SBTs...>::GuessYield(builder_variant, structure_variant);
  }

  /*!
   * \brief Shorthand for yielding optional data in cases where a simple emplacement operation does not suffice.
   * \tparam BT The type of the yielding builder.
   * \param[in] builder The yielding builder.
   * \param[in] setter A function to perform on the yielded value, usually a setter or emplacement method.
   * \return The result of the yield operation if it happened, else an empty result.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class BT>
  static auto YieldToOptional(BT const& builder,
                              vac::language::UniqueFunction<void(typename BT::YieldedType const&)>&& setter) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};

    if (builder.IsCompleted()) {
      retval = builder.Yield().Inspect(setter).template Replace<void>();
    } else {
      // An empty optional is not an error.
      retval.EmplaceValue();
    }

    return retval;
  }

  /*!
   * \brief Shorthand for yielding optional data in most cases.
   * \tparam BT The type of the yielding builder.
   * \param[in] builder The yielding builder.
   * \param[out] destination The destination to write the yielded value to.
   * \return The result of the yield operation if it happened, else an empty result.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class BT>
  static auto YieldToOptional(BT const& builder, amsr::core::Optional<typename BT::YieldedType>& destination) noexcept
      -> amsr::core::Result<void> {
    return YieldToOptional(
        builder, [&destination](typename BT::YieldedType const& yielded_value) { destination.emplace(yielded_value); });
  }

  /*!
   * \brief Shorthand for yielding data with a default value in most cases.
   * \tparam BT The type of the yielding builder.
   * \param[in] builder The yielding builder.
   * \param[out] destination The destination to write the yielded value to.
   * \return The result of the yield operation if it happened, else an empty result.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class BT>
  static auto YieldToDefault(BT const& builder, typename BT::YieldedType& destination) noexcept
      -> amsr::core::Result<void> {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAASN1_AutosarC++17_10-M0.1.8_yield_to_default_false_positive
    return YieldToOptional(
        builder, [&destination](typename BT::YieldedType const& yielded_value) { destination = yielded_value; });
  }

  /*!
   * \brief Shorthand for yielding data that is not optional, defaulted, a choice or similar.
   * \tparam BT The type of the yielding builder.
   * \param[in] builder The yielding builder.
   * \param[out] destination The destination to write the yielded value to.
   * \return An empty result if successful, or an error if not.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class BT>
  static auto YieldTo(BT const& builder, typename BT::YieldedType& destination) noexcept -> amsr::core::Result<void> {
    return builder.Yield()
        .Inspect([&destination](typename BT::YieldedType const& yielded_value) { destination = yielded_value; })
        .template Replace<void>();
  }

  /*!
   * \brief Convenience function to call \c Reset for a variant of builders.
   * \tparam SBTs The types of builders the variant can take up.
   * \param[in] builder_variant The variant builder to reset.
   * \return void
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <class... SBTs>
  static auto ResetVariantBuilder(amsr::core::Variant<SBTs...>& builder_variant) noexcept -> void {
    TypeList<SBTs...>::GuessReset(builder_variant);
  }

  /*! Indicator of this builder accepting any input tag initially. */
  static constexpr ElementIdentifier const kAnyElementId{};

  /*! Indicator of initial state. */
  static constexpr ElementIdentifier const kInitId{std::numeric_limits<std::uint64_t>::max(), ElementPresence::kPresent,
                                                   false};
  /*! Indicator of completed state. */
  static constexpr ElementIdentifier const kCompletedId{std::numeric_limits<std::uint64_t>::max() - 2,
                                                        ElementPresence::kPresent, true};

 private:
  /*!
   * \brief Receives notification from child upon exiting final state.
   * \return A result indicating whether the concrete builder state successfully accepted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Accept() noexcept -> amsr::core::Result<void> override {
    amsr::core::Result<void> const retval{PreAccept()};
    current_state_ = nullptr;
    return retval;
  }

  Asn1Builder* current_state_{};                     /*!< Current state handler. */
  ElementIdentifier current_element_{kAnyElementId}; /*!< Current state identifier. */
  ElementIdentifier const init_element_{};           /*!< Current state identifier. */
  std::set<Transition> transitions_{};               /*!< Set of transitions. */
  std::uint64_t count_{};                            /*!< Count number of mandatory elements processed. */
  std::uint64_t required_{};                         /*!< Required number of mandatory elements processed. */
};
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementIdentifier const ConstructedTypeBuilderState<T>::kAnyElementId;
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementIdentifier const ConstructedTypeBuilderState<T>::kInitId;
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementIdentifier const ConstructedTypeBuilderState<T>::kCompletedId;
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementIdentifier const ConstructedTypeBuilderState<T>::kRawDataId;
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementInput const ConstructedTypeBuilderState<T>::kChoiceInput;
/*!
 * \brief Class template static constexpr storage.
 * \tparam T Irrelevant from this perpective: Any template argument.
 */
template <class T>
constexpr ElementInput const ConstructedTypeBuilderState<T>::kRawDataInput;
}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_CONSTRUCTED_TYPE_BUILDER_STATE_H_
