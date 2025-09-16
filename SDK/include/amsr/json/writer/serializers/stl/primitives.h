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
/**       \file     serializers/stl/primitives.h
 *        \brief    A collection of serializers for primitive data types.
 *
 *        \details  Provides serializers for pointer, bool, number, and string types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_PRIMITIVES_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_PRIMITIVES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <type_traits>
#include <utility>

#include "ara/core/string_view.h"

#include "amsr/json/util/types.h"
#include "amsr/json/writer/types/basic_types.h"

namespace amsr {
namespace json {
/*!
 * \brief Forward declaration for the GenericValueSerializer.
 * \tparam Return type after using operator<<().
 *
 * \vpublic
 */
template <typename Return>
class GenericValueSerializer;

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a null value directly from a nullptr literal.
 * \tparam Next type of serializer.
 * \param serializer instance to write into.
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
template <typename Next>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::nullptr_t) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JNull();
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a value directly from a pointer.
 * \tparam Next type of serializer.
 * \param serializer instance to write into.
 * \param ptr The pointer whose value to serialize.
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
 *
 * \internal
 * - If the given pointer is a nullptr:
 *   - Serialize it as a null type.
 * - Otherwise:
 *   - Serialize the pointer's value.
 * \endinternal
 */
template <typename Next, typename T>
auto operator<<(GenericValueSerializer<Next>&& serializer, T const* ptr) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  // VCA_VAJSON_EXTERNAL_CALL
  return (ptr == nullptr) ? (std::move(serializer) << JNull()) : (std::move(serializer) << *ptr);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a bool value directly.
 * \tparam Next type of serializer.
 * \param serializer instance to write into.
 * \param b Bool value to serialize.
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
template <typename Next>
auto operator<<(GenericValueSerializer<Next>&& serializer, bool b) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JBool(b);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a number value directly.
 * \tparam Next type of serializer.
 * \tparam N Type of number.
 * \param serializer instance to write into.
 * \param number to serialize.
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
template <typename Next, typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
auto operator<<(GenericValueSerializer<Next>&& serializer, N number) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JNumber(number);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a string value directly.
 * \tparam Next type of serializer.
 * \param serializer instance to write into.
 * \param string to serialize.
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
template <typename Next>
auto operator<<(GenericValueSerializer<Next>&& serializer, std::string const& string) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JString(string);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STL_PRIMITIVES_H_
