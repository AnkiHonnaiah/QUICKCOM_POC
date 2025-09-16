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
/*!        \file  rebind_allocator.h
 *        \brief  Reimplements functionality from the C++ standard library.
 *         \unit  VaCommonLib::LanguageSupport::UniqueFunction
 *
 *      \details  Provides implementations from the C++ standard library for constructs that are not working
 *                correctly in all toolchains.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_REBINDALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_REBINDALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <type_traits>

#include "vac/language/cpp17_backport.h"

namespace vac {
namespace language {
namespace rebindallocator {

namespace detail {

/*!
 * \brief  Determines whether Alloc supports rebind<U>::other.
 * \tparam Alloc Allocator type. Must fulfill C++ Named Requirements: Allocator.
 * \tparam U The type that will be rebinded to. Must fulfill C++ Named Requirements: Allocator, for type T.
 */
template <typename Alloc, typename U, typename = void>
struct HasRebindOther : std::false_type {};

/*!
 * \brief  Specialization when Alloc has rebind<U>::other.
 * \tparam Alloc Allocator type. Must fulfill C++ Named Requirements: Allocator.
 * \tparam U The type that will be rebinded to. Must fulfill C++ Named Requirements: Allocator, for type T.
 */
template <typename Alloc, typename U>
struct HasRebindOther<Alloc, U, vac::language::void_t<typename Alloc::template rebind<U>::other>> : std::true_type {};

}  // namespace detail

/*!
 * \brief  Rebinds the allocator Alloc for the type U.
 * \tparam Alloc Allocator type. Must fulfill C++ Named Requirements: Allocator.
 * \tparam U The type that will be rebinded to. Must fulfill C++ Named Requirements: Allocator, for type T.
 * \trace  CREQ-VaCommonLib-RebindAllocator
 */
template <typename Alloc, typename U, bool = detail::HasRebindOther<Alloc, U>::value>
class RebindAllocator;

/*!
 * \brief  Rebinds the allocator Alloc for the type U when Alloc implements rebind.
 * \tparam Alloc Allocator class. Must fulfill C++ Named Requirements: Allocator.
 * \tparam T The value type of Alloc. Must fulfill C++ Named Requirements: Allocator, for type T.
 * \tparam Args Additional types for Alloc.
 * \tparam U The type that will be rebinded to. Must fulfill C++ Named Requirements: Allocator, for type T.
 * \trace  CREQ-VaCommonLib-RebindAllocator
 */
template <template <typename, typename...> class Alloc, typename T, typename... Args, typename U>
class RebindAllocator<Alloc<T, Args...>, U, true> {
 public:
  /*! \brief Rebound allocator type. */
  using type = typename Alloc<T, Args...>::template rebind<U>::other;
};

/*!
 * \brief  Rebinds the allocator Alloc for the type U when Alloc does not implement rebind.
 * \tparam Alloc Allocator class. Must fulfill C++ Named Requirements: Allocator.
 * \tparam T The value type of Alloc. Must fulfill C++ Named Requirements: Allocator, for type T.
 * \tparam Args Additional types for Alloc.
 * \tparam U The type that will be rebinded to. Must fulfill C++ Named Requirements: Allocator, for type T.
 * \trace CREQ-VaCommonLib-RebindAllocator
 */
template <template <typename, typename...> class Alloc, typename T, typename... Args, typename U>
class RebindAllocator<Alloc<T, Args...>, U, false> {
 public:
  /*! \brief Rebound allocator type. */
  using type = Alloc<U, Args...>;
};

}  // namespace rebindallocator
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_REBINDALLOCATOR_H_
