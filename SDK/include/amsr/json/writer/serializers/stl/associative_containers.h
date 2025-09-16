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
/**       \file     serializers/stl/associative_containers.h
 *        \brief    A collection of serializers for associative STL containers.
 *
 *        \details  Provides serializers for (unordered) set, (unordered) map, and (unordered) multimap types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_ASSOCIATIVE_CONTAINERS_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_ASSOCIATIVE_CONTAINERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "amsr/json/writer/types/array_type.h"
#include "amsr/json/writer/types/object_type.h"

namespace amsr {
namespace json {
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a set of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Value Type of value.
 * \tparam Cmp Type of comparison function.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param set Set to serialize.
 * \return The succeeding serializer.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vpublic
 *
 * \spec
 * requires true;
 * \endspec
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Next, typename Value, typename Cmp, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::set<Value, Cmp, Alloc> const& set) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JArray(set);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an unordered set of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Value Type of value.
 * \tparam Hash Type of hash function.
 * \tparam Pred Type of predicate function.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param set Unordered set to serialize.
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
template <typename Next, typename Value, typename Hash, typename Pred, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer,
                std::unordered_set<Value, Hash, Pred, Alloc> const& set) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JArray(set);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a map of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Key Type of key. Must be convertible to a JKey.
 * \tparam Value Type of value.
 * \tparam Cmp Type of comparison function.
 * \tparam Alloc  Type of allocator.
 * \param serializer instance to write into.
 * \param map Map to serialize.
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
template <typename Next, typename Key, typename Value, typename Cmp, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::map<Key, Value, Cmp, Alloc> const& map) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JObject(map);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an unordered map of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Key Type of key. Must be convertible to a JKey.
 * \tparam Value Type of value.
 * \tparam Hash Type of hash function.
 * \tparam Pred Type of predicate function.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param map Unordered map to serialize.
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
template <typename Next, typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer,
                std::unordered_map<Key, Value, Hash, Pred, Alloc> const& map) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JObject(map);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a multimap of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Key Type of key. Must be convertible to a JKey.
 * \tparam Value Type of value.
 * \tparam Cmp Type of comparison function.
 * \tparam Alloc  Type of allocator.
 * \param serializer instance to write into.
 * \param map Multimap to serialize.
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
template <typename Next, typename Key, typename Value, typename Cmp, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::multimap<Key, Value, Cmp, Alloc> const& map) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JObject(map);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an unordered multimap of serializable elements.
 * \tparam Next type of serializer.
 * \tparam Key Type of key. Must be convertible to a JKey.
 * \tparam Value Type of value.
 * \tparam Hash Type of hash function.
 * \tparam Pred Type of predicate function.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param map Unordered multimap to serialize.
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
template <typename Next, typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
auto operator<<(GenericValueSerializer<Next>&& serializer,
                std::unordered_multimap<Key, Value, Hash, Pred, Alloc> const& map) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JObject(map);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_ASSOCIATIVE_CONTAINERS_H_
