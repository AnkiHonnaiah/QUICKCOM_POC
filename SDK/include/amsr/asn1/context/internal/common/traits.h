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
/*!        \file  amsr/asn1/context/internal/common/traits.h
 *        \brief  Traits and other metafunctions for contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TRAITS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TRAITS_H_

#include <type_traits>
#include "amsr/core/optional.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief Trait that checks whether a type has a \c StructureType member. Used to detect contexts.
 * \details Default case: False
 * \tparam T The type to check
 */
template <typename T, typename = void>
class HasStructureType : public std::false_type {};

/*!
 * \brief Trait that checks whether a type has a \c StructureType member. Used to detect contexts.
 * \details Special case: True
 * \tparam T The type to check
 */
template <typename T>
class HasStructureType<T, std::__void_t<typename T::StructureType>> : public std::true_type {};

/*!
 * \brief Trait that checks whether a type has a \c StructureType member of a given type.
 *      Used to detect contexts that concern a certain data type.
 * \details Note that this condition is stricter than the \c ConcernsType trait which also accepts structure types.
 *      This will return \c false for \c structure::Boolean and the like.
 * \details Default case: False
 * \see amsr::asn1::context::ConcernsType
 * \tparam T The potential context type to check
 * \tparam U The structure type for which to check T
 */
template <typename T, typename U, typename = void>
class HasStructureTypeOfType : public std::false_type {};

/*!
 * \brief Trait that checks whether a type has a \c StructureType member of a given type.
 *      Used to detect contexts that concern a certain data type.
 * \details Note that this condition is stricter than the \c ConcernsType trait which also accepts structure types.
 *      This will return \c false for \c structure::Boolean and the like.
 * \details Special case: True
 * \see amsr::asn1::context::ConcernsType
 * \tparam T The potential context type to check
 * \tparam U The structure type for which to check T
 */
template <typename T, typename U>
class HasStructureTypeOfType<T, U, std::__void_t<typename T::StructureType>>
    : public std::is_same<typename T::StructureType, U> {};

/*!
 * \brief Trait that checks whether a type passes as input for a utility (encoder, decoder, validator...).
 *      This is the case if it's either a context-like type with a defined structure type that's equal to T
 *      or if T and U are equal. In other words: Context<Boolean> concerns Boolean, Boolean concerns Boolean.
 *      This allows utilities to be used with raw types instead of contexts for easier client-side handling.
 *      Moreover, if the structure type of a context is an optional T, it is considered to concern T.
 * \tparam T The potential context type to check
 * \tparam U The structure type for which to check T
 */
template <typename T, typename U>
using ConcernsType = vac::language::compile_time::any<std::is_same<T, U>::value, HasStructureTypeOfType<T, U>::value,
                                                      HasStructureTypeOfType<T, amsr::core::Optional<U>>::value>;

/*!
 * \brief Shorthand notation for ConcernsType in Utility template parameters
 * \tparam T See ConcernsType
 * \tparam U See ConcernsType
 */
template <typename T, typename U>
using EnableFor = typename std::enable_if<context::ConcernsType<T, U>::value>::type;

/*!
 * \brief Trait that checks whether a type is an \c amsr::core::Optional type.
 * \details Default case: False
 */
template <typename>
class IsAmsrCoreOptional : public std::false_type {};

/*!
 * \brief Trait that checks whether a type is an \c amsr::core::Optional type.
 * \details Special case for Optional<T>: True
 */
template <typename T>
class IsAmsrCoreOptional<amsr::core::Optional<T>> : public std::true_type {};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TRAITS_H_
