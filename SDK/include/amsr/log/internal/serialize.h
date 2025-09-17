/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       serialize.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::Serializer
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SERIALIZE_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SERIALIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/message/ipc_message_type.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace log {
namespace internal {
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;

/*!
 * \brief Helper for the serializers
 *
 * \tparam Fn    The function to evaluate
 *               A function that takes no arguments.
 * \param buffer The buffer to write into
 * \param msg    The message to output in case of errors
 * \param length The length to write
 * \param fn     The actual serialization
 * \return       The serializer result
 */
template <typename Fn>
auto SerializeInternal(ByteView buffer, vac::container::CStringView msg, std::size_t length, Fn const& fn) noexcept
    -> SerResult {
  SerResult result{Err(LogErrc::kBufferTooSmall, msg.c_str())};
  if (buffer.size() >= length) {
    fn();

    result.EmplaceValue(buffer.subspan(length));
  }
  return result;
}

namespace native {
/*!
 * \brief Serialize a type with native endianness.
 *
 * \details buffer.data() can not be nullptr due to the requirement of buffer.size() in SerializeInternal.
 *
 * \tparam T     The type to serialize.
 *               Must be an unsigned integer.
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 *
 * \trace DSGN-LogAndTrace-Ipc
 */
template <typename T>
inline auto Serialize(ByteView buffer, T value, vac::container::CStringView msg = "Serialize native"_sv) noexcept
    -> SerResult {
  constexpr std::size_t kSize{sizeof(T)};
  return SerializeInternal(buffer, msg, kSize, [&buffer, value]() {
    // VCA_LOGAPI_VALID_OBJECT
    std::memcpy(buffer.data(), &value, kSize);
  });
}
}  // namespace native

/*!
 * \brief Serialize a byte
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, Byte value, vac::container::CStringView msg = "Serialize byte"_sv) noexcept
    -> SerResult {
  return SerializeInternal(buffer, msg, 1, [&buffer, value]() { buffer[0] = value; });
}

/*!
 * \brief Serialize the bool
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, bool value, vac::container::CStringView msg = "Serialize bool"_sv) noexcept
    -> SerResult {
  return Serialize(buffer, static_cast<Byte>(value), msg);
}

/*!
 * \brief Serialize the uint16
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, std::uint16_t value,
                      vac::container::CStringView msg = "Serialize uint16"_sv) noexcept -> SerResult {
  return native::Serialize(buffer, value, msg);
}

/*!
 * \brief Serialize the uint32
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, std::uint32_t value,
                      vac::container::CStringView msg = "Serialize uint32"_sv) noexcept -> SerResult {
  return native::Serialize(buffer, value, msg);
}

/*!
 * \brief Serialize the uint64
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, std::uint64_t value,
                      vac::container::CStringView msg = "Serialize uint64"_sv) noexcept -> SerResult {
  return native::Serialize(buffer, value, msg);
}

/*!
 * \brief Serialize the string
 * \details String must have one and only one null terminator character
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \param msg    The message to output in case of errors
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, core::StringView value,
                      vac::container::CStringView msg = "Serialize string"_sv) noexcept -> SerResult {
  std::size_t const copy_count{value.size() + 1};  // Add 1 because of zero termination
  return SerializeInternal(buffer, msg, copy_count, [&buffer, value, copy_count]() {
    // VCA_LOGAPI_VALID_OBJECT
    std::memcpy(buffer.data(), value.data(), copy_count);
  });
}

/*!
 * \brief Serialize the IpcMessageType
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, IpcMessageType value,
                      vac::container::CStringView msg = "Serialize IpcMessageType"_sv) noexcept -> SerResult {
  return Serialize(buffer, static_cast<Byte>(value), msg);
}

/*!
 * \brief Serialize the LogLevel
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, LogLevel value,
                      vac::container::CStringView msg = "Serialize LogLevel"_sv) noexcept -> SerResult {
  return Serialize(buffer, static_cast<Byte>(value), msg);
}

/*!
 * \brief Serialize the LogMode
 *
 * \param buffer The buffer to serialize into
 * \param value  The value to serialize
 * \return       The serializer result
 */
inline auto Serialize(ByteView buffer, LogMode value, vac::container::CStringView msg = "Serialize LogMode"_sv) noexcept
    -> SerResult {
  return Serialize(buffer, static_cast<Byte>(value), msg);
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SERIALIZE_H_
