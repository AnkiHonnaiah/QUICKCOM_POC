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
/**       \file     serializers/vac/sequence_containers.h
 *        \brief    A collection of serializers for libVac sequence containers.
 *
 *        \details  Provides serializers for amsr::core::Span, vac::container::StaticVector, and
 *                  vac::container::StaticList types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_SEQUENCE_CONTAINERS_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_SEQUENCE_CONTAINERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/span.h"
#include "ara/core/vector.h"
#include "vac/container/static_list.h"
#include "vac/container/static_vector.h"

#include "amsr/json/writer/types/array_type.h"

namespace amsr {
namespace json {
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an array view of serializable elements.
 * \tparam Serializer Type of serializer.
 * \tparam Value Type of value.
 * \param serializer instance to write into.
 * \param view to serialize.
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
template <typename Serializer, typename Value>
auto operator<<(Serializer&& serializer, ::amsr::core::Span<Value> const& view) noexcept -> typename Serializer::Next {
  return std::forward<Serializer>(serializer) << JArray(view);
}

/*!
 * \brief Serializes a vector of serializable elements.
 * \tparam Serializer Type of serializer.
 * \tparam Value Type of value.
 * \tparam Alloc Vector allocator.
 * \param[in,out] serializer instance to write into.
 * \param[in] vector Data to serialize.
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
template <typename Serializer, typename Value, typename Alloc>
auto operator<<(Serializer&& serializer, ::ara::core::Vector<Value, Alloc> const& vector) noexcept ->
    typename Serializer::Next {
  return std::forward<Serializer>(serializer) << JArray(vector);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a static vector of serializable elements.
 * \tparam Serializer Type of serializer.
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
 * \vpublic
 *
 * \spec
 * requires true;
 * \endspec
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Serializer, typename Value, typename Alloc>
auto operator<<(Serializer&& serializer, ::vac::container::StaticVector<Value, Alloc> const& vector) noexcept ->
    typename Serializer::Next {
  return std::forward<Serializer>(serializer) << JArray(vector);
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a static list of serializable elements.
 * \tparam Serializer Type of serializer.
 * \tparam Value Type of value.
 * \tparam Alloc Type of allocator.
 * \param serializer instance to write into.
 * \param list to serialize.
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
template <typename Serializer, typename Value, typename Alloc>
auto operator<<(Serializer&& serializer, ::vac::container::StaticList<Value, Alloc> const& list) noexcept ->
    typename Serializer::Next {
  return std::forward<Serializer>(serializer) << JArray(list);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_SEQUENCE_CONTAINERS_H_
