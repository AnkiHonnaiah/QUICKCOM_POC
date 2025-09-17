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
/*!        \file  amsr/asn1/context/internal/common/nature_metafunctions.h
 *        \brief  Metafunctions for use in contexts. They manipulate the "nature" of types, i.e. whether something is
 *                a context or a structure type.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_NATURE_METAFUNCTIONS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_NATURE_METAFUNCTIONS_H_

#include "amsr/asn1/context/internal/common/context.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief Metafunction that contains its return value in a static typedef called \c Type which is guaranteed to exist.
 *      This type is further guaranteed to be a context-like type, meaning utilities (encoder, decoder, validator...)
 *      can use it internally.
 * \details Default case: Map a structure type \c T to \c Context<T>
 * \tparam T The type to convert to a context.
 * \tparam Enable Used for SFINAE: Specializations fail here to disable themselves.
 */
template <typename T, typename Enable = void>
class EnsureContextNature {
 public:
  /*!
   * \brief Return value of the metafunction
   */
  using Type = Context<T>;
};

/*!
 * \brief Metafunction that contains its return value in a static typedef called \c Type which is guaranteed to exist.
 *      This type is further guaranteed to be a context-like type, meaning utilities (encoder, decoder, validator...)
 *      can use it internally.
 * \details Special case: Map a context type \c T to \c T
 * \tparam T The type to convert to a context.
 */
template <typename T>
class EnsureContextNature<T, typename std::enable_if<HasStructureType<T>::value>::type> {
 public:
  /*!
   * \brief Return value of the metafunction
   */
  using Type = T;
};

/*!
 * \brief Metafunction that contains its return value in a static typedef called \c Type which is guaranteed to exist.
 *      This type is further guaranteed to be a structure type.
 * \details Default case: Map a structure type \c T to \c T
 * \tparam T The determinant type to convert to a structure type.
 * \tparam Enable Used for SFINAE: Specializations fail here to disable themselves.
 */
template <typename T, typename Enable = void>
class EnsureStructureNature {
 public:
  /*!
   * \brief Return value of the metafunction
   */
  using Type = T;
};

/*!
 * \brief Metafunction that contains its return value in a static typedef called \c Type which is guaranteed to exist.
 *      This type is further guaranteed to be a structure type.
 * \details Special case: Recursively call this metafunction to unwrap contexts
 * \tparam T The determinant type to convert to a structure type.
 * \tparam Enable Used for SFINAE: Specializations fail here to disable themselves.
 */
template <typename T>
class EnsureStructureNature<T, typename std::enable_if<HasStructureType<T>::value>::type> {
 public:
  /*!
   * \brief Return value of the metafunction
   */
  using Type = typename EnsureStructureNature<typename T::StructureType>::Type;
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_NATURE_METAFUNCTIONS_H_
