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
/**       \file     escaped_json_string.h
 *        \brief    Serializer for JSON string literals.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_ESCAPED_JSON_STRING_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_ESCAPED_JSON_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>

#include "amsr/core/string_view.h"

#include "amsr/iostream/output_stream.h"
#include "amsr/json/util/types.h"
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/types/basic_types.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief An escaped JSON string type.
 * \vprivate component private
 */
class EscapedJsonString {
 public:
  /*!
   * \brief Constructs an EscapedJsonString from a JSON key.
   * \param key to serialize.
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
  explicit EscapedJsonString(JKeyType key) noexcept : EscapedJsonString(key.GetValue()) {}

  /*!
   * \brief Constructs an EscapedJsonString from a JSON string.
   * \param string to serialize.
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
  explicit EscapedJsonString(JStringType string) noexcept : EscapedJsonString(string.GetValue()) {}

  /*!
   * \brief Returns the contained string.
   * \return The contained string.
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto GetValue() const noexcept -> amsr::core::StringView { return this->value_; }

 private:
  /*!
   * \brief Constructs an EscapedJsonString from a StringView.
   * \param value The value to serialize.
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
  explicit EscapedJsonString(amsr::core::StringView value) noexcept : value_{value} {}

  /*!
   * \brief Value to write as a JSON string literal.
   */
  amsr::core::StringView value_;
};

// VECTOR NC AutosarC++17_10-M5.0.16, Metric-HIS.VG: MD_JSON_AutosarC++17_10-M5.0.16_pointer_arithmetic, MD_JSON_Metric-HIS.VG_json_value
/*!
 * \brief Serializes an escaped string literal type.
 * \param[out] os Output stream to write into.
 * \param string to escape and serialize.
 * \return A reference to the output stream.
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
 *
 * \internal
 * - If the string contains a character that needs to be escaped in JSON:
 *   - Serialize the escaped character.
 * - Otherwise:
 *   - Serialize the character directly.
 * \endinternal
 */
auto inline operator<<(amsr::stream::StatefulOutputStream& os, EscapedJsonString string) noexcept
    -> amsr::stream::StatefulOutputStream& {
  for (char const ch : string.GetValue()) {
    switch (ch) {
      case '"': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\")"_sv);
        break;
      }
      case '\\': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\\)"_sv);
        break;
      }
      case '/': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\/)"_sv);
        break;
      }
      case '\b': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\b)"_sv);
        break;
      }
      case '\f': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\f)"_sv);
        break;
      }
      case '\n': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\n)"_sv);
        break;
      }
      case '\r': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\r)"_sv);
        break;
      }
      case '\t': {
        // VCA_VAJSON_OUTPUTSTREAM
        os.Write(R"(\t)"_sv);
        break;
      }
      default:
        // VCA_VAJSON_OUTPUTSTREAM
        os.Put(ch);
        break;
    }
  }

  return os;
}
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_ESCAPED_JSON_STRING_H_
