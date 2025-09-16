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
/*!        \file  utf8_to_utf16.h
 *         \brief Serialization to UFT-16 string data.
 *         \unit  SomeIpProtocol::Serdes::Utf8ToUtf16
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_UTF8_TO_UTF16_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_UTF8_TO_UTF16_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {
namespace detail {

/*!
 * \brief   UTF-8 to UTF-16 converter.
 * \details A string containing UTF-8 characters will be converted into UTF-16 data.
 * \tparam  Endianness The configured endianness.
 */
template <typename Endianness>
class Utf8ToUtf16 {
 public:
  /*!
   * \brief         Serialize string containing UTF-8 characters into UTF-16 data.
   *                That data will be written via a Writer into the serialization byte stream.
   * \tparam String String type.
   * \param[in]     str     Input string.
   * \param[in,out] writer  Writer for the serialization byte stream.
   * \pre           The writer holds a memory buffer which is enough to store the serialized data.
   * \context       Reactor|App
   * \reentrant     TRUE for different writer objects.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Get code points from the input string and call and call AddCodePointAsUtf16 for each code point.
   * - If the input is invalid log fatal message and abort.
   * \endinternal
   */
  template <typename String>
  static void WriteStringAsUtf16(String const& str, Writer& writer) noexcept {
    if (!GetCodePoint(str, [&writer](std::uint32_t const code_point) { AddCodePointAsUtf16(code_point, writer); })) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Characters with values greater than 0x10FFFF cannot be encoded in UTF-16."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
      // clang-format on
    }
  }

  /*!
   * \brief     Calculate the length of the UTF-16 output data, from a UTF-8 string.
   * \tparam String String type.
   * \param[in] str Input string.
   * \return    Required buffer size for the UTF-16 output data.
   * \pre       -
   * \context   Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Get code points from the input string and call GetCodePointUtf16Length for each code point.
   *    - Accumulate the size for each code point.
   * - If the input is invalid log fatal message and abort.
   * \endinternal
   */
  template <typename String>
  static std::size_t GetUtf16Length(String const& str) noexcept {
    std::size_t result{0U};
    if (!GetCodePoint(str,
                      [&result](std::uint32_t const code_point) { result += GetCodePointUtf16Length(code_point); })) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Characters with values greater than 0x10FFFF cannot be encoded in UTF-16."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                    ara::core::StringView{AMSR_FILE_LINE});
      // clang-format on
    }
    return result;
  }

 private:
  /*!
   * \brief Code point callback.
   */
  using CodePointFunction = vac::language::UniqueFunction<void(std::uint32_t const code_point)>;

  /*!
   * \brief     Get the UTF-16 character size in bytes for a given code point.
   * \param[in] code_point UNICODE code point.
   * \return    Required buffer size for the UTF-16 character.
   * \pre       -
   * \context   Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Code points with values less than 0x10000 are represented as a single 16-bit integer.
   * - Code points with values between 0x10000 and 0x10FFFF are represented as with two 16-bit integers.
   * - Code points with values greater than 0x10FFFF cannot be encoded in UTF-16.
   * \endinternal
   */
  static std::size_t GetCodePointUtf16Length(std::uint32_t const code_point) noexcept {
    constexpr std::uint32_t code_point_max{0x10FFFFU};
    constexpr std::uint32_t second_word_offset{0x10000U};

    std::size_t result{0U};
    if (code_point < second_word_offset) {
      result = sizeof(std::uint16_t);
    }
    if ((code_point >= second_word_offset) && (code_point <= code_point_max)) {
      result = 2 * sizeof(std::uint16_t);
    }

    return result;
  }

  /*!
   * \brief         Write the UTF-16 character into the serialization byte stream.
   * \param[in]     code_point  UNICODE code point.
   * \param[in,out] writer      Writer for the serialization byte stream.
   * \pre           The writer holds a memory buffer which is enough to store the serialized data.
   * \context       Reactor|App
   * \reentrant     TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Code points with values less than 0x10000 are represented as a single 16-bit integer.
   * - Code points with values between 0x10000 and 0x10FFFF are represented as with two 16-bit integers.
   * - Code points with values greater than 0x10FFFF cannot be encoded in UTF-16.
   * \endinternal
   */
  static void AddCodePointAsUtf16(std::uint32_t const code_point, Writer& writer) noexcept {
    constexpr std::uint32_t code_point_max{0x10FFFFU};
    constexpr std::uint32_t second_word_offset{0x10000U};

    if (code_point < second_word_offset) {
      std::uint16_t const character{static_cast<std::uint16_t>(code_point)};
      writer.writePrimitive<std::uint16_t, Endianness>(character);
    }
    if ((code_point >= second_word_offset) && (code_point <= code_point_max)) {
      constexpr std::uint16_t bit_mask_10_bits{0x3FFU};
      constexpr std::uint16_t first_character_bit_mask{0xD800U};
      constexpr std::uint16_t second_character_bit_mask{0xDC00U};

      std::uint32_t const remaining{code_point - second_word_offset};
      std::uint16_t const character1{static_cast<std::uint16_t>(
          first_character_bit_mask | static_cast<std::uint16_t>((remaining >> 10U) & bit_mask_10_bits))};
      writer.writePrimitive<std::uint16_t, Endianness>(character1);
      std::uint16_t const character2{static_cast<std::uint16_t>(
          second_character_bit_mask | static_cast<std::uint16_t>(remaining & bit_mask_10_bits))};
      writer.writePrimitive<std::uint16_t, Endianness>(character2);
    }
  }

  /*!
   * \brief     Get UNICODE code point from UTF-8 string.
   * \tparam String String type.
   * \param[in] str   UTF-8 string input.
   * \param[in] func  Functions called for each found code point.
   * \return    True, if valid code points were found.
   * \pre       -
   * \context   Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - For each UTF-8 character in the input string, not including the null termination.
   *    - If less than 0x7F, then use the one byte encoding as code point.
   *    - If the first two bits are set (11xxxxxx), then this is a 2, 3 or 4 byte encoding.
   *        - Loop over the expected UTF-8 bytes.
   *            - Accumulate the code point value.
   *    Else, break on invalid UTF-8 encoding.
   *
   * \endinternal
   */
  template <typename String>
  static bool GetCodePoint(String const& str, CodePointFunction func) noexcept {
    constexpr std::uint32_t full_byte_mask{0xFFU};
    constexpr std::uint32_t bit_mask_6_bits{0x3FU};
    constexpr std::uint8_t first_utf8_byte_mask{0xC0U};
    constexpr std::uint8_t valid_first_utf8_byte{0xC0U};
    constexpr std::uint8_t consecutive_utf8_byte_mask{0xC0U};
    constexpr std::uint8_t valid_consecutive_utf8_byte{0x80U};

    bool result{false};
    if (str.empty()) {
      result = true;
    }
    typename String::const_iterator ii{str.cbegin()};
    while ((func != nullptr) && (ii != str.cend())) {
      std::uint8_t character{static_cast<std::uint8_t>(*ii)};
      if (character <= 0x7FU) {
        std::uint32_t const code_point{character};
        func(code_point);  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
        result = true;
      } else if ((character & first_utf8_byte_mask) == valid_first_utf8_byte) {  // At least 2 bytes
        std::size_t const number_of_bytes{GetNumberOfBytes(character)};
        result = true;  // Expect success
        std::uint32_t code_point{character};
        code_point = code_point & (full_byte_mask >> (number_of_bytes + 1U));

        for (size_t i{1}; i < number_of_bytes; ++i) {
          static_cast<void>(i);
          ii++;
          if (ii == str.cend()) {
            result = false;
            break;
          } else {
            character = static_cast<std::uint8_t>(*ii);
            if ((character & consecutive_utf8_byte_mask) != valid_consecutive_utf8_byte) {
              result = false;
              break;
            } else {
              code_point = static_cast<std::uint32_t>((code_point << 6) |
                                                      (static_cast<std::uint32_t>(character) & bit_mask_6_bits));
            }
          }
        }
        if (result) {
          func(code_point);  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
        } else {
          break;
        }
      } else {
        break;
      }
      ii++;
    }

    return result;
  }

  /*!
   * \brief     Get number of UTF-8 bytes.
   * \param[in] character UTF-8 string input.
   * \return    Number of UTF-8 bytes.
   * \pre       Check that this is a multibyte encoding. ((character & 0xC0U) == 0xC0U)
   * \context   Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If masked value is 0xF0 there are 4 bytes.
   * - If masked value is 0xE0 there are 3 bytes.
   * - Otherwise it has to be 2 bytes.
   *
   * \endinternal
   */
  static std::size_t GetNumberOfBytes(std::uint8_t character) noexcept {
    constexpr std::uint8_t first_utf8_byte_count_mask{0xF0U};
    constexpr std::uint8_t utf8_byte_count_3{0xE0U};
    constexpr std::uint8_t utf8_byte_count_4{0xF0U};

    std::size_t result{2U};
    if ((character & first_utf8_byte_count_mask) == utf8_byte_count_4) {
      result = 4U;
    }
    if ((character & first_utf8_byte_count_mask) == utf8_byte_count_3) {
      result = 3U;
    }
    return result;
  }

  /*! \brief Friend declarations for testing purposes */
  FRIEND_TEST(UT__Utf8ToUtf16, Serialize_InvalidFunction);
  FRIEND_TEST(UT__Utf8ToUtf16, Serialize_InvalidGetCodePointUtf16Length);
  FRIEND_TEST(UT__Utf8ToUtf16, Serialize_InvalidAddCodePointAsUtf16);
};

}  // namespace detail
}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_UTF8_TO_UTF16_H_
