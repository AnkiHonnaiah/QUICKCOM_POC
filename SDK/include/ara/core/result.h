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
/*!     \file    ara/core/result.h
 *      \brief   SWS core type ara::core::Result.
 *      \unit    VaCommonLib::ErrorHandling::Result
 *
 *      \details A Result contains either a Value representing the actual result of a function or an Error
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_RESULT_H_
#define LIB_VAC_INCLUDE_ARA_CORE_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "ara/core/abort.h"
#include "ara/core/error_code.h"
#include "ara/core/utility.h"
#include "vac/language/compile_exceptions.h"
#include "vac/language/cpp17_backport.h"
#include "vac/language/detail/callable_check.h"
#include "vac/language/detail/either.h"
#include "vac/testing/test_adapter.h"

namespace ara {
namespace core {
namespace detail {

/*!
 * \brief  Extracts the first type of a parameter pack.
 * \tparam Args... the types of arguments given to this function.
 * \vprivate
 */
template <typename... Args>
class FirstType;

/*!
 * \brief Template specialization for an empty parameter pack.
 * \vprivate
 */
template <>
class FirstType<> : public std::false_type {};

/*!
 * \brief  The function with at least one argument of the Result.
 * \tparam First The first type of arguments given to this function.
 * \tparam Args... The types of arguments given to this function.
 * \vprivate
 */
template <typename First, typename... Args>
class FirstType<First, Args...> {
 public:
  /*!
   * \brief The type of the first argument.
   */
  using type = First;
};

/*!
 * \brief  First type of the Result.
 * \tparam Args... the types of arguments given to this function.
 * \vprivate
 */
template <typename... Args>
using FirstTypeT = typename FirstType<Args...>::type;

/*!
 * \brief Phantom Type to mark that the Result is empty
 */
struct Empty {};

}  // namespace detail

// Forward declaration for Result.
template <typename T, typename E = ErrorCode>
class Result;

/*!
 * \brief   Trait that detects whether a type is a Result<...>.
 * \details SFINAE pattern: General template with typedef type false.
 * \tparam  U Arbitrary placeholder for SFINAE pattern.
 * \trace   DSGN-VaCommonLib-ErrorHandling
 * \vprivate
 */
template <typename U>
struct is_result : std::false_type {};

/*!
 * \brief   Trait that detects whether a type is a Result<...>.
 * \details SFINAE pattern: Specialization for Result<U, G>.
 * \tparam  U Value of Result.
 * \tparam  G Error of Result.
 * \vprivate
 */
template <typename U, typename G>
struct is_result<Result<U, G>> : std::true_type {};

// VECTOR NC AutosarC++17_10-A1.1.1: MD_VAC_A1.1.1_externalIdentifiers
// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief  This class is a type that contains either a value or an error.
 * \tparam T The type of value. Must be object/integral type, and must be move-constructible.
 * \tparam E The type of error. Must be object/integral type, must be move-constructible, must be copy-constructible,
 *         and must not be (implicitly) convertible to T.
 * \trace  DSGN-VaCommonLib-ErrorHandling
 * \vpublic
 */
template <typename T, typename E>
class Result final {
  // Assertions for T
  static_assert(std::is_object<T>::value, "T must be object/integral type.");
  static_assert(std::is_move_constructible<T>::value,
                "T must be move-constructible. Otherwise it cannot be used as return type!");

  // Assertions for E
  static_assert(std::is_object<E>::value, "E must be object/integral type.");
  static_assert(std::is_move_constructible<E>::value,
                "E must be move-constructible. Otherwise it cannot be used as return type!");
  static_assert(std::is_copy_constructible<E>::value,
                "E must be copy-constructible. Move-only Error types may require additional r-value overloads of "
                "member functions for Result.");
  static_assert(!std::is_convertible<E, T>::value,
                "E must not be (implicitly) convertible to T! Otherwise, undesired effects may occur, because "
                "Result(T const&) and Result(T&&) are not explicit.");

  /*!
   * \brief  SFINAE for callable returning Result<...>.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsResult = std::enable_if_t<is_result<std::result_of_t<F(T const&)>>::value>;

  /*!
   * \brief  SFINAE for callable returning void.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsVoid = std::enable_if_t<std::is_void<std::result_of_t<F(T const&)>>::value>;

  /*!
   * \brief  SFINAE for callable not returning Result<...>.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsNoResult = std::enable_if_t<(!is_result<std::result_of_t<F(T const&)>>::value) &&
                                                   (!std::is_void<std::result_of_t<F(T const&)>>::value)>;

 public:
  /*! \brief Value type of the Result. */
  using value_type = T;
  /*! \brief Error type of the Result. */
  using error_type = E;
  /*! \brief In-place construct constant for value_type */
  constexpr static in_place_type_t<value_type> kInPlaceValue{};
  /*! \brief In-place construct constant for error_type */
  constexpr static in_place_type_t<error_type> kInPlaceError{};

  /*!
   * \brief     Build a new Result from a value that is constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in T the process is terminated.
   *            This function shall not participate in overload resolution unless:
   *            std::is_constructible<T, Args&&...>::value is true, and the first type of the expanded parameter pack is
   *            not T, and the first type of the expanded parameter pack is not a specialization of Result.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args the arguments used for constructing the value.
   * \pre       -
   * \return    A Result that contains a value.
   * \vpublic
   */
  template <typename... Args, typename = std::enable_if_t<(std::is_constructible<T, Args&&...>::value) &&
                                                          (!std::is_same<T, detail::FirstTypeT<Args...>>::value) &&
                                                          (!is_result<detail::FirstTypeT<Args...>>::value)>>
  constexpr static auto FromValue(Args&&... args) noexcept -> Result {
    return Result{Result::kInPlaceValue, std::forward<Args>(args)...};
  }

  /*!
   * \brief   Build a new Result from the specified value (given as lvalue).
   * \details If an exception is raised by the copy constructor in T the process is terminated.
   * \param   t The value to put into the Result.
   * \pre     -
   * \return  A Result that contains the value t.
   * \vpublic
   */
  constexpr static auto FromValue(T const& t) noexcept -> Result { return Result{Result::kInPlaceValue, t}; }

  /*!
   * \brief     Build a new Result from the specified value (given as rvalue).
   * \details   If an exception is raised by the move constructor in T the process is terminated.
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \return    A Result that contains the value t.
   * \vpublic
   */
  constexpr static auto FromValue(T&& t) noexcept -> Result { return Result{Result::kInPlaceValue, std::move(t)}; }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   *            This function shall not participate in overload resolution unless:
   *            std::is_constructible<E, Args&&...>::value is true, and the first type of the expanded parameter pack is
   *            not E, and the first type of the expanded parameter pack is not a specialization of Result.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args The arguments used for constructing the error.
   * \pre       -
   * \return    A Result that contains an error.
   * \vpublic
   */
  template <typename... Args, typename = std::enable_if_t<(std::is_constructible<E, Args&&...>::value) &&
                                                          (!std::is_same<E, detail::FirstTypeT<Args...>>::value) &&
                                                          (!is_result<detail::FirstTypeT<Args...>>::value)>>
  constexpr static auto FromError(Args&&... args) noexcept -> Result {
    return Result{Result::kInPlaceError, std::forward<Args>(args)...};
  }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] error_value A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \return    A Result that contains an error.
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  static constexpr auto FromError(EnumT error_value, StringView user_msg) noexcept -> Result {
    return Result{ErrorCode(error_value, user_msg.data())};
  }
  /*!
   * \brief   Build a new Result from the specified error (given as lvalue).
   * \details If an exception is raised by the copy constructor in E the process is terminated.
   * \param   e The error to put into the Result.
   * \pre     -
   * \return  A Result that contains the error e.
   * \vpublic
   */
  static auto FromError(E const& e) noexcept -> Result { return Result{Result::kInPlaceError, e}; }

  /*!
   * \brief   Build a new Result from the specified error (given as rvalue).
   * \details If an exception is raised by the constructor in E the process is terminated.
   * \param   e The error to put into the Result.
   * \pre     -
   * \return  A Result that contains the error e.
   * \vpublic
   */
  static auto FromError(E&& e) noexcept -> Result { return Result{Result::kInPlaceError, std::move(e)}; }

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief   Construct a new Result from the specified value (given as lvalue).
   * \details If an exception is raised by the copy constructor in T the process is terminated.
   * \param   t The value to put into the Result.
   * \pre     -
   * \vpublic
   */
  constexpr Result(T const& t) noexcept : Result{Result::kInPlaceValue, t} {}  // NOLINT(runtime/explicit)

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct a new Result from the specified value (given as rvalue).
   * \details   If an exception is raised by the move constructor in T the process is terminated.
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \vpublic
   */
  constexpr Result(T&& t) noexcept : Result{Result::kInPlaceValue, std::move(t)} {}  // NOLINT(runtime/explicit)

  /*!
   * \brief   Construct a new Result from the specified error (given as lvalue).
   * \details If an exception is raised by the copy constructor in E the process is terminated.
   * \param   e The error to put into the Result.
   * \pre     -
   * \vpublic
   */
  constexpr explicit Result(E const& e) noexcept : Result{Result::kInPlaceError, e} {}

  /*!
   * \brief     Construct a new Result from the specified error (given as rvalue).
   * \details   If an exception is raised by the move constructor in E the process is terminated.
   * \param[in] e The error to put into the Result.
   * \pre       -
   * \vpublic
   */
  constexpr explicit Result(E&& e) noexcept : Result{Result::kInPlaceError, std::move(e)} {}

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     In-place tag constructor for a Value.
   * \details   If an exception is raised by the constructor in T the process is terminated.
   * \param[in] args Construction parameter.
   * \pre       -
   * \vprivate
   */
  template <typename... Args>
  constexpr explicit Result(in_place_type_t<T>, Args&&... args) noexcept
      : data_{Result::kInPlaceValue, std::forward<Args>(args)...} {}

  /*!
   * \brief     In-place tag constructor for an Error.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   * \param[in] args Construction parameter.
   * \pre       -
   * \vprivate
   */
  template <typename... Args>
  constexpr explicit Result(in_place_type_t<E>, Args&&... args) noexcept
      : data_{Result::kInPlaceError, std::forward<Args>(args)...} {}

  /*!
   * \brief   Copy-construct a new Result from another instance.
   * \details If an exception is raised by the copy constructor in T or E the process is terminated.
   * \param   other The other instance.
   * \vpublic
   */
  constexpr Result(Result const& other) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Move-construct a new Result from another instance.
   * \details If an exception is raised by the move constructor in T or E the process is terminated.
   * \param   other The other instance.
   * \vpublic
   */
  constexpr Result(Result&& other) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  ~Result() noexcept = default;

  /*!
   * \brief   Copy-assign another Result to this instance.
   * \details If an exception is raised by the copy assignment in T or E the process is terminated.
   * \param   other The other instance.
   * \return  *this, containing the contents of other.
   * \vpublic
   */
  auto operator=(Result const& other) & -> Result& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Move-assign another Result to this instance.
   * \details If an exception is raised by the move assignment in T or E the process is terminated.
   * \param   other The other instance.
   * \return  *this, containing the contents of other.
   * \vpublic
   */
  auto operator=(Result&& other) & -> Result& = default;

  /*!
   * \brief     Put a new value into this instance, constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in T the process is terminated.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args  The arguments used for constructing the value.
   * \pre       -
   * \return
   * \vpublic
   */
  template <typename... Args>
  auto EmplaceValue(Args&&... args) noexcept -> void {
    this->data_.EmplaceRight(std::forward<Args>(args)...);
  }

  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_dynamicMemPossiblyUsedAfterReleased
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args The arguments used for constructing the error.
   * \pre       -
   * \return
   * \vpublic
   */
  template <typename... Args>
  auto EmplaceError(Args&&... args) noexcept -> void {
    this->data_.EmplaceLeft(std::forward<Args>(args)...);
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] e A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \return
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  auto EmplaceError(EnumT e, StringView user_msg) noexcept -> void {
    this->data_.EmplaceLeft(ErrorCode{e, user_msg.data()});
  }
  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief         Exchange the contents of this instance with those of other.
   * \param[in,out] other The other instance.
   * \pre           -
   * \spec
   *   requires true;
   * \endspec
   * \return
   * \vpublic
   */
  auto Swap(Result& other) noexcept -> void {
    using std::swap;
    swap(data_, other.data_);
  }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains a value, false otherwise.
   * \vpublic
   */
  constexpr explicit operator bool() const noexcept { return HasValue(); }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains a value, false otherwise.
   * \vpublic
   */
  constexpr auto HasValue() const noexcept -> bool { return this->data_.IsRight(); }

  /*!
   * \brief   Access the contained value.
   * \details This function's behavior is undefined if *this does not contain a value.
   * \pre     This function should only be called if *this contains a value.
   * \return  A reference to the contained value.
   * \vpublic
   */
  constexpr auto operator*() const noexcept -> T const& { return Value(); }

  // VECTOR NC AutosarC++17_10-A5.0.3: MD_VAC_A5.0.3_NoMoreThanThreeLevelsOfPointers
  /*!
   * \brief   Access the contained value.
   * \details This function's behavior is undefined if *this does not contain a value.
   * \pre     -
   * \return  A pointer to the contained value.
   * \vpublic
   */
  constexpr auto operator->() const noexcept -> T const* { return std::addressof(Value()); }

  /*!
   * \brief   Access the contained value.
   * \details The behavior of this function is undefined if *this does not contain a value.
   * \pre     This function should only be called if *this contains a value.
   * \return  A reference to the contained value.
   * \vpublic
   */
  constexpr auto Value() const& noexcept -> T const& {
    assert(HasValue());
    return this->data_.RightUnsafe();
  }

  /*!
   * \brief  Non-const access the contained value.
   *         The behavior of this function is undefined if *this does not contain a value.
   * \pre    This function should only be called if *this contains a value.
   * \return A non-const reference to the contained value.
   * \vprivate
   */
  auto Value() & noexcept -> T& {
    assert(HasValue());  // COV_MSR_INV_STATE_ASSERT
    return this->data_.RightUnsafe();
  }

  /*!
   * \brief   Access the contained value.
   * \details The behavior of this function is undefined if *this does not contain a value.
   * \pre     This function should only be called if *this contains a value.
   * \return  A reference to the contained value.
   * \vpublic
   */
  auto Value() && noexcept -> T&& { return std::move(this->Value()); }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     This function should only be called if *this contains an error.
   * \return  A reference to the contained error.
   * \vpublic
   */
  constexpr auto Error() const& noexcept -> E const& {
    assert(!HasValue());
    return this->data_.LeftUnsafe();
  }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     This function should only be called if *this contains an error.
   * \return  A rvalue reference to the contained error.
   * \vpublic
   */
  auto Error() && noexcept -> E&& {
    assert(!HasValue());  // COV_MSR_INV_STATE_ASSERT
    return std::move(this->data_.LeftUnsafe());
  }

  /*!
   * \brief     Return the contained value or the given default value.
   * \details   If *this contains a value, it is returned. Otherwise, the specified default value is returned,
   *            converted to T.
   * \tparam    U the type of defaultValue. U&& must be (implicitly) convertible to T.
   * \param[in] defaultValue  The value to use if *this does not contain a value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value.
   * \vpublic
   */
  template <typename U>
  constexpr auto ValueOr(U&& defaultValue) const& noexcept -> T {
    static_assert(std::is_convertible<U&&, T>::value, "U&& must be convertible to T.");
    return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Return the contained value or the given default value.
   * \details   If *this contains a value, it is returned. Otherwise, the specified default value is returned,
   *            converted to T.
   * \tparam    U the type of defaultValue. U&& must be (implicitly) convertible to T.
   * \param[in] defaultValue  The value to use if *this does not contain a value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value.
   * \vpublic
   */
  template <typename U>
  auto ValueOr(U&& defaultValue) && noexcept -> T {
    static_assert(std::is_convertible<U&&, T>::value, "U&& must be convertible to T.");
    return HasValue() ? std::move(*this).Value() : static_cast<T>(std::forward<U>(defaultValue));
  }

  /*!
   * \brief     Return the contained error or the given default error.
   * \details   If *this contains an error, it is returned. Otherwise, the specified default error is returned,
   *            converted to E.
   * \tparam    G The type of defaultError.
   * \param[in] defaultError The error to use if *this does not contain an error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The error.
   * \vpublic
   */
  template <typename G>
  constexpr auto ErrorOr(G&& defaultError) const noexcept -> E {
    static_assert(std::is_convertible<G&&, E>::value, "G&& must be convertible to E.");
    return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
  }

  /*!
   * \brief     Return whether this instance contains the given error.
   * \tparam    G The type of the error e.
   * \param[in] error The error to check.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    True if *this contains the given error, false otherwise.
   * \vpublic
   */
  template <typename G>
  constexpr auto CheckError(G&& error) const noexcept -> bool {
    static_assert(std::is_convertible<G&&, E>::value, "G&& must be convertible to E.");
    return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
  }

  /*!
   * \brief   Return the contained value or throw an exception.
   * \details This function does not participate in overload resolution when the compiler toolchain
   *          does not support C++ exceptions.
   * \pre     ErrorType must have function ThrowAsException().
   * \return  The value.
   * \throws  <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() const& noexcept(false) -> T const& {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    if (!HasValue()) {
      E const e{this->data_.LeftUnsafe()};
      e.ThrowAsException();
    }
    return Value();
  }

  /*!
   * \brief   Return the contained value or throw an exception.
   * \details This function does not participate in overload resolution when the compiler toolchain
   *          does not support C++ exceptions.
   * \pre     ErrorType must have function ThrowAsException().
   * \return  The value.
   * \throws  <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() && noexcept(false) -> T&& {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    if (!HasValue()) {
      E const e{this->data_.LeftUnsafe()};
      e.ThrowAsException();
    }
    return std::move(Value());
  }

  /*!
   * \brief     Returns the contained value or return the result of a function call.
   * \details   If *this contains a value, it is returned. Otherwise, the specified callable is invoked and its return
   *            value which is to be compatible to type T is returned from this function.
   *            The Callable is expected to be compatible to this interface:
   *            <code>
   *            T f(E const&);
   *            </code>
   *            If the callable is an nullptr, abortion will be triggered.
   * \tparam    F The type of the Callable f.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    The contained value or return the result of a function call.
   * \vpublic
   */
  template <typename F>
  constexpr auto Resolve(F&& callable_f) const& noexcept -> T {
    static_assert(std::is_convertible<std::result_of_t<F(error_type const&)>, T>::value,
                  "Return type of f must be (implicitly) convertible to T.");
    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable_f)};
    if (is_null_ptr) {
      ara::core::Abort("ara::core::Result::Resolve: The callable cannot be nullptr!");
    }
    return HasValue()
               ? Value()
               // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
               : static_cast<T>(std::forward<F>(callable_f)(Error()));  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Returns the contained value or return the result of a function call.
   * \details   If *this contains a value, it is returned. Otherwise, the specified callable is invoked and its return
   *            value which is to be compatible to type T is returned from this function.
   *            The Callable is expected to be compatible to this interface:
   *            <code>
   *            T f(E const&);
   *            </code>
   *            If the callable is an nullptr, abortion will be triggered.
   * \tparam    F The type of the Callable f.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    The contained value or return the result of a function call.
   * \vpublic
   */
  template <typename F>
  auto Resolve(F&& callable_f) && noexcept -> T {
    return HasValue() ? std::move(*this).Value() : std::forward<F>(callable_f)(std::move(*this).Error());
  }

  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the
   *            result of the call.
   * \details   The Callable is expected to be compatible to one of these two interfaces:
   *            <code>
   *            Result<XXX, E> f(T const&);
   *            XXX f(T const&);
   *            </code>
   *            meaning that the Callable either returns a Result<XXX> or a XXX directly,
   *            where XXX can be any type that is suitable for use by class Result.
   *            The return type of this function is always Result<XXX, E>.
   *            If this instance does not contain a value, a new Result<XXX, E> is still
   *            created and returned, with the original error contents of this instance
   *            being copied into the new instance.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsResult<F> Enable template when callable F returns Result<...> type.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type.
   * \vpublic
   */
  template <typename F, typename = CallableReturnsResult<F>>
  constexpr auto Bind(F&& callable_f) const noexcept -> std::result_of_t<F(value_type const&)> {
    return AndThen(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the
   *            result of the call.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsNoResult<F> Enable template when callable f does not return Result<...> type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   * \vpublic
   */
  template <typename F, typename = CallableReturnsNoResult<F>>
  constexpr auto Bind(F&& callable_f) const noexcept -> Result<std::result_of_t<F(value_type const&)>, E> {
    return Map(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the
   *            result of the call.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsVoid<F> Enable template when callable f returns void.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   * \vpublic
   */
  template <typename F, typename = CallableReturnsVoid<F>>
  constexpr auto Bind(F&& callable_f) const& noexcept -> Result<void, E> {
    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable_f)};
    if (is_null_ptr) {
      ara::core::Abort("ara::core::Result::Bind: The callable cannot be nullptr!");
    }
    return AndThen([&callable_f](value_type const& value) -> Result<void, E> {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(value);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      return Result<void, E>{Result<void, E>::kInPlaceValue};
    });
  }

  /*!
   * \brief     Executes the given Callable with the value of this instance, or returns the contained error.
   * \tparam    F The type of the Callable f. Must be compatible with the signature XX f(T const&).
   *            Return type must not be void.
   * \tparam    U The return value of the Function F.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type or the contained error.
   * \remark    This function can be used to transform the contained value.
   * \vprivate
   */
  template <typename F, typename U = std::result_of_t<F(value_type const&)>>
  constexpr auto Map(F&& callable_f) const& noexcept -> Result<U, E> {
    static_assert(!std::is_void<U>::value, "Return type of f must not be void. Use Inspect()/Drop() instead.");
    using R1 = Result<U, E>;
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return HasValue() ? R1{R1::kInPlaceValue, std::forward<F>(callable_f)(Value())}
                      : R1{Result::kInPlaceError, Error()};
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Executes the given Callable with the value of this instance, or returns the contained error.
   * \tparam    F The type of the Callable f. Must be compatible with the signature XX f(T&&).
   *            Return type must not be void.
   * \tparam    U The return value of the Function F.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type or the contained error.
   * \remark    This function can be used to transform the contained value.
   * \vprivate
   */
  template <typename F, typename U = std::result_of_t<F(value_type&&)>>
  auto Map(F&& callable_f) && noexcept -> Result<U, E> {
    static_assert(!std::is_void<U>::value, "Return type of f must not be void. Use Inspect()/Drop() instead.");
    /*! \brief Local result type alias. */
    using R2 = Result<U, E>;
    // If value, the value is moved to the provided function and the returned value of the function is forwarded to the
    // Result constructor, otherwise the error is moved to the Result constructor.
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return HasValue() ? R2{R2::kInPlaceValue, std::forward<F>(callable_f)(std::move(*this).Value())}
                      : R2{Result::kInPlaceError, std::move(*this).Error()};
  }

  /*!
   * \brief     Executes the given Callable with the error of this instance, or returns the untouched value.
   * \tparam    F the type of the Callable f. Must be compatible with the signature XX f(E const&).
   *            Return Type must be an object/integral type.
   * \tparam    E1 The return value of the Function F.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed error type or the unchanged value.
   * \remark    This function can be used to transform the contained error, or perform error handling in general.
   * \vprivate
   */
  template <typename F, typename E1 = std::result_of_t<F(error_type const&)>>
  constexpr auto MapError(F&& callable_f) const& noexcept -> Result<T, E1> {
    /*! \brief Local result type alias. */
    using R3 = Result<T, E1>;
    return HasValue()
               ? R3{Result::kInPlaceValue, Value()}
               : R3{R3::kInPlaceError, std::forward<F>(callable_f)(Error())};  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Executes the given Callable with the error of this instance, or returns the untouched value.
   * \tparam    F The type of the Callable f. Must be compatible with the signature XX f(E&&).
   *            Return Type must be an object/integral type.
   * \tparam    E1 The return value of the Function F
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed error type or the unchanged value.
   * \remark    This function can be used to transform the contained error, or perform error handling in general.
   * \vprivate
   */
  template <typename F, typename E1 = std::result_of_t<F(error_type&&)>>
  auto MapError(F&& callable_f) && noexcept -> Result<T, E1> {
    /*! \brief Local result type alias. */
    using R4 = Result<T, E1>;
    return HasValue() ? R4{Result::kInPlaceValue, std::move(*this).Value()}
                      : R4{R4::kInPlaceError,
                           std::forward<F>(callable_f)(std::move(*this).Error())};  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief     Create a new Result with the given new value if this instance contains a value, otherwise it
   *            return the contained error.
   * \details   The new value is inplace-constructed from the given parameters args.
   * \tparam    U Rhe type of the given value, defaulted to T.
   * \tparam    Args Construction arguments for U.
   * \tparam    Res The return type of the Replace function.
   * \param[in] args Arguments for the in place construction of the replacement value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance with the constructed replacement value or the contained error.
   * \remark    This function can be used to replace the contained value, which type may differ from T.
   * \vprivate
   */
  template <typename U = T, typename... Args, typename Res = Result<U, E>>
  constexpr auto Replace(Args&&... args) const& noexcept -> Res {
    return HasValue() ? Res{Res::kInPlaceValue, std::forward<Args>(args)...} : Res{Result::kInPlaceError, Error()};
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Create a new Result with the given new value if this instance contains a value, otherwise it
   *            return the contained error.
   * \details   The new value is inplace-constructed from the given parameters args.
   * \tparam    U The type of the given value, defaulted to T.
   * \tparam    Args Construction arguments for U.
   * \tparam    Res The return type of the Replace function.
   * \param[in] args Arguments for the in place construction of the replacement value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance with the constructed replacement value or the contained error.
   * \remark    This function can be used to replace the contained value, which type may differ from T.
   * \vprivate
   */
  template <typename U = T, typename... Args, typename Res = Result<U, E>>
  auto Replace(Args&&... args) && noexcept -> Res {
    return HasValue() ? Res{Res::kInPlaceValue, std::forward<Args>(args)...}
                      : Res{Result::kInPlaceError, std::move(*this).Error()};
  }

  /*!
   * \brief  Returns the contained value, else it returns the given alternative.
   * \tparam E1 New error type.
   * \param  alternative Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The value of this, else alternative.
   * \vprivate
   */
  template <typename E1>
  constexpr auto Or(Result<T, E1> const& alternative) const& noexcept -> Result<T, E1> {
    return HasValue() ? Result<T, E1>{Result::kInPlaceValue, Value()} : alternative;
  }

  /*!
   * \brief     Returns the contained value, else it returns the given alternative.
   * \tparam    E1 New error type.
   * \param[in] alternative Other result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value of this, else alternative.
   * \vprivate
   */
  template <typename E1>
  auto Or(Result<T, E1>&& alternative) && noexcept -> Result<T, E1> {
    /*! \brief Local result type alias. */
    return HasValue() ? Result<T, E1>{Result::kInPlaceValue, std::move(*this).Value()} : std::move(alternative);
  }

  /*!
   * \brief     Returns the contained value, else transforms the contained error using the passed function.
   * \tparam    F Transform function for the error. Must be compatible to the signature Result<T,XX> f(E const&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value of this, or the transformation result of the error.
   * \vprivate
   */
  template <typename F>
  constexpr auto OrElse(F&& callable_f) const& noexcept -> std::result_of_t<F(error_type const&)> {
    /*! \brief Local result type alias. */
    using R5 = std::result_of_t<F(error_type const&)>;
    static_assert(is_result<R5>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<value_type, typename R5::value_type>::value, "Value type must not change.");
    return HasValue() ? R5{Result::kInPlaceValue, Value()}
                      : std::forward<F>(callable_f)(Error());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Returns the contained value, else transforms the contained error using the passed function.
   * \tparam    F Transform function for the error. Must be compatible to the signature Result<T,XX> f(E&&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value of this, or the transformation result of the error.
   * \vprivate
   */
  template <typename F>
  auto OrElse(F&& callable_f) && noexcept -> std::result_of_t<F(error_type&&)> {
    /*! \brief Local result type alias. */
    using R6 = std::result_of_t<F(error_type &&)>;
    static_assert(is_result<R6>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<value_type, typename R6::value_type>::value, "Value type must not change.");
    return HasValue() ? R6{Result::kInPlaceValue, std::move(*this).Value()}
                      : std::forward<F>(callable_f)(std::move(*this).Error());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief  Returns the passed Result if this has a value, else it returns the error of this.
   * \tparam U New value type.
   * \param  other Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return other if this has a value, else the error of this.
   * \vprivate
   */
  template <typename U>
  constexpr auto And(Result<U, E> const& other) const& noexcept -> Result<U, E> {
    return HasValue() ? other : Result<U, E>{Result::kInPlaceError, Error()};
  }

  /*!
   * \brief  Returns the passed Result if this has a value, else it returns the error of this.
   * \tparam U New value type.
   * \param  other Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return other if this has a value, else the error of this.
   * \vprivate
   */
  template <typename U>
  auto And(Result<U, E>&& other) && noexcept -> Result<U, E> {
    return HasValue() ? std::move(other) : Result<U, E>{Result::kInPlaceError, std::move(*this).Error()};
  }

  /*!
   * \brief     Transforms the contained value using the passed function, else returns the contained error.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F Transform function for the value. Must be compatible with the signature Result<XX,E> f(T const&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \return    The transformation result of the value, else the error of this.
   * \vprivate
   */
  template <typename F>
  constexpr auto AndThen(F&& callable_f) const& noexcept -> std::result_of_t<F(value_type const&)> {
    /*! \brief Local result type alias. */
    using R7 = std::result_of_t<F(value_type const&)>;
    static_assert(is_result<R7>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<error_type, typename R7::error_type>::value, "Error type must not change.");
    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable_f)};
    if (is_null_ptr) {
      ara::core::Abort("ara::core::Result::AndThen: The callable cannot be nullptr!");
    }
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
    return HasValue() ? std::forward<F>(callable_f)(Value())  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
                      : R7{Result::kInPlaceError, Error()};
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Transforms the contained value using the passed function, else returns the contained error.
   * \tparam    F Transform function for the value. Must be compatible with the signature Result<XX,E> f(T&&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \return    The transformation result of the value, else the error of this.
   * \vprivate
   */
  template <typename F>
  auto AndThen(F&& callable_f) && noexcept -> std::result_of_t<F(value_type&&)> {
    /*! \brief Local result type alias. */
    using R8 = std::result_of_t<F(value_type &&)>;
    static_assert(is_result<R8>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<error_type, typename R8::error_type>::value, "Error type must not change.");
    return HasValue() ? std::forward<F>(callable_f)(std::move(*this).Value())  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                      : R8{Result::kInPlaceError, std::move(*this).Error()};
  }

  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred.
   * \details   The function has three possible results:
   *            (1) the contained error if this holds an error;
   *            (2) the contained value if this value fulfills the predicate pred;
   *            (3) the provided error if this value does not fulfill the predicate pred.
   * \tparam    F Unary function. Result must be convertible to bool. Must accept T const& as parameter.
   * \tparam    E1 Must be convertible to E.
   * \param[in] pred Unary function for checking the value.
   * \param[in] error Error that is returned if the value does not fulfill the predicate pred.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not error is returned. Otherwise, the contained error.
   * \vprivate
   */
  template <typename F, typename E1>
  constexpr auto Filter(F&& pred, E1&& error) const& noexcept -> Result {
    static_assert(std::is_convertible<std::result_of_t<F(value_type const&)>, bool>::value,
                  "Return type of predicate must be convertible to boolean.");
    static_assert(std::is_convertible<E1&&, E>::value, "E1 must be convertible to E.");
    return AndThen([&pred, &error](value_type const& value) -> Result {
      return pred(value) ? Result{Result::kInPlaceValue, value}                     // VCA_VAC_VALID_ARGUMENT
                         : Result{Result::kInPlaceError, std::forward<E1>(error)};  // VCA_VAC_VALID_ARGUMENT
    });
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred.
   * \details   The function has three possible results:
   *            (1) the contained error if this holds an error;
   *            (2) the contained value if this value fulfills the predicate pred;
   *            (3) the provided error if this value does not fulfill the predicate pred.
   * \tparam    F Unary function. Result must be convertible to bool. Must accept T const& as parameter.
   * \param[in] pred Unary function for checking the value.
   * \param[in] error Error that is returned if the value does not fulfill the predicate pred.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not error is returned. Otherwise, the contained error.
   * \vprivate
   */
  template <typename F, typename E1>
  auto Filter(F&& pred, E1&& error) && noexcept -> Result {
    static_assert(std::is_convertible<std::result_of_t<F(value_type const&&)>, bool>::value,
                  "Return type of predicate must be convertible to boolean.");
    static_assert(std::is_convertible<E1&&, E>::value, "E1 must be convertible to E.");
    return std::move(*this).AndThen([&pred, &error](value_type&& value) -> Result {
      return pred(static_cast<value_type const&&>(value))  // VCA_VAC_TEMPLATE_REQ_MET
                 ? Result{Result::kInPlaceValue, std::move(value)}
                 : Result{Result::kInPlaceError, std::forward<E1>(error)};  // VCA_VAC_VALID_ARGUMENT
    });
  }

  // VECTOR Disable AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred, which returns a Result.
   * \details   The function has three possible results:
   *            (1) the contained error if this holds an error. Depending on the result of the pred call with
   *            this value as parameter it returns the contained value of this (2), or the returned error (3).
   * \tparam    F Function returning a Result. Must accept T const& as parameter.
   * \param[in] pred Function for checking the contained value.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not the error of pred is returned. Otherwise,
   *            the error contained in this is returned.
   * \vprivate
   */
  template <typename F>
  constexpr auto Filter(F&& pred) const& noexcept -> Result {
    /*! \brief Local result type alias. */
    using R9 = std::result_of_t<F(value_type const&)>;
    static_assert(is_result<R9>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<error_type, typename R9::error_type>::value, "Error type must not change.");
    return AndThen(
        // VCA_VAC_VALID_ARGUMENT
        [&pred](value_type const& value) -> Result { return pred(value).template Replace<value_type>(value); });
  }

  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred, which returns a Result.
   * \details   The function has three possible results:
   *            (1) the contained error if this holds an error. Depending on the result of the pred call with
   *            this value as parameter it returns the contained value of this (2), or the returned error (3).
   * \tparam    F Function returning a Result. Must accept T const& as parameter.
   * \param[in] pred Function for checking the contained value.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not the error of pred is returned. Otherwise,
   *            the error contained in this is returned.
   * \vprivate
   */
  template <typename F>
  auto Filter(F&& pred) && noexcept -> Result {
    /*! \brief Local result type alias. */
    using R10 = std::result_of_t<F(value_type const&&)>;
    static_assert(is_result<R10>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<error_type, typename R10::error_type>::value, "Error type must not change.");
    return std::move(*this).AndThen([&pred](value_type&& value) -> Result {
      return pred(static_cast<value_type const&&>(value))  // VCA_VAC_TEMPLATE_REQ_MET
          .template Replace<value_type>(std::move(value));
    });
  }
  // VECTOR Enable AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded

  /*!
   * \brief   Removes the value of the result.
   * \details If this Result object contains a value, the function returns an empty Result, else the contained error.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  An empty Result, or the contained error.
   * \vprivate
   */
  constexpr auto Drop() const noexcept -> Result<void, E> { return Replace<void>(); }

  /*!
   * \brief     Executes a given callable with the contained value as parameter. If this contains an error the callable
   *            is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F Callable, must have the signature void f(T const&).
   * \param[in] callable_f Callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  constexpr auto Inspect(F&& callable_f) const& noexcept -> Result {
    static_assert(std::is_void<std::result_of_t<F(value_type const&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::Inspect: The callable cannot be nullptr!");
    }
    return AndThen([&callable_f](value_type const& value) -> Result {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(value);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      return Result{Result::kInPlaceValue, value};
    });
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Executes a given callable with the contained value as parameter. If this contains an error the callable
   *            is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F Callable, must have the signature void f(T const&).
   * \param[in] callable_f Callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  auto Inspect(F&& callable_f) && noexcept -> Result {
    static_assert(std::is_void<std::result_of_t<F(value_type const&&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::Inspect: The callable cannot be nullptr!");
    }
    return std::move(*this).AndThen([&callable_f](value_type&& value) -> Result {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(static_cast<value_type const&&>(value));  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      return Result{Result::kInPlaceValue, std::move(value)};
    });
  }

  /*!
   * \brief     Executes a given callable with the contained error as parameter. If this contains a value the callable
   *            is not executed.
   * \tparam    F Callable, must have the signature void f(E const&).
   * \param[in] callable_f callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  constexpr auto InspectError(F&& callable_f) const& noexcept -> Result {
    static_assert(std::is_void<std::result_of_t<F(error_type const&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::InspectError: The callable cannot be nullptr!");
    }
    return OrElse([&callable_f](error_type const& error) -> Result {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(error);  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      return Result{Result::kInPlaceError, error};
    });
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Executes a given callable with the contained error as parameter. If this contains a value the callable
   *            is not executed.
   * \tparam    F Callable, must have the signature void f(E const&).
   * \param[in] callable_f callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  auto InspectError(F&& callable_f) && noexcept -> Result {
    static_assert(std::is_void<std::result_of_t<F(error_type const&&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::InspectError: The callable cannot be nullptr!");
    }
    return std::move(*this).OrElse([&callable_f](error_type&& error) -> Result {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(static_cast<error_type const&&>(error));  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      return Result{Result::kInPlaceError, std::move(error)};
    });
  }

  /*!
   * \brief     Allows to inspect and remove a value from a Result. Executes the given callable with the contained value
   *            as parameter. If this result contains an error, the callable is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F           Type of the callable, must have the signature 'void f(T const&)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \return    A Result<void> containing either nothing or the original error.
   * \vprivate
   */
  template <typename F>
  auto Consume(F&& callable_f) const noexcept -> Result<void, E> {
    static_assert(std::is_void<std::result_of_t<F(value_type const&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::Consume: The callable cannot be nullptr!");
    }

    Result<void, E> result{Result<void, E>::FromValue()};
    if (HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(Value());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    } else {
      result.EmplaceError(Error());
    }
    return result;
  }

  /*!
   * \brief     Allows to inspect and move a value out of a Result. Executes the given callable with the contained value
   *            as parameter. If this result contains an error, the callable is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F           Callable, must have the signature 'void f(T &&)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \return    A Result<void> containing either nothing or the original error.
   * \vprivate
   */
  template <typename F>
  auto Consume(F&& callable_f) noexcept -> Result<void, E> {
    static_assert(std::is_void<std::result_of_t<F(value_type &&)>>::value, "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::Consume: The callable cannot be nullptr!");
    }

    Result<void, E> result{Result<void, E>::FromValue()};
    if (HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(std::move(*this).Value());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    } else {
      result.EmplaceError(std::move(*this).Error());
    }
    return result;
  }

  /*!
   * \brief  Checks for equality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   * \vprivate
   */
  auto operator==(Result const& that) const noexcept -> bool {  // Comment to force line break. See TAR-22835.
    return this->data_ == that.data_;
  }

  /*!
   * \brief  Checks for inequality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   * \vprivate
   */
  auto operator!=(Result const& that) const noexcept -> bool {  // Comment to force line break. See TAR-22835.
    return !(*this == that);
  }

 private:
  /*!
   * \brief Union like container holding either the Good Type T or the Error Type E.
   */
  vac::language::detail::Either<E, T> data_;
};

// VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
/*!
 * \brief     Swap the contents of the two given arguments.
 * \tparam    T The type of value.
 * \tparam    E The type of error.
 * \param[in] lhs Left hand side Result to swap.
 * \param[in] rhs Right hand side Result to swap.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \vpublic
 */
template <typename T, typename E>
inline void swap(Result<T, E>& lhs, Result<T, E>& rhs) noexcept {
  lhs.Swap(rhs);
}

/*!
 * \brief  Checks for equality of a result and a value.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \param  result The result to compare.
 * \param  value A value.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the result contains a value and both are equal
 * \vprivate
 */
template <typename T, typename E>
constexpr auto operator==(Result<T, E> const& result, T const& value) noexcept -> bool {
  return result.HasValue() ? (result.Value() == value) : false;
}

/*!
 * \brief  Checks for inequality of two results.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \param  result The result to compare.
 * \param  value A value.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return False if the result contains a value or both are equal.
 * \vprivate
 */
template <typename T, typename E>
constexpr auto operator!=(Result<T, E> const& result, T const& value) noexcept -> bool {
  return !(result == value);
}

/*!
 * \brief  Checks for equality of a result and a value.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \param  value A value.
 * \param  result The result to compare.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the result contains a value and both are equal.
 * \vprivate
 */
template <typename T, typename E>
constexpr auto operator==(T const& value, Result<T, E> const& result) noexcept -> bool {
  return result == value;
}

/*!
 * \brief  Checks for inequality of two results.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \param  value A value.
 * \param  result The result to compare.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return False if the result contains a value or both are equal.
 * \vprivate
 */
template <typename T, typename E>
constexpr auto operator!=(T const& value, Result<T, E> const& result) noexcept -> bool {
  return result != value;
}

/*!
 * \brief  Checks for equality of a result and an error.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \param  err An error.
 * \param  result The result to compare.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the result contains an error and both are equal.
 * \vprivate
 */
template <typename T, typename E>
constexpr auto operator==(Result<T, E> const& result, E const& err) noexcept -> bool {
  return !result.HasValue() ? (result.Error() == err) : false;
}

/*!
 * \brief  Checks for equality of a result's error and a type from which an ErrorCode can be constructed.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \tparam Ec The type of error code.
 * \param  err An error.
 * \param  result The result to compare.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the result contains an error and both are equal.
 * \vprivate
 */
template <typename T, typename E, typename Ec,
          typename std::enable_if_t<std::is_constructible<E, Ec>::value, std::int32_t> = 0>
constexpr auto operator==(Result<T, E> const& result, Ec const& err) noexcept -> bool {
  return !result.HasValue() ? (result.Error() == E{err}) : false;
}

/*!
 * \brief  Checks for inequality of two results.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \tparam Ec The type of error code.
 * \param  result The result to compare.
 * \param  err An error.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return False if the result contains a value or both are equal.
 * \vprivate
 */
template <typename T, typename E, typename Ec,
          typename std::enable_if_t<std::is_constructible<E, Ec>::value, std::int32_t> = 0>
constexpr auto operator!=(Result<T, E> const& result, Ec const& err) noexcept -> bool {
  return !(result == err);
}

/*!
 * \brief  Checks for equality of a result and an error.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \tparam Ec The type of error code.
 * \param  result The result to compare.
 * \param  err An error.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the result contains an error and both are equal.
 * \vprivate
 */
template <typename T, typename E, typename Ec,
          typename std::enable_if_t<std::is_constructible<E, Ec>::value, std::int32_t> = 0>
constexpr auto operator==(Ec const& err, Result<T, E> const& result) noexcept -> bool {
  return result == err;
}

/*!
 * \brief  Checks for inequality of two results.
 * \tparam T The type of value.
 * \tparam E The type of error.
 * \tparam Ec The type of error code.
 * \param  err An error.
 * \param  result The result to compare.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return False if the result contains a value or both are equal.
 * \vprivate
 */
template <typename T, typename E, typename Ec,
          typename std::enable_if_t<std::is_constructible<E, Ec>::value, std::int32_t> = 0>
constexpr auto operator!=(Ec const& err, Result<T, E> const& result) noexcept -> bool {
  return result != err;
}

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief  Specialization of Result for void.
 * \tparam E The type of error.
 * \trace  DSGN-VaCommonLib-ErrorHandling
 * \vpublic
 */
template <typename E>
class Result<void, E> final {
  // Assertions for E
  static_assert(std::is_object<E>::value, "E must be object/integral type.");
  static_assert(std::is_move_constructible<E>::value,
                "E must be move-constructible. Otherwise it cannot be used as return type!");
  static_assert(std::is_copy_constructible<E>::value,
                "E must be copy-constructible. Move-only Error types may require additional r-value overloads of "
                "member functions for Result.");

  /*! \brief Storage type */
  using StorageType = Result<detail::Empty, E>;

  /*!
   * \brief  SFINAE for callable returning Result<...>.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsResult = std::enable_if_t<is_result<std::result_of_t<F()>>::value>;

  /*!
   * \brief  SFINAE for callable returning void.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsVoid = std::enable_if_t<std::is_void<std::result_of_t<F()>>::value>;

  /*!
   * \brief  SFINAE for callable not returning Result<...>.
   * \tparam F Callable.
   * \vprivate
   */
  template <typename F>
  using CallableReturnsNoResult =
      std::enable_if_t<(!is_result<std::result_of_t<F()>>::value) && (!std::is_void<std::result_of_t<F()>>::value)>;

  /*!
   * \brief  Helper wrapper template in order pass a callable to the delegate.
   * \tparam F Callable. Must accept empty argument list.
   * \vprivate
   */
  template <typename F>
  class VoidToAnyFuncT final {
   public:
    /*! \brief Default copy constructor.
     * \spec
     *   requires true;
     * \endspec
     */
    VoidToAnyFuncT(VoidToAnyFuncT const&) noexcept = default;
    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*! \brief Default move constructor.
     * \spec
     *   requires true;
     * \endspec
     */
    VoidToAnyFuncT(VoidToAnyFuncT&&) noexcept = default;
    /*! \brief  Default copy assignment.
     *  \return lvalue.
     *  \spec
     *    requires true;
     *  \endspec
     */
    VoidToAnyFuncT& operator=(VoidToAnyFuncT const&) & noexcept = default;
    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*! \brief  Default move assignment.
     *  \return rvalue.
     *  \spec
     *    requires true;
     *  \endspec
     */
    VoidToAnyFuncT& operator=(VoidToAnyFuncT&&) & noexcept = default;
    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*! \brief Default destructor.
     * \spec
     *   requires true;
     * \endspec
     */
    ~VoidToAnyFuncT() noexcept = default;

    /*!
     * \brief  Constructor.
     * \tparam F1 Callable. Must accept empty argument list.
     * \param[in] callable_f Callable.
     * \pre       -
     * \spec
     *   requires true;
     * \endspec
     */
    template <typename F1>
    constexpr explicit VoidToAnyFuncT(F1&& callable_f) noexcept : f_{std::forward<F>(callable_f)} {}

    // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
    // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
    /*!
     * \brief  Executes the function given in the constructor.
     * \pre    -
     * \return Callable.
     * \spec
     *   requires true;
     * \endspec
     */
    template <typename U>
    constexpr auto operator()(U&&) noexcept -> typename std::result_of<F()>::type {
      return std::forward<F>(f_)();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }

   private:
    /*! \brief Callable. */
    F&& f_;
  };

  /*!
   * \brief     Returns a callable ignoring it's argument.
   * \details   If the function gets the provide callable f is executed without any arguments.
   * \tparam    F Callable. Must have the signature XX f().
   * \param[in] callable_f  Callable to be executed.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    Callable ignoring it's argument.
   * \vprivate
   */
  template <typename F>
  static constexpr auto VoidToAnyFunc(F&& callable_f) noexcept -> VoidToAnyFuncT<F> {
    return VoidToAnyFuncT<F>{std::forward<F>(callable_f)};
  }

 public:
  /*! \brief Value type of the Result. */
  using value_type = void;
  /*! \brief Error type of the Result. */
  using error_type = E;
  /*! \brief In-place construct constant for value_type. */
  constexpr static in_place_type_t<value_type> kInPlaceValue{};
  /*! \brief In-place construct constant for error_type. */
  constexpr static in_place_type_t<error_type> kInPlaceError{};

  /*!
   * \brief  Build a new Result from void Result type.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Result that contains a void Result type.
   */
  constexpr static auto FromValue() noexcept -> Result { return Result{Result::kInPlaceValue}; }

  /*!
   * \brief   Build a new Result from the specified error (given as lvalue).
   * \details If an exception is raised by the copy constructor in E the process is terminated.
   * \param   e The error to put into the Result.
   * \pre     -
   * \return  Result that contains a void Result type.
   */
  constexpr static auto FromError(E const& e) noexcept -> Result { return Result{Result::kInPlaceError, e}; }

  /*!
   * \brief     Build a new Result from the specified error (given as rvalue).
   * \details   If an exception is raised by the move constructor in E the process is terminated.
   * \param[in] e The error to put into the Result.
   * \pre       -
   * \return    Result that contains a void Result type.
   */
  constexpr static auto FromError(E&& e) noexcept -> Result { return Result{Result::kInPlaceError, std::move(e)}; }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args The arguments used for constructing the error.
   * \pre       -
   * \return    A Result that contains an error.
   * \vpublic
   */
  template <typename... Args, typename = std::enable_if_t<(std::is_constructible<E, Args&&...>::value) &&
                                                          (!std::is_same<E, detail::FirstTypeT<Args...>>::value) &&
                                                          (!is_result<detail::FirstTypeT<Args...>>::value)>>
  constexpr static auto FromError(Args&&... args) noexcept -> Result {
    return Result{Result::kInPlaceError, std::forward<Args>(args)...};
  }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] error_value A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \return    A Result that contains an error.
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  static constexpr auto FromError(EnumT error_value, StringView user_msg) noexcept -> Result {
    return Result{ErrorCode(error_value, user_msg.data())};
  }

  /*!
   * \brief Construct a new Result of value type void.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  constexpr Result() noexcept : Result{Result::kInPlaceValue} {}

  // VECTOR Disable AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief Construct a new Result of value type void.
   * \spec
   *   requires true;
   * \endspec
   * \vprivate
   */
  constexpr explicit Result(in_place_type_t<void>) noexcept : data_{StorageType::kInPlaceValue} {}

  /*!
   * \brief   Construct a new Result from the specified error (given as lvalue).
   * \details If an exception is raised by the copy constructor in E the process is terminated.
   * \param   e The error to put into the Result.
   * \pre     -
   * \vpublic
   */
  constexpr explicit Result(E const& e) noexcept : Result{Result::kInPlaceError, e} {}

  /*!
   * \brief     Construct a new Result from the specified error (given as rvalue).
   * \details   If an exception is raised by the move constructor in E the process is terminated.
   * \param[in] e The error to put into the Result.
   * \pre       -
   * \vpublic
   */
  constexpr explicit Result(E&& e) noexcept : Result{Result::kInPlaceError, std::move(e)} {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     In-place tag constructor for an Error.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   * \param[in] args Construction parameter.
   * \pre       -
   * \vprivate
   */
  template <typename... Args>
  constexpr Result(in_place_type_t<E>, Args&&... args) noexcept
      : data_{Result::kInPlaceError, std::forward<Args>(args)...} {}

  /*!
   * \brief     Storage constructor for delegate usage.
   * \param[in] storage The storage to be constructed from.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vprivate
   */
  constexpr explicit Result(StorageType&& storage) noexcept : data_{std::move(storage)} {}

  /*!
   * \brief   Copy-construct a new Result from another instance.
   * \details If an exception is raised by the copy constructor in E the process is terminated.
   * \param   other The other instance.
   * \vpublic
   */
  constexpr Result(Result const& other) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Move-construct a new Result from another instance.
   * \details If an exception is raised by the move constructor in E the process is terminated.
   * \param   other The other instance.
   * \vpublic
   */
  constexpr Result(Result&& other) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  ~Result() noexcept = default;

  /*!
   * \brief   Copy-assign another Result to this instance.
   * \details If an exception is raised by the assignment operator in E the process is terminated.
   * \param   other The other instance.
   * \return  *this, containing the contents of other.
   * \vpublic
   */
  auto operator=(Result const& other) & -> Result& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Move-assign another Result to this instance.
   * \details If an exception is raised by the assignment operator in E the process is terminated.
   * \param   other The other instance.
   * \return  *this, containing the contents of other.
   * \vpublic
   */
  auto operator=(Result&& other) & -> Result& = default;

  /*!
   * \brief Reset error code.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \return
   * \vpublic
   */
  auto EmplaceValue() noexcept -> void { this->data_.EmplaceValue(); }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \details   If an exception is raised by the constructor in E the process is terminated.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args The arguments used for constructing the error.
   * \pre       -
   * \return
   * \vpublic
   */
  template <typename... Args>
  auto EmplaceError(Args&&... args) noexcept -> void {
    this->data_.EmplaceError(std::forward<Args>(args)...);
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] error_value A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \return
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  constexpr auto EmplaceError(EnumT error_value, StringView user_msg) noexcept -> void {
    this->data_.EmplaceError(ErrorCode{error_value, user_msg.data()});
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief         Exchange the contents of this instance with those of other.
   * \param[in,out] other The other instance.
   * \pre           -
   * \spec
   *   requires true;
   * \endspec
   * \return
   * \vpublic
   */
  auto Swap(Result& other) noexcept -> void { this->data_.Swap(other.data_); }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains a value, false otherwise.
   * \vpublic
   */
  constexpr explicit operator bool() const noexcept { return this->HasValue(); }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains no error type.
   * \vpublic
   */
  constexpr auto HasValue() const noexcept -> bool { return this->data_.HasValue(); }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief   Do nothing.
   * \details This function only exists for helping with generic programming.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  void
   * \vpublic
   */
  constexpr void Value() const noexcept { assert(HasValue()); }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     -
   * \return  A reference to the contained error.
   * \vpublic
   */
  constexpr auto Error() const& noexcept -> E const& { return this->data_.Error(); }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     -
   * \return  A rvalue reference to the contained error.
   * \vpublic
   */
  auto Error() && noexcept -> E&& { return std::move(data_).Error(); }

  /*!
   * \brief     Return the contained error or the given default error.
   * \details   If *this contains an error, it is returned. Otherwise, the specified default error is returned,
   *            static_cast'd to E.
   * \tparam    G The type of defaultError.
   * \param[in] defaultError The error to use if *this does not contain an error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The error.
   * \vpublic
   */
  template <typename G>
  auto ErrorOr(G&& defaultError) const noexcept -> E {
    return this->data_.ErrorOr(std::forward<G>(defaultError));
  }

  /*!
   * \brief     Return whether this instance contains the given error.
   * \tparam    G The type of the error e.
   * \param[in] error The error to check.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    True if *this contains the given error, false otherwise.
   * \vpublic
   */
  template <typename G>
  auto CheckError(G&& error) const noexcept -> bool {
    return this->data_.CheckError(std::forward<G>(error));
  }

  /*!
   * \brief   Throws an exception when error occurred.
   * \details This function does not participate in overload resolution when the compiler toolchain does not support
   *          C++ exceptions.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  void
   * \throws <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() const noexcept(false) -> void {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    static_cast<void>(this->data_.ValueOrThrow());
  }

  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \details   The Callable is expected to be compatible to one of these two interfaces:
   *            <code>
   *            Result<XXX, E> f();
   *            XXX f();
   *            </code>
   *            meaning that the Callable either returns a Result<XXX> or a XXX directly,
   *            where XXX can be any type that is suitable for use by class Result.
   *            The return type of this function is always Result<XXX, E>.
   *            If this instance does not contain a value, a new Result<XXX, E> is still
   *            created and returned, with the original error contents of this instance
   *            being copied into the new instance.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsResult<F> Enable template when callable F returns Result<...> type.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type.
   * \vprivate
   */
  template <typename F, typename = CallableReturnsResult<F>>
  constexpr auto Bind(F&& callable_f) const noexcept -> decltype(callable_f()) {
    return AndThen(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsNoResult<F> Enable template when callable f does not return Result<...> type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   * \vprivate
   */
  template <typename F, typename = CallableReturnsNoResult<F>>
  constexpr auto Bind(F&& callable_f) const noexcept -> Result<std::result_of_t<F()>, E> {
    return Map(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsNoResult<F> Enable template when callable f does not return Result<...> type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   * \vprivate
   */
  template <typename F, typename = CallableReturnsVoid<F>>
  constexpr auto Bind(F&& callable_f) const noexcept -> Result {
    return Inspect(std::forward<F>(callable_f));
  }

  /*!
   * \brief     Executes the given Callable if this contains a value, or returns the contained error.
   * \tparam    F The type of the Callable f. Must have the signature XXX f(). Return type must not be void.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type or the contained error.
   * \remark    This function can be used to transform the contained value.
   * \vprivate
   */
  template <typename F>
  auto Map(F&& callable_f) const noexcept -> Result<std::result_of_t<F()>, E> {
    return data_.Map(VoidToAnyFunc(std::forward<F>(callable_f)));
  }

  /*!
   * \brief     Executes the given callable with the error of this instance, or returns the untouched value.
   * \tparam    F the type of the Callable f. Must be compatible with the signature XX f(E const&).
   *            The return type must be an object/integral type.
   * \tparam    E1 The return value of the Function F.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed error type or the unchanged value.
   * \remark    This function can be used to transform the contained error, or perform error handling in general.
   * \vprivate
   */
  template <typename F>
  constexpr auto MapError(F&& callable_f) const noexcept -> Result<void, std::result_of_t<F(error_type const&)>> {
    // VECTOR Next Construct AutosarC++17_10-A2.11.2: MD_VAC_A2.11.2_usingNameReusedInLocalScope
    /*! \brief Local error type alias. */
    using E1 = std::result_of_t<F(error_type const&)>;
    return Result<void, E1>{data_.MapError(std::forward<F>(callable_f))};
  }

  /*!
   * \brief     Create a new Result with the given new value if this instance contains a value, otherwise it
   *            return the contained error.
   * \details   The new value is inplace-constructed from the given parameters args.
   * \tparam    U Rhe type of the given value, defaulted to void.
   * \tparam    Args Construction arguments for U.
   * \param[in] args Arguments for the in place construction of the replacement value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance with the constructed replacement value or the contained error.
   * \remark    This function can be used to replace the contained value, which type may differ from void.
   * \vprivate
   */
  template <typename U = void, typename... Args>
  constexpr auto Replace(Args&&... args) const noexcept -> Result<U, E> {
    return data_.template Replace<U>(std::forward<Args>(args)...);
  }

  /*!
   * \brief  Returns an empty Result if this has a value, else it returns the given alternative.
   * \tparam E1 New error type.
   * \param  alternative Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Empty Result, else alternative.
   * \vprivate
   */
  template <typename E1>
  constexpr auto Or(Result<void, E1> const& alternative) const noexcept -> Result<void, E1> {
    return HasValue() ? Result<void, E1>{Result::kInPlaceValue} : alternative;
  }

  /*!
   * \brief     Returns an empty Result if this has a value, else it transforms the contained Error using the passed
   *            function.
   * \tparam    F Transform function for the error. Must be compatible to the signature Result<void,E1> f(E const&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    An empty Result if this has a value, else the transformation result of the error.
   * \vprivate
   */
  template <typename F>
  constexpr auto OrElse(F&& callable_f) const noexcept -> std::result_of_t<F(error_type const&)> {
    /*! \brief Local result type alias. */
    using R11 = std::result_of_t<F(error_type const&)>;
    static_assert(is_result<R11>::value, "Return type of f must be a Result.");
    static_assert(std::is_void<typename R11::value_type>::value, "Value type must not change (must be void).");
    return HasValue() ? R11{Result::kInPlaceValue}
                      : std::forward<F>(callable_f)(Error());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief  Returns the passed Result if this has a value, else it returns the error of this.
   * \tparam U New value type.
   * \param  other Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return other if this has a value, else the error of this.
   * \vprivate
   */
  template <typename U>
  constexpr auto And(Result<U, E> const& other) const noexcept -> Result<U, E> {
    return data_.And(other);
  }

  /*!
   * \brief     Returns the result the passed function if this has a value, else returns the contained error.
   * \tparam    F Transform function for the value. Must be compatible with the signature Result<XX,E> f().
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The transformation result of the value, else the error of this.
   * \vprivate
   */
  template <typename F>
  constexpr auto AndThen(F&& callable_f) const noexcept -> std::result_of_t<F()> {
    return data_.AndThen(VoidToAnyFunc(std::forward<F>(callable_f)));
  }

  /*!
   * \brief     Executes a given callable if this contains a value. If this contains an error the callable is not
   *            executed.
   * \tparam    F Callable, must have the signature void f().
   * \param[in] callable_f Callable function.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  constexpr auto Inspect(F&& callable_f) const noexcept -> Result {
    return Result{data_.Inspect(VoidToAnyFunc(std::forward<F>(callable_f)))};
  }

  /*!
   * \brief     Executes a given callable with the contained error as parameter. If this contains a value the callable
   *            is not executed.
   * \tparam    F Callable, must have the signature void f(E const&).
   * \param[in] callable_f callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  constexpr auto InspectError(F&& callable_f) const noexcept -> Result {
    return Result{data_.InspectError(std::forward<F>(callable_f))};
  }

  /*!
   * \brief     Allows to inspect and remove an error from a Result. Executes the given callable with the contained
   *            error as parameter. If this result contains a value, the callable is not executed.
   * \tparam    F           Type of the callable, must have the signature 'void f(E const&)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \vprivate
   */
  template <typename F>
  constexpr void ConsumeError(F&& callable_f) const noexcept {
    static_assert(std::is_void<std::result_of_t<F(error_type const&)>>::value,
                  "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::ConsumeError: The callable cannot be nullptr!");
    }
    if (!HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(Error());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  /*!
   * \brief     Allows to inspect and move an error out of a Result. Executes the given callable with the contained
   *            error as parameter. If this result contains a value, the callable is not executed.
   * \tparam    F           Type of the callable, must have the signature 'void f(E &&)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \vprivate
   */
  template <typename F>
  constexpr void ConsumeError(F&& callable_f) noexcept {
    static_assert(std::is_void<std::result_of_t<F(error_type &&)>>::value, "Return type of the callable must be void.");
    if (vac::language::detail::IsCallableNullPtr(callable_f)) {
      ara::core::Abort("ara::core::Result::ConsumeError: The callable cannot be nullptr!");
    }
    if (!HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      std::forward<F>(callable_f)(std::move(*this).Error());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }

  /*!
   * \brief  Checks for equality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   * \vprivate
   */
  auto operator==(Result const& that) const noexcept -> bool {
    bool error_eq{false};
    if ((!this->HasValue()) && (!that.HasValue())) {
      error_eq = this->Error() == that.Error();
    }
    return (this->HasValue() && that.HasValue()) || error_eq;
  }

  /*!
   * \brief  Checks for inequality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   * \vprivate
   */
  auto operator!=(Result const& that) const noexcept -> bool {  // Comment to force line break. See TAR-22835.
    return !(*this == that);
  }

 private:
  /*! \brief Optional containing error type E, when error occurred. */
  StorageType data_;
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief  This class is a type that contains either a value or an error.
 * \tparam T the type of value
 * \tparam E the type of error
 * \trace  DSGN-VaCommonLib-ErrorHandling
 * \vprivate
 */
template <typename T, typename E>
class Result<T&, E> final {
  // Assertions for E
  static_assert(std::is_object<E>::value, "E must be object/integral type.");
  static_assert(std::is_move_constructible<E>::value,
                "E must be move-constructible. Otherwise it cannot be used as return type!");
  static_assert(std::is_copy_constructible<E>::value,
                "E must be copy constructible! Move-only Error types may require additional r-value overloads of "
                "member functions for Result.");

  /*! \brief Wrapper type for storing the reference. */
  using WrappedReferenceType = std::reference_wrapper<T>;

  /*! \brief Storage type. */
  using StorageType = Result<WrappedReferenceType, E>;

  /*!
   * \brief  SFINAE for callable returning Result<...>.
   * \tparam F Callable.
   */
  template <typename F>
  using CallableReturnsResult = std::enable_if_t<is_result<std::result_of_t<F(T const&)>>::value>;

  /*!
   * \brief  SFINAE for callable returning void.
   * \tparam F Callable.
   */
  template <typename F>
  using CallableReturnsVoid = std::enable_if_t<std::is_void<std::result_of_t<F(T const&)>>::value>;

  /*!
   * \brief  SFINAE for callable not returning Result<...>.
   * \tparam F Callable.
   */
  template <typename F>
  using CallableReturnsNoResult = std::enable_if_t<(!is_result<std::result_of_t<F(T const&)>>::value) &&
                                                   (!std::is_void<std::result_of_t<F(T const&)>>::value)>;

 public:
  /*! \brief Value type of the Result. */
  using value_type = T&;
  /*! \brief Error type of the Result. */
  using error_type = E;
  /*! \brief In-place construct constant for value_type. */
  constexpr static in_place_type_t<value_type> kInPlaceValue{};
  /*! \brief In-place construct constant for error_type. */
  constexpr static in_place_type_t<error_type> kInPlaceError{};

  /*!
   * \brief     Build a new Result from the specified value (given as lvalue).
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    Result that contains the value t.
   */
  static auto FromValue(T& t) noexcept -> Result { return Result{kInPlaceValue, t}; }

  /*!
   * \brief  Build a new Result from the specified error (given as lvalue).
   * \param  e The error to put into the Result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return a Result that contains the error e.
   */
  static auto FromError(E const& e) noexcept -> Result { return Result{Result::kInPlaceError, e}; }

  /*!
   * \brief     Build a new Result from the specified error (given as rvalue).
   * \param[in] e The error to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A Result that contains the error e.
   */
  static auto FromError(E&& e) noexcept -> Result { return Result{Result::kInPlaceError, std::move(e)}; }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \tparam    Args... The types of arguments given to this function.
   * \param[in] args The arguments used for constructing the error.
   * \pre       -
   * \return    A Result
   * \spec
   *   requires true;
   * \endspec that contains an error.
   */
  template <typename... Args, typename = std::enable_if_t<(std::is_constructible<E, Args&&...>::value) &&
                                                          (!std::is_same<E, detail::FirstTypeT<Args...>>::value) &&
                                                          (!is_result<detail::FirstTypeT<Args...>>::value)>>
  static auto FromError(Args&&... args) noexcept -> Result {
    return Result{Result::kInPlaceError, std::forward<Args>(args)...};
  }

  /*!
   * \brief     Build a new Result from an error that is constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] error_value A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \return    A Result that contains an error.
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  static constexpr auto FromError(EnumT error_value, StringView user_msg) noexcept -> Result {
    return Result{ErrorCode(error_value, user_msg.data())};
  }
  /*!
   * \brief     Construct a new Result from the specified value (given as lvalue).
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Result(T& t) noexcept : Result{Result::kInPlaceValue, t} {}

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     Construct a new Result from the specified value (given as lvalue).
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  Result(in_place_type_t<T&>, T& t) noexcept : data_{StorageType::kInPlaceValue, t} {}

  /*!
   * \brief Construct a new Result from the specified error (given as lvalue).
   * \param e The error to put into the Result.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Result(E const& e) noexcept : Result{Result::kInPlaceError, e} {}

  /*!
   * \brief     Construct a new Result from the specified error (given as rvalue).
   * \param[in] e The error to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Result(E&& e) noexcept : Result{Result::kInPlaceError, std::move(e)} {}

  /*!
   * \brief     In-place tag constructor for an Error.
   * \param[in] args Construction parameter.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Args>
  Result(in_place_type_t<E>, Args&&... args) noexcept : data_{Result::kInPlaceError, std::forward<Args>(args)...} {}

  /*!
   * \brief     Storage constructor for delegate usage.
   * \param[in] storage The storage to be constructed from.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Result(StorageType&& storage) noexcept : data_{std::move(storage)} {}

  /*!
   * \brief Copy-construct a new Result from another instance.
   * \param other The other instance.
   * \spec
   *   requires true;
   * \endspec
   */
  Result(Result const& other) = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Move-construct a new Result from another instance.
   * \param other The other instance.
   * \spec
   *   requires true;
   * \endspec
   */
  Result(Result&& other) = default;

  /*!
   * \brief  Copy-assign another Result to this instance.
   * \param  other The other instance.
   * \return *this, containing the contents of other.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Result const& other) & -> Result& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Move-assign another Result to this instance.
   * \param  other The other instance.
   * \return *this, containing the contents of other.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Result&& other) & -> Result& = default;

  /*!
   * \brief     Put a new value into this instance, constructed in-place from the given arguments.
   * \param[in] t The value to put into the Result.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return
   */
  auto EmplaceValue(T& t) noexcept -> void { this->data_.EmplaceValue(WrappedReferenceType{t}); }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \tparam    Args...  The types of arguments given to this function.
   * \param[in] args  The arguments used for constructing the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return
   */
  template <typename... Args>
  auto EmplaceError(Args&&... args) noexcept -> void {
    this->data_.EmplaceError(std::forward<Args>(args)...);
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Put a new error into this instance, constructed in-place from the given arguments.
   * \details   This method does not participate in overload resolution unless E is an ErrorCode.
   * \tparam    EnumT An enum type should be used to construct an ErrorCode, i.e. MakeErrorCode(EnumT,...) must exist.
   * \param[in] error_value A domain-specific error code value used for constructing the error.
   * \param[in] user_msg User-defined custom message used for constructing the error.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return
   * \vprivate  Product Private
   */
  template <typename EnumT,
            std::enable_if_t<(std::is_enum<EnumT>::value) && (std::is_same<std::decay_t<E>, ErrorCode>::value),
                             std::int32_t> = 0>
  auto EmplaceError(EnumT error_value, StringView user_msg) noexcept -> void {
    this->data_.EmplaceError(ErrorCode{error_value, user_msg.data()});
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief         Exchange the contents of this instance with those of other.
   * \param[in,out] other The other instance.
   * \pre           -
   * \spec
   *   requires true;
   * \endspec
   * \return
   */
  auto Swap(Result& other) noexcept -> void { this->data_.Swap(other.data_); }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains a value, false otherwise.
   */
  constexpr explicit operator bool() const noexcept { return this->HasValue(); }

  /*!
   * \brief  Check whether *this contains a value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if *this contains a value, false otherwise.
   */
  constexpr auto HasValue() const noexcept -> bool { return this->data_.HasValue(); }

  /*!
   * \brief   Access the contained value.
   * \details This function's behavior is undefined if *this does not contain a value.
   * \pre     -
   * \return  A reference to the contained value.
   */
  auto operator*() const noexcept -> T const& { return Value(); }

  /*!
   * \brief   Access the contained value.
   * \details This function's behavior is undefined if *this does not contain a value.
   * \pre     -
   * \return  A pointer to the contained value.
   */
  auto operator->() const noexcept -> T const* { return std::addressof(Value()); }

  /*!
   * \brief   Access the contained value.
   * \details The behavior of this function is undefined if *this does not contain a value.
   * \pre     -
   * \return  A reference to the contained value.
   */
  auto Value() const& noexcept -> T const& { return this->data_.Value(); }

  /*!
   * \brief  Non-const access the contained value.
   *         The behavior of this function is undefined if *this does not contain a value.
   * \pre    -
   * \return A non-const reference to the contained value.
   */
  auto Value() & noexcept -> T& { return this->data_.Value(); }

  /*!
   * \brief  Non-const access the contained value.
   *         The behavior of this function is undefined if *this does not contain a value.
   * \pre    -
   * \return A non-const reference to the contained value.
   * \vprivate
   */
  auto Value() && noexcept -> T& { return this->data_.Value(); }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     -
   * \return  A reference to the contained error.
   */
  auto Error() const& noexcept -> E const& { return this->data_.Error(); }

  /*!
   * \brief   Access the contained error.
   * \details The behavior of this function is undefined if *this does not contain an error.
   * \pre     -
   * \return  A rvalue reference to the contained error.
   */
  auto Error() && noexcept -> E&& { return std::move(data_).Error(); }

  /*!
   * \brief     Return the contained value or the given default value.
   * \details   If *this contains a value, it is returned. Otherwise, the specified default value is returned.
   * \param[in] defaultValue The value to use if *this does not contain a value.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The value.
   */
  auto ValueOr(T& defaultValue) const& noexcept -> T& {
    return this->data_.ValueOr(defaultValue);  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief     Return the contained error or the given default error.
   * \details   If *this contains an error, it is returned. Otherwise, the specified default error is returned,
   *            static_cast'd to E.
   * \tparam    G The type of defaultError.
   * \param[in] defaultError The error to use if *this does not contain an error.
   * \return    The error.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename G>
  auto ErrorOr(G&& defaultError) const noexcept -> E {
    return this->data_.ErrorOr(std::forward<G>(defaultError));
  }

  /*!
   * \brief     Return whether this instance contains the given error.
   * \tparam    G The type of the error e.
   * \param[in] error The error to check.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    True if *this contains the given error, false otherwise.
   */
  template <typename G>
  auto CheckError(G&& error) const noexcept -> bool {
    return this->data_.CheckError(std::forward<G>(error));
  }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Return the contained value or throw an exception.
   * \details This function does not participate in overload resolution when the compiler toolchain does not support
   *          C++ exceptions.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  The value.
   * \throws  <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() & noexcept(false) -> T& {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    return this->data_.ValueOrThrow();
  }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Return the contained value or throw an exception.
   * \details This function does not participate in overload resolution when the compiler toolchain
   *          does not support C++ exceptions.
   * \pre     ErrorType must have function ThrowAsException().
   * \return  The value.
   * \throws  <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() && noexcept(false) -> T& {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    return this->data_.ValueOrThrow();
  }

  /*!
   * \brief   Return the contained value or throw an exception.
   * \details This function does not participate in overload resolution when the compiler toolchain
   *          does not support C++ exceptions.
   * \pre     ErrorType must have function ThrowAsException().
   * \return  The value.
   * \throws  <TYPE> The exception type associated with the contained error, exceptionsafety{Strong}.
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  auto ValueOrThrow() const& noexcept(false) -> T const& {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert(enabled, "Should not be enabled when exceptions are disabled.");
    return this->data_.ValueOrThrow();
  }

  /*!
   * \brief     Returns the contained value or return the result of a function call.
   * \details   If *this contains a value, it is returned. Otherwise, the specified callable is invoked and its return
   *            value which is to be compatible to type T is returned from this function.
   *            The Callable is expected to be compatible to this interface:
   *            <code>
   *            T f(E const&);
   *            </code>
   *            If the callable is an nullptr, abortion will be triggered.
   * \tparam    F The type of the Callable f.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    The contained value or return the result of a function call.
   */
  template <typename F>
  auto Resolve(F&& callable_f) const noexcept -> value_type {
    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable_f)};
    if (is_null_ptr) {
      ara::core::Abort("ara::core::Result::Resolve: The callable cannot be nullptr!");
    }
    return data_.Resolve(std::forward<F>(callable_f));  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
  }

  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \details   The Callable is expected to be compatible to one of these two interfaces:
   *            <code>
   *            Result<XXX, E> f(T const&);
   *            XXX f(T const&);
   *            </code>
   *            meaning that the Callable either returns a Result<XXX> or a XXX directly,
   *            where XXX can be any type that is suitable for use by class Result.
   *            The return type of this function is always Result<XXX, E>.
   *            If this instance does not contain a value, a new Result<XXX, E> is still
   *            created and returned, with the original error contents of this instance
   *            being copied into the new instance.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsResult<F> Enable template when callable F returns Result<...> type.
   * \param[in] callable_f  The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type.
   */
  template <typename F, typename = CallableReturnsResult<F>>
  auto Bind(F&& callable_f) const noexcept -> std::result_of_t<F(value_type&)> {
    return this->data_.Bind(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsNoResult<F> Enable template when callable f does not return Result<...> type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   */
  template <typename F, typename = CallableReturnsNoResult<F>>
  auto Bind(F&& callable_f) const noexcept -> Result<std::result_of_t<F(value_type&)>, E> {
    return this->data_.Bind(std::forward<F>(callable_f));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Apply the given Callable to the value of this instance, and return a new Result with the result of the
   *            call.
   * \tparam    F The type of the Callable f.
   * \tparam    CallableReturnsNoResult<F> Enable template when callable f does not return Result<...> type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Result instance of the possibly transformed type.
   */
  template <typename F, typename = CallableReturnsVoid<F>>
  auto Bind(F&& callable_f) const noexcept -> Result<void, E> {
    return this->data_.Bind(std::forward<F>(callable_f));
  }

  /*!
   * \brief     Executes the given Callable with the value of this instance, or returns the contained error.
   * \tparam    F The type of the Callable f. Must be compatible with the signature XX f(T const&).
   *            Return type must not be void.
   * \tparam    U The return value of the Function F.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed type or the contained error.
   * \remark    This function can be used to transform the contained value.
   * \vprivate
   */
  template <typename F>
  auto Map(F&& callable_f) const noexcept -> Result<std::result_of_t<F(value_type&)>, E> {
    return data_.Map(std::forward<F>(callable_f));
  }

  /*!
   * \brief     Executes the given Callable with the error of this instance, or returns the untouched value.
   * \tparam    F the type of the Callable f. Must be compatible with the signature XX f(E const&).
   *            Return Type must be an object/integral type.
   * \param[in] callable_f The Callable.
   * \pre       -
   * \return    A new Result instance of the possibly transformed error type or the unchanged value.
   * \remark    This function can be used to transform the contained error, or perform error handling in general.
   * \vprivate
   */
  template <typename F>
  auto MapError(F&& callable_f) const noexcept -> Result<value_type, std::result_of_t<F(error_type const&)>> {
    // VECTOR Next Construct AutosarC++17_10-A2.11.2: MD_VAC_A2.11.2_usingNameReusedInLocalScope
    /*! \brief Local error type alias. */
    using E1 = std::result_of_t<F(error_type const&)>;

    return Result<value_type, E1>{data_.MapError(std::forward<F>(callable_f))};
  }

  /*!
   * \brief     Create a new Result with the given new value if this instance contains a value, otherwise it
   *            return the contained error.
   * \details   The new value is inplace-constructed from the given parameters args.
   * \tparam    U The type of the given value, defaulted to T&.
   * \tparam    Args Construction arguments for U.
   * \param[in] args Arguments for the in place construction of the replacement value.
   * \return    A new Result instance with the constructed replacement value or the contained error.
   * \remark    This function can be used to replace the contained value, which type may differ from T&.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename U = T&, typename... Args>
  auto Replace(Args&&... args) const noexcept -> Result<U, E> {
    return data_.template Replace<U>(std::forward<Args>(args)...);
  }

  /*!
   * \brief  Returns the contained value, else it returns the given alternative.
   * \tparam E1 New error type.
   * \param  alternative Other result.
   * \pre    -
   * \return The value of this, else alternative.
   * \vprivate
   */
  template <typename E1>
  auto Or(Result<T&, E1> const& alternative) const& noexcept -> Result<T&, E1> {
    // VCA_VAC_VALID_ARGUMENT
    return HasValue() ? Result<T&, E1>{Result::kInPlaceValue, GetRefUnsafe()} : alternative;
  }

  /*!
   * \brief     Returns the contained value, else transforms the contained error using the passed function.
   * \tparam    F Transform function for the error. Must be compatible to the signature Result<T&,E1> f(E const&);
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \return    The value of this, or the transformation result of the error.
   */
  template <typename F>
  auto OrElse(F&& callable_f) const& noexcept -> std::result_of_t<F(error_type const&)> {
    /*! \brief Local result type alias. */
    using R12 = std::result_of_t<F(error_type const&)>;
    static_assert(is_result<R12>::value, "Return type of f must be a Result.");
    static_assert(std::is_same<value_type, typename R12::value_type>::value, "Value type must not change.");
    return HasValue() ? R12{Result::kInPlaceValue, GetRefUnsafe()}  // VCA_VAC_VALID_ARGUMENT
                      : std::forward<F>(callable_f)(Error());       // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief  Returns the passed Result if this has a value, else it returns the error of this.
   * \tparam U New value type.
   * \param  other Other result.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return other if this has a value, else the error of this.
   */
  template <typename U>
  auto And(Result<U, E> const& other) const noexcept -> Result<U, E> {
    return data_.And(other);
  }

  /*!
   * \brief     Transforms the contained value using the passed function, else returns the contained error.
   * \tparam    F Transform function for the value. Must be compatible with the signature Result<XX,E> f(T const&).
   * \param[in] callable_f Transform function for the error.
   * \pre       -
   * \return    The transformation result of the value, else the error of this.
   */
  template <typename F>
  auto AndThen(F&& callable_f) const noexcept -> std::result_of_t<F(value_type&)> {
    return data_.AndThen(std::forward<F>(callable_f));
  }

  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred.
   * \details   The function has three possible results: (1) the contained error if this holds an error; (2) the
   *            contained value if this value fulfills the predicate pred; (3) the provided error if this
   *            value does not fulfill the predicate pred.
   * \tparam    F Unary function. Result must be convertible to bool. Must accept T const& as parameter.
   * \param[in] pred Unary function for checking the value.
   * \param     error Error that is returned if the value does not fulfill the predicate pred.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not error is returned. Otherwise, the contained error.
   */
  template <typename F>
  auto Filter(F&& pred, E const& error) const noexcept -> Result {
    return Result{data_.Filter(std::forward<F>(pred), error)};
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Returns the unchanged value if it fulfills the predicate pred, which returns a Result.
   * \details   The function has three possible results: (1) the contained error if this holds an error. Depending on
   *            the result of the pred call with this value as parameter it returns the contained value of this
   *            (2), or the returned error (3).
   * \tparam    F Function returning a Result. Must accept T const& as parameter.
   * \param[in] pred Function for checking the contained value.
   * \pre       -
   * \return    The unchanged value if fulfills the pred, if not the error of pred is returned.
   *            Otherwise, the error contained in this is returned.
   */
  template <typename F>
  auto Filter(F&& pred) const noexcept -> Result {
    return Result{data_.Filter(std::forward<F>(pred))};
  }

  /*!
   * \brief   Removes the value of the result.
   * \details If this contains a value the function returns an empty Result, else the contained error.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  An empty Result, or the contained error.
   */
  auto Drop() const noexcept -> Result<void, E> { return data_.Drop(); }

  /*!
   * \brief     Executes a given callable with the contained value as parameter. If this contains an error the callable
   *            is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F Callable, must have the signature void f(T const&).
   * \param[in] callable_f Callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  auto Inspect(F&& callable_f) const noexcept -> Result {
    return Result{data_.Inspect(std::forward<F>(callable_f))};
  }

  /*!
   * \brief     Executes a given callable with the contained error as parameter. If this contains a value the callable
   *            is not executed.
   * \tparam    F Callable, must have the signature void f(E const&).
   * \param[in] callable_f callable function.
   * \pre       -
   * \return    The unchanged Result.
   * \vprivate
   */
  template <typename F>
  auto InspectError(F&& callable_f) const noexcept -> Result {
    return Result{data_.InspectError(std::forward<F>(callable_f))};
  }

  /*!
   * \brief     Allows to inspect and remove a value from a Result. Executes the given callable with the contained value
   *            as parameter. If this result contains an error, the callable is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F           Type of the callable, must have the signature 'void f(T const&)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \return    A Result<void> containing either nothing or the original error.
   * \vprivate
   */
  template <typename F>
  auto Consume(F&& callable_f) const noexcept -> Result<void, E> {
    return data_.Consume(std::forward<F>(callable_f));
  }

  /*!
   * \brief     Allows to inspect and move a value out of a Result. Executes the given callable with the contained value
   *            as parameter. If this result contains an error, the callable is not executed.
   * \details   If the callable is an nullptr, abortion will be triggered.
   * \tparam    F           Callable, must have the signature 'void f(T &)'.
   * \param[in] callable_f  Callable function.
   * \pre       -
   * \return    A Result<void> containing either nothing or the original error.
   * \vprivate
   */
  template <typename F>
  auto Consume(F&& callable_f) noexcept -> Result<void, E> {
    return data_.Consume(std::forward<F>(callable_f));
  }

  /*!
   * \brief  Checks for equality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   */
  auto operator==(Result const& that) const noexcept -> bool {  // Comment to force line break. See TAR-22835.
    return this->data_ == that.data_;
  }

  /*!
   * \brief  Checks for inequality of two results.
   * \param  that Another instance.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if both are equal else false.
   */
  auto operator!=(Result const& that) const noexcept -> bool {  // Comment to force line break. See TAR-22835.
    return !(*this == that);
  }

 private:
  /*! \brief Union like container holding either the Good Type T or the Error Type E. */
  StorageType data_;

  /*!
   * \brief       Returns the contained reference.
   * \pre         -
   * \return      The contained reference.
   * \internal
   *              Justification for the const_cast: The const_cast is needed to be able to use the implementation for 'T
   *              const' and 'T'. As Value() does not return a reference of class-data, it is not violation of the
   *              encapsulation of this class.
   * \spec
   *   requires true;
   * \endspec
   * \endinternal
   */
  T& GetRefUnsafe() const noexcept {
    // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    return const_cast<Result*>(this)->Value();
  }
};

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_RESULT_H_

//  COV_JUSTIFICATION_BEGIN
//    \ID COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
//      \ACCEPT XX
//      \REASON The function is designed to throw an exeception and should not be used when compiling without exception
//              support. Therefore, a flag is provided to enable/disable compiling with exceptions. However, function
//              cannot be covered because a compile-time flag prevents it to be called.
//  COV_JUSTIFICATION_END
