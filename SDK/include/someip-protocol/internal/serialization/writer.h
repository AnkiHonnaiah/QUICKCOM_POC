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
/**        \file writer.h
 *        \brief SOME/IP protocol specific writer.
 *        \unit SomeIpProtocol::BasicSerdes::Writer
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_WRITER_H_
#define LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>
#include <type_traits>
#include "amsr/core/abort.h"
#include "ara/core/array.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/byteorder.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/traits.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Buffer view to wrap the buffer to write.
 */
using BufferView = amsr::core::Span<std::uint8_t>;

/*!
 * \brief Alias for the type of parameters that indicate the size or the number of values.
 */
using SizeType = BufferView::size_type;

/*!
 * \brief Writes an unsigned integer to a byte buffer.
 * \tparam UintSize Size of integer to write in bytes.
 * \tparam Endian Endianness of the payload to write.
 */
template <std::size_t UintSize, typename Endian>
class UintWrite {};

/*!
 * \brief Writes a single byte integer.
 * \tparam Endian Ignored as there is only one byte.
 */
template <typename Endian>
class UintWrite<1, Endian> {
 public:
  /*! \brief Type of the unsigned integer. */
  using type = std::uint8_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // clang-format off
  static void write(std::uint8_t* buffer, type value) noexcept { *buffer = value; } // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  // clang-format on
};

/*!
 * \brief Write a two-byte integer using specified endian.
 * \tparam Endian The endianness of the two-byte integer.
 */
template <typename Endian>
class UintWrite<2, Endian> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint16_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the configured endianess is not equal to the host byte order
   *   - perform a byte swap on the value.
   * - Copy value to the buffer.
   * \endinternal
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap16(value);
    }
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  }
};

/*!
 * \brief Write a two-byte integer using specified endian, specialization for MachineSpecificEndianness.
 */
template <>
class UintWrite<2, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint16_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_MEMCPY_USAGE
  }
};

/*!
 * \brief Write a four-byte integer using specified endian.
 * \tparam Endian The endianness of the four-byte integer.
 */
template <typename Endian>
class UintWrite<4, Endian> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint32_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the configured endianess is not equal to the host byte order
   *   - perform a byte swap on the value.
   * - Copy value to the buffer.
   * \endinternal
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap32(value);
    }
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  }
};

/*!
 * \brief Write a four-byte integer using specified endian, specialization for MachineSpecificEndianness.
 */
template <>
class UintWrite<4, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint32_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_MEMCPY_USAGE
  }
};

/*!
 * \brief Write an eight-byte integer using specified endian.
 * \tparam Endian The endianness of the eight-byte integer.
 */
template <typename Endian>
class UintWrite<8, Endian> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint64_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the configured endianess is not equal to the host byte order
   *   - perform a byte swap on the value.
   * - Copy value to the buffer.
   * \endinternal
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap64(value);
    }
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  }
};

/*!
 * \brief Write a eight-byte integer using specified endian, specialization for MachineSpecificEndianness.
 */
template <>
class UintWrite<8, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type of the unsigned integer.
   */
  using type = std::uint64_t;
  /*!
   * \brief Write an integer to the byte buffer.
   * \param[out] buffer Buffer to store the written data
   * \param[in] value Data to write
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void write(std::uint8_t* buffer, type value) noexcept {
    std::memcpy(buffer, &value, sizeof(value));  // VCA_SOMEIPPROTOCOL_MEMCPY_USAGE
  }
};

/*!
 * \brief Writes a float value to a byte buffer.
 * \tparam Endian Endianness of the payload to write.
 */
template <typename Endian>
class FloatWrite;
/*!
 * \brief FloatWriteData Structure
 */
struct FloatWriteData {
  /*!
   * \brief Type alias for buffer.
   */
  using BufferType = std::uint8_t*;
  /*!
   * \brief Type alias for value.
   */
  using ValueType = float;
  /*!
   * \brief The buffer for storing serialized data.
   */
  BufferType buffer;
  /*!
   * \brief The float value to be serialized.
   */
  ValueType value;
};
/*!
 * \brief Writes a float value to a byte buffer.
 * \tparam Endian Endianness of the payload to write.
 */
template <typename Endian>
class FloatWrite {
 public:
  /*!
   * \brief Write float value into the given buffer .
   * \param[in] data Structure contains function and line details to be logged.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the configured endianess is not equal to the host byte order
   *   - perform a byte swap on the value.
   * - Copy value to the buffer.
   * \endinternal
   */
  static void write(FloatWriteData data) {
    std::uint32_t tmp_value{};
    std::memcpy(&tmp_value, &data.value, sizeof(float));
    if (Endian::value != GetHostByteOrder()) {
      tmp_value = ByteSwap32(tmp_value);
    }
    // clang-format off
    std::memcpy(data.buffer, &tmp_value, sizeof(std::uint32_t)); // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
    // clang-format on
  }
};

/*!
 * \brief Writes a float value to a byte buffer, specialization for MachineSpecificEndianness.
 * \tparam Endian Endianness of the payload to write.
 */
template <>
class FloatWrite<MachineSpecificEndianness> {
 public:
  /*!
   * \brief Write float value into the given buffer.
   * \param[in] data Structure contains function and line details to be logged.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // clang-format off
  static void write(FloatWriteData data) { std::memcpy(data.buffer, &data.value, sizeof(float)); } // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  // clang-format on
};

/*!
 * \brief Writes a double value to a byte buffer.
 * \tparam Endian Endianness of the payload to write.
 */
template <typename Endian>
class DoubleWrite;
/*!
 * \brief DoubleWriteData Structure
 */
struct DoubleWriteData {
  /*!
   * \brief Type alias for buffer.
   */
  using BufferType = std::uint8_t*;
  /*!
   * \brief Type alias for value.
   */
  using ValueType = double;
  /*!
   * \brief The buffer for storing serialized data.
   */
  BufferType buffer;
  /*!
   * \brief The double value to be serialized.
   */
  ValueType value;
};
/*!
 * \brief Writes a double value to a byte buffer.
 * \tparam Endian Endianness of the payload to write.
 */
template <typename Endian>
class DoubleWrite {
 public:
  /*!
   * \brief Write double value into the given buffer.
   * \param[in] data Structure contains function and line details to be logged.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the configured endianess is not equal to the host byte order
   *   - perform a byte swap on the value.
   * - Copy value to the buffer.
   * \endinternal
   */
  static void write(DoubleWriteData data) {
    std::uint64_t tmp_value{};
    std::memcpy(&tmp_value, &data.value, sizeof(double));
    if (Endian::value != GetHostByteOrder()) {
      tmp_value = ByteSwap64(tmp_value);
    }
    // clang-format off
    std::memcpy(data.buffer, &tmp_value, sizeof(std::uint64_t)); // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
    // clang-format on
  }
};

/*!
 * \brief Writes a double value to a byte buffer, specialization for MachineSpecificEndianness.
 * \tparam Endian Endianness of the payload to write.
 */
template <>
class DoubleWrite<MachineSpecificEndianness> {
 public:
  /*!
   * \brief Write double value into the given buffer.
   * \param[in] data Structure contains function and line details to be logged.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // clang-format off
  static void write(DoubleWriteData data) { std::memcpy(data.buffer, &data.value, sizeof(double)); } // VCA_SOMEIPPROTOCOL_WRITER_PRIMITIVE
  // clang-format on
};
/*!
 * \brief WriteStruct Structure
 */
template <typename InputIt>
struct WriteStruct {
  /*!
   * \brief The start of the range to write.
   */
  typename InputIt::const_iterator begin;
  /*!
   * \brief The end of the range to write.
   */
  typename InputIt::const_iterator end;
};

/*!
 * \brief     SOME/IP protocol specific writer class.
 * \details   Support for SOME/IP protocol specific serialization of array, vector and string datatypes.
 *            Furthermore specialization is required to allow argument-dependent lookup for SomeIpProtocolSerialize
 *            functions.
 * \vprivate  Product private
 */
class Writer {
 public:
  /*!
   * \brief Constructor from a BufferViewNonConst.
   * \param[in, out] buffer_view Buffer view.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  explicit Writer(BufferView const& buffer_view) : buffer_view_{buffer_view} {}

  /*!
   * \brief Writes a primitive to the buffer using the specified endian.
   * \tparam T Primitive type.
   * \tparam Endian Endianness to use when writing to the buffer.
   * \param[in] t Value to serialize.
   * \return void.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to write the primitive to it
   *   - Log fatal message and abort.
   * - Write primitive to the buffer.
   * - Increase the write index with the size of the primitive.
   * \endinternal
   */
  template <typename T, typename Endian>
  auto writePrimitive(T const& t) noexcept
      -> std::enable_if_t<std::is_integral<T>::value && (!std::is_same<T, bool>::value), void> {
    if (!hasSize(sizeof(T))) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to write primitive."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    UintWrite<sizeof(T), Endian>::write(data(), static_cast<std::make_unsigned_t<T>>(t));
    write_index_ += sizeof(T);
  }

  /*!
   * \brief Specialization for bool.
   * \tparam T Type of primitive to be write. Must be bool.
   * \tparam Endian Endianness.
   * \param[in] t Boolean value, the data shall be deserialized into.
   * \return void.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - The value of sizeof(bool) is implementation defined and might differ from 1.
   * - But for SOME/IP the serialized size of a bool is 1 byte.
   * - If there is not sufficient amount of memory in the buffer to write the bool to it
   *   - Log fatal message and abort.
   * - Write primitive to the buffer.
   * - Increase the write index with the size of bool.
   * \endinternal
   */
  template <typename T, typename Endian>
  auto writePrimitive(bool t) noexcept -> std::enable_if_t<std::is_same<T, bool>::value, void> {
    if (!hasSize(sizeof(std::uint8_t))) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to write bool."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clanf-format on
    UintWrite<sizeof(std::uint8_t), Endian>::write(data(), t);
    write_index_ += sizeof(std::uint8_t);
  }

  /*!
   * \brief Specialization for float.
   * \tparam T Type of primitive to be write. Must be of type float in this case.
   * \tparam Endian  Endianness.
   * \param[in] t Float value, the data shall be deserialized into.
   * \return void.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to write the float to it
   *   - Log fatal message and abort.
   * - Write the value into the buffer.
   * - Increase the write index with the size of the float.
   * \endinternal
   */
  template <typename T, typename Endian>
  auto writePrimitive(T const& t) noexcept -> std::enable_if_t<std::is_same<T, float>::value, void> {
    static_assert(sizeof(float) == 4, "");
    if (!hasSize(sizeof(float))) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to write float."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                    ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    FloatWriteData const float_data{data(), t};
    FloatWrite<Endian>::write(float_data);
    write_index_ += sizeof(T);
  }

  /*!
   * \brief Specialization for double.
   * \tparam T Type of primitive to be write. Must be of type double in this case.
   * \tparam Endian Endianness.
   * \param[in] t Double value, the data shall be deserialized into.
   * \return void.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to write the double to it
   *   - Log fatal message and abort.
   * - Write the value into the buffer.
   * - Increment the write index with the number of bytes written.
   * \endinternal
   */
  template <typename T, typename Endian>
  auto writePrimitive(T const& t) noexcept -> std::enable_if_t<std::is_same<T, double>::value, void> {
    static_assert(sizeof(double) == 8, "");
    if (!hasSize(sizeof(double))) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to write double."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    DoubleWriteData const write_data{data(), t};
    DoubleWrite<Endian>::write(write_data);
    write_index_ += sizeof(T);
  }

  /*!
   * \brief Skips over the next n bytes in the write buffer.
   * \param[in] length The number of bytes to skip.
   * \return False if the number of bytes to skip is larger than the number of bytes left, and true otherwise.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to add the n bytes
   *   - Log fatal message and abort.
   * - Add 'length' to the write index.
   * \endinternal
   */
  void skip(SizeType length) noexcept {
    if (!hasSize(length)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to complete the serialization."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    write_index_ += length;
  }

  /*!
   * \brief Writes an unsigned integer of specified size.
   * \tparam UintSize The number of bytes to write.
   * \tparam Endian The endianness.
   * \param[in] t The input data to write.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to write the data to it
   *   - Log fatal message and abort.
   * - Call writePrimitive for the configured size to write the data into the buffer.
   * \endinternal
   */
  template <std::size_t UintSize, typename Endian>
  void writeUintOfSize(typename UintWrite<UintSize, Endian>::type const& t) noexcept {
    if (!hasSize(UintSize)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to write uint."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    writePrimitive<typename UintWrite<UintSize, Endian>::type, Endian>(t);
  }

  /*!
   * \brief Consumes the next n bytes from the write buffer and returns a new Writer object.
   * \param[in] count The number of bytes to consume.
   * \return The writer object handling the sub-stream.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer to consume n bytes
   *   - Log fatal message and abort.
   * - Create a 'count' bytes long subspan of the buffer indexed from 'write_index_'.
   * - Create a Writer with the subspan.
   * - Add 'count' to the write index.
   * - Return the created Writer.
   * \endinternal
   */
  Writer consumeSubStream(SizeType count) noexcept {
    if (!hasSize(count)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to complete the serialization."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    Writer const ret{buffer_view_.subspan(write_index_, count)};
    write_index_ += count;
    return ret;
  }

  /*!
   * \brief Writes the contents of a span holding bytes.
   * \tparam T Element type of the span. Underlying type must be std::uint8.
   * \param[in] span The span to write.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename T>
  void writeSpan(amsr::core::Span<T> const& span) noexcept {
    static_assert(std::is_same<std::uint8_t, std::remove_cv_t<T>>::value, "The underlying type must be std::uint8_t");
    WriteStruct<amsr::core::Span<T>> const write_data{span.cbegin(), span.cend()};
    writeRange(write_data);
    // clang-format off
  } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
  // clang-format on

  /*!
   * \brief Writes the data of an iterator range pointing to bytes.
   * \tparam InputIt The iterator type. Underlying type must be std::uint8.
   * \param[in] write_range Structure contains begin and end details to be logged.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Calculate number of bytes to be serialized.
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Copy the data into the buffer.
   * - Increment the write index with the number of bytes written.
   * \endinternal
   */
  template <typename InputIt>
  void writeRange(InputIt const& write_range) noexcept {
    using DataType = typename std::iterator_traits<decltype(write_range.begin)>::value_type;
    static_assert(std::is_same<DataType, std::uint8_t>::value, "Iterator underlying type must be std::uint8_t");

    std::size_t const data_length{static_cast<std::size_t>(std::distance(write_range.begin, write_range.end))};
    std::size_t const byte_count{data_length * sizeof(DataType)};

    if (!hasSize(byte_count)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize iterator range."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    std::uint8_t* const buffer_data{data()};
    static_cast<void>(std::copy(write_range.begin, write_range.end, buffer_data));  // VCA_SOMEIPPROTOCOL_COPY
    write_index_ += data_length;
  }

  /*!
   * \brief Writes an array of an arithmetic type with same endianness.
   * \tparam N The size of the array.
   * \param[in] arr The array to write.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Calculate number of bytes to be serialized.
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Copy the array into the buffer.
   * - Increment the write index with the number of bytes written.
   * \endinternal
   */
  template <typename T, std::size_t N, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
  void writeArray(ara::core::Array<T, N> const& arr) noexcept {
    static_assert(!std::is_same<T, bool>::value, "Invalid write operation for bool array.");
    std::size_t const bytes_count{N * sizeof(T)};
    if (!hasSize(bytes_count)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize array<T, N>."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    std::uint8_t* const buffer{data()};
    std::memcpy(buffer, arr.data(), bytes_count);  // VCA_SOMEIPPROTOCOL_WRITER_CHECK

    write_index_ += bytes_count;
  }

  /*!
   * \brief Writes an array of booleans.
   * \tparam N The size of the array.
   * \param[in] arr The array to write.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Calculate number of bytes to be serialized.
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Loop over array
   *   - write the current bool into the buffer.
   *   - Increment the writer index by one.
   */
  template <std::size_t N>
  void writeArray(ara::core::Array<bool, N> const& arr) noexcept {
    if (!hasSize(N)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize array<bool, N>."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    typename ara::core::Array<bool, N>::const_iterator const start{arr.cbegin()};
    typename ara::core::Array<bool, N>::const_iterator const end{arr.cend()};
    static_cast<void>(std::for_each(start, end, [this](bool const& val) { // VCA_SOMEIPPROTOCOL_FOR_EACH
      UintWrite<1, BigEndian>::write(data(), static_cast<uint8_t>(val)); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE
      write_index_ += 1U;
    }));
    // clang-format on
  }

  /*!
   * \brief Writes an empty array of an arithmetic type.
   * \tparam T The element type of the array. Template is enabled only if T is an arithmetic type.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  template <typename T,
            typename = typename std::enable_if_t<std::is_arithmetic<T>::value && (!std::is_same<T, bool>::value)>>
  static void writeArray(ara::core::Array<T, 0> const&) noexcept {}

  /*!
   * \brief Writes a vector of an arithmetic type with same endianness.
   * \tparam T The element type of the vector. Template is enabled only if T is an arithmetic type.
   * \tparam Alloc Vector allocator.
   * \param[in] vec The input vector.
   * \param[in] no_of_elements The number of elements from the input vector that should be written.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Calculate number of bytes to be serialized.
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Copy the vector into the buffer when it is valid.
   * - Increment the write index with the number of bytes written.
   * \endinternal
   */
  // VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  template <typename T, typename Alloc, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
  void writeVector(ara::core::Vector<T, Alloc> const& vec, size_t const no_of_elements) noexcept {
    static_assert(!std::is_same<T, bool>::value, "Invalid write operation for bool vector.");
    // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    std::size_t const bytes_count{no_of_elements * sizeof(T)};
    if (!hasSize(bytes_count)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize vector<T>"}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
      // clang-format off
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on

    if (no_of_elements > 0) {
      std::uint8_t* const buffer{data()};
      std::memcpy(buffer, vec.data(), bytes_count);  // VCA_SOMEIPPROTOCOL_WRITER_CHECK
      write_index_ += bytes_count;
    }
  }

  /*!
   * \brief Write vector<bool> to BufferViewNonConst.
   * \tparam Alloc Vector allocator.
   * \param[in] vec The input vector.
   * \param[in] no_of_elements The number of elements from the input vector that should be written(<= vec.size()).
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Loop over vector
   *   - write the current bool into the buffer.
   *   - Increment the writer index.
   */
  // VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  // VECTOR NL AutosarC++17_10-A18.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A18.1.2_use_of_vector_bool
  template <typename Alloc>
  void writeVector(ara::core::Vector<bool, Alloc> const& vec, size_t const no_of_elements) noexcept {
    using DiffType = typename ara::core::Vector<bool, Alloc>::difference_type;
    if (!hasSize(no_of_elements)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize vector<bool>."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    static_cast<void>(std::for_each(vec.cbegin(), std::next(vec.cbegin(),static_cast<DiffType>(no_of_elements)), [this](bool const& val) { // VCA_SOMEIPPROTOCOL_FOR_EACH
      UintWrite<1, BigEndian>::write(data(), val); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE
      write_index_ += 1U;
    }));  // VCA_SOMEIPPROTOCOL_FOR_EACH
    // clang-format on
  }

  /*!
   * \brief Write a string.
   * \tparam String String type.
   * \param[in] str The input string.
   * \pre The writer should hold sufficient buffer for the value to be written.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Calculate number of bytes to be serialized.
   * - If there is not sufficient amount of memory in the buffer
   *   - Log fatal message and abort.
   * - Copy the vector into the buffer.
   * - Increment the write index with the number of bytes written.
   * \endinternal
   */
  // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  template <typename String, std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
  void writeString(String const& str) noexcept {
    // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    size_t const stringLength{str.size()};
    if (!hasSize(stringLength)) {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Insufficient buffer size to serialize string."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
    } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format on
    std::uint8_t* const buffer{data()};
    std::memcpy(buffer, str.c_str(), stringLength);  // VCA_SOMEIPPROTOCOL_WRITER_CHECK

    write_index_ += stringLength;
  }

  /*!
   * \brief Get the remaining size of the buffer view.
   * \return Remaining size.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  SizeType Size() const noexcept { return (buffer_view_.size() - write_index_); }

  /*!
   * \brief Get the current data pointer to write.
   * \return Data pointer to write.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  std::uint8_t* data() const noexcept {
    return std::next(buffer_view_.data(), static_cast<std::ptrdiff_t>(write_index_));
  }

  /*!
   * \brief Checks if the buffer is large enough for a write of size n.
   * \param[in] size Bytes to be check against the remaining bufferView size.
   * \return True if the amount could be written, false otherwise.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  bool hasSize(SizeType size) const noexcept { return size <= Size(); }

 private:
  /*!
   * \brief The managed BufferView
   */
  BufferView buffer_view_;
  /*!
   * \brief Write Index holding the current index to write
   */
  SizeType write_index_{0U};
};

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_WRITER_H_
