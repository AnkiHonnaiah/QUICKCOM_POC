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
/*!      \file  function_signature.h
 *      \brief  Compile time templates to extract properties from a function signature.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_FUNCTION_SIGNATURE_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_FUNCTION_SIGNATURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "vac/language/cpp17_backport.h"
#include "vac/language/cpp20_backport.h"

namespace vac {
namespace language {
namespace detail {

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
/*!
 * \brief  Adds const to T if IsConst is true, else return T as is.
 * \tparam T        The type to add const to.
 * \tparam IsConst  The bool to check if const should be added.
 */
template <typename T, bool IsConst>
struct AddConditionalConst : vac::language::type_identity<T> {};
/*!
 * \brief  Adds const to T.
 * \tparam T  The type to add const to.
 */
template <typename T>
struct AddConditionalConst<T, true> : std::add_const<T> {};

/*!
 * \brief  Adds lvalue reference to T if IsLRef is true, else return T as is.
 * \tparam T       The type to add lvalue reference to.
 * \tparam IsLRef  The bool to check if lvalue reference should be added.
 */
template <typename T, bool IsLRef>
struct AddConditionalLRef : vac::language::type_identity<T> {};

/*!
 * \brief  Adds lvalue reference to T.
 * \tparam T  The type to add lvalue reference to.
 */
template <typename T>
struct AddConditionalLRef<T, true> : std::add_lvalue_reference<T> {};

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
/*!
 * \brief  Adds rvalue reference to T if IsRRef is true, else return T as is.
 * \tparam T       The type to add rvalue reference to.
 * \tparam IsRRef  The bool to check if rvalue reference should be added.
 */
template <typename T, bool IsRRef>
struct AddConditionalRRef : vac::language::type_identity<T> {};
/*!
 * \brief  Adds rvalue reference to T.
 * \tparam T  The type to add lvalue reference to.
 */
template <typename T>
struct AddConditionalRRef<T, true> : std::add_rvalue_reference<T> {};

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
/*!
 * \brief  Adds ref-qualifiers to T according to the bools IsLRef and IsRRef.
 * \tparam T       The type to add ref-qualifiers to.
 * \tparam IsLRef  The bool to determine if lvalue reference should be added or not.
 * \tparam IsRRef  The bool to determine if rvalue reference should be added or not.
 */
template <typename T, bool IsLRef, bool IsRRef>
struct AddConditionalLRRef : AddConditionalRRef<typename AddConditionalLRef<T, IsLRef>::type, IsRRef> {};

/*!
 * \brief  Adds const and ref-qualifiers to T according to the bools IsConst, IsLRef and IsRRef.
 * \tparam T        The type to add const and ref-qualifiers to.
 * \tparam IsConst  The bool to determine if const should be added or not.
 * \tparam IsLRef   and IsRRef The bools to determine ref-qualifiers.
 */
template <typename T, bool IsConst, bool IsLRef, bool IsRRef>
struct AddConditionalCLRRef : AddConditionalLRRef<typename AddConditionalConst<T, IsConst>::type, IsLRef, IsRRef> {};

namespace signature {

/*!
 * \brief  An argument pack.
 * \tparam Args  The types of signature parameters.
 */
template <typename... Args>
struct SignatureArgs {};

/*!
 * \brief Enumeration for ref qualifiers.
 */
enum class SignatureRefQual : std::size_t {
  None,
  LRef,
  RRef,
};

/*!
 * \brief  Qualifiers of signature, i.e. not return type or argument types.
 * \tparam IsConst     The bool type to define signature is const/non-const.
 * \tparam IsNoExcept  The bool type to define signature is noexcept.
 * \tparam RefQual     The enum type to define ref qualifiers is None/LRef/RRef.
 */
template <bool IsConst, bool IsNoExcept, SignatureRefQual RefQual>
struct SignatureQualifiers {
  /*!
   * \brief Is const.
   */
  static constexpr bool kIsConst{IsConst};

  /*!
   * \brief Is noexcept.
   */
  static constexpr bool kIsNoExcept{IsNoExcept};

  /*!
   * \brief Ref qualifier.
   */
  static constexpr SignatureRefQual kRef{RefQual};
};

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
/*!
 * \brief  Full description of a signature in an easy to access way.
 * \tparam SigQuals  The type of ref qualifiers.
 * \tparam R         The type of return type.
 * \tparam Args      The types of arguments of the callable.
 */
template <typename SigQuals, typename R, typename... Args>
class SignatureProperties {
 public:
  /*!
   * \brief Return type.
   */
  using RetT = R;

  /*!
   * \brief Argument types.
   */
  using ArgsT = SignatureArgs<Args...>;

  /*!
   * \brief Const and ref qualifiers.
   */
  using Qualifiers = SigQuals;
};
// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Since C++17 Noexcept is included in the signature, for earlier versions the noexcept specifier will be
 *          ignored.
 */
template <typename Signature>
struct HasNoexceptSpecifier : std::false_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with lvalue reference and noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) & noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with rvalue reference and noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) && noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with const and noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) const noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with lvalue reference, const and noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) const & noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Specialization for signatures with rvalue reference, const and noexcept specifier.
 */
template <typename R, typename... Args>
struct HasNoexceptSpecifier<R(Args...) const && noexcept> : std::true_type {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief   Helper type to determine if the signature contains the noexcept specifier.
 * \details Since C++17 Noexcept is included in the signature, for earlier versions the noexcept specifier will be
 *          ignored, hence HasNoExcept will always inherit from std::true_type.
 */
template <typename Signature>
struct HasNoexcept : HasNoexceptSpecifier<Signature> {};

/*!
 * \brief  Extracts types and qualifiers from signature. Three dimensions: const, noexcept, ref => 2*2*3 = 12
 *         possibilities.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier.
 * \tparam Noexcept   True if the signature contains the noexcept specifier.
 */
template <typename Signature, bool Noexcept = HasNoexcept<Signature>::value>
struct GetSigProps;

/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...), false>
    : SignatureProperties<SignatureQualifiers<false, false, SignatureRefQual::None>, R, Args...> {};
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const, false>
    : SignatureProperties<SignatureQualifiers<true, false, SignatureRefQual::None>, R, Args...> {};

/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) &, false>
    : SignatureProperties<SignatureQualifiers<false, false, SignatureRefQual::LRef>, R, Args...> {};
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const &, false>
    : SignatureProperties<SignatureQualifiers<true, false, SignatureRefQual::LRef>, R, Args...> {};

/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) &&, false>
    : SignatureProperties<SignatureQualifiers<false, false, SignatureRefQual::RRef>, R, Args...> {};
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const &&, false>
    : SignatureProperties<SignatureQualifiers<true, false, SignatureRefQual::RRef>, R, Args...> {};

// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) noexcept, true>
    : SignatureProperties<SignatureQualifiers<false, true, SignatureRefQual::None>, R, Args...> {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const noexcept, true>
    : SignatureProperties<SignatureQualifiers<true, true, SignatureRefQual::None>, R, Args...> {};

// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) & noexcept, true>
    : SignatureProperties<SignatureQualifiers<false, true, SignatureRefQual::LRef>, R, Args...> {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const & noexcept, true>
    : SignatureProperties<SignatureQualifiers<true, true, SignatureRefQual::LRef>, R, Args...> {};

// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) && noexcept, true>
    : SignatureProperties<SignatureQualifiers<false, true, SignatureRefQual::RRef>, R, Args...> {};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for signature properties.
 * \tparam R     The right type of the parameter.
 * \tparam Args  The types of Signature parameters.
 */
template <typename R, typename... Args>
struct GetSigProps<R(Args...) const && noexcept, true>
    : SignatureProperties<SignatureQualifiers<true, true, SignatureRefQual::RRef>, R, Args...> {};

/*!
 * \brief  Expands argument pack for invocability requirement for a functor against a signature.
 * \tparam F         The type of the functor.
 * \tparam SigProps  The type of the signature.
 * \tparam ArgsT     The types of signature parameters.
 */
template <typename F, typename SigProps, typename ArgsT>
class IsInvocableForSigArgExpander {
  /*!
   * \brief Always false.
   */
  constexpr static bool always_false{sizeof(F) != sizeof(F)};
  static_assert(always_false, "ArgsT must be a SignatureArgs.");
};

/*!
 * \brief  Checks if invocable for signature with expanded arguments.
 * \tparam F         The type of the functor.
 * \tparam SigProps  The type of the signature.
 * \tparam ArgsT     The types of signature parameters.
 */
template <typename F, typename SigProps, typename... Args>
class IsInvocableForSigArgExpander<F, SigProps, SignatureArgs<Args...>> {
  /*!
   * \brief IsRRef The bool to check if it is rvalue reference.
   */
  static constexpr bool IsRRef{SigProps::Qualifiers::kRef == SignatureRefQual::RRef};
  /*!
   * \brief IsLRef The bool to check if it is lvalue reference.
   */
  static constexpr bool IsLRef{!IsRRef};
  /*!
   * \brief IsConst The bool to check if it is const.
   */
  static constexpr bool IsConst{SigProps::Qualifiers::kIsConst};
  /*!
   * \brief Return type.
   */
  using R = typename SigProps::RetT;

 public:
  /*!
   * \brief Value.
   */
  static constexpr bool value{
      vac::language::is_invocable_r<R, typename AddConditionalCLRRef<F, IsConst, IsLRef, IsRRef>::type,
                                    Args...>::value};
};

/*!
 * \brief  Checks if F is invocable for signature.
 * \tparam F          The type of the functor.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier.
 */
template <typename F, typename Signature>
class IsInvocableForSig {
  /*!
   * \brief Signature properties.
   */
  using SigProps = signature::GetSigProps<Signature>;

 public:
  /*!
   * \brief Value.
   */
  static constexpr bool value{IsInvocableForSigArgExpander<F, SigProps, typename SigProps::ArgsT>::value};
};

}  // namespace signature

/*!
 * \brief    Check if a type has an operator().
 * \tparam   T  The type to check.
 * \vprivate Component Private
 */
template <typename T, typename = void>
struct HasOperator final : public std::false_type {};
/*!
 * \brief    Check if a type has an operator().
 * \tparam   T  The type to check.
 * \vprivate Component Private
 */
template <typename T>
struct HasOperator<T, vac::language::void_t<decltype(&T::operator())>> final : public std::true_type {};

/*!
 * \brief    Check if a type could possibly be a lambda.
 * \details  A lambda is a closure type, which is a unique, unnamed non-union class type, and has an operator of
 *           undefined signature. See [expr.prim.lambda] in C++14 or [expr.prim.lambda.closure] in C++17.
 * \tparam   T  The type to check.
 * \vprivate Component Private
 */
template <typename T>
struct IsPossiblyLambda final : public std::integral_constant<bool, std::is_class<T>::value && HasOperator<T>::value> {
};

/*!
 * \brief  Check if a type is comparable with nullptr.
 * \tparam T  The type of callable.
 */
template <typename T, typename = void>
struct IsPossiblyNullptr : std::false_type {};

/*!
 * \brief  Check if a type is comparable with nullptr.
 * \tparam T  The type of callable.
 */
template <typename T>
struct IsPossiblyNullptr<T,
                         vac::language::void_t<decltype(std::declval<T>() == nullptr),
                                               std::enable_if_t<!std::is_function<std::remove_reference_t<T>>::value &&
                                                                !IsPossiblyLambda<std::decay_t<T>>::value>>>
    : std::true_type {};

}  // namespace detail
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_FUNCTION_SIGNATURE_H_
