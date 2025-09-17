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
/**        \file  either.h
 *        \brief  Internal union-like class that represents either the Left type or Right type.
 *         \unit  VaCommonLib::MemoryManagement::Either
 *
 *********************************************************************************************************************/
#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_EITHER_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_EITHER_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <type_traits>
#include <utility>

#include "ara/core/utility.h"
#include "vac/language/cpp17_backport.h"
#include "vac/language/detail/either_storage.h"
#include "vac/language/detail/either_trait.h"

namespace vac {
namespace language {
namespace detail {

/*!
 * \brief  An alias for ara::core::in_place_type_t.
 * \tparam T The type of in-place.
 */
template <typename T>
using in_place_type_t = ara::core::in_place_type_t<T>;

/*!
 * \brief  Internal union-like class that represents either the Left type or Right type.
 * \tparam L The left type for the Either union. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \tparam R The right type for the Either union. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \trace  CREQ-VaCommonLib-Either
 */
template <typename L, typename R>
class Either final {
 public:
  /*! \brief The trait used for analyzing the eithers traits. */
  using Trait = EitherTrait<L, R>;

  /*!
   * \brief     Constructor from left rvalue.
   * \param[in] l The value from which to construct the Either.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr explicit Either(L&& l) noexcept : Either(in_place_type_t<L>{}, std::move(l)) {}
  /*!
   * \brief     Constructor from right rvalue.
   * \param[in] r The value from which to construct the Either.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr explicit Either(R&& r) noexcept : Either(in_place_type_t<R>{}, std::move(r)) {}
  /*!
   * \brief     Constructor from left value.
   * \param     l The value from which to construct the Either.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr explicit Either(L const& l) noexcept : Either(in_place_type_t<L>{}, l) {}
  /*!
   * \brief     Constructor from right value.
   * \param     r The value from which to construct the Either.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr explicit Either(R const& r) noexcept : Either(in_place_type_t<R>{}, r) {}

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     In-place constructor for the left value.
   * \tparam    Args The type of arguments for the called constructor.
   * \param[in] args Arguments of construction. L must be constructible when using Args.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<L, Args...>::value>>
  constexpr explicit Either(in_place_type_t<L>, Args&&... args) noexcept
      : storage_{in_place_type_t<L>{}, std::forward<Args>(args)...} {}

  /*!
   * \brief     In-place constructor for the right value.
   * \tparam    Args The type of arguments for the called constructor.
   * \param[in] args Arguments of construction. R must be constructible when using Args.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<R, Args...>::value>>
  constexpr explicit Either(in_place_type_t<R>, Args&&... args) noexcept
      : storage_{in_place_type_t<R>{}, std::forward<Args>(args)...} {}

  /*!
   * \brief Copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr Either(Either const&) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_defaultedFunctionInheritsNoexcept
  /*!
   * \brief Move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr Either(Either&&) = default;

  /*!
   * \brief  Copy assignment.
   * \return A reference to this.
   * \spec
   *  requires true;
   * \endspec
   */
  auto operator=(Either const&) & -> Either& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_defaultedFunctionInheritsNoexcept
  /*!
   * \brief  Move assignment.
   * \return A reference to this.
   * \spec
   *  requires true;
   * \endspec
   */
  auto operator=(Either&&) & -> Either& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~Either() noexcept = default;

  /*!
   * \brief  Returns true if it is a left value.
   * \pre    -
   * \return True if the left value is contained.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr auto IsLeft() const& noexcept -> bool { return storage_.IsLeft(); }

  /*!
   * \brief  Returns true if it is a right value.
   * \pre    -
   * \return True if the right value is contained.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr auto IsRight() const& noexcept -> bool { return !this->IsLeft(); }

  /*!
   * \brief  Gets the Left value.
   * \pre    -
   * \return The left value.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr auto LeftUnsafe() const& noexcept -> L const& { return storage_.Left(); }

  /*!
   * \brief  Gets the Right value.
   * \pre    -
   * \return The right value.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr auto RightUnsafe() const& noexcept -> R const& { return storage_.Right(); }

  /*!
   * \brief  Gets the Left value.
   * \pre    -
   * \return The left value.
   * \spec
   *  requires true;
   * \endspec
   */
  auto LeftUnsafe() & noexcept -> L& { return storage_.Left(); }

  /*!
   * \brief  Gets the Right value.
   * \pre    -
   * \return The right value.
   * \spec
   *  requires true;
   * \endspec
   */
  auto RightUnsafe() & noexcept -> R& { return storage_.Right(); }

  /*!
   * \brief  Gets the Left value.
   * \pre    -
   * \return The left value.
   * \spec
   *  requires true;
   * \endspec
   */
  auto LeftUnsafe() && noexcept -> L&& { return std::move(this->LeftUnsafe()); }

  /*!
   * \brief  Gets the Right value.
   * \pre    -
   * \return The right value.
   * \spec
   *  requires true;
   * \endspec
   */
  auto RightUnsafe() && noexcept -> R&& { return std::move(this->RightUnsafe()); }

  /*!
   * \brief     Emplace a left value.
   * \tparam    Args The types of the values. L must be constructible when using Args.
   * \param[in] args The values that will construct the type to be assigned.
   * \pre       -
   * \return    A reference to the constructed value.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<L, Args...>::value>>
  auto EmplaceLeft(Args&&... args) noexcept -> L& {
    storage_.EmplaceLeft(std::forward<Args>(args)...);
    return this->LeftUnsafe();
  }

  /*!
   * \brief     Emplace a right value.
   * \tparam    Args The types of the values. R must be constructible when using Args.
   * \param[in] args The values that will construct the type to be assigned.
   * \pre       -
   * \return    A reference to the constructed value.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<R, Args...>::value>>
  auto EmplaceRight(Args&&... args) noexcept -> R& {
    storage_.EmplaceRight(std::forward<Args>(args)...);
    return this->RightUnsafe();
  }

 private:
  /*! \brief Container that stores the left or right value. */
  EitherStorage<L, R> storage_;
};

/*!
 * \brief  Checks for equality for eithers.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \param  a The first either to compare.
 * \param  b The second either to compare.
 * \pre    -
 * \return True if both eithers are the same, else false.
 * \spec
 *  requires true;
 * \endspec
 * \trace  CREQ-VaCommonLib-Either
 */
template <typename L, typename R>
constexpr auto operator==(Either<L, R> const& a, Either<L, R> const& b) noexcept -> bool {
  // VECTOR Disable AutosarC++17_10-A5.0.1: MD_VAC_A5.0.1_functionOrderIsNotRelevant
  bool res{false};
  if (a.IsLeft() == b.IsLeft()) {
    if (a.IsLeft()) {
      res = a.LeftUnsafe() == b.LeftUnsafe();
    } else {
      res = a.RightUnsafe() == b.RightUnsafe();
    }
  }
  // VECTOR Enable AutosarC++17_10-A5.0.1
  return res;
}

namespace either_swap {

/*!
 * \brief  Moves the left type content of an Either.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 *           Must be move constructible.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \param  e The either to extract the value from. Must hold a value of type L.
 * \return The content of e, as an r-value of the left type.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename L, typename R, std::enable_if_t<std::is_move_constructible<L>::value, std::int8_t> = 0>
auto MoveCopyL(Either<L, R>& e) noexcept -> L&& {
  assert(e.IsLeft());  // COV_MSR_INV_STATE_ASSERT
  return std::move(e).LeftUnsafe();
}

/*!
 * \brief  Copies the left type content of an Either.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 *           Must not be move constructible.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \param  e The either to extract the value from. Must hold a value of type L.
 * \return The content of e, as a const l-value of the left type.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename L, typename R, std::enable_if_t<!std::is_move_constructible<L>::value, std::int8_t> = 0>
auto MoveCopyL(Either<L, R> const& e) noexcept -> L const& {
  assert(e.IsLeft());  // COV_MSR_INV_STATE_ASSERT
  return e.LeftUnsafe();
}

/*!
 * \brief  Moves the right type content of an Either.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 *           Must be move constructible.
 * \param  e The either to extract the value from. Must hold a value of type R.
 * \return The content of e, as an r-value of the right type.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename L, typename R, std::enable_if_t<std::is_move_constructible<R>::value, std::int8_t> = 0>
auto MoveCopyR(Either<L, R>& e) noexcept -> R&& {
  assert(e.IsRight());  // COV_MSR_INV_STATE_ASSERT
  return std::move(e).RightUnsafe();
}

/*!
 * \brief  Copies the right type content of an Either.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 *           Must not be move constructible.
 * \param  e The either to extract the value from. Must hold a value of type R.
 * \return The content of e, as a const l-value of the right type.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename L, typename R, std::enable_if_t<!std::is_move_constructible<R>::value, std::int8_t> = 0>
auto MoveCopyR(Either<L, R> const& e) noexcept -> R const& {
  assert(e.IsRight());  // COV_MSR_INV_STATE_ASSERT
  return e.RightUnsafe();
}

/*!
 * \brief  Make the input parameter into an r-value.
 * \tparam T The type to convert. Must be move constructible.
 * \param  t The object to convert.
 * \return An r-value of t.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename T, std::enable_if_t<std::is_move_constructible<T>::value, std::int8_t> = 0>
auto MoveCopyT(T& t) noexcept -> T&& {
  return std::move(t);
}

/*!
 * \brief  Make the input parameter into an l-value.
 * \tparam T The type to convert. Must not be move constructible.
 * \param  t The object to convert.
 * \return An l-value of t.
 * \pre    Should only be called by vac::language::detail::swap() for Either<L, R>.
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename T, std::enable_if_t<!std::is_move_constructible<T>::value, std::int8_t> = 0>
auto MoveCopyT(T& t) noexcept -> T& {
  return t;
}

}  // namespace either_swap

/*!
 * \brief  Swap the content of two Either.
 * \tparam L The left type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \tparam R The right type of Either. Must fulfill vac::language::detail::EitherUnion type requirements.
 * \param  a The first either to swap.
 * \param  b The second either to swap.
 * \pre    -
 * \spec requires true; \endspec
 * \trace  CREQ-VaCommonLib-Either
 * \vprivate Component Private
 */
template <typename L, typename R>
void swap(Either<L, R>& a, Either<L, R>& b) noexcept {
  static_assert(std::is_move_constructible<L>::value || std::is_copy_constructible<L>::value,
                "Cannot swap the value of a type that can neither be copied nor moved.");
  static_assert(std::is_move_constructible<R>::value || std::is_copy_constructible<R>::value,
                "Cannot swap the value of a type that can neither be copied nor moved.");
  if (a.IsLeft() == b.IsLeft()) {  // Both hold the same type, we can easily swap.
    using std::swap;
    if (a.IsLeft()) {
      swap(a.LeftUnsafe(), b.LeftUnsafe());
    } else {
      swap(a.RightUnsafe(), b.RightUnsafe());
    }
  } else if (a.IsLeft()) {
    // They hold different types; we create a temporary variable and construct new ones in the the right places.
    L al{either_swap::MoveCopyL(a)};
    static_cast<void>(a.EmplaceRight(either_swap::MoveCopyR(b)));
    static_cast<void>(b.EmplaceLeft(either_swap::MoveCopyT(al)));
  } else {
    // Same as above, but b holds left.
    L bl{either_swap::MoveCopyL(b)};
    static_cast<void>(b.EmplaceRight(either_swap::MoveCopyR(a)));
    static_cast<void>(a.EmplaceLeft(either_swap::MoveCopyT(bl)));
  }
}

}  // namespace detail
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_EITHER_H_
