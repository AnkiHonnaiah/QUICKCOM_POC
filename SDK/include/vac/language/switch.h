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
/*!        \file    vac/language/switch.h
 *         \brief   Implements a switch that works for non primitive types.
 *         \details Switch that works on complex data and may be used as expression.
 *         \unit    VaCommonLib::LanguageSupport::Switch
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_SWITCH_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_SWITCH_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <type_traits>
#include <utility>

#include "ara/core/optional.h"

namespace vac {
namespace language {
namespace internal {
/*!
 * \brief Token type used to construct a SwitchType from a movable type.
 * \vprivate Component Private
 */
struct movable_in_t {};

/*!
 * \brief Named token instance to use when constructing a SwitchType from a movable object.
 * \vprivate Component Private
 */
constexpr movable_in_t kFromMovable{};
}  // namespace internal

/*!
 * \brief   A switch 'expression' for complex data.
 * \details May be used on any type that has an operator== implementation.
 * \tparam  Out The output type for the switch expression.
 * \tparam  In The type on which the switch operates.
 * \trace   CREQ-VaCommonLib-Switch
 * \vprivate Product Private
 */
template <typename Out, typename In>
class SwitchType final {
 public:
  /*! \brief Alias for In const
   */
  using InConst = std::add_const_t<In>;

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief Constructs a switch 'expression' from the value to match on by taking a copy of the provided in object.
   * \param in The value to match on.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit SwitchType(InConst& in) noexcept : in_{in} {}

  /*!
   * \brief     Constructs a switch 'statement' from the value to match on by taking over the ownership of the provided
   *            in object.
   * \param[in] in The value to match on.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  SwitchType(internal::movable_in_t, In&& in) noexcept : in_{std::move(in)} {}

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief     A case 'expression' for the current switch.
   * \pre       -
   * \tparam    SupplierType The function type for the value producer.
   * \tparam    SupplierResult The type deduces the return type of SupplierType().
   * \param     input The case value.
   * \param[in] supplier Returns the data if the case matches.
   * \return    The Switch itself to enable chaining.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename SupplierType, typename SupplierResult = std::result_of_t<SupplierType()>,
            typename = std::enable_if_t<std::is_constructible<Out, SupplierResult>::value>>
  auto Case(InConst& input, SupplierType&& supplier) && noexcept(false) -> SwitchType&& {
    if ((!out_) && (in_ == input)) {
      out_.emplace(std::forward<SupplierResult>(supplier()));
    }
    return std::move(*this);
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
  /*!
   * \brief     A case 'expression' for the current switch.
   * \pre       -
   * \tparam    OutputType The output type of the value.
   * \param     input The case value.
   * \param[in] value Returns the data if the case matches.
   * \return    The Switch itself to enable chaining.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename OutputType, typename = std::enable_if_t<std::is_constructible<Out, OutputType>::value>>
  auto Case(InConst& input, OutputType&& value) && noexcept(false) -> SwitchType&& {
    if ((!out_) && (in_ == input)) {
      out_.emplace(std::forward<OutputType>(value));
    }
    return std::move(*this);
  }

  /*!
   * \brief     A default case 'expression' for the current switch.
   * \tparam    Supplier The function type for the value producer.
   * \tparam    SupplierResult The type deduces the return type of Supplier().
   * \param[in] supplier Returns the data if no other case matched.
   * \pre       -
   * \return    Either the supplied value or a value produced before.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename Supplier, typename SupplierResult = std::result_of_t<Supplier()>,
            typename = std::enable_if_t<std::is_constructible<Out, SupplierResult>::value>>
  auto Default(Supplier&& supplier) && noexcept(false) -> Out {
    return out_ ? *(std::move(this)->out_) : static_cast<Out>(std::forward<SupplierResult>(supplier()));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     A default case 'expression' for the current switch.
   * \tparam    Out1 The function type for the value producer.
   * \param[in] value Returns the data if no other case matched.
   * \pre       -
   * \return    Either the supplied value or a value produced before.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename Out1, std::enable_if_t<std::is_constructible<Out, Out1>::value, std::int32_t> = 0>
  auto Default(Out1&& value) && noexcept(false) -> Out {
    return out_ ? *(std::move(this)->out_) : static_cast<Out>(std::forward<Out1>(value));
  }

  /*!
   * \brief  Gets the unexhaustive result.
   * \pre    -
   * \return Either the produced value or an empty optional.
   * \spec
   *  requires true;
   * \endspec
   */
  auto Get() const&& -> ara::core::Optional<Out> { return std::move(this)->out_; }

 private:
  /*! \brief The container for the return value.
   */
  ara::core::Optional<Out> out_{};
  /*! \brief The switch value to match on.
   */
  In in_;
};

/*!
 * \brief   A switch 'statement' for complex data.
 * \details May be used on any type that has an operator== implementation.
 * \tparam  In The type on which the switch operates.
 * \trace   CREQ-VaCommonLib-Switch
 * \vprivate Product Private
 */
template <typename In>
class SwitchType<void, In> final {
 public:
  /*! \brief Alias for In const
   */
  using InConst = std::add_const_t<In>;

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief Constructs a switch 'statement' from the value to match on by taking a copy of the provided in object.
   * \param in The value to match on.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit SwitchType(InConst& in) noexcept : in_{in} {}  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief     Constructs a switch 'statement' from the value to match on by taking over the ownership of the provided
   *            in object.
   * \param[in] in The value to match on.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  SwitchType(internal::movable_in_t, In&& in) noexcept : in_{std::move(in)} {}

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief     A case 'statement' for the current switch.
   * \tparam    EffectType The function type that produces the side effect.
   * \param     input The case value.
   * \param[in] effect Performs the side effect if the case matches.
   * \pre       -
   * \return    The Switch itself to enable chaining.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename EffectType>
  auto Case(InConst& input, EffectType&& effect) && noexcept(false) -> SwitchType&& {
    if (need_match_ && (in_ == input)) {
      need_match_ = false;
      effect();  // VCA_VAC_TEMPLATE_REQ_MET
    }
    return std::move(*this);
  }

  /*!
   * \brief     A default case 'statement' for the current switch.
   * \tparam    Effect The function type that produces the side effect.
   * \param[in] effect Performs the side effect if no other case matched.
   * \pre       -
   * \return    Either the supplied value or a value produced before.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename Effect>
  auto Default(Effect&& effect) && noexcept(false) -> void {
    if (need_match_) {
      need_match_ = false;
      effect();  // VCA_VAC_TEMPLATE_REQ_MET
    }
  }

 private:
  /*! \brief The switch value to match on.
   */
  In in_;
  /*! \brief Marker if further case statements need to be considered.
   */
  bool need_match_{true};
};

// VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
/*!
 * \brief   Constructs a switch 'expression'/'statement' from the value to match on.
 * \details May be used on any type that has an operator== implementation.
 * \tparam  Out The output type for the switch expression.
 * \tparam  In The type on which the switch operates.
 * \param   in The value to match on.
 * \pre     -
 * \return  A switch 'expression'/'statement'.
 * \spec
 *  requires true;
 * \endspec
 * \trace   CREQ-VaCommonLib-Switch
 * \vprivate Product Private
 */
template <typename Out = void, typename In>
auto Switch(In const& in) noexcept(false) -> SwitchType<Out, In> {
  return SwitchType<Out, In>{in};
}

// VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
/*!
 * \brief     Constructs a switch 'expression'/'statement' from the value to match on.
 * \details   May be used on any type that has an operator== implementation.
 * \tparam    Out The output type for the switch expression.
 * \tparam    In The type on which the switch operates.
 * \remark    Function is enabled using SFINAE only for In types that are move constructible and not references.
 * \param[in] in The value to match on.
 * \pre       -
 * \return    A switch 'expression'/'statement'.
 * \spec
 *  requires true;
 * \endspec
 * \trace     CREQ-VaCommonLib-Switch
 * \vprivate  Product Private
 */
template <typename Out = void, typename In,
          typename = std::enable_if_t<std::is_move_constructible<In>::value && (!std::is_reference<In>::value)>>
auto Switch(In&& in) noexcept(false) -> SwitchType<Out, In> {
  return SwitchType<Out, In>{internal::kFromMovable, std::forward<In>(in)};
}

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_SWITCH_H_
