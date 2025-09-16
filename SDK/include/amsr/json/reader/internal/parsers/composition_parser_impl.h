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
/**     \file       composition_parser_impl.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_IMPL_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/internal/parsers/composition_parser.h"

#include "amsr/json/reader/internal/parsers/array_parser.h"
#include "amsr/json/reader/internal/parsers/binary_parser.h"
#include "amsr/json/reader/internal/parsers/bool_parser.h"
#include "amsr/json/reader/internal/parsers/key_parser.h"
#include "amsr/json/reader/internal/parsers/number_parser.h"
#include "amsr/json/reader/internal/parsers/object_parser.h"
#include "amsr/json/reader/internal/parsers/string_parser.h"
#include "amsr/json/reader/json_data.h"

namespace amsr {
namespace json {
namespace internal {

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::Key(Fn fn) noexcept -> CallableReturnsResult<Fn, amsr::core::StringView, R> {
  static_assert(ReturnsResultVoid<Fn, amsr::core::StringView>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return KeyParser(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::Bool(Fn fn) noexcept -> CallableReturnsResult<Fn, bool, R> {
  static_assert(ReturnsResultVoid<Fn, bool>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return BoolParser(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename T, typename Fn>
auto CompositionParser<Mixin>::Number(Fn fn) noexcept -> CallableReturnsResult<Fn, T, R> {
  static_assert(ReturnsResultVoid<Fn, T>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return NumberParser<T>(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::String(Fn fn) noexcept -> CallableReturnsResult<Fn, amsr::core::StringView, R> {
  static_assert(ReturnsResultVoid<Fn, amsr::core::StringView>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return StringParser(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::Binary(Fn fn) noexcept -> CallableReturnsResult<Fn, Bytes, R> {
  static_assert(ReturnsResultVoid<Fn, Bytes>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return BinaryParser(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::Array(Fn fn) noexcept -> CallableReturnsResult<Fn, std::size_t, R> {
  static_assert(ReturnsResultVoid<Fn, std::size_t>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return ArrayParser(this->doc_, std::forward<Fn>(fn)).SubParse();
}

template <typename Mixin>
template <typename Fn>
auto CompositionParser<Mixin>::Object(Fn fn, bool object_already_open) noexcept
    -> CallableReturnsResult<Fn, amsr::core::StringView, R> {
  static_assert(ReturnsResultVoid<Fn, amsr::core::StringView>::value, "Must return Result<void>");
  // VCA_VAJSON_INTERNAL_CALL
  return ObjectParser(doc_, std::forward<Fn>(fn), object_already_open).SubParse();
}

}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_COMPOSITION_PARSER_IMPL_H_
