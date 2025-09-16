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
/*!        \file ser_string.h
 *        \brief Defines the serialization for the value of a string.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_STRING_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ser_forward.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/utf8_to_utf16.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serialize the string(UTF-8/UTF-16) according to the given transformation properties and item configuration.
 * \tparam String type of String
 * \tparam TpPack Transformation properties parameter pack.
 * \param[in] str String to be serialized.
 * \param[in,out] w Writer holding the bufferView to which the string is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - If BOM is used
 *   - serialize BOM.
 * - Serialize the string.
 * - If null termination is used
 *   - serialize the null termination character.
 * \endinternal
 */
template <typename TpPack, typename String, std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
void serializeString(String const& str, Writer& w) noexcept {
  /*! The byte order. */
  using StringEndian = typename Tp<TpPack>::ByteOrder;

  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
  if (Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16) {
    // Append the BOM
    if (Tp<TpPack>::StringIsBomActive::value) {
      if (StringEndian::value == ByteOrder::kMostSignificantByteLast) {
        w.writeArray(kBomUTF_16_LE);
      } else if (StringEndian::value == ByteOrder::kMostSignificantByteFirst) {
        w.writeArray(kBomUTF_16_BE);
      } else {
        // unsupported endianess for string.
      }
    }

    // Write the string
    if (StringEndian::value == ByteOrder::kMostSignificantByteLast) {
      // Write the string
      detail::Utf8ToUtf16<LittleEndian>::WriteStringAsUtf16(str, w);
    } else if (StringEndian::value == ByteOrder::kMostSignificantByteFirst) {
      detail::Utf8ToUtf16<BigEndian>::WriteStringAsUtf16(str, w);
    } else {
      // unsupported endianess for string.
    }

    if (Tp<TpPack>::StringIsNullTerminationActive::value) {
      w.writeUintOfSize<sizeof(char16_t), StringEndian>(kNullValue_UTF_16);
    }
    // VCA_SOMEIPPROTOCOL_BASIC_STRING_FUNC_CONTRACT
  } else {  // UTF-8
    // Append the BOM
    if (Tp<TpPack>::StringIsBomActive::value) {
      w.writeArray(kBomUTF_8);
    }
    // Write the string
    w.writeString(str);  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE

    if (Tp<TpPack>::StringIsNullTerminationActive::value) {
      w.writeUintOfSize<sizeof(std::uint8_t), StringEndian>(kNullValue_UTF_8);
    }
  }
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_STRING_H_
