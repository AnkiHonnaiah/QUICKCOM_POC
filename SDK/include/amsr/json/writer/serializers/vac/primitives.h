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
/**       \file     serializers/vac/primitives.h
 *        \brief    A collection of serializers for libVac primitive data types.
 *
 *        \details  Provides serializers for amsr::core::String, amsr::core::StringView, and vac::language::byte
 *types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_PRIMITIVES_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_PRIMITIVES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/string_view.h"
#include "vac/language/byte.h"

#include "amsr/json/util/types.h"
#include "amsr/json/writer/types/basic_types.h"

namespace amsr {
namespace json {
/*!
 * \brief Forward declaration for the GenericValueSerializer.
 *
 * \vprivate component private
 */
template <typename Return>
class GenericValueSerializer;

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
auto operator<<(GenericValueSerializer<Next>&& serializer, amsr::core::String const& string) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JString(string);
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
auto operator<<(GenericValueSerializer<Next>&& serializer, ::amsr::core::StringView string) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << JString(string);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a vac::language::byte value directly.
 * \tparam Next type of serializer.
 * \param serializer instance to write into.
 * \param byte to serialize.
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
auto operator<<(GenericValueSerializer<Next>&& serializer, ::vac::language::byte byte) noexcept ->
    typename GenericValueSerializer<Next>::Next {
  return std::move(serializer) << ::vac::language::to_integer<std::uint16_t>(byte);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_PRIMITIVES_H_
