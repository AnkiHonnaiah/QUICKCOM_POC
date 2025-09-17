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
/**       \file     bin_types.h
 *        \brief    Implementation of methods for Bin type.
 *
 *        \details  Provides serializers for arrays and tuples.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BIN_TYPES_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BIN_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>

#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief Safely convert a length.
 * \param length to convert.
 * \return The converted length.
 *
 * \spec
 * requires true;
 * \endspec
 */
inline auto ConvertLength(std::size_t const length) noexcept -> std::uint32_t {
  bool const length_valid{length <= std::numeric_limits<std::uint32_t>::max()};
  AssertCondition(length_valid, "ConvertLength: Length exceeds size of std::uint32_t.");
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_JSON_AutosarC++17_10-A4.7.1_truncating_cast
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_JSON_AutosarC++17_10-M0.3.1_OOB
  return static_cast<std::uint32_t>(length);
}
}  // namespace internal

inline namespace types {
/*!
 * \brief A binary type.
 *
 * \vprivate Component private.
 */
class JBinType final {
 public:
  /*!
   * \brief Constructs a binary type.
   * \param b Bytes to serialize.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \vprivate Component private.
   * \reentrant FALSE
   *
   * \spec
   * requires true;
   * \endspec
   */
  constexpr explicit JBinType(amsr::core::Span<char const> b) noexcept : value_(b) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> amsr::core::Span<char const> { return this->value_; }

  /*!
   * \brief Returns the length.
   * \return The length.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetLength() const noexcept -> std::uint32_t { return internal::ConvertLength(this->value_.size()); }

 private:
  /*!
   * \brief Wrapped binary value.
   */
  amsr::core::Span<char const> value_;
};

/*!
 * \brief Serializes a binary value.
 * \param b Bytes to serialize.
 * \return The serializable binary value.
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
constexpr auto JBin(amsr::core::Span<char const> b) noexcept -> JBinType { return JBinType{b}; }

/*!
 * \brief A binary string type.
 *
 * \vprivate Component private.
 */
class JBinStringType final {
 public:
  /*!
   * \brief Constructs a binary string.
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
  constexpr explicit JBinStringType(StringView s) noexcept : value_(s) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> StringView { return this->value_; }

  /*!
   * \brief Returns the length.
   * \return The length.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetLength() const noexcept -> std::uint32_t { return internal::ConvertLength(this->value_.size()); }

 private:
  /*!
   * \brief Wrapped string value.
   */
  StringView value_;
};

/*!
 * \brief Serializes a string as binary.
 * \param s The string to serialize.
 * \return The serializable string value.
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
constexpr auto JBinString(StringView s) noexcept -> JBinStringType { return JBinStringType{s}; }

/*!
 * \brief Serializes a string as binary.
 * \param s The string to serialize.
 * \return The serializable string value.
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
inline auto JBinString(amsr::core::String const& s) noexcept -> JBinStringType {
  return JBinString(amsr::core::StringView{s});
}

/*!
 * \brief Serializes a string as binary.
 * \param s The string to serialize.
 * \return The serializable string value.
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
inline auto JBinString(std::string const& s) noexcept -> JBinStringType {
  return JBinString(amsr::core::StringView{s});
}

/*!
 * \brief A binary key type.
 *
 * \vprivate Component private.
 */
class JBinKeyType final {
 public:
  /*!
   * \brief Constructs a binary key.
   * \param s Key to serialize.
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
  constexpr explicit JBinKeyType(StringView s) noexcept : value_(s) {}

  /*!
   * \brief Returns the contained value.
   * \return The value.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> StringView { return this->value_; }

  /*!
   * \brief Returns the length.
   * \return The length.
   * \vprivate Component private.
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetLength() const noexcept -> std::uint32_t { return internal::ConvertLength(this->value_.size()); }

 private:
  /*!
   * \brief Wrapped key value.
   */
  StringView value_;
};

/*!
 * \brief Serializes a key as binary.
 * \param s The key to serialize.
 * \return The serializable key value.
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
constexpr auto JBinKey(StringView s) noexcept -> JBinKeyType { return JBinKeyType{s}; }

/*!
 * \brief Serializes a key as binary.
 * \param s The key to serialize.
 * \return The serializable key value.
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
inline auto JBinKey(amsr::core::String const& s) noexcept -> JBinKeyType { return JBinKey(amsr::core::StringView{s}); }

/*!
 * \brief Serializes a key as binary.
 * \param s The key to serialize.
 * \return The serializable key value.
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
inline auto JBinKey(std::string const& s) noexcept -> JBinKeyType { return JBinKey(amsr::core::StringView{s}); }
}  // namespace types
}  // namespace json
}  // namespace amsr
#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_BIN_TYPES_H_
