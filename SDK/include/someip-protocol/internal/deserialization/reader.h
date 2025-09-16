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
/**        \file reader.h
 *        \brief SOME/IP protocol specific reader.
 *        \unit SomeIpProtocol::BasicSerdes::Reader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_READER_H_
#define LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <type_traits>
#include "ara/core/array.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/byteorder.h"
#include "someip-protocol/internal/traits.h"
#include "someip-protocol/internal/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief Buffer view to wrap the data to inspect.
 */
using BufferView = amsr::core::Span<std::uint8_t const>;

/*!
 * \brief Alias for the type of parameters that indicate a size or a number of values.
 */
using SizeType = BufferView::size_type;

/*!
 * \brief   Functions for reading primitives from a byte buffer.
 * \tparam  UintSize size of uint.
 * \tparam  Endian configured endianness.
 * \trace   SPEC-4981316
 */
template <std::size_t UintSize, typename Endian>
class UintRead;

/*!
 * \brief   Read uint8 value from buffer.
 * \tparam  Endian The configured endianness. Ignored for uint8.
 */
template <typename Endian>
class UintRead<1U, Endian> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint8_t;

  /*!
   * \brief   Read uint8 value from the given buffer.
   * \param   buffer - Serialized data.
   * \return  Deserialized uint8 value.
   * \pre     buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context ANY
   */
  static std::uint8_t Read(std::uint8_t const* buffer) noexcept {
    type const value{*buffer};
    return value;
  }
};

/*!
 * \brief   Read uint16 value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class UintRead<2U, Endian> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint16_t;

  /*!
   * \brief     Read uint16 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint16 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static std::uint16_t Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap16(value);
    }
    return value;
  }
};

/*!
 * \brief  Read uint16 value from buffer, specialization for MachineSpecificEndianness.
 */
template <>
class UintRead<2U, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint16_t;

  /*!
   * \brief     Read uint16 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint16 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static std::uint16_t Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    return value;
  }
};

/*!
 * \brief   Read uint32 value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class UintRead<4U, Endian> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint32_t;

  /*!
   * \brief     Read uint32 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint32 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static std::uint32_t Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap32(value);
    }
    return value;
  }
};

/*!
 * \brief Read uint32 value from buffer, specialization for MachineSpecificEndianness.
 */
template <>
class UintRead<4U, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint32_t;

  /*!
   * \brief     Read uint32 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint32 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static std::uint32_t Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    return value;
  }
};

/*!
 * \brief   Read uint64 value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class UintRead<8U, Endian> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint64_t;

  /*!
   * \brief     Read uint64 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint64 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (Endian::value != GetHostByteOrder()) {
      value = ByteSwap64(value);
    }
    return value;
  }
};

/*!
 * \brief Read uint64 value from buffer, specialization for MachineSpecificEndianness.
 */
template <>
class UintRead<8U, MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type alias for the underlying datatype.
   */
  using type = std::uint64_t;

  /*!
   * \brief     Read uint64 value from the given buffer.
   * \param[in] buffer - Serialized data.
   * \return    Deserialized uint64 value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) noexcept {
    type value{};
    std::memcpy(&value, buffer, sizeof(value));
    return value;
  }
};

/*!
 * \brief   Read float value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class FloatRead;

/*!
 * \brief   Read float value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class FloatRead {
 public:
  /*!
   * \brief Type alias for float datatype.
   */
  using type = float;

  /*!
   * \brief     Read float value from the given buffer.
   * \param[in] buffer The buffer containing serialized data.
   * \return    Deserialized float value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) {
    std::uint32_t tmp_value{};
    type value{};
    std::memcpy(&tmp_value, buffer, sizeof(std::uint32_t));
    if (Endian::value != GetHostByteOrder()) {
      tmp_value = ByteSwap32(tmp_value);
    }
    std::memcpy(&value, &tmp_value, sizeof(float));
    return value;
  }
};

/*!
 * \brief Read float value from buffer, specialization for MachineSpecificEndianness.
 */
template <>
class FloatRead<MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type alias for float datatype.
   */
  using type = float;

  /*!
   * \brief     Read float value from the given buffer.
   * \param[in] buffer The buffer containing serialized data.
   * \return    Deserialized float value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) {
    type value{};
    std::memcpy(&value, buffer, sizeof(float));
    return value;
  }
};

/*!
 * \brief   Read double value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class DoubleRead;

/*!
 * \brief   Read double value from buffer according to the given endianness.
 * \tparam  Endian The configured endianness.
 */
template <typename Endian>
class DoubleRead {
 public:
  /*!
   * \brief Type alias for double datatype.
   */
  using type = double;

  /*!
   * \brief     Read double value from the given buffer.
   * \param[in] buffer The buffer containing serialized data.
   * \return    Deserialized double value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) {
    std::uint64_t tmp_value{};
    type value{};
    std::memcpy(&tmp_value, buffer, sizeof(std::uint64_t));
    if (Endian::value != GetHostByteOrder()) {
      tmp_value = ByteSwap64(tmp_value);
    }
    std::memcpy(&value, &tmp_value, sizeof(double));
    return value;
  }
};

/*!
 * \brief Read double value from buffer, specialization for MachineSpecificEndianness.
 */
template <>
class DoubleRead<MachineSpecificEndianness> {
 public:
  /*!
   * \brief Type alias for double datatype.
   */
  using type = double;

  /*!
   * \brief     Read double value from the given buffer.
   * \param[in] buffer The buffer containing serialized data.
   * \return    Deserialized double value.
   * \pre       buffer can not be nullptr and must point to a valid std::uint8_t value.
   * \context   ANY
   */
  static type Read(std::uint8_t const* buffer) {
    type value{};
    std::memcpy(&value, buffer, sizeof(double));
    return value;
  }
};

/*!
 * \brief     SOME/IP protocol reader class.
 * \details   Support for SOME/IP protocol specific deserialization of array, vector and string datatypes.
 *            Furthermore specialization is require to allow argument-dependent lookup for SomeIpProtocolDeserialize
 *            functions.
 * \vprivate  Product private
 */
class Reader {
 public:
  /*!
   * \brief Buffer view to wrap the data to inspect.
   */
  using BufferView = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief         Create reader holding the provided BufferView on the serialized data.
   * \param[in,out] buffer_view  Buffer view.
   * \pre           -
   * \context       ANY
   * \spec
   *   requires true;
   * \endspec
   */
  explicit constexpr Reader(BufferView const& buffer_view) : buffer_view_{buffer_view} {}

  /*!
   * \brief       Read arithmetic array<T,N> from bufferView.
   * \pre         Source and destination endianness must be same.
   * \tparam      N Size of the uint8 array.
   * \tparam      T Type of array. Template enabled only if T contains arithmetic elements.
   * \param[out]  arr Array, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   *
   * \internal
   *  - Copy the serialized bytes into the destination array and increase the internal read counter.
   * \endinternal
   */
  template <typename T, std::size_t N, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
  void ReadArray(ara::core::Array<T, N>& arr) noexcept {
    static_assert(!std::is_same<T, bool>::value, "Invalid read operation for bool array.");
    std::size_t const bytes_count{N * sizeof(T)};

    std::uint8_t const* const buffer{Data()};
    std::memcpy(arr.data(), buffer, bytes_count);  // VCA_SOMEIPPROTOCOL_MEMCPY_USAGE

    read_index_ += bytes_count;
  }

  /*!
   * \brief       Read array<bool,N> from bufferView.
   * \tparam      N Size of the uint8 array.
   * \param[out]  arr Array, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   * \trace       SPEC-4981317
   *
   * \internal
   *  - For each byte for the destination array
   *    - Read the byte from source array and map first bit of byte to bool value.
   *    - Increase read index by 1.
   * \endinternal
   */
  template <std::size_t N>
  void ReadArray(ara::core::Array<bool, N>& arr) noexcept {
    // VECTOR NL Compiler-#186: MD_SOMEIPPROTOCOL_Compiler-#186_PointlessComparison
    for (std::size_t idx{0U}; idx < N; ++idx) {
      std::uint8_t const value{UintRead<1U, BigEndian>::Read(Data())};
      arr[idx] = static_cast<bool>(value & 0x01U);
      read_index_ += 1U;
    }
  }

  /*!
   * \brief       Read vector<T> from bufferView.
   * \pre         Source and destination endianness must be same.
   * \tparam      T Datatype stored in the vector. Template enabled only if T contains arithmetic elements.
   * \tparam      Alloc Vector allocator.
   * \param[out]  vec Vector, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   *
   * \internal
   * - Calculate the number of elements that could fit into the byte stream
   * - Copy the corresponding number of bytes into the destination vector.
   * - Increase the read index to the number of bytes that have been read.
   * \endinternal
   */
  // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  template <typename T, typename Alloc, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
  void ReadVector(ara::core::Vector<T, Alloc>& vec) noexcept {
    // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    std::size_t const element_count{Size() / sizeof(T)};
    std::size_t const bytes_count{element_count * sizeof(T)};

    // VECTOR NL AutosarC++17_10-A5.2.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A5.2.4_REINTERPRET_CAST
    T const* const serialized_data_ptr{reinterpret_cast<T const*>(Data())};
    amsr::core::Span<T const> const data_view{serialized_data_ptr, element_count};
    static_cast<void>(vec.insert(vec.end(), data_view.cbegin(), data_view.cend()));  // VCA_SOMEIPPROTOCOL_VEC_INSERT

    read_index_ += bytes_count;
  }

  /*!
   * \brief       Read vector<T> from bufferView.
   * \pre         Source and destination endianness must be same.
   * \tparam      ArraySizeConf Array size item configuration: max. number of allowed vector elements.
   * \tparam      T Datatype stored in the vector. Template enabled only if T contains arithmetic elements.
   * \param[out]  vec Vector, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   *
   * \internal
   * - Calculate the number of stored elements.
   * - If the number of elements exceeds the max. permittted value, limit the number of bytes to the max. size.
   * - Copy the number of bytes to be copied to the destination vector.
   * - Increase the read index regardless of the ignored elements to the number of bytes that represent the calculated
   *   number of elements.
   * \endinternal
   */
  template <typename ArraySizeConf, typename T, typename Alloc,
            typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
  void ReadVector(ara::core::Vector<T, Alloc>& vec) noexcept {
    size_t element_count{Size() / sizeof(T)};
    size_t const bytes_count{element_count * sizeof(T)};

    // Reduce actual number of deserialized elements exceed the ArraySize limit
    if (element_count > ArraySizeConf::value) {
      element_count = ArraySizeConf::value;
    }

    // VECTOR NL AutosarC++17_10-A5.2.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A5.2.4_REINTERPRET_CAST
    T const* const serialized_data_ptr{reinterpret_cast<T const*>(Data())};

    amsr::core::Span<T const> const data_view{serialized_data_ptr, element_count};
    static_cast<void>(vec.insert(vec.end(), data_view.cbegin(), data_view.cend()));  // VCA_SOMEIPPROTOCOL_VEC_INSERT

    // Mark all bytes of the vector as read, including ignored elements due to ArraySize limit.
    read_index_ += bytes_count;
  }

  /*!
   * \brief       Read vector<bool> from bufferView.
   * \tparam      Alloc Vector allocator.
   * \param[out]  vec Vector, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   * \trace       SPEC-4981317
   *
   * \internal
   * - Iterate through all bytes of the serialized buffer.
   *   - Evaluate the first bit and cast the first bit to a bool value.
   *   - Store the bool value to the destination array.
   *   - Increase the read index.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A18.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A18.1.2_use_of_vector_bool
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  template <typename Alloc>
  void ReadVector(ara::core::Vector<bool, Alloc>& vec) noexcept {
    // element count is equal to buffer size (bytes) because a serialized bool is always 1 byte in SOME/IP protocol.
    size_t const element_count{Size()};
    vec.resize(element_count);  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC

    // clang-format off
    static_cast<void>(std::transform(vec.cbegin(), vec.cend(), vec.begin(),  // VCA_SOMEIPPROTOCOL_TRANSFORM
                         [this](bool) {
                         std::uint8_t const single_byte{UintRead<1U, BigEndian>::Read(Data())};  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
                         // clang-format on
                         read_index_ += 1U;
                         // Evaluate only the lowest bit of the byte
                         return static_cast<bool>(single_byte & 0x01U);
                       }));
  }

  /*!
   * \brief       Read vector<bool> from bufferView.
   * \tparam      ArraySizeConf Array size item configuration: max. number of allowed vector elements.
   * \tparam      Alloc Vector allocator.
   * \param[out]  vec Vector, the deserialized value shall be stored into.
   * \pre         Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context     Reactor|App
   * \reentrant   FALSE
   * \trace       SPEC-4981317
   *
   * \internal
   * - If the size of source byte stream exceeds the max. limit of the destination vector
   *   - Set the number of bytes to be copied to the max. limit.
   * - Iterate over the bytes to be copied
   *   - Evaluate the first bit and cast the first bit to a bool value.
   *   - Store the bool value to the destination array.
   *   - Increase the read index.
   * - Increase the read index by the number of ignored bytes.
   * \endinternal
   */
  template <typename ArraySizeConf, typename Alloc>
  // VECTOR NC AutosarC++17_10-A18.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A18.1.2_use_of_vector_bool
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  void ReadVector(ara::core::Vector<bool, Alloc>& vec) noexcept {
    // element count is equal to buffer size (bytes) because a serialized bool is always 1 byte in SOME/IP protocol.
    size_t element_count{Size()};
    size_t skipped_elements_count{0U};

    // Reduce actual number of deserialized elements if it exceeds the ArraySize limit
    if (element_count > ArraySizeConf::value) {
      skipped_elements_count = element_count - ArraySizeConf::value;
      element_count = ArraySizeConf::value;
    }

    vec.resize(element_count);  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC

    // clang-format off
    static_cast<void>(std::transform(vec.cbegin(), vec.cend(), vec.begin(),  // VCA_SOMEIPPROTOCOL_TRANSFORM
                         [this](bool) {
                         std::uint8_t const single_byte{UintRead<1U, BigEndian>::Read(Data())};  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
                         // clang-format on
                         read_index_ += 1U;
                         // Evaluate only the lowest bit of the byte
                         return static_cast<bool>(single_byte & 0x01U);
                       }));

    // Skip unexpected elements
    read_index_ += skipped_elements_count;
  }

  /*!
   * \brief Read string from bufferView.
   * \tparam String String type.
   * \param[out] str String, the deserialized value shall be stored into.
   * \param[in] str_length string length in bytes.
   * \pre Caller has to verify the underlying buffer has enough bytes to read via VerifySize.
   * \context Reactor|App
   * \reentrant FALSE
   *
   * \internal
   * - Copy str_length bytes into the destination string.
   * - Increase read index by str_length.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  template <typename String, std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
  void ReadString(String& str, std::size_t const str_length) noexcept {
    static_cast<void>(
        std::copy_n(Data(), str_length, std::back_inserter(str)));  // VCA_SOMEIPPROTOCOL_READ_STRING_COPY_N

    read_index_ += str_length;
  }

  /*!
   * \brief     Return true if at least the given number of bytes can be read from the view.
   * \details   Updates the verified_count_ with 'size' bytes.
   * \param[in] size Bytes to be checked against the remaining BufferView size.
   * \return    True if the amount could be read, false otherwise.
   * \pre       -
   * \context   ANY
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr bool VerifySize(SizeType const size) noexcept {
    bool result{false};
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (size <= (buffer_view_.size() - verified_count_)) {
      verified_count_ += size;
      result = true;
    }
    return result;
  }

  /*!
   * \brief   Get the current data pointer to read.
   * \return  Data pointer to read.
   * \pre     -
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   */
  std::uint8_t const* Data() const noexcept {
    return std::next(buffer_view_.data(), static_cast<std::ptrdiff_t>(read_index_));
  }

  /*!
   * \brief   Get the remaining size on the buffer view.
   * \return  Remaining size.
   * \pre     -
   * \context ANY
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr SizeType Size() const noexcept { return (buffer_view_.size() - read_index_); }

  /*!
   * \brief       Read integral value from the buffer view.
   * \tparam      T Type of primitive to be read.
   * \tparam      Endian Endianness.
   * \param[out]  t Primitive, the data shall be deserialized into.
   * \return      void.
   * \pre         -
   * \context     Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename Endian>
  auto ReadPrimitive(T& t) noexcept
      -> std::enable_if_t<std::is_integral<T>::value && (!std::is_same<T, bool>::value), void> {
    t = static_cast<T>(UintRead<sizeof(T), Endian>::Read(Data()));  // VCA_SOMEIPPROTOCOL_READER_PRIMITIVE
    read_index_ += sizeof(T);
  }

  /*!
   * \brief       Read bool value from the buffer view.
   * \tparam      T Type of primitive to be read. Must be bool.
   * \tparam      Endian Endianness.
   * \param[out]  t Boolean value, the data shall be deserialized into.
   * \pre         -
   * \context     Reactor|App
   * \trace       SPEC-4981317
   * \trace       SPEC-4980150
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - The value of sizeof(bool) is implementation defined and might differ from 1.
   * - But for SOME/IP the serialized size of a bool is 1 byte.
   * \endinternal
   */
  template <typename T, typename Endian>
  void ReadPrimitive(bool& t) noexcept {
    std::uint8_t const tmp{UintRead<sizeof(std::uint8_t), Endian>::Read(Data())};
    t = static_cast<bool>(tmp & 0x01U);
    read_index_ += sizeof(std::uint8_t);
  }

  /*!
   * \brief       Read float value from the buffer view.
   * \tparam      T Type of primitive to be read. Must be of type float in this case.
   * \tparam      Endian Endianness.
   * \param[out]  t Float value, the data shall be deserialized into.
   * \return      void.
   * \pre         -
   * \context     Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename Endian>
  auto ReadPrimitive(T& t) noexcept -> std::enable_if_t<std::is_same<T, float>::value, void> {
    static_assert(sizeof(float) == 4U, "");

    t = static_cast<T>(FloatRead<Endian>::Read(Data()));
    read_index_ += sizeof(T);
  }

  /*!
   * \brief       Read double value from the buffer view.
   * \tparam      T Type of primitive to be read. Must be of type double in this case.
   * \tparam      Endian Endianness.
   * \param[out]  t Double value, the data shall be deserialized into.
   * \return      void.
   * \pre -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename Endian>
  auto ReadPrimitive(T& t) noexcept -> std::enable_if_t<std::is_same<T, double>::value, void> {
    static_assert(sizeof(double) == 8U, "");
    t = static_cast<T>(DoubleRead<Endian>::Read(Data()));  // VCA_SOMEIPPROTOCOL_READER_PRIMITIVE
    read_index_ += sizeof(T);
  }

  /*!
   * \brief       Generic function to read primitive data (of type integral or floating point).
   * \tparam      size Size of the underlying data type in bytes.
   * \tparam      Endian The endianness.
   * \param[out]  t Destination, the deserialized value shall be stored into.
   * \pre         -
   * \context     Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  template <std::size_t const size, typename Endian>
  void ReadUintOfSize(typename UintRead<size, Endian>::type& t) noexcept {
    ReadPrimitive<typename UintRead<size, Endian>::type, Endian>(t);
  }

  /*!
   * \brief     Create a Subreader holding a BufferView starting from the current position with the given length.
   * \details   Consumes the given number of bytes from this reader.
   * \param[in] count - Size of the subview in bytes.
   * \return    Reader holding a subview on the serialized data.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  Reader ConsumeSubStream(SizeType const count) noexcept {
    Reader const ret{buffer_view_.subspan(read_index_, count)};
    read_index_ += count;
    return ret;
  }

  /*!
   * \brief     Skip the number of bytes.
   * \param[in] length - Number of bytes to be skipped.
   * \return    True, if the number of bytes could be skipped. False otherwise.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  bool Skip(SizeType const length) noexcept {
    bool const could_skip{length <= Size()};

    if (could_skip) {
      read_index_ += length;
    }

    return could_skip;
  }

  /*!
   * \brief   Gets the current buffer view for this reader.
   * \return  Buffer view as span.
   * \pre     -
   * \context Reactor|App
   */
  BufferView View() const { return ara::core::MakeSpan(Data(), Size()); }

 private:
  /*!
   * \brief The managed Bufferview
   */
  BufferView buffer_view_;

  /*!
   * \brief Read Index holding the current index to read
   */
  SizeType read_index_{0U};

  /*!
   * \brief Count of bytes already verified against buffer length.
   */
  SizeType verified_count_{0U};
};

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_READER_H_
