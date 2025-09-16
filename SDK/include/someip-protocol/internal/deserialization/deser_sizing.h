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
/**        \file deser_sizing.h
 *        \brief Static size checking functionality.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SIZING_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SIZING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {

// ----------------------------------Primitive Start---------------------------------

/*!
 * \brief Get the static size for primitive data type.
 * \tparam TpPack Transformation Property.
 * \tparam Endian Endianness information
 * \tparam Primitive Type of primitive.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename Endian, typename Primitive>
constexpr auto SomeIpProtocolGetStaticSize(SizeToken<Primitive>) noexcept
    -> std::enable_if_t<IsPrimitiveType<Primitive>::value, std::size_t> {
  return sizeof(Primitive);
}

/*!
 * \brief Get the static size for bool data type.
 * \tparam TpPack Transformation Property.
 * \tparam Endian Endianness information
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - The value of sizeof(bool) is implementation defined and might differ from 1.
 * - But for SOME/IP the serialized size of a bool is 1 byte.
 * \endinternal
 */
template <typename TpPack, typename Endian>
constexpr auto SomeIpProtocolGetStaticSize(SizeToken<bool>) noexcept -> std::size_t {
  return sizeof(std::uint8_t);
}

// ----------------------------------Primitive End---------------------------------

// ----------------------------------Enum Start---------------------------------
/*!
 * \brief Get the static size for Enum data type.
 * \tparam TpPack Transformation Property.
 * \tparam Endian Endianness information
 * \tparam EnumType Type of enum.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR AV NC Architecture-ApiAnalysis Template_Argument: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename Endian, typename EnumType>
constexpr auto SomeIpProtocolGetStaticSize(SizeToken<EnumType>) noexcept
    -> std::enable_if_t<IsEnumType<EnumType>::value, std::size_t> {
  using UnderlyingType = typename std::underlying_type<EnumType>::type;
  return sizeof(UnderlyingType);
}

// ----------------------------------Enum End---------------------------------

// ----------------------------------Struct Start---------------------------------

/*!
 * \brief Calculate the static size for struct data type.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the struct.
 * \tparam T Struct type.
 */
template <typename TpPack, typename LengthConf>
class SomeIpProtocolGetStaticSizeForUserStruct {
 public:
  /*!
   * \brief Get the static size for struct data type.
   * \details Calls the generated functions.
   * \tparam T Type of struct.
   * \return Returns the calculated size.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  static constexpr std::size_t GetSize(SizeToken<T>) noexcept {
    return LengthConf::value;
  }
};

/*!
 * \brief Calculate the static size for struct data type.
 * \details Specialization for length field zero.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the struct.
 */
template <typename TpPack, typename Endian>
class SomeIpProtocolGetStaticSizeForUserStruct<TpPack, LengthSize<0, Endian>> {
 public:
  /*!
   * \brief Get the static size for struct data type with length field zero.
   * \details Calls the generated functions.
   * \tparam T Type of struct.
   * \return Returns the calculated size.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  static constexpr std::size_t GetSize(SizeToken<T>) noexcept {
    return SomeIpProtocolGetStaticSize<TpPack>(SizeToken<T>{});  // NOTE calls back to the generated function.
  }
};

/*!
 * \brief Catch-all for user generated data types.
 * \details This is less specialized than other templates, and will only be chosen when
 *          no other signature matches
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the struct.
 * \tparam UserStruct Struct type.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename UserStruct>
constexpr auto SomeIpProtocolGetStaticSize(SizeToken<UserStruct>) noexcept
    -> std::enable_if_t<(!IsPrimitiveType<UserStruct>::value) && (!IsEnumType<UserStruct>::value) &&
                            (!traits::IsBasicString<UserStruct>::value),
                        std::size_t> {
  return SomeIpProtocolGetStaticSizeForUserStruct<TpPack, LengthConf>::GetSize(SizeToken<UserStruct>{});
}

// ----------------------------------Struct End---------------------------------

// ----------------------------------Array Start---------------------------------

/*!
 * \brief Utility struct to calculate static size of array with length field.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf LengthField configuration for the array.
 * \tparam ItemConf 0...n Configuration (holding Length Size and/ or endianness) for the sub-elements.
 */
template <typename TpPack, typename LengthConf, typename... ItemConf>
class SomeIpProtocolGetStaticSizeArray {
 public:
  /*!
   * \brief Get the static size for array data type.
   * \tparam T Array type.
   * \tparam N Number of array elements.
   * \return Returns the calculated size.
   * \pre -
   * \context   Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, std::size_t N>
  static constexpr std::size_t GetSize(SizeToken<ara::core::Array<T, N>>) noexcept {
    return LengthConf::value;
  }
};

/*!
 * \brief Specialized utility struct to calculate statically configured arrays without a length field.
 * \tparam TpPack Transformation Property.
 * \tparam Endian Endianness information for the array.
 * \tparam ItemConf Configuration (holding Length Size and/ or endianness) for the sub-elements.
 */
template <typename TpPack, typename Endian, typename... ItemConf>
class SomeIpProtocolGetStaticSizeArray<TpPack, LengthSize<0, Endian>, ItemConf...> {
 public:
  /*!
   * \brief Calculate the static size for array data type.
   * \details Arrays are the only containers which may skip the length field.
   * \tparam T Array type.
   * \tparam N Number of array elements.
   * \return Returns the calculated size
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, std::size_t N>
  static constexpr std::size_t GetSize(SizeToken<ara::core::Array<T, N>>) noexcept {
    /*! Calculate size of all subelement */
    constexpr std::size_t static_size{(SomeIpProtocolGetStaticSize<TpPack, ItemConf...>(SizeToken<T>{})) * N};
    return static_size;
  }
};

/*!
 * \brief Get the static size for array data type.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf LengthField configuration for the array.
 * \tparam Conf Configuration for the subelements.
 * \tparam T Array type.
 * \tparam N Number of array elements.
 * \param t SizeToken encapsulating the type information of an array including number of elements and
 *          underlying type.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename... Conf, typename T, std::size_t N>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<ara::core::Array<T, N>> t) noexcept {
  return SomeIpProtocolGetStaticSizeArray<TpPack, LengthConf, Conf...>::GetSize(t);
}

// ----------------------------------Array Start---------------------------------

// ----------------------------------Vector Start---------------------------------

/*!
 * \brief  Get the static size for vector data type.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the vector.
 * \tparam Conf ItemConf Configuration for the sub-elements.
 * \tparam T data type.
 * \tparam Alloc Vector allocator.
 * \return Returns the static size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename... Conf, typename T, typename Alloc>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<ara::core::Vector<T, Alloc>>) noexcept {
  return LengthConf::value;
}

// ----------------------------------Vector End---------------------------------

// ----------------------------------Map Start----------------------------------

/*!
 * \brief Get the static size for map data type.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length configuration of the map itself.
 * \tparam ValueConf Determines the value length field size and endianness.
 * \tparam K Type of key.
 * \tparam V Type of value.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \return Returns the static size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename LengthConf, typename... ItemConf, typename K, typename V, typename C,
          typename Alloc>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<ara::core::Map<K, V, C, Alloc>>) noexcept {
  return LengthConf::value;
}

// ----------------------------------Map End--------------------------------------

// ----------------------------------String Start---------------------------------

/*!
 * \brief Get the static size for string data type.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the vector.
 * \tparam String String type.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename String,
          std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<String>) noexcept {
  return LengthConf::value;
}

// ----------------------------------String End---------------------------------

// ----------------------------------Variant Start------------------------------

/*!
 * \brief Get the static size for the given size information of a variant.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length information of the variant.
 * \tparam ItemConf Item configuration of the sub-elements.
 * \tparam Ts Types of the underlying variants of the union.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename... Ts>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<ara::core::Variant<Ts...>>) noexcept {
  return LengthConf::value;
}

// ----------------------------------Variant End------------------------------------

// ----------------------------------TLV Start--------------------------------------

/*!
 * \brief Get the static size for TLV struct.
 * \tparam TpPack Transformation Property.
 * \tparam LengthConf Length field configuration for the vector.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf>
constexpr std::size_t GetSize() noexcept {
  return LengthConf::value;
}

// ----------------------------------TLV End--------------------------------------

/*!
 * \brief Get the static size for Optional of type T.
 * \tparam TpPack Transformation Property.
 * \tparam Conf Length configurations of sub-elements.
 * \tparam T Encapsulated type.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename T>
constexpr std::size_t SomeIpProtocolGetStaticSize(SizeToken<ara::core::Optional<T>>) noexcept {
  return SomeIpProtocolGetStaticSize<TpPack, Conf...>(SizeToken<T>{});
}

/*!
 * \brief Get static size of type T.
 * \details Called when the passed data type reference is not valid. (From generated code)
 * \tparam TpPack Transformation Property.
 * \tparam Head Length field/ Endianess configuration for the data type.
 * \tparam Confs Configuration for the subelements.
 * \tparam T data type.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Wrap T into a SizeToken and call get static size of SizeToken<T>.
 * \endinternal
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr auto SomeIpProtocolGetStaticSizeTokenCheck(T const&) noexcept
    -> std::enable_if_t<!IsSizeToken<std::decay_t<T>>::value, std::size_t> {
  return detail::SomeIpProtocolGetStaticSize<TpPack, Head, Confs...>(SizeToken<std::decay_t<T>>{});
}

/*!
 * \brief Get static size of size token.
 * \details Called when the passed data type reference is valid. (From static code)
 * \tparam TpPack Transformation Property.
 * \tparam Head Length field/ Endianess configuration for the data type.
 * \tparam Confs Configuration for the subelements.
 * \tparam T data type.
 * \param[in] t SizeToken encapsulating the type information.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr auto SomeIpProtocolGetStaticSizeTokenCheck(T const& t) noexcept
    -> std::enable_if_t<IsSizeToken<std::decay_t<T>>::value, std::size_t> {
  return detail::SomeIpProtocolGetStaticSize<TpPack, Head, Confs...>(t);
}

}  // namespace detail

/*!
 * \brief   Work around to make argument-dependent lookup work.
 * \details This function definition is required to introduce SomeIpProtocolGetStaticSize
 *          to the compiler, so that argument-dependent lookup can run.
 * \tparam  K Dummy template argument.
 * \return  The calculated size.
 * \pre     -
 * \context Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
template <std::int32_t K>
constexpr std::size_t SomeIpProtocolGetStaticSize() noexcept;

/*!
 * \brief Get static size of T.
 * \details Implementation of forward declared top-level symbol.
 * \tparam TpPack Transformation Property.
 * \tparam Head Length field/ Endianess configuration for the data type.
 * \tparam Confs Configuration for the sub-elements.
 * \tparam T data type.
 * \param[in] t Data element.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr std::size_t SomeIpProtocolGetStaticSize(T const& t) noexcept {
  return detail::SomeIpProtocolGetStaticSizeTokenCheck<TpPack, Head, Confs...>(t);
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SIZING_H_
