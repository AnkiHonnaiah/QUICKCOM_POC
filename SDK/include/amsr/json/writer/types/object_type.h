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
/**       \file     object_type.h
 *        \brief    A collection of serializers for objects.
 *
 *        \details  Provides serializers for homogeneous C++ pair-ranges and Object types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "amsr/core/string_view.h"

#include "amsr/json/util/types.h"

#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/types/basic_types.h"

namespace amsr {
namespace json {
inline namespace types {
/*!
 * \brief A serializer type for predefined keys.
 *
 * \vprivate Component private.
 */
class DefaultKeySerializer {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Call operator.
   * \tparam T Type of value.
   * \param value to serialize as a key. Must be convertible to a StringView type.
   * \return The serializable key type.
   * \vprivate Component private.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \spec
   * requires true;
   * \endspec
   */
  template <typename T>
  auto operator()(T const& value) const noexcept -> JKeyType {
    static_assert(std::is_constructible<amsr::core::StringView, T>::value, "Keys must be convertible to a StringView");
    return JKeyType{amsr::core::StringView{value}};
  }
};

/*!
 * \brief A serializer type for a JSON array from a homogeneous C++ pair-range.
 * \tparam Range Type of range to serialize.
 * \tparam KeyFn Type of key function.
 * \tparam ValueFn Type of value function.
 *
 * \vprivate Component private.
 */
template <typename Range, typename KeyFn, typename ValueFn>
class PairRangeSerializer final {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Constructs a PairRangeSerializer.
   * \tparam KeyFn1 Type of key function.
   * \tparam ValueFn1 Type of value function.
   * \param range to serialize.
   * \param key_fn Function used to serialize single keys.
   * \param value_fn Function used to serialize single values.
   *
   * \context ANY
   * \pre The passed functions & range do not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private.
   */
  template <typename KeyFn1 = KeyFn, typename ValueFn1 = ValueFn>
  PairRangeSerializer(Range const& range, KeyFn1&& key_fn, ValueFn1&& value_fn) noexcept
      : map_{range}, key_function_{std::forward<KeyFn>(key_fn)}, value_function_{std::forward<ValueFn>(value_fn)} {}

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Call operator.
   * \tparam KS Type of key serializer.
   * \param os Object serializer to write into.
   * \return The serializer state after serializing the range.
   *
   * \context ANY
   * \pre  The functions contained in the class do not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private.
   */
  template <typename KS = KeySerializer>
  auto operator()(KS os) const noexcept -> KS;

 private:
  /*!
   * \brief Range instance to be serialized.
   */
  internal::Ref<Range const> map_;

  /*!
   * \brief Function used to serialize single keys.
   */
  KeyFn key_function_;

  /*!
   * \brief Function used to serialize single values.
   */
  ValueFn value_function_;
};

/*!
 * \brief An Object type.
 * \tparam Fn Type of serializer function.
 *
 * \vprivate Component private.
 */
template <typename Fn>
// VCA_VAJSON_MOLE_1298
struct JObjectType final {
  /*!
   * \brief Function used to serialize the object.
   */
  Fn fn;
};

// VECTOR NC AutosarC++17_10-A13.3.1: MD_JSON_rvalue_ref
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an object value.
 * \tparam Fn Type of serializer function. Must take an ObjectStart&& and return the follow-up serializer.
 * \param fn Function used to serialize the object.
 * \return The serializable object type.
 *
 * \context ANY
 * \pre The passed function does not throw any exceptions
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vpublic
 */
template <typename Fn, typename = std::enable_if_t<std::is_rvalue_reference<Fn&&>::value>>
auto JObject(Fn&& fn) noexcept -> JObjectType<Fn> {
  return {std::forward<Fn>(fn)};
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_JSON_rvalue_ref
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a homogeneous C++ pair-range (e.g. a map) as a JSON object.
 * \tparam Range Type of range.
 * \tparam KeyFn Type of key serializer function. Must take the range's key type and return a JSON type.
 * \tparam ValueFn Type of value serializer function. Must take the range's value type and return a JSON type.
 * \param range instance to be serialized.
 * \param key_fn Function used to serialize single keys.
 * \param value_fn Function used to serialize single values.
 * \return The serializable JSON object.
 *
 * \context ANY
 * \pre The passed range & functions do not throw any exceptions
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vpublic
 */
template <typename Range, typename KeyFn = DefaultKeySerializer, typename ValueFn = IdSerializer<Range>>
auto JObject(Range const& range, KeyFn&& key_fn = DefaultKeySerializer{},
             ValueFn&& value_fn = IdSerializer<Range>{}) noexcept
    -> JObjectType<PairRangeSerializer<Range, KeyFn, ValueFn>> {
  return {
      PairRangeSerializer<Range, KeyFn, ValueFn>{range, std::forward<KeyFn>(key_fn), std::forward<ValueFn>(value_fn)}};
}
}  // namespace types
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_H_
