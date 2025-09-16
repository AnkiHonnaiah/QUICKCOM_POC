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
/**        \file /deserialization/types.h
 *        \brief Types specific to deserialization.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_TYPES_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "someip-protocol/internal/byteorder.h"
#include "someip-protocol/internal/types.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief Type alias for marshalling result.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using Result = bool;

/*!
 * \brief Type alias for endianness 'BigEndian'.
 */
using BigEndian = ::amsr::someip_protocol::internal::BigEndian;

/*!
General Tp class template required to permit specialisation.
*/
template <typename Tps>
struct Tp;

/*!
 * \brief   Partially specialized template class retrieving all transformation properties related info from template
 *          arguments.
 * \details This struct relies on the ordered list of template arguments. TpPack is an instantiation containing all the
 *          configuration to retrieve.
 * \tparam  ByteOrderIn                       The byte order: LittleEndian, BigEndian, or MachineSpecificEndianness.
 * \tparam  SizeOfArrayLengthFieldValue       Size of the array length field value.
 * \tparam  SizeOfVectorLengthFieldValue      Size of the vector length field value.
 * \tparam  SizeOfMapLengthFieldValue         Size of the map length field value.
 * \tparam  SizeOfStringLengthFieldValue      Size of the string length field value.
 * \tparam  SizeOfStructLengthFieldValue      Size of the struct length field value.
 * \tparam  SizeOfUnionLengthFieldValue       Size of the union length field value.
 * \tparam  SizeOfUnionTypeSelectorFieldValue Size of the union type selector field value.
 * \tparam  StringIsBomActiveIn               Determines if the byte order mark is used in strings.
 * \tparam  StringIsNullTerminationActiveIn   Determines if a null termination character is used in strings.
 * \tparam  StringEncodingIn                  The encoding type for strings. It can be Utf8Encoding or Utf16Encoding.
 */
template <typename ByteOrderIn, std::size_t SizeOfArrayLengthFieldValue, std::size_t SizeOfVectorLengthFieldValue,
          std::size_t SizeOfMapLengthFieldValue, std::size_t SizeOfStringLengthFieldValue,
          std::size_t SizeOfStructLengthFieldValue, std::size_t SizeOfUnionLengthFieldValue,
          std::size_t SizeOfUnionTypeSelectorFieldValue, typename StringIsBomActiveIn,
          typename StringIsNullTerminationActiveIn, typename StringEncodingIn>
struct Tp<
    TpPack<ByteOrderIn, SizeOfArrayLengthField<SizeOfArrayLengthFieldValue>,
           SizeOfVectorLengthField<SizeOfVectorLengthFieldValue>, SizeOfMapLengthField<SizeOfMapLengthFieldValue>,
           SizeOfStringLengthField<SizeOfStringLengthFieldValue>, SizeOfStructLengthField<SizeOfStructLengthFieldValue>,
           SizeOfUnionLengthField<SizeOfUnionLengthFieldValue>,
           SizeOfUnionTypeSelectorField<SizeOfUnionTypeSelectorFieldValue>, StringIsBomActiveIn,
           StringIsNullTerminationActiveIn, StringEncodingIn>> {
  // validation
  static_assert(std::is_same<ByteOrderIn, LittleEndian>::value || std::is_same<ByteOrderIn, BigEndian>::value ||
                    std::is_same<ByteOrderIn, MachineSpecificEndianness>::value,
                "Invalid type");

  static_assert(std::is_same<StringIsBomActiveIn, StringBomActive>::value ||
                    std::is_same<StringIsBomActiveIn, StringBomInactive>::value,
                "Invalid type");

  static_assert(std::is_same<StringIsNullTerminationActiveIn, StringNullTerminationActive>::value ||
                    std::is_same<StringIsNullTerminationActiveIn, StringNullTerminationInactive>::value,
                "Invalid type");

  static_assert(std::is_same<StringEncodingIn, Utf8Encoding>::value ||
                    std::is_same<StringEncodingIn, Utf16Encoding>::value,
                "Invalid type");

  /*! \brief The byte order. */
  using ByteOrder = ByteOrderIn;

  /*! \brief Size of array length field. */
  constexpr static std::size_t kSizeOfArrayLengthField{SizeOfArrayLengthFieldValue};
  /*! \brief Size of vector length field. */
  constexpr static std::size_t kSizeOfVectorLengthField{SizeOfVectorLengthFieldValue};
  /*! \brief Size of map length field. */
  constexpr static std::size_t kSizeOfMapLengthField{SizeOfMapLengthFieldValue};
  /*! \brief Size of string length field. */
  constexpr static std::size_t kSizeOfStringLengthField{SizeOfStringLengthFieldValue};
  /*! \brief Size of struct length field. */
  constexpr static std::size_t kSizeOfStructLengthField{SizeOfStructLengthFieldValue};
  /*! \brief Size of union length field. */
  constexpr static std::size_t kSizeOfUnionLengthField{SizeOfUnionLengthFieldValue};
  /*! \brief Size of type selector field in variant context. */
  constexpr static std::size_t kSizeOfUnionTypeSelectorField{SizeOfUnionTypeSelectorFieldValue};

  /*! \brief True if byte order mark (BOM) is used. */
  using StringIsBomActive = StringIsBomActiveIn;

  /*! \brief True if null termination character is used. */
  using StringIsNullTerminationActive = StringIsNullTerminationActiveIn;

  /*! \brief The String encoding type */
  using StringEncoding = StringEncodingIn;
};

/*!
 * \internal
 * "SizeToken" is equivalent/same on both serialization/deserialization but moving it to
 * "someip-protocol/internal/types.h", causes hard to debug argument-dependent-lookup (ADL) issues on the code given the
 * current (3-2022 implementation), so it's kept here.
 * \endinternal
 */

/*!
 * \brief Encapsulates the size information of the underlying type that needs to be deserialized / validated.
 * \tparam T Type.
 */
template <typename T>
struct SizeToken {};

/*!
 * \brief Return false, if T is anything else but SizeToken.
 * \tparam T Type.
 */
template <typename T>
struct IsSizeToken : std::false_type {};

/*!
 * \brief Return true, if T is a SizeToken.
 * \tparam T Type.
 */
template <typename T>
struct IsSizeToken<SizeToken<T>> : std::true_type {};

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_TYPES_H_
