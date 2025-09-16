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
/**     \file       utils.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::Utility
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_UTILS_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/core/optional.h"         // IWYU pragma: export
#include "vac/language/function_ref.h"  // IWYU pragma: export

namespace amsr {
namespace log {
/*!
 * \brief   Drops the value that's passed into the function
 *
 * \tparam  T  The type whose value is dropped
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component private.
 */
template <typename T>
void Drop(T&&) noexcept {}  // VECTOR SL AutosarC++17_10-M0.1.8: MD_LOG_AutosarC++17_10-M0.1.8_drop_unused

/*!
 * \brief   Copies the value
 *
 * \tparam  T     The type to copy.
 *                T must implement the CopyConstructible named requirement.
 *
 * \param   value The value to copy
 *
 * \return  The copied value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component private.
 */
template <typename T>
auto Clone(T const& value) noexcept -> T {
  return {value};
}

/*!
 * \brief  A boxed type
 *
 * \tparam T       The type to box
 * \tparam Deleter The deleter type to use
 *
 * \vprivate Component private.
 */
template <typename T, typename Deleter = std::default_delete<T>>
using Box = std::unique_ptr<T, Deleter>;

/*!
 * \brief   Create a boxed value
 *
 * \tparam  T      The type to box.
 *
 * \tparam  Args  The argument types for the construction
 * \param   args  The arguments for the construction
 *
 * \return        The boxed value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component private.
 */
template <class T, class... Args>
auto MakeBox(Args&&... args) noexcept(std::is_nothrow_constructible<T>::value) -> Box<T> {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

/*!
 * \brief   An optional value
 *
 * \tparam  T  The type that's optional
 *
 * \vprivate Component private.
 */
template <typename T>
using Optional = core::Optional<T>;

/*!
 * \brief   A non-owning type-erased callable
 *
 * \tparam  Signature  The signature of the callable.
 */
template <typename Signature>
using FunctionRef = vac::language::FunctionRef<Signature>;

/*!
 * \brief   Convert signed number to two's complement presentation
 *
 * \tparam  SignedType  The signed integer type
 *
 * \param   value       Signed value converted to two's complement
 *
 * \return  Two's complement of input value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \details As defined in SWS_LOG_00015
 * \vprivate Component private.
 */
template <typename SignedType>
constexpr auto ConvertToTwosComplement(SignedType value) noexcept -> typename std::make_unsigned<SignedType>::type {
  /*! Typedef to have a readable, unsigned type of the same size as the signed type used here */
  using UnsignedType = typename std::make_unsigned<SignedType>::type;
  static_assert(std::is_signed<SignedType>::value, "Type SignedType is not signed type.");
  static_assert(sizeof(UnsignedType) == sizeof(SignedType), "Size of signed and unsigned type are not equal");

  // Two's complement: 2^(8 * TypeSize) + NegativeValue
  constexpr UnsignedType kMaxValue{std::numeric_limits<UnsignedType>::max()};
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1_use_type_limits_for_complement
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_LOG_AutosarC++17_10-A4.7.1
  return static_cast<UnsignedType>((kMaxValue + static_cast<UnsignedType>(value)) + 1u);
}

}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_UTILS_H_
