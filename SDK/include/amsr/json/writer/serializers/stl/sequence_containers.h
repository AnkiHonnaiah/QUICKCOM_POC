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
/**       \file     serializers/stl/sequence_containers.h
 *        \brief    A collection of serializers for STD sequence containers.
 *
 *        \details  Provides serializers for std::array, std::vector, and std::deque types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_SEQUENCE_CONTAINERS_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_SEQUENCE_CONTAINERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <deque>
#include <type_traits>
#include <utility>
#include <vector>

#include "amsr/json/writer/types/array_type.h"

namespace amsr {
namespace json {
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an array of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Value Type of value.
 * \tparam N Size of the array.
 * \param serializer instance to write into.
 * \param array to serialize.
 * \return The succeeding serializer.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \spec
 * requires true;
 * \endspec
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Next, typename Value, std::size_t N>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::array<Value, N> const& array) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JArray(array);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a vector of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Value Type of value.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param vector to serialize.
 * \return The succeeding serializer.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \spec
 * requires true;
 * \endspec
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Next, typename Value, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::vector<Value, Alloc> const& vector) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JArray(vector);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a deque of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Value Type of value.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param deque to serialize.
 * \return The succeeding serializer.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \spec
 * requires true;
 * \endspec
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Next, typename Value, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::deque<Value, Alloc> const& deque) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JArray(deque);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_SEQUENCE_CONTAINERS_H_
