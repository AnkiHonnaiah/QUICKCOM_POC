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
/*!        \file someip-protocol/internal/types.h
 *        \brief Common types between serialization and deserialization.
 *         \unit SomeIpProtocol::Serdes::Types
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TYPES_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TYPES_H_

#include <cstdint>
#include <string>
#include <type_traits>
#include "ara/core/array.h"
#include "ara/core/map.h"
#include "ara/core/string.h"
#include "someip-protocol/internal/byteorder.h"
#include "someip-protocol/internal/traits.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * \brief Denotes the byte order to apply
 */
enum class EStringEncoding : std::uint8_t { kUTF_8, kUTF_16 };

/*!
 * \brief Byte order mark (BOM) for UTF-8
 */
constexpr ara::core::Array<std::uint8_t, 3U> kBomUTF_8{0xEFU, 0xBBU, 0xBFU};

/*!
 * \brief Byte order mark (BOM) for UTF-16BE
 */
constexpr ara::core::Array<std::uint8_t, 2U> kBomUTF_16_BE{0xFEU, 0xFFU};

/*!
 * \brief Byte order mark (BOM) for UTF-16LE
 */
constexpr ara::core::Array<std::uint8_t, 2U> kBomUTF_16_LE{0xFFU, 0xFEU};

/*!
 * \brief Null value for UTF-16
 */
constexpr std::uint32_t kNullValue_UTF_16{0x0000U};

/*!
 * \brief Null value for UTF-8
 */
constexpr std::uint8_t kNullValue_UTF_8{0x00U};

/*!
 * \brief Defines the size and endianess of a length field.
 * \tparam Size Size of the length field in bytes.
 * \tparam Endianness Endianess of the length field.
 * \trace SPEC-4980063
 */
template <std::size_t Size, typename Endianness = ::amsr::someip_protocol::internal::BigEndian>
struct LengthSize : std::integral_constant<std::size_t, Size> {
  /*!
   * \brief Type Alias for the endianness.
   */
  using endian = Endianness;
};

/*!
 * \brief Defines the size of an array length field.
 * \tparam N Size of array length field in bytes.
 */
template <std::size_t N>
struct SizeOfArrayLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of a vector length field.
 * \tparam N Size of vector length field in bytes.
 */
template <std::size_t N>
struct SizeOfVectorLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of a map length field.
 * \tparam N Size of vector length field in bytes.
 */
template <std::size_t N>
struct SizeOfMapLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of a string length field.
 * \tparam N Size of length field in bytes.
 */
// VECTOR AV NC Architecture-ApiAnalysis Template_Argument: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <std::size_t N>
struct SizeOfStringLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of a using length field.
 * \tparam N Size of length field in bytes.
 */
template <std::size_t N>
struct SizeOfStructLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of the using length field.
 * \tparam N Size of length field in bytes.
 */
template <std::size_t N>
struct SizeOfUnionLengthField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the size of the type selector field of unions.
 * \tparam N Size of type selector field in bytes.
 */
template <std::size_t N>
struct SizeOfUnionTypeSelectorField : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Defines the array and vector container size.
 * \tparam N array / vector container size in bytes. 0: No size configured / enabled.
 */
template <std::size_t N>
// VECTOR NL AutosarC++17_10-A12.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A12.4.1_no_virtual_destructor
struct ArraySize : std::integral_constant<std::size_t, N> {};

/*!
 * \brief Specialization of ArraySize for a disabled / inactive/ not configured array size transformation property.
 * \details Only a value bigger than 0 enables array size handling.
 */
struct ArraySizeInactive : public ArraySize<0U> {};

/*!
 * \brief Defines that the data type string is used along with the byte order mark.
 */
struct StringBomActive : std::true_type {};

/*!
 * \brief Defines that the data type string is not used along with the byte order mark.
 */
struct StringBomInactive : std::false_type {};

/*!
 * \brief Defines that the data type string is terminated with null termination character.
 */
struct StringNullTerminationActive : std::true_type {};

/*!
 * \brief Defines that the data type string is not terminated with null termination character.
 */
struct StringNullTerminationInactive : std::false_type {};

/*!
 * \brief Type for Utf-16 string encoding.
 */
struct Utf16Encoding {
  /*!
   * \brief Define a constant expression for the conventional de/serializer.
   */
  static constexpr EStringEncoding value{EStringEncoding::kUTF_16};
};

/*!
 * \brief Type for Utf-16 string encoding.
 */
struct Utf8Encoding {
  /*!
   * \brief Define a constant expression for the conventional de/serializer.
   */
  static constexpr EStringEncoding value{EStringEncoding::kUTF_8};
};

/*!
 * \brief Template to hold the transformation properties parameter pack containing an ordered list of template
 *        parameters.
 * \tparam Items parameter pack listing all transformation property items.
 */
template <typename... Items>
struct TpPack {};

/*! Template to hold the parameter pack containing the length field configurations for the sub-elements of a complex
 *  data type.*/
template <typename... Confs>
struct ConfPack {};

/*! Type definition for DataId used in the TAG field in TLV context. */
using DataId = std::uint16_t;

/*!
 * \brief Check if the provided template type is a primitive (integral or floating point).
 * \tparam T Type to be checked.
 */
template <typename T>
struct IsPrimitiveType : public std::integral_constant<bool, std::is_integral<std::decay_t<T>>::value ||
                                                                 std::is_floating_point<std::decay_t<T>>::value> {};
/*!
 * \brief Check if the provided template type is a enum.
 * \tparam T Type to be checked.
 */
template <typename T>
struct IsEnumType : public std::integral_constant<bool, std::is_enum<std::decay_t<T>>::value> {};

/*!
 * \brief Check if the provided template type is a ara::core::Map.
 * \tparam T Type to be checked.
 */
template <typename T>
struct IsMap : public std::false_type {};

/*!
 * \brief Check if the provided template type is a ara::core::Map.
 * \tparam Key Key type
 * \tparam Value Value type
 * \tparam Comp Comparison functor
 * \tparam Alloc allocator
 */
template <typename Key, typename Value, typename Comp, typename Alloc>
struct IsMap<ara::core::Map<Key, Value, Comp, Alloc>> : public std::true_type {};

/*!
 * \brief Compile-time access to the first element of the ItemConf parameter pack.
 * \tparam ItemConf parameter pack.
 */
template <typename... ItemConf>
using GetFirstItemConf = typename vac::language::compile_time::get_from_index<0U, ItemConf...>;

/*!
 * \brief Type-trait condition for optimized deserialization of array and vector data types.
 *
 * \details Optimized deserialization can be applied for any of the following cases:
 *          - array/vector of single bytes numeric values (e.g. vector<uint8>, vector<int8>, ...)
 *          - array/vector of multi-byte numeric values encoded in host-byte-order (e.g. vector<uint32>, ...)
 *
 * \tparam T Value type of vector.
 * \tparam ItemConf Item configuration parameter pack.
 * \return true if optimized deserialization can be used for the array/vector data type, otherwise false is returned.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Check if T is arithmetic and (T represents a single byte or configured endianness matches the host-byte-order)
 * \endinternal
 */
template <typename T, typename... ItemConf>
static constexpr bool TypeTraitIsOptimizableContainer() noexcept(true) {
  /*!
   * \brief Type alias for byte order enum.
   */
  using ByteOrderType = ::amsr::someip_protocol::internal::ByteOrder;
  /*!
   * \brief Type alias for endianness base class.
   */
  using EndiannessType = ::amsr::someip_protocol::internal::Endianness;
  return
      // Numeric type?
      std::is_arithmetic<T>::value &&
      // single byte?
      ((sizeof(T) == 1U) ||
       // Check if the first ItemConf from the parameter pack is of type Endianness.
       // This check is necessary if the value type is a primitive datatype (Endianness passed in parameter pack)
       // or a complex type with additional length field (in this case a LengthSize<Size, Endianness> is passed in the
       // parameter pack).
       (std::is_base_of<EndiannessType, GetFirstItemConf<ItemConf...>>::value &&
        // using host byte order?
        // Case necessary to avoid compile errors in case of complex datatypes (LengthSize instead of Endianess passed)
        // due to the previous is_same check the downcast is safe.
        (static_cast<std::underlying_type_t<ByteOrderType>>(GetFirstItemConf<ItemConf...>::value) ==
         static_cast<std::underlying_type_t<ByteOrderType>>(GetHostByteOrder()))));
}

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_TYPES_H_
