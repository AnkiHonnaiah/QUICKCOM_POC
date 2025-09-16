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
/*!        \file  utf16_to_utf8.h
 *         \brief Deserialization of UFT-16 string data.
 *         \unit  SomeIpProtocol::Serdes::Utf16ToUtf8
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_UTF16_TO_UTF8_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_UTF16_TO_UTF8_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "amsr/core/optional.h"
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {
namespace detail {

/*!
 * \brief UTF-16 to UTF-8 converter.
 * \details The Reader will provide access to UTF-16 data. That data will be converted into
 * UTF-8 characters, that are concatenated to a string.
 * \tparam Endian The configured endianness.
 */
template <typename Endian>
class Utf16ToUtf8 {
 public:
  /*!
   * \brief Deserializes the UTF-16 from the byte stream.
   * \tparam String String type.
   * \param[in,out] reader Reader holding the view on the serialized data.
   * \param[out] str application error which has to be deserialized.
   * \return true if deserialization was successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE for different reader objects.
   * \trace SPEC-4981355, SPEC-4981353
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Accepts the UTF-16 accessible range 0x0000..0x10FFFF.
   * - Return false otherwise.
   * \endinternal
   */
  template <typename String>
  static bool ReadUtf8String(Reader& reader, String& str) noexcept {
    bool result{true};
    std::size_t const input_size{reader.Size() / sizeof(std::uint16_t)};  // Reader size may be odd.
    std::size_t i{0};
    while (i < input_size) {
      GetCodePointResult const code_point_result{GetCodePoint(reader)};
      i += code_point_result.word_count;
      if (code_point_result.word_count == 0U) {
        result = false;
        break;
      } else {
        if (code_point_result.code_point == 0) {
          result = true;  // Null termination found. It should not be part of this reader.
          break;
        } else {
          result = AddCodePointToUtf8String(code_point_result.code_point, str);
        }
      }
    }
    return result;
  }

 private:
  /*!
   * \brief Return type for GetCodePoint.
   */
  struct GetCodePointResult {
    /*!
     * \brief Resulting code point.
     */
    std::uint32_t code_point;

    /*!
     * \brief Number of processed words.
     */
    std::size_t word_count;
  };

  /*!
   * \brief Add the UNICODE code point as UTF-8 to the string.
   * \tparam String String type.
   * \details A code point will result in one, two, three or four UTF-8 bytes.
   * \param[in] code_point UNICODE code point.
   * \param[in,out] str The resulting string.
   * \return True, if the code point could be converted into UTF-8.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If the code point can be encoded as single UTF-8 byte.
   *    - Add that byte to the string.
   * - If the code point has a multi byte UTF-8 representation.
   *    - Calculate number of bytes needed.
   *    - Loop over the number of bytes.
   *        - Create the UTF-8 byte.
   *        - Add that byte to the string.
   * \endinternal
   *
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  template <typename String>
  static bool AddCodePointToUtf8String(std::uint32_t const code_point, String& str) noexcept {
    constexpr std::uint32_t full_byte_mask{0xFFU};
    constexpr std::uint32_t single_character_max{0x7FU};

    bool result{false};

    if (code_point <= single_character_max) {
      result = true;
      str += static_cast<char>(code_point & full_byte_mask);  // VCA_SOMEIPPROTOCOL_BASIC_STRING_FUNC_CONTRACT
    } else {
      constexpr std::uint32_t code_point_max{0x10FFFFU};

      if (code_point <= code_point_max) {
        constexpr std::uint32_t double_character_max{0x7FFU};
        constexpr std::uint32_t triple_character_max{0xFFFFU};

        std::size_t number_of_bytes{0U};
        std::size_t first_shift{0U};
        std::size_t second_shift{0U};
        if (code_point <= double_character_max) {
          number_of_bytes = 2U;
          first_shift = 6U;
          second_shift = 1U;
        } else if (code_point <= triple_character_max) {
          number_of_bytes = 3U;
          first_shift = 5U;
          second_shift = 2U;
        } else {
          number_of_bytes = 4U;
          first_shift = 4U;
          second_shift = 3U;
        }

        result = true;
        // clang-format off
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue_CheckPerformed
        str += static_cast<char>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(0xFFU << first_shift) & full_byte_mask) |  // VCA_SOMEIPPROTOCOL_BASIC_STRING_FUNC_CONTRACT
                                 static_cast<std::uint8_t>((code_point >> (6U * second_shift)) & full_byte_mask));

        constexpr std::uint8_t consecutive_utf8_byte{0x80U};
        constexpr std::uint32_t bit_mask_6_bits{0x3FU};
        // clang-format on
        for (size_t i{1}; i < number_of_bytes; ++i) {
          // clang-format off
          // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
          // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue_CheckPerformed
          // clang-format on
          str += static_cast<char>(  // VCA_SOMEIPPROTOCOL_BASIC_STRING_FUNC_CONTRACT
              consecutive_utf8_byte |
              (bit_mask_6_bits &
               static_cast<std::uint8_t>((code_point >> (6U * (number_of_bytes - 1 - i))) & full_byte_mask)));
        }
      } else {
        result = false;
      }
    }
    return result;
  }

  /*!
   * \brief Get the UNICODE code point from the UTF-16 data.
   * \details A code point will result in one, two, three or four UTF-8 bytes.
   * \param[in,out] reader UTF-16 data.
   * \return A GetCodePointResult containing the code point and the number of UTF-16 data read in words.
   * Word count of zero indicate an error.
   * \pre The reader has to have at least 2 bytes of data.
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If the reader have two byte of data.
   *    - Read UTF-16 data.
   *    - If single word character.
   *        - Create code point.
   *    - Else if two word character.
   *        - Create code point.
   * \endinternal
   *
   */
  static GetCodePointResult GetCodePoint(Reader& reader) noexcept {
    constexpr std::uint16_t single_character_low_limit{0xD800U};
    constexpr std::uint16_t single_character_high_limit{0xDFFFU};
    constexpr std::uint16_t double_character_max{0xDC00U};

    GetCodePointResult result{};
    std::uint16_t character{};
    reader.ReadPrimitive<std::uint16_t, Endian>(character);
    result.word_count = 1U;
    if ((character < single_character_low_limit) || (character > single_character_high_limit)) {
      result.code_point = character;
    } else if (character < double_character_max) {
      constexpr std::uint32_t bit_mask_10_bits{0x3FFU};

      result.code_point = (static_cast<std::uint32_t>(character) & bit_mask_10_bits) << 10U;
      if (reader.Size() >= sizeof(std::uint16_t)) {
        constexpr std::uint16_t second_character_min{0xDC00U};
        constexpr std::uint16_t second_character_max{0xDFFFU};

        reader.ReadPrimitive<std::uint16_t, Endian>(character);
        result.word_count = 2U;
        if ((character >= second_character_min) && (character <= second_character_max)) {
          constexpr std::uint32_t second_word_offset{0x10000U};

          result.code_point = result.code_point | (static_cast<std::uint32_t>(character) & bit_mask_10_bits);
          result.code_point = static_cast<std::uint32_t>(result.code_point + second_word_offset);
        } else {
          result.word_count = 0;
        }
      } else {
        result.word_count = 0;
      }
    } else {
      result.word_count = 0;
    }

    return result;
  }

  /*! \brief Friend declaration for testing purposes */
  FRIEND_TEST(UT__Utf16ToUtf8, Deserialize_AddCodePointToUtf8String);
};

}  // namespace detail
}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_UTF16_TO_UTF8_H_
