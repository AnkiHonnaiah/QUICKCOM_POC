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
/*!        \file  amsr/asn1/validator/internal/traits.h
 *        \brief  Traits and metafunctions for use in Validators
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_TRAITS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_TRAITS_H_

#include <type_traits>

namespace amsr {
namespace asn1 {
namespace validator {

/*!
 * \brief A trait that checks whether a specific validator has validity criteria (a.k.a. Constraints)
 * \details Default case: False
 * \tparam T The validator to check
 */
template <typename T, typename = void>
class HasValidityCriteria : public std::false_type {};

/*!
 * \brief A trait that checks whether a specific validator has validity criteria (a.k.a. Constraints)
 * \details Special case: True
 * \tparam T The validator to check
 */
template <typename T>
class HasValidityCriteria<T, std::__void_t<decltype(T::EvaluateValidity)>> : public std::true_type {};

/*!
 * \brief A trait that checks whether a specific validator has special well-formedness criteria
 * \details Default case: False
 * \tparam T The validator to check
 */
template <typename T, typename = void>
class HasWellFormednessCriteria : public std::false_type {};

/*!
 * \brief A trait that checks whether a specific validator has special well-formedness criteria
 * \details Default case: True
 * \tparam T The validator to check
 */
template <typename T>
class HasWellFormednessCriteria<T, std::__void_t<decltype(T::EvaluateSpecialWellFormedness)>> : public std::true_type {
};

}  // namespace validator
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_VALIDATOR_TRAITS_H_
