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
/**       \file
 *        \brief    Serializer for length tags.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_LENGTH_SERIALIZER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_LENGTH_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/endianness/endianness.h"

#include "amsr/json/util/types.h"
#include "amsr/json/writer/serializers/structures/serializer.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief Serializes a length value as big endian.
 * \param os Stream to write to.
 * \param length to serialize.
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
 * - Convert the value to network byte order / big endian if necessary.
 * - Write the value to the stream.
 * \endinternal
 */
inline void SerializeLength(WriterType os, std::uint32_t length) noexcept {
  constexpr std::size_t kPrefixSize{4};
  std::uint32_t const be_length{amsr::endianness::HostToNetwork<std::uint32_t>(length)};
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_JSON_AutosarC++17_10-A5.2.4_reinterpret_cast
  StringView const view{reinterpret_cast<char const*>(&be_length), kPrefixSize};
  // VCA_VAJSON_OUTPUTSTREAM
  os->Write(view);
}
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_UTIL_LENGTH_SERIALIZER_H_
