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
/**       \file     array_type.h
 *        \brief    A collection of serializers for range-based containers.
 *
 *        \details  Provides serializers for arrays and tuples.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_ARRAY_TYPE_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_ARRAY_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <type_traits>
#include <utility>

#include "ara/core/string_view.h"

#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/writer/serializers/stl/primitives.h"
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/serializers/vac/primitives.h"
#include "amsr/json/writer/types/basic_types.h"

namespace amsr {
namespace json {
inline namespace types {
/*!
 * \brief A serializer type for a JSON array from a homogeneous C++ range.
 * \tparam Range Type of range.
 * \tparam Fn The function type for this serializer.
 *
 * \vprivate Component private.
 */
template <typename Range, typename Fn>
class RangeSerializer final {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Constructs a RangeSerializer.
   * \tparam Fn1 Type of function.
   * \param range to serialize.
   * \param fn Function used to serialize. Must not throw exceptions.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private
   * \spec
   * requires true;
   * \endspec
   */
  template <typename Fn1 = Fn>
  RangeSerializer(Range const& range, Fn1&& fn) noexcept : container_{range}, function_{std::forward<Fn>(fn)} {}

  // VECTOR NC AutosarC++17_10-M0.3.1: MD_JSON_AutosarC++17_10-M0.3.1_null_deref
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Call operator.
   * \tparam AS Type of array serializer.
   * \param as Array serializer to write into.
   *
   * \context ANY
   * \pre The function contained in the class does not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private
   *
   * \internal
   * - Serialize every element of the array as a JSON value.
   * \endinternal
   */
  template <typename AS = ArrayStart>
  void operator()(AS as) const noexcept {
    // VECTOR NC AutosarC++17_10-A7.1.5: MD_JSON_AutosarC++17_10-A7.1.5_auto_it
    // VCA_VAJSON_THIS_DEREF
    for (auto const& value : *this->container_) {
      as = std::move(as) << this->function_(value);
    }  // VCA_VAJSON_EXTERNAL_CALL
  }

 private:
  /*!
   * \brief Container instance to be serialized.
   */
  internal::Ref<Range const> container_;

  /*!
   * \brief Function to serialize single items with.
   */
  Fn function_;
};

/*!
 * \brief Serialize an ad-hoc defined Tuple as heterogeneous array.
 * \tparam Fn The function type that defines the serialization.
 *
 * \vprivate Component private
 */
template <typename Fn>
struct JArrayType final {
  /*!
   * \brief Wrapped function value.
   */
  Fn fn;
};

// VECTOR NC AutosarC++17_10-A13.3.1: MD_JSON_rvalue_ref
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes an ad-hoc defined Tuple as a heterogeneous array.
 * \tparam Fn Type of serializer function. Must take an ArrayStart&& and return the follow-up serializer.
 * \param fn Function used to serialize the tuple.
 * \return A serializable Tuple type.
 *
 * \details The function can be used to define a tuple by adding values.
 *
 * \context ANY
 * \pre The passed function does not throw any exceptions
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 */
template <typename Fn, typename = std::enable_if_t<std::is_rvalue_reference<Fn&&>::value>>
auto JArray(Fn&& fn) noexcept -> JArrayType<Fn> {
  return {std::forward<Fn>(fn)};
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_JSON_rvalue_ref
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a homogeneous C++ range as a JSON array.
 * \tparam Range Type of range.
 * \tparam Fn Type of value serializer function. Must take the range's value type and return a JSON type.
 * \param range instance to be serialized.
 * \param fn Function used to serialize single elements.
 * \return A serializable JSON array.
 *
 * \context ANY
 * \pre The passed range & function do not throw any exceptions
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 */
template <typename Range, typename Fn = IdSerializer<Range>>
auto JArray(Range const& range, Fn&& fn = IdSerializer<Range>{}) noexcept -> JArrayType<RangeSerializer<Range, Fn>> {
  return {RangeSerializer<Range, Fn>{range, std::forward<Fn>(fn)}};
}
}  // namespace types
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_ARRAY_TYPE_H_
