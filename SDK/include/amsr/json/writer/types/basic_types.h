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
/**       \file     basic_types.h
 *        \brief    A collection of serializers for basic JSON types.
 *
 *        \details  Provides serializers for Null, Bool, Key, Number, and String types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BASIC_TYPES_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BASIC_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/string_view.h"

#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
inline namespace types {
/*!
 * \brief A Null type.
 *
 * \vprivate Component private.
 */
struct JNullType final {};

/*!
 * \brief Serializes a Null value.
 * \return The serializable null type.
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
 */
constexpr inline auto JNull() noexcept -> JNullType { return JNullType{}; }

/*!
 * \brief A Bool type.
 *
 * \vprivate Component private.
 */
struct JBoolType final {
  /*!
   * \brief Wrapped bool value.
   */
  bool value;
};

/*!
 * \brief Serializes a Bool value.
 * \param b Bool value to serialize.
 * \return The serializable bool type.
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
 */
constexpr inline auto JBool(bool b) noexcept -> JBoolType { return {b}; }

/*!
 * \brief A Key type.
 *
 * \details -
 *
 * \vprivate Component private.
 */
class JKeyType final {
 public:
  /*!
   * \brief Constructs a Key type.
   * \param s Key to serialize.
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
  explicit constexpr JKeyType(amsr::core::StringView s) noexcept : value_(s) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> amsr::core::StringView { return this->value_; }

 private:
  /*!
   * \brief Wrapped string value.
   */
  amsr::core::StringView value_;
};

/*!
 * \brief Serializes a Key value.
 * \param s Key to serialize.
 * \return The serializable key type.
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
 */
constexpr auto JKey(amsr::core::StringView s) noexcept -> JKeyType { return JKeyType{s}; }

/*!
 * \brief Serializes a Key value.
 * \param s Key to serialize.
 * \return The serializable key type.
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
 */
constexpr auto JKey(amsr::core::String const& s) noexcept -> JKeyType { return JKey(amsr::core::StringView{s}); }

/*!
 * \brief Serializes a Key value.
 * \param s Key to serialize.
 * \return The serializable key type.
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
 */
constexpr auto JKey(std::string const& s) noexcept -> JKeyType { return JKey(amsr::core::StringView(s)); }

inline namespace literals {
/*!
 * \brief Serializes a Key value.
 * \param s String literal to serialize.
 * \param size Size of the pointer.
 * \return The serializable key type.
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
 */
constexpr auto operator""_key(char const* s, std::size_t size) noexcept -> JKeyType {
  return JKey(vac::container::CStringView::FromLiteral(s, size));
}
}  // namespace literals

/*!
 * \brief A Number type.
 * \tparam N Type of number.
 *
 * \details -
 *
 * \vprivate Component private.
 */
template <typename N, typename = typename std::enable_if<std::is_arithmetic<N>::value>::type>
class JNumberType final {
 public:
  /*!
   * \brief Constructs a Number type.
   * \param num Number to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JNumberType(N num) noexcept : value_(num) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> N { return this->value_; }

 private:
  /*!
   * \brief Wrapped number value.
   */
  N value_;
};

/*!
 * \brief A char Number type.
 *
 * \details -
 *
 * \vprivate Component private.
 */
template <>
class JNumberType<char> final {
 public:
  /*!
   * \brief Constructs a Number type.
   * \param num Number to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JNumberType(char num) noexcept : value_(static_cast<std::int32_t>(num)) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate Component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> std::int32_t { return this->value_; }

 private:
  /*!
   * \brief Wrapped number value.
   */
  std::int32_t value_;
};

/*!
 * \brief A std::uint8_t Number type.
 * \vprivate Component private.
 */
template <>
class JNumberType<std::uint8_t> final {
 public:
  /*!
   * \brief Constructs a Number type.
   * \param num Number to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JNumberType(std::uint8_t num) noexcept : value_(static_cast<std::uint32_t>(num)) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> std::uint32_t { return this->value_; }

 private:
  /*!
   * \brief Wrapped number value.
   */
  std::uint32_t value_;
};

/*!
 * \brief A std::int8_t Number type.
 *
 * \details -
 *
 * \vprivate Component private.
 */
template <>
class JNumberType<std::int8_t> final {
 public:
  /*!
   * \brief Constructs a Number type.
   * \param num Number to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JNumberType(std::int8_t num) noexcept : value_(static_cast<std::int32_t>(num)) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> std::int32_t { return this->value_; }

 private:
  /*!
   * \brief Wrapped number value.
   */
  std::int32_t value_;
};

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a Number value.
 * \tparam N Type of number.
 * \param n The number to serialize.
 * \return The serializable number type.
 *
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vpublic
 * \spec
 * requires true;
 * \endspec
 */
template <typename N, typename = typename std::enable_if<std::is_arithmetic<N>::value>::type>
constexpr auto JNumber(N n) noexcept -> JNumberType<N> {
  return JNumberType<N>{n};
}

/*!
 * \brief A String type.
 *
 * \details -
 *
 * \vprivate Component private.
 */
class JStringType final {
 public:
  /*!
   * \brief Constructs a String type.
   * \param s String to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JStringType(amsr::core::StringView s) noexcept : value_(s) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> amsr::core::StringView { return this->value_; }

 private:
  /*!
   * \brief Wrapped string value.
   */
  amsr::core::StringView value_;
};

/*!
 * \brief Serializes a String value.
 * \param s String to serialize.
 * \return The serializable string type.
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
 */
constexpr auto JString(amsr::core::StringView s) noexcept -> JStringType { return JStringType(s); }

/*!
 * \brief Serializes a String value.
 * \param s String to serialize.
 * \return The serializable string type.
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
 */
inline auto JString(amsr::core::String const& s) noexcept -> JStringType { return JString(amsr::core::StringView{s}); }

/*!
 * \brief Serializes a String value.
 * \param s String to serialize.
 * \return The serializable string type.
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
 */
inline auto JString(std::string const& s) noexcept -> JStringType { return JString(amsr::core::StringView{s}); }

/*!
 * \brief A function object used to serialize predefined serializers.
 * \tparam Container Type of container to serialize.
 *
 * \details -
 *
 * \vprivate Component private.
 */
template <typename Container>
class IdSerializer {
 public:
  /*!
   * \brief Value Type of container.
   * \vprivate Component private.
   */
  using value_type = typename Container::value_type;

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Returns the unchanged value.
   * \param v Value to return.
   * \return The unchanged value.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  template <typename Value = value_type>
  auto operator()(Value&& v) const noexcept -> Value {
    return std::forward<Value>(v);
  }
};
}  // namespace types
}  // namespace json
}  // namespace amsr
#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BASIC_TYPES_H_
