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
/*!        \file  amsr/asn1/validator/internal/validator.h
 *        \brief  General templates for validator utilities. Do not include as client, but validators.h instead.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATOR_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATOR_H_

#include "amsr/asn1/asn1_der_parser.h"
#include "amsr/asn1/asn1_parser.h"
#include "amsr/asn1/context/internal/common/choice.h"
#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/explicit.h"
#include "amsr/asn1/context/internal/common/nature_metafunctions.h"
#include "amsr/asn1/context/internal/common/optional.h"
#include "amsr/asn1/context/internal/common/sequence_of.h"
#include "amsr/asn1/context/internal/common/set_of.h"
#include "amsr/asn1/validator/internal/traits.h"

namespace amsr {
namespace asn1 {
namespace validator {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief General validation functionality outside of the ValidatorBase hierarchy.
 * \details This mainly enables template specializations to "resume validation as the base class would".
 * \tparam T A structure or context type to validate
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename T, typename DerivedThis>
class GeneralValidation {
 public:
  /*!
   * \brief Expose the context that will be validated
   */
  using ContextType = typename context::EnsureContextNature<T>::Type;

  /*!
   * \brief Validates a given data structure w.r.t. constraints, non-recursively.
   *        The default validator has no constraints about validity.
   * \details This version is a general version for contexts without own validity criteria.
   * \param to_validate The data structure to validate
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return An empty result.
   */
  template <typename D = DerivedThis, typename std::enable_if<!HasValidityCriteria<D>::value, bool>::type = true>
  static auto IsValid(typename ContextType::StructureType const& to_validate) noexcept -> amsr::core::Result<void> {
    static_cast<void>(to_validate);
    return amsr::core::Result<void>::FromValue();
  }

  /*!
   * \brief Validates a given data structure w.r.t. constraints by delegating to the active subclass, non-recursively.
   * \details This version handles non-optional values. Optional values must be unwrapped first, and it is not the
   *        concern of underlying template specializations to check for optionality. Hence, the general class template
   *        performs this check.
   * \param to_validate The data structure to validate
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return The result returned by the subclass validator
   */
  template <typename D = DerivedThis, typename std::enable_if<HasValidityCriteria<D>::value, bool>::type = true>
  static auto IsValid(typename ContextType::StructureType const& to_validate) noexcept -> amsr::core::Result<void> {
    return DerivedThis::EvaluateValidity(to_validate);
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness where the special validator has additional
   *    criteria, non-recursively.
   * \param der_to_validate The data structure to validate
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  template <typename D = DerivedThis, typename std::enable_if<HasWellFormednessCriteria<D>::value, bool>::type = true>
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> const general_result{EvaluateGeneralWellFormedness(der_to_validate)};
    return general_result.And(DerivedThis::EvaluateSpecialWellFormedness(der_to_validate));
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness where the special validator has no
   *    own well-formedness criteria, non-recursively.
   * \param der_to_validate The data structure to validate
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  template <typename D = DerivedThis, typename std::enable_if<!HasWellFormednessCriteria<D>::value, bool>::type = true>
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    return EvaluateGeneralWellFormedness(der_to_validate);
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness for criteria that all DER-encoded objects
   *    share. This encapsulates things like tag and length from subclasses.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data's T and L values are consistent, else an error.
   */
  static auto EvaluateGeneralWellFormedness(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};

    retval =
        Asn1DERParser::GetObject(der_to_validate)
            .AndThen([&der_to_validate](Asn1Parser::ObjectInfo const& object_info) {
              amsr::core::Result<Asn1Parser::ObjectInfo> inner_retval{object_info};

              // TODO: Unify tag class systems to avoid bit shifting and casting
              std::uint8_t expected_tag_class{static_cast<uint8_t>(ContextType::TagClass())};
              expected_tag_class = expected_tag_class / 64;
              std::uint8_t const actual_tag_class{static_cast<uint8_t>(object_info.element_class)};

              if (expected_tag_class != actual_tag_class) {
                inner_retval.EmplaceError(Asn1Errc::kUnknownTag,
                                          "Unexpected Tag class (universal, context-specific, application, private)");
              } else if ((static_cast<uint64_t>(ContextType::TagNumber()) != object_info.element_tag)) {
                // NOTE: We do not check if the tag number is kUndefined because this only applies to untagged CHOICEs
                //  which have their own well-formedness checks.
                inner_retval.EmplaceError(Asn1Errc::kUnknownTag, "Unexpected Tag number");
              } else if (ContextType::IsConstructed() != object_info.is_constructed) {
                inner_retval.EmplaceError(Asn1Errc::kUnknownTag, "Unexpected Primitive/Constructed flag value");
              } else if (der_to_validate.size() != (object_info.element_length + object_info.header_size)) {
                inner_retval.EmplaceError(Asn1Errc::kInvalidInput, "Inconsistent length of DER object");
              } else {
                // Return default
              }
              return inner_retval;
            })
            .template Replace<void>();

    return retval;
  }
};

// VECTOR NC AutosarC++17_10-A12.4.1: MD_VAASN1_AutosarC++17_10-A12.4.1_template_dtor_not_public_virtual_false_positive
/*!
 * \brief Generic validator template for general contexts
 * \tparam T A structure or context type to validate
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename T, typename DerivedThis>
class ValidatorBase {
 public:
  /*!
   * \brief Expose the context that will be validated
   */
  using ContextType = typename context::EnsureContextNature<T>::Type;

  /*!
   * \brief Default constructor
   */
  ValidatorBase() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  ValidatorBase(ValidatorBase const&) = delete;
  /*!
   * \brief Defaulted move constructor
   */
  ValidatorBase(ValidatorBase&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(ValidatorBase const&) & -> ValidatorBase& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(ValidatorBase&&) & noexcept -> ValidatorBase& = delete;
  /*!
   * \brief Defaulted destructor
   */
  virtual ~ValidatorBase() = default;

  /*!
   * \brief Validates a given data structure w.r.t. constraints, non-recursively.
   *        The default validator has no constraints about validity.
   * \param to_validate The data structure to validate
   * \return An empty result.
   */
  static auto IsValid(typename ContextType::StructureType const& to_validate) noexcept -> amsr::core::Result<void> {
    return GeneralValidation<T, DerivedThis>::IsValid(to_validate);
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness where the special validator has additional
   *    criteria, non-recursively.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    return GeneralValidation<T, DerivedThis>::IsWellFormed(der_to_validate);
  }
};

/*!
 * \brief The template to derive from when defining a special validator
 * \tparam ContextType The validator's context
 */
template <typename ContextType, typename Enable = void>
class Validator final : public ValidatorBase<ContextType, Validator<ContextType, Enable>> {};

/*!
 * \brief Special validator base for optional values
 * \tparam T A structure or context type to validate
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename T, typename DerivedThis>
class ValidatorBase<context::Optional<T>, DerivedThis> {
 public:
  /*!
   * \brief The context for the type inside the optional as if it were present
   */
  using InnerContextType = typename context::EnsureContextNature<T>::Type;

  /*!
   * \brief Validates a given data structure w.r.t. constraints.
   *        The default validator has no constraints about validity.
   * \details This version is a general version for contexts without own validity criteria.
   * \param to_validate The data structure to validate
   * \tparam Ignore The type to validate, i.e. T or Optional<T>.
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \param to_validate
   * \return An empty result.
   */
  template <typename Ignore, typename D = DerivedThis,
            typename std::enable_if<!HasValidityCriteria<D>::value, bool>::type = true>
  static auto IsValid(Ignore const& to_validate) noexcept -> amsr::core::Result<void> {
    static_cast<void>(to_validate);
    return amsr::core::Result<void>::FromValue();
  }

  /*!
   * \brief Validates a given optional data structure w.r.t. constraints by delegating to the active subclass.
   * \details This version is a short-hand passing an unwrapped, present value to a validator for optional contexts.
   * \param to_validate The data structure to validate in its unpacked form (for convenience)
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return The result returned by the subclass validator
   */
  template <typename D = DerivedThis, typename std::enable_if<HasValidityCriteria<D>::value, bool>::type = true>
  static auto IsValid(typename InnerContextType::StructureType const& to_validate) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
    retval = Validator<InnerContextType>::IsValid(to_validate);
    return retval;
  }

  /*!
   * \brief Validates a given optional data structure w.r.t. constraints by delegating to the active subclass.
   * \details This version is the general version for optional data types.
   * \param to_validate The data structure to validate
   * \tparam D A type parameter that makes the template specialization selection arguments dependent names for SFINAE.
   * \return The result returned by the subclass validator, or a valid result if the optional is empty
   */
  template <typename D = DerivedThis, typename std::enable_if<HasValidityCriteria<D>::value, bool>::type = true>
  static auto IsValid(amsr::core::Optional<typename InnerContextType::StructureType> const& to_validate) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
    if (to_validate.has_value()) {
      retval = Validator<InnerContextType>::IsValid(*to_validate);
    } else {
      // Optionals are allowed to be empty.
      retval.EmplaceValue();
    }
    return retval;
  }

  /*!
   * \brief Validates a given DER data representation of an OPTIONAL value. Empty input is expressly permitted.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
    if (der_to_validate.empty()) {
      // Optionals are allowed to be empty.
      retval.EmplaceValue();
    } else {
      retval = Validator<InnerContextType>::IsWellFormed(der_to_validate);
    }
    return retval;
  }
};

/*!
 * \brief Special validator base for EXPLICITly tagged values
 * \tparam T A structure or context type to validate
 * \tparam NewTag See Explicit
 * \tparam NewClass See Explicit
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename T, context::Tag NewTag, context::Class NewClass, typename DerivedThis>
class ValidatorBase<context::Explicit<T, NewTag, NewClass>, DerivedThis> {
 public:
  /*!
   * \brief Context to call for validation of the contained element
   */
  using InnerContextType = typename context::EnsureContextNature<T>::Type;

  /*!
   * \brief Context to call for validation of the container element
   */
  using OuterContextType = context::Explicit<T, NewTag, NewClass>;
  /*!
   * \brief Validates a given data structure w.r.t. constraints by delegating to the inner validator.
   * \param to_validate The data structure to validate
   * \return The result returned by the inner validator, or a valid result if the optional is empty
   */
  static auto IsValid(typename OuterContextType::StructureType const& to_validate) -> amsr::core::Result<void> {
    return Validator<InnerContextType>::IsValid(to_validate);
  }

  /*!
   * \brief Validates a given DER data representation of an EXPLICTly tagged value.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    // Check well-formedness from the perspective of the outer tag.
    // NOTE: There is no special well-formedness for outer tags as they don't exist in the schema.
    return GeneralValidation<OuterContextType, DerivedThis>::EvaluateGeneralWellFormedness(der_to_validate)
        .AndThen([&der_to_validate]() { return Asn1DERParser::WithoutHeader(der_to_validate); })
        .AndThen([](amsr::core::Span<std::uint8_t const> const der_to_validate_no_header) {
          amsr::core::Result<void> retval{Asn1Errc::kRuntimeFault};
          if (der_to_validate_no_header.empty()) {
            retval.EmplaceError(Asn1Errc::kInvalidInput, "An EXPLICIT tag contains nothing");
          } else {
            retval = Validator<InnerContextType>::IsWellFormed(der_to_validate_no_header);
          }
          return retval;
        });
  }
};

/*!
 * \brief Special validator base for SET OF values
 * \tparam T A structure or context type to validate
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename T, typename DerivedThis>
class ValidatorBase<context::SetOf<T>, DerivedThis> {
 public:
  /*!
   * \brief Expose the context that will be validated
   */
  using ContextType = context::SetOf<T>;

  /*!
   * \brief Validates a given data structure w.r.t. constraints, non-recursively.
   *        The default validator has no constraints about validity.
   * \param to_validate The data structure to validate
   * \return An empty result.
   */
  static auto IsValid(typename ContextType::StructureType const& to_validate) noexcept -> amsr::core::Result<void> {
    return GeneralValidation<ContextType, DerivedThis>::IsValid(to_validate);
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness where the special validator has additional
   *    criteria, non-recursively.
   * \details In DER, SET OF values must be lexicographically ordered by the encodings of the elements.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    return GeneralValidation<ContextType, DerivedThis>::EvaluateGeneralWellFormedness(der_to_validate)
        .AndThen([&der_to_validate]() { return Asn1DERParser::WithoutHeader(der_to_validate); })
        .AndThen([](amsr::core::Span<std::uint8_t const> const& der_to_validate_no_header) {
          // SETs OF are generally allowed to be empty, hence we initialize with a valid result.
          amsr::core::Result<void> retval{};
          std::size_t offset{0};
          amsr::core::Span<std::uint8_t const> previous_element{};

          while (offset < der_to_validate_no_header.size()) {
            retval = CheckOrder(der_to_validate_no_header, offset, previous_element);
            if (!retval) {
              break;
            }
          }
          return retval;
        });
  }

 private:
  /*!
   * \brief Makes sure the next element in the SET OF is lexicographically ordered after the current element.
   * \details This is a private method with highly coupled parameters.
   * \param der_to_validate_no_header The total DER object, one contained object of which to check
   * \param offset The offset from where to interpret the next object
   * \param previous_element The previous or empty element
   * \return An empty result on success and an error if the element order is not DER-conforming.
   */
  static auto CheckOrder(amsr::core::Span<std::uint8_t const> const& der_to_validate_no_header, std::size_t& offset,
                         amsr::core::Span<std::uint8_t const>& previous_element) -> amsr::core::Result<void> {
    return Asn1DERParser::FirstObject(der_to_validate_no_header.subspan(offset))
        .AndThen([&offset, &previous_element](amsr::core::Span<std::uint8_t const> const current_element) {
          amsr::core::Result<void> retval{
              amsr::core::Result<void>::FromError(Asn1Errc::kInvalidInput, "Element order in SET OF violates DER"_sv)};
          offset += current_element.size();
          if (!std::lexicographical_compare(current_element.cbegin(), current_element.cend(), previous_element.cbegin(),
                                            previous_element.cend())) {
            retval.EmplaceValue();
            previous_element = current_element;
          }
          return retval;
        });
  }
};

/*!
 * \brief Generic validator template for any choice context. Note that there is no forseeable reason to specialize this.
 * \details Neither validity nor well-formedness of a CHOICE as a whole are impacted by the schema. All is determined
 *          by the alternatives contained. Since CHOICE validators need only delegate to the correct context validator,
 *          there shouldn't be a reason to make template specializations for this.
 * \tparam Ts The structure or data types from which to choose
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename... Ts, typename DerivedThis>
class ValidatorBase<context::Choice<Ts...>, DerivedThis> {
 public:
  /*!
   * \brief Expose the context that will be validated
   */
  using ContextType = context::Choice<Ts...>;

  /*!
   * \brief Validates a given data structure w.r.t. constraints.
   *        The default validator has no constraints about validity.
   * \param to_validate The data structure to validate
   * \return An empty result if the data is valid, else the first encountered validation error.
   */
  static auto IsValid(typename ContextType::StructureType const& to_validate) noexcept -> amsr::core::Result<void> {
    return TypeList<typename context::EnsureContextNature<Ts>::Type...>::DelegateValidityCheck(to_validate)
        .template Replace<void>();
  }

  /*!
   * \brief Validates a given DER data representation w.r.t. well-formedness.
   * \param der_to_validate The data structure to validate
   * \return An empty result if the data is well-formed, else the first encountered validation error.
   */
  static auto IsWellFormed(amsr::core::Span<std::uint8_t const> const& der_to_validate) noexcept
      -> amsr::core::Result<void> {
    return Asn1DERParser::GetObject(der_to_validate)
        .AndThen([&der_to_validate](Asn1Parser::ObjectInfo const& object_info) {
          return TypeList<typename context::EnsureContextNature<Ts>::Type...>::DelegateWellFormednessCheck(
              der_to_validate, object_info);
        })
        .AndThen([](bool type_found) {
          amsr::core::Result<void> retval{
              amsr::core::Result<void>::FromError(Asn1Errc::kInvalidInput, "Unrecognized tag in CHOICE"_sv)};
          if (type_found) {
            retval.EmplaceValue();
          }
          return retval;
        });
  }

 protected:
  /*!
   * \brief Typelist used for iterating CHOICE alternatives
   * \tparam Head The "current element" of the iteration
   * \tparam Tail The remainder of the list to iterate
   */
  template <typename Head, typename... Tail>
  class TypeList {
   public:
    /*!
     * \brief Searches the available types for one that \c to_validate contains and delegates to the corresponding
     *      validator.
     * \param to_validate The data structure to validate
     * \return An error if validation failed, else an empty result
     */
    static auto DelegateValidityCheck(typename ContextType::StructureType const& to_validate) noexcept
        -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{TypeList<Head>::DelegateValidityCheck(to_validate)};
      if ((retval.HasValue()) && (!retval.Value())) {
        retval = TypeList<Tail...>::DelegateValidityCheck(to_validate);
      }
      return retval;
    }

    /*!
     * \brief Searches the available types for one that \c der_to_validate indicates by its tag and delegates to the
     *      corresponding validator.
     * \param der_to_validate The data structure to validate
     * \param object_info Metadata about the passed structure
     * \return An error if validation failed, else an empty result
     */
    static auto DelegateWellFormednessCheck(amsr::core::Span<std::uint8_t const> const& der_to_validate,
                                            Asn1Parser::ObjectInfo const& object_info) noexcept
        -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{TypeList<Head>::DelegateWellFormednessCheck(der_to_validate, object_info)};
      if ((retval.HasValue()) && (!retval.Value())) {
        retval = TypeList<Tail...>::DelegateWellFormednessCheck(der_to_validate, object_info);
      }
      return retval;
    }
  };

  /*!
   * \brief Typelist used for iterating CHOICE alternatives
   * \tparam Head The only remaining element of the iteration
   */
  template <typename Head>
  class TypeList<Head> {
   public:
    /*!
     * \brief If \c to_validate contains the structural type of the \c Head context, this delegates to the validator
     *      For \c Head
     * \param to_validate The data structure to validate
     * \return An error if validation failed, a result containing "false" if the type didn't match and a "true" result
     *      when the type matched and validation succeeded.
     */
    static auto DelegateValidityCheck(typename ContextType::StructureType const& to_validate) noexcept
        -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{amsr::core::Result<bool>::FromError(
          Asn1Errc::kRuntimeFault, "Encountered an unexpected type in a CHOICE"_sv)};
      if (typename Head::StructureType const* const guess_ptr{
              to_validate.template get_if<typename Head::StructureType>()}) {
        retval = Validator<Head>::IsValid(*guess_ptr).template Replace<bool>(true);
      } else {
        retval.EmplaceValue(false);
      }
      return retval;
    }

    /*!
     * \brief If \c der_to_validate contains the structural type of the \c Head context, this delegates to the validator
     *      For \c Head
     * \param der_to_validate The data structure to validate
     * \param object_info Metadata about the passed structure
     * \return An error if validation failed, a result containing "false" if the type didn't match and a "true" result
     *      when the type matched and validation succeeded.
     */
    static auto DelegateWellFormednessCheck(amsr::core::Span<std::uint8_t const> const& der_to_validate,
                                            Asn1Parser::ObjectInfo const& object_info) noexcept
        -> amsr::core::Result<bool> {
      amsr::core::Result<bool> retval{
          amsr::core::Result<bool>::FromError(Asn1Errc::kRuntimeFault, "Encountered an unexpected tag in a CHOICE"_sv)};

      // TODO: Unify tag class systems to avoid bit shifting and casting
      std::uint8_t expected_tag_class{static_cast<uint8_t>(Head::TagClass())};
      expected_tag_class = expected_tag_class / 64;
      std::uint8_t const actual_tag_class{static_cast<uint8_t>(object_info.element_class)};

      // Type checking in CHOICEs: Since CHOICE alternatives have unique tags, we can compare tags.
      // An exception must be made for untagged, nested CHOICEs. For them, the tag tree must be searched further.
      // If it has no tag, it cannot be wrong.
      bool const is_untagged_nested_choice{Head::TagNumber() ==
                                           static_cast<context::Tag>(context::UniversalTag::kUndefined)};

      // NOTE: Constructedness is not checked even though it's part of the tag: The same class-number combination cannot
      //  appear twice in a CHOICE. The constructedness of the alternative itself is checked by the underlying context.
      bool const tags_match{(actual_tag_class == expected_tag_class) &&
                            (object_info.element_tag == static_cast<uint64_t>(Head::TagNumber()))};

      if (is_untagged_nested_choice) {
        retval = Validator<Head>::IsWellFormed(der_to_validate).template Replace<bool>(true);
        // If, by delegation, no suitable alternative is found, we tell containing CHOICE validators to keep looking.
        // An error means "definitely wrong", a false result means "found nothing".
        if (!retval) {
          retval.EmplaceValue(false);
        }
      } else if (tags_match) {
        retval = Validator<Head>::IsWellFormed(der_to_validate).template Replace<bool>(true);
      } else {
        retval.EmplaceValue(false);
      }
      return retval;
    }
  };
};

/*!
 * \brief Validator for IMPLICITly tagged CHOICEs
 * \details Since there is no universal CHOICE tag, IMPLICIT tagging with an expressly defined tag like [0] makes
 *      the CHOICE itself have a tag. This behaves exactly like an EXPLICITly tagged CHOICE.
 * \tparam Ts The structure or data types from which to choose
 * \tparam NewTag see Tagged
 * \tparam NewClass see Tagged
 * \tparam DerivedThis The concrete Validator type for CRTP
 */
template <typename... Ts, context::Tag NewTag, context::Class NewClass, typename DerivedThis>
class ValidatorBase<context::Tagged<context::Choice<Ts...>, NewTag, NewClass>, DerivedThis>
    : public ValidatorBase<context::Explicit<context::Choice<Ts...>, NewTag, NewClass>, DerivedThis> {};

}  // namespace validator
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_VALIDATOR_H_
