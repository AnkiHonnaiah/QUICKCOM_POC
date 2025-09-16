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
/**        \file deser_string.h
 *        \brief Contains static functions to deserialize strings.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_STRING_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/array.h"
#include "deser_forward.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip-protocol/internal/deserialization/utf16_to_utf8.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {
namespace detail {

/*!
 * \brief Verify UTF-8 byte order mark (BOM).
 * \param[in,out] r Reader holding the bufferView on the serialized data.
 * \return true if the BOM is correct, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 *   - Verify the byte stream contains at least the number of required bytes to represent the BOM value.
 *   - Verify the received BOM value is correct.
 * \endinternal
 */
inline Result VerifyUTF8Bom(Reader& r) noexcept {
  Result result{false};
  // Check if the buffer at least contains the minimum string length
  result = r.VerifySize(kBomUTF_8.size());

  if (result) {
    ara::core::Array<std::uint8_t, 3U> bom{};
    r.ReadArray(bom);
    result = bom == kBomUTF_8;
  }

  return result;
}

/*!
 * \brief Verify UTF-16 Byte order mark (BOM).
 * \param[in,out] r Reader holding the bufferView on the serialized data.
 * \param[out] isLittleEndian endianess from BOM.
 * \return true if the BOM is correct, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 *   - Verify the byte stream contains at least the number of required bytes to represent the BOM value.
 *   - Verify the received BOM value is correct.
 * \endinternal
 */
// VECTOR NL AutosarC++17_10-A8.4.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.4.4_UsingReturnValue
inline Result VerifyUTF16Bom(Reader& r, bool& isLittleEndian) noexcept {
  Result result{false};
  constexpr std::size_t BomSize{2U};

  // Check if the buffer at least contains the minimum string length
  result = r.VerifySize(BomSize);

  if (result) {
    ara::core::Array<std::uint8_t, 2U> bom{};
    r.ReadArray(bom);

    isLittleEndian = false;
    result = (kBomUTF_16_BE == bom) || (kBomUTF_16_LE == bom);
    isLittleEndian = result && (kBomUTF_16_LE == bom);
  }

  return result;
}

/*!
 * \brief Verify null-termination for UTF-8/UTF-16 encoded strings.
 * \tparam StringEncoding The string encoding type.
 * \param[in,out] r Reader holding the bufferView on the serialized data.
 * \return true if the null-termination is correct, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 *   - Verify the byte stream still contains a byte to represent the null-termination character.
 *   - Verify the null-termination character is correct.
 * \endinternal
 */
template <EStringEncoding StringEncoding>
inline Result VerifyNullTermination(Reader& r) noexcept {
  Result result{false};
  std::uint8_t length_string_null_termination{1U};  // Default UTF-8 encoding.

  if (StringEncoding == EStringEncoding::kUTF_16) {
    length_string_null_termination = 2U;
  }

  result = r.VerifySize(length_string_null_termination);

  if (result) {
    if (StringEncoding == EStringEncoding::kUTF_16) {
      typename UintRead<sizeof(char16_t), LittleEndian>::type null_character{};
      r.ReadUintOfSize<sizeof(char16_t), LittleEndian>(null_character);
      result = null_character == kNullValue_UTF_16;
    } else {
      typename UintRead<sizeof(std::uint8_t), LittleEndian>::type null_character{};
      r.ReadUintOfSize<sizeof(std::uint8_t), LittleEndian>(null_character);
      result = null_character == kNullValue_UTF_8;
    }
  }

  return result;
}

}  // namespace detail

// VECTOR NC Metric-HIS.VG: MD_SOMEIPPROTOCOL_Metric-HIS.VG_MultipleChoicesStatement
/*!
 * \brief Deserializes the string(UTF-8/UTF-16) according to the given transformation properties and item configuration.
 * \tparam TpPack Transformation properties parameter pack
 * \tparam String type of String
 * \param[out] str String, the deserialized value shall be stored into.
 * \param[in,out] r Reader holding the bufferView on the serialized data.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace SPEC-4981369
 *
 * \internal
 * - Verify the BOM.
 * - Read string value.
 * - Verify null-termination.
 * \endinternal
 */
template <typename TpPack, typename String, std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
static Result DeserializeString(String& str, Reader& r) noexcept {
  Result result{true};
  bool isUtf16LE{false};
  constexpr bool utf16Encoding{Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16};

  // Check if BOM is active
  if (Tp<TpPack>::StringIsBomActive::value) {
    result = utf16Encoding ? detail::VerifyUTF16Bom(r, isUtf16LE) : detail::VerifyUTF8Bom(r);
  }

  if (result) {
    if (Tp<TpPack>::StringIsNullTerminationActive::value) {
      constexpr std::size_t length_null_termination{
          (Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16) ? std::size_t(2U) : std::size_t(1U)};

      std::size_t const length_without_null_termination{r.Size() - length_null_termination};

      if (utf16Encoding) {
        Reader reader_without_null_termination{r.ConsumeSubStream(length_without_null_termination)};
        if (isUtf16LE) {
          result = detail::Utf16ToUtf8<LittleEndian>::ReadUtf8String(reader_without_null_termination, str);
        } else {
          result = detail::Utf16ToUtf8<BigEndian>::ReadUtf8String(reader_without_null_termination, str);
        }
      } else {
        result = r.VerifySize(length_without_null_termination);
        if (result) {
          r.ReadString(str, length_without_null_termination);
        }
      }

      result = detail::VerifyNullTermination<Tp<TpPack>::StringEncoding::value>(r);
    } else {  // String Null Termination is not active
      if (utf16Encoding) {
        if (isUtf16LE) {
          result = detail::Utf16ToUtf8<LittleEndian>::ReadUtf8String(r, str);
        } else {
          result = detail::Utf16ToUtf8<BigEndian>::ReadUtf8String(r, str);
        }
      } else {
        r.ReadString(str, r.Size());
      }
    }
  }

  return result;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_STRING_H_
