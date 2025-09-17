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
/**       \file     structure_parser_impl.h
 *        \brief    The implementation of a SAX-Style JSON parser.
 *
 *        \details  Parses JSON text from a stream and sends events synchronously to an Implementer.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_IMPL_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/internal/parsers/structure_parser.h"

#include <algorithm>
#include <cstring>
#include <utility>

#include "amsr/endianness/endianness.h"
#include "amsr/json/reader/internal/depth_counter.h"
#include "amsr/json/reader/internal/json_ops.h"
#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/number.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
namespace internal {
// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
template <typename Implementer>
StructureParser<Implementer>::StructureParser(Implementer& implementer, JsonData& doc) noexcept
    // VCA_VAJSON_INTERNAL_CALL
    : implementer_{implementer}, json_ops_(doc) {}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the document is not empty:
 *   - As long as the current state is kRunning, parse the next value.
 * - Return the Result of the parsing or an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::Parse() noexcept -> Result<void> {
  // Skip all whitespace to so that we will detect empty documents immediately.
  // VCA_VAJSON_INTERNAL_CALL
  ParserResult result{MakeResult(this->GetJsonOps().SkipWhitespace(), JsonErrc::kInvalidJson,
                                 "StructureParser::Parse: Document is empty.")
                          .Map([]() { return ParserState::kRunning; })};

  // Run parser
  while (result == ParserState::kRunning) {
    result = this->ParseValue();
  }

  // In case of an error, add the current location to the support data
  return result
      .MapError([this](ErrorCode ec) {
        ec.SetSupportData(
            // VCA_VAJSON_INTERNAL_CALL
            static_cast<ErrorDomain::SupportDataType::CodeType>(this->json_ops_.Tell().ValueOr(0)));
        return ec;
      })
      .Drop();
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Parse the file.
 * - If no error occurred:
 *   - Return kRunning.
 * - Otherwise:
 *   - Return the error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::SubParse() noexcept -> ParserResult {
  // VCA_VAJSON_THIS_DEREF
  return this->implementer_->GetChild().Parse().Map([]() { return ParserState::kRunning; });
}

template <typename Implementer>
auto StructureParser<Implementer>::GetJsonDocument() noexcept -> JsonData& {
  // VCA_VAJSON_INTERNAL_CALL
  return this->json_ops_.GetJsonDocument();
}

template <typename Implementer>
auto StructureParser<Implementer>::GetJsonDocument() const noexcept -> JsonData const& {
  // VCA_VAJSON_INTERNAL_CALL
  return this->json_ops_.GetJsonDocument();
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the parsed string is "null" and a value is expected:
 *   - Call the OnNull callback and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseNull() noexcept -> ParserResult {
  // VCA_VAJSON_INTERNAL_CALL
  return (this->GetJsonOps().CheckString("ull"_sv, "StructureParser::ParseNull: Expected 'null'"_sv))
      .AndThen([this]() { return this->GetState().AddValue(); })
      .AndThen([this]() {
        // VCA_VAJSON_LAMBDA_CAPTURE
        return ParserResult{this->implementer_->GetChild().OnNull()};
      });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the parsed string is "true" and a value is expected:
 *   - Call the OnBool callback with the value "true".
 *   - Return the Result of the callback.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseTrue() noexcept -> ParserResult {
  // VCA_VAJSON_INTERNAL_CALL
  return (this->GetJsonOps().CheckString("rue"_sv, "StructureParser::ParseTrue: Expected 'true'"_sv))
      .AndThen([this]() { return this->GetState().AddValue(); })
      .AndThen([this]() {
        // VCA_VAJSON_LAMBDA_CAPTURE
        return ParserResult{this->implementer_->GetChild().OnBool(true)};
      });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the parsed string is "false" and a value is expected:
 *   - Call the OnBool callback with the value "false" and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseFalse() noexcept -> ParserResult {
  // VCA_VAJSON_INTERNAL_CALL
  return (this->GetJsonOps().CheckString("alse"_sv, "StructureParser::ParseFalse: Expected 'false'"_sv))
      .AndThen([this]() { return this->GetState().AddValue(); })
      .AndThen([this]() {
        // VCA_VAJSON_LAMBDA_CAPTURE
        return ParserResult{this->implementer_->GetChild().OnBool(false)};
      });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If a value is expected:
 *   - Parse the number string.
 *   - Create a new JsonNumber from the string.
 *   - If the JsonNumber could be successfully created:
 *     - Call the OnNumber callback with the JsonNumber and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseNumber(char const cur) noexcept -> ParserResult {
  return (this->GetState().AddValue())
      .AndThen([this, cur]() { return JsonNumber::New(this->GetNumber(cur)); })
      .AndThen([this](JsonNumber num) {
        // VCA_VAJSON_LAMBDA_CAPTURE
        return ParserResult{this->implementer_->GetChild().OnNumber(num)};
      });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Convert a JSON string to a C++ string.
 * - Parse this unescaped string.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseString() noexcept -> ParserResult {
  return ReadJsonString().AndThen([this](CStringView string) { return ParseUnescapedString(string); });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Skip all whitespace characters.
 * - If the string is followed by a colon and a key is expected:
 *   - Store the string as the current key.
 *   - Call the OnKey callback with the key.
 * - If the string is not followed by a colon a value is expected:
 *   - Call the OnString callback with the string.
 * - Return the Result of the callback.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseUnescapedString(CStringView string) noexcept -> ParserResult {
  // VCA_VAJSON_INTERNAL_CALL
  static_cast<void>(this->GetJsonOps().SkipWhitespace());
  ParserResult result{ParserState::kRunning};

  // VCA_VAJSON_INTERNAL_CALL
  if (this->GetJsonOps().Skip(':')) {
    result = this->GetState().AddKey().AndThen([this, &string]() {
      amsr::core::StringView const view{string};
      this->GetJsonDocument().StoreCurrentKey(view);
      // VCA_VAJSON_LAMBDA_CAPTURE
      return ParserResult{this->implementer_->GetChild().OnKey(string)};
    });
  } else {
    result = this->GetState().AddValue().AndThen([this, &string]() {
      // VCA_VAJSON_LAMBDA_CAPTURE
      return ParserResult{this->implementer_->GetChild().OnString(string)};
    });
  }

  return result;
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Read the string until either an escape sequence or a string termination occurs.
 * - While all operations are successful & an escape sequence is encountered
 *  - Unescape the character sequence
 *  - And read the string until either an escape sequence or a string termination occurs.
 * - Take the string termination character
 * - Return the result.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ReadJsonString() noexcept -> Result<CStringView> {
  String& buffer{this->GetJsonDocument().GetClearedStringBuffer()};

  auto const reader = [this, &buffer]() {
    // VCA_VAJSON_INTERNAL_CALL
    return this->GetJsonOps().ReadUntil(R"("\)"_sv,
                                        // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
                                        [&buffer](StringView buf) {
                                          // VCA_VAJSON_LAMBDA_CAPTURE
                                          buffer.append(buf.begin(), buf.end());
                                        });
  };

  Result<OptChar> ret_val{reader()};
  while (ret_val.HasValue() && *ret_val == '\\') {
    // We encountered an escape.

    // Take the backslash itself
    // VCA_VAJSON_INTERNAL_CALL
    static_cast<void>(this->GetJsonOps().Move());

    // Unescape the escaped character
    ret_val = this->GetJsonOps()
                  // VCA_VAJSON_INTERNAL_CALL
                  .TryTake()
                  .AndThen([this](char const escaped) { return this->UnescapeChar(escaped); })
                  .AndThen([&reader, &buffer](char const unescaped) {
                    // VCA_VAJSON_WITHIN_SPEC
                    buffer.push_back(unescaped);
                    // And retry
                    // VCA_VAJSON_LAMBDA_CAPTURE
                    return reader();
                  });
  }

  return ret_val
      .Filter([](OptChar const opt) { return opt.HasValue(); },
              Err(JsonErrc::kInvalidJson, "ReadJsonString: Runaway string."))
      .Drop()
      // Take the " character
      // VCA_VAJSON_INTERNAL_CALL
      .Inspect([this]() { static_cast<void>(this->GetJsonOps().Move()); })
      .Map([this]() { return this->GetJsonDocument().GetCurrentString(); });
}

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Map all characters to the appropriate escape sequence.
 * - If a unicode escape is encountered return an error.
 * - If an unknown/invalid escape is encountered return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::UnescapeChar(char const escaped) noexcept -> Result<char> {
  Result<char> ret_val{escaped};
  switch (escaped) {
    case 'b':
      ret_val = '\b';
      break;
    case 'f':
      ret_val = '\f';
      break;
    case 'n':
      ret_val = '\n';
      break;
    case 'r':
      ret_val = '\r';
      break;
    case 't':
      ret_val = '\t';
      break;
    case '\\':
    case '/':
    case '"':
      break;
    case 'u':
      // VCA_VAJSON_EXTERNAL_CALL
      ret_val.EmplaceError(JsonErrc::kInvalidJson, "Unicode escape: \\u notation is not supported!");
      break;
    default:
      // VCA_VAJSON_EXTERNAL_CALL
      ret_val.EmplaceError(JsonErrc::kInvalidJson, "Unknown escape sequence!");
      break;
  }
  return ret_val;
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If an object can be added to the stack:
 *   - Call the OnStartObject callback and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseStartObject() noexcept -> ParserResult {
  return this->GetState().AddObject().AndThen([this]() {
    // VCA_VAJSON_LAMBDA_CAPTURE
    return ParserResult{this->implementer_->GetChild().OnStartObject()};
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Move the position in the file past the closing brace.
 * - Pop the Object from the ItemStack.
 * - If the pop object call was successful:
 *   - Call the OnStartObject callback with the number of parsed keys and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseEndObject() noexcept -> ParserResult {
  return this->GetState().PopObject().AndThen([this](std::size_t count) {
    // VCA_VAJSON_THIS_DEREF
    return ParserResult{this->implementer_->GetChild().OnEndObject(count)};
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If an array can be added to the stack:
 *   - Call the OnStartArray callback and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseStartArray() noexcept -> ParserResult {
  return this->GetState().AddArray().AndThen(
      // VCA_VAJSON_THIS_DEREF
      [this]() { return ParserResult{this->implementer_->GetChild().OnStartArray()}; });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Move the position in the file past the closing bracket.
 * - Pop the Array from the ItemStack.
 * - If the pop array call was successful:
 *   - Call the OnEndArray callback with the number of parsed elements and return its Result.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseEndArray() noexcept -> ParserResult {
  return this->GetState().PopArray().AndThen([this](std::size_t count) {
    // VCA_VAJSON_THIS_DEREF
    return ParserResult{this->implementer_->GetChild().OnEndArray(count)};
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - If the comma at this position is valid:
 *   - Return kRunning to continue parsing.
 * - Otherwise:
 *   - Return the error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseComma() noexcept -> ParserResult {
  return MakeResult(this->GetState().AddComma(), JsonErrc::kInvalidJson,
                    "StructureParser::ParseComma: Unexpected comma.")
      .Map([]() { return ParserState::kRunning; });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Read the next four bytes.
 * - If necessary, convert the value from network to platform endianness.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseLength() noexcept -> Result<std::uint32_t> {
  constexpr std::uint8_t kPrefixSize{4};
  std::uint32_t result{0};

  return this
      ->GetJsonOps()
      // VCA_VAJSON_INTERNAL_CALL
      .ReadExactly(kPrefixSize,
                   [&result](StringView view) {
                     // VCA_VAJSON_LAMBDA_CAPTURE
                     std::memcpy(&result, view.data(), sizeof(result));
                     // VCA_VAJSON_LAMBDA_CAPTURE
                     result = amsr::endianness::NetworkToHost<std::uint32_t>(result);
                   })
      .Map([&result]() { return result; });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Parse the length of the binary content.
 * - Read the binary content.
 * - Execute the callback and return its result.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ReadBinary(vac::language::FunctionRef<ParserResult(StringView)> callback) noexcept
    -> ParserResult {
  return this->ParseLength().AndThen([this, &callback](std::uint32_t length) {
    // Will always be overwritten.
    // VCA_VAJSON_EXTERNAL_CALL
    ParserResult result{JsonErrc::kInvalidJson};

    return this
        ->GetJsonOps()
        // VCA_VAJSON_INTERNAL_CALL
        .ReadExactly(length,
                     // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
                     [&callback, &result](StringView view) {
                       // VCA_VAJSON_LAMBDA_CAPTURE
                       result = callback(view);
                     })
        .And(result);
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Add a key to the stack.
 * - If the key could be added:
 *   - Read the binary content.
 *   - Store the key.
 *   - Pass the stored key to the OnBinaryKey callback.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseBinaryKey() noexcept -> ParserResult {
  return this->GetState().AddKey().AndThen([this]() {
    // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
    return this->ReadBinary([this](StringView buf) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      this->GetJsonDocument().StoreCurrentKey(buf);
      StringView const view{this->GetJsonDocument().GetCurrentKey()};
      // VCA_VAJSON_LAMBDA_CAPTURE
      return ParserResult{this->implementer_->GetChild().OnBinaryKey(view)};
    });
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Add a value to the stack.
 * - If the value could be added:
 *   - Read the binary content.
 *   - Pass it to the OnBinaryString callback.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseBinaryString() noexcept -> ParserResult {
  return this->GetState().AddValue().AndThen([this]() {
    return this->ReadBinary(
        // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
        [this](StringView buf) {
          // VCA_VAJSON_LAMBDA_CAPTURE
          return ParserResult{this->implementer_->GetChild().OnBinaryString(buf)};
        });
  });
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Add a value to the stack.
 * - If the value could be added:
 *   - Read the binary content.
 *   - Pass it to the OnBinary callback.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseBinaryValue() noexcept -> ParserResult {
  return this->GetState().AddValue().AndThen([this]() {
    // VECTOR NL AutosarC++17_10-A5.1.8: MD_JSON_nested_lambda_constructs
    return this->ReadBinary([this](StringView buf) {
      amsr::core::Span<char const> const view{buf.data(), buf.size()};
      // VCA_VAJSON_LAMBDA_CAPTURE
      return ParserResult{this->implementer_->GetChild().OnBinary(view)};
    });
  });
}

// VECTOR NC Metric-HIS.VG: MD_JSON_Metric-HIS.VG_json_value
/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Skip all whitespace characters.
 * - If the stream has ended and the ItemStack is empty:
 *   - Return kFinished.
 * - If there are still new elements in the stream:
 *   - Parse the next character.
 *   - Determine the expected type to follow based on the parsed character.
 *   - Call the specialized parser method for this type.
 *   - Return the Result of the call.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::ParseValue() noexcept -> ParserResult {
  ParserResult result{ParserState::kRunning};

  // VCA_VAJSON_INTERNAL_CALL
  if (not this->GetJsonOps().SkipWhitespace()) {
    result = this->GetState().CheckEndOfFile();
  }

  if (result == ParserState::kRunning) {
    static_cast<void>(this->GetJsonDocument().GetClearedStringBuffer());
    // VCA_VAJSON_INTERNAL_CALL
    char const cur{this->GetJsonOps().Take()};
    switch (cur) {
      case 'n':
        result = this->ParseNull();
        break;
      case 't':
        result = this->ParseTrue();
        break;
      case 'f':
        result = this->ParseFalse();
        break;
      case '"':
        result = this->ParseString();
        break;
      case '{':
        result = this->ParseStartObject();
        break;
      case '}':
        result = this->ParseEndObject();
        break;
      case '[':
        result = this->ParseStartArray();
        break;
      case ']':
        result = this->ParseEndArray();
        break;
      case ',':
        result = this->ParseComma();
        break;
      case 'b':
        result = this->ParseBinaryValue();
        break;
      case 'k':
        result = this->ParseBinaryKey();
        break;
      case 's':
        result = this->ParseBinaryString();
        break;
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        result = this->ParseNumber(cur);
        break;
      default:
        // VCA_VAJSON_EXTERNAL_CALL
        result.EmplaceError(JsonErrc::kInvalidJson, "ParseValue: Got unknown JSON token.");
        break;
    }
  }
  return result;
}

/*!
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Parse all characters and add them to the number string until a comma, a closing brace/bracket, or a whitespace
 *   character is encountered.
 * \endinternal
 */
template <typename Implementer>
auto StructureParser<Implementer>::GetNumber(char const first) noexcept -> CStringView {
  constexpr static CStringView kLimiterChars{",}] \n\r\t"_sv};
  String& buffer{this->GetJsonDocument().GetStringBuffer()};
  // VCA_VAJSON_WITHIN_SPEC
  buffer.push_back(first);
  static_cast<void>(this->GetJsonOps()
                        // VCA_VAJSON_INTERNAL_CALL
                        .ReadUntil(kLimiterChars,
                                   [&buffer](StringView ch) {
                                     // VCA_VAJSON_LAMBDA_CAPTURE
                                     buffer.append(ch.begin(), ch.end());
                                   })
                        .Value());

  // Create a string view from the number-string
  return this->GetJsonDocument().GetCurrentString();
}

template <typename Implementer>
auto StructureParser<Implementer>::GetState() noexcept -> DepthCounter& {
  return this->GetJsonDocument().GetState();
}

template <typename Implementer>
auto StructureParser<Implementer>::GetJsonOps() noexcept -> JsonOps& {
  return this->json_ops_;
}

}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_IMPL_H_
