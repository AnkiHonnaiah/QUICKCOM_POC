/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file ara/core/utility.h
 *        \brief SWS core type generic helpers.
 *         \unit VaCommonLib::Utility
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_UTILITY_H_
#define LIB_VAC_INCLUDE_ARA_CORE_UTILITY_H_

#include <functional>
#include <initializer_list>
#include <utility>

namespace ara {
namespace core {
/*!
 * \brief   Denote an operation to be performed in-place.
 * \details An instance of this type can be passed to certain constructors of ara::core::Optional to denote
 *          the intention that construction of the contained type shall be done in-place, i.e. without any
 *          copying taking place.
 * \trace   SPEC-7552598, CREQ-VaCommonLib-GenericHelpers
 */
struct in_place_t {};

// VECTOR Next Construct AutosarC++17_10-M17.0.2: MD_VAC_M17.0.2_standardLibraryObjectNameOverride
/*!
 * \brief The singleton instance of in_place_t.
 * \trace SPEC-7552600, CREQ-VaCommonLib-GenericHelpers
 */
constexpr in_place_t in_place{};

/*!
 * \brief   Denote a type-distinguishing operation to be performed in-place.
 * \details An instance of this type can be passed to certain constructors of ara::core::Variant to denote
 *          the intention that construction of the contained type shall be done in-place, i.e. without any
 *          copying taking place.
 * \tparam  T The type of in_place_type_t
 * \trace   SPEC-7552601, CREQ-VaCommonLib-GenericHelpers
 */
template <typename T>
struct in_place_type_t {};

/*!
 * \brief   Denote an index-distinguishing operation to be performed in-place.
 * \details An instance of this type can be passed to certain constructors of ara::core::Variant to denote
 *          the intention that construction of the contained type shall be done in-place, i.e. without any
 *          copying taking place.
 * \tparam  I of type std::size_t
 * \trace   SPEC-7552603, CREQ-VaCommonLib-GenericHelpers
 */
template <std::size_t I>
struct in_place_index_t {};

/*!
 * \brief    Helper struct to check if a index-distinguishing operation is denotable.
   \details  If this overload is selected then it is NOT an index-distinguishing operation
 * \tparam   T The type to be checked if it can denote an index-distinguishing operation. Can be any type.
 * \pre      -
 * \spec     requires true; \endspec
 * \trace    CREQ-VaCommonLib-GenericHelpers
 * \vprivate Component Private
 */
template <typename T>
struct is_index_spec : std::false_type {};

/*!
 * \brief    Helper struct to check if a index-distinguishing operation is denotable.
   \details  If this overload is selected then it is an index-distinguishing operation
 * \tparam   I of type std::size_t
 * \pre      -
 * \spec     requires true; \endspec
 * \trace    CREQ-VaCommonLib-GenericHelpers
 * \vprivate Component Private
 */
template <std::size_t I>
struct is_index_spec<in_place_index_t<I>> : std::true_type {};

/*!
 * \brief    Helper struct to check if a type-distinguishing operation is denotable.
   \details  If this overload is selected then it is NOT a type-distinguishing operation
 * \tparam   T The type to be checked if it can denote a type-distinguishing operation. Can be any type.
 * \pre      -
 * \spec     requires true; \endspec
 * \trace    CREQ-VaCommonLib-GenericHelpers
 * \vprivate Component Private
 */
template <typename T>
struct is_type_spec : std::false_type {};

/*!
 * \brief    Helper struct to check if a type-distinguishing operation is denotable.
   \details  If this overload is selected then it is a type-distinguishing operation
 * \tparam   U The type to be checked if it can denote a type-distinguishing operation. Can be any type.
 * \pre      -
 * \spec     requires true; \endspec
 * \trace    CREQ-VaCommonLib-GenericHelpers
 * \vprivate Component Private
 */
template <typename U>
struct is_type_spec<in_place_type_t<U>> : std::true_type {};

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief     Return a pointer to the block of memory that contains the elements of a container
 * \tparam    Container The type of the container. Container must be a type with a data() method
 * \param[in] c An instance of Container
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A pointer to the block of memory that contains the elements of a container
 * \trace     SPEC-7552605, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename Container>
constexpr auto data(Container& c) noexcept -> decltype(c.data()) {
  return c.data();
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return a const_pointer to the block of memory that contains the elements of a container.
 * \tparam  Container The type of the container. Container must be a type with a data() method
 * \param   c An instance of Container
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  A const_pointer to the block of memory that contains the elements of a container
 * \trace   SPEC-7552606, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename Container>
constexpr auto data(Container const& c) noexcept -> decltype(c.data()) {
  return c.data();
}

// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
// VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
/*!
 * \brief     Return a pointer to the block of memory that contains the elements of a raw array
 * \tparam    T The type of array elements. The C++ language does not allow storing references
 *            in an array(n4659, 11.3.2 #5).
 * \tparam    N The number of elements in the array, of type std::size_t
 * \param[in] array Reference to a raw array
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A pointer to the block of memory that contains the elements of a raw array
 * \trace     SPEC-7552607, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept {
  return array;
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief     Return a pointer to the block of memory that contains the elements of a std::initializer_list
 * \tparam    E The type of elements in the std::initializer_list. The C++ language does not allow
 *            storing references in an array(n4659, 11.3.2 #5), which means references cannot be
 *            used for std::initializer_list.(n4659, 21.9.1, #1)
 * \param[in] il The std::initializer_list
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A pointer to the block of memory that contains the elements of a std::initializer_list
 * \trace     SPEC-7552608, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename E>
constexpr E const* data(std::initializer_list<E> il) noexcept {
  return il.begin();
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return the size of a container
 * \tparam  Container The type of the container. Container must be type with a size() method
 * \param   c An instance of Container
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  The size of a container
 * \trace   SPEC-7552609, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename Container>
constexpr auto size(Container const& c) noexcept -> decltype(c.size()) {
  return c.size();
}

// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return the size of a raw array
 * \tparam  T The type of array elements. The C++ language does not allow storing references in
 *          an array(n4659, 11.3.2 #5).
 * \tparam  N The number of elements in the array, of type std::size_t.
 *          Raw arrays in C++ have a minimum size of 1.(n4659, 11.3.4, #1)
 * \param   array Reference to a raw array
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  The size of a raw array.
 * \trace   SPEC-7552610, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename T, std::size_t N>
constexpr auto size(T const (&array)[N]) noexcept -> std::size_t {
  static_cast<void>(array);
  return N;
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return whether the given container is empty
 * \tparam  Container A type with an empty() method
 * \param   c An instance of Container
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  Whether the given container is empty
 * \trace   SPEC-7552611, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename Container>
constexpr auto empty(Container const& c) noexcept -> decltype(c.empty()) {
  return c.empty();
}

// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return whether the given raw array is empty
 * \tparam  T The type of array elements. The C++ language does not allow storing references
 *          in an array(n4659, 11.3.2 #5).
 * \tparam  N The number of elements in the array, of type std::size_t
 *          Raw arrays in C++ have a minimum size of 1.(n4659, 11.3.4, #1)
 * \param   array The raw array
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  As raw arrays cannot have zero elements in C++, this function always returns false
 * \trace   SPEC-7552612, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename T, std::size_t N>
constexpr auto empty(T const (&array)[N]) noexcept -> bool {
  static_cast<void>(array);
  return false;
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief   Return whether the given std::initializer_list is empty
 * \tparam  E The type of elements in the std::initializer_list. The C++ language does not allow
 *            storing references in an array(n4659, 11.3.2 #5), which means references cannot be
 *            used for std::initializer_list.(n4659, 21.9.1, #1)
 * \param   il The std::initializer_list
 * \pre     -
 * \spec
 *   requires true;
 * \endspec
 * \return  Whether the given std::initializer_list is empty
 * \trace   SPEC-7552613, CREQ-VaCommonLib-GenericHelpers
 * \vpublic
 */
template <typename E>
constexpr auto empty(std::initializer_list<E> il) noexcept -> bool {
  return il.size() == 0;
}

/*!
 * \brief  A wrapper for std::hash for hashing standard types.
 * \tparam T The key to be hashed. Must fulfill C++ named requirements: Hash.
 * \trace  CREQ-VaCommonLib-GenericHelpers
 */
template <typename T>
struct hash : public std::hash<T> {
  // VECTOR Next Construct VectorC++-V11.0.3: MD_VAC_V11-0-3_structShallContainOnlyPublicMember
  /*! \brief Inherited callable operator from std::hash. */
  using std::hash<T>::operator();
};

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_UTILITY_H_
