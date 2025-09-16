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
/*!        \file is_static_size.h
 *        \brief Defines the IsStaticSizeForConf and ValidateBufferSize functions for all supported primitives and
 *               containers.
 *      \details Types with conditional length fields require an instantiation of the LengthSize tag type. Containers
 *               use a parameter pack named ItemConf which must contain all configuration required to validate an item.
 *               This function is an optional optimization that is not necessary to get performance improvements from
 *               separating size checks from actual deserialization/serialization.
 *      \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_IS_STATIC_SIZE_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_IS_STATIC_SIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <string>
#include <type_traits>
#include "amsr/core/optional.h"
#include "ara/core/array.h"
#include "ara/core/map.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "serialization/ser_forward.h"
#include "someip-protocol/internal/serialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

namespace detail {
/*!
 * \brief Checks whether primitive type data has static size.
 * \details Arrays are static size if they have no length field and their contained
 *          item type is static size. Vector, Map, and String are never static size because they require a length field
 *          according to SOME/IP specification. Arrays with static length are the only containers which are allowed to
 *          omit a length field. Variant is also allowed to omit a length field, but is never static size because of its
 *          dynamic type.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \return Always returns true for primitive data type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename T>
constexpr auto IsStaticSizeForConf(SizeToken<T>) noexcept -> std::enable_if_t<IsPrimitiveType<T>::value, bool> {
  return true;
}

/*!
 * \brief Checks whether enum type data has static size.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \return Always returns true for enum type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename T>
constexpr auto IsStaticSizeForConf(SizeToken<T>) noexcept -> std::enable_if_t<IsEnumType<T>::value, bool> {
  return true;
}

/*!
 * \brief Checks if the user struct type has static size.
 * \details This overload is less specialized than all competing overloads and is used for user structs. User structs
 *          are possibly static size if they have no length field. Calls into user generated code to find out.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \return True when the user struct has static size, and false otherwise.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename... Conf, typename T>
constexpr auto IsStaticSizeForConf(SizeToken<T>) noexcept
    -> std::enable_if_t<(!IsPrimitiveType<T>::value) && (!IsEnumType<T>::value) && (!traits::IsBasicString<T>::value),
                        bool> {
  return (LengthConf::value == 0) && (IsStaticSize<TpPack>(SizeToken<T>{}));
}

/*!
 * \brief Checks if the array type has static size.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \tparam N Size of the array.
 * \return True when the array has static size, and false otherwise.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename LengthConf, typename... Conf, typename T, std::size_t N>
constexpr bool IsStaticSizeForConf(SizeToken<ara::core::Array<T, N>>) noexcept {
  return (LengthConf::value == 0) && (IsStaticSize<TpPack, Conf...>(SizeToken<T>{}));
}

/*!
 * \brief Get size property of non-TLV Optional. Refer to the size property of its contained type.
 * \details This is NOT how TLV treat Optionals. Instead this implementation is here to allow compilation, and
 * NOT usage, of modeled data types with optional members. These are used for signal-based update bits. The
 * Signal2ServiceSerDes generator will create serializers/deserializers for this use-case. However, the SomeIpProtocol
 * generator will also generate serializers/deserializers. These will never be used, but still have to compile.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the contained item.
 * \tparam T Contained type.
 * \return True if the contined type has static size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Get the size property of the contained type.
 * - Assert that it has a static size.
 * \endinternal
 */
template <typename TpPack, typename... Conf, typename T>
constexpr bool IsStaticSizeForConf(SizeToken<ara::core::Optional<T>>) noexcept {
  constexpr bool result{IsStaticSize<TpPack, Conf...>(SizeToken<T>{})};
  static_assert(
      result,
      "Contained type must have static size. Non-TLV optionals for signal-based update bits require static size.");
  return result;
}

/*!
 * \brief Checks if the vector type has static size.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \tparam Alloc Vector allocator.
 * \return Always return false for vector type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename T, typename Alloc>
constexpr bool IsStaticSizeForConf(SizeToken<ara::core::Vector<T, Alloc>>) noexcept {
  return false;
}

/*!
 * \brief Checks if the map type has static size.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \tparam K Type of used keys.
 * \tparam V Type of values.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \return Always return false for map type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename K, typename V, typename C, typename Alloc>
constexpr bool IsStaticSizeForConf(SizeToken<ara::core::Map<K, V, C, Alloc>>) noexcept {
  return false;
}

/*!
 * \brief Checks if the string type has static size.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam String String type.
 * \return Always return false for string type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename String,
          std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
constexpr bool IsStaticSizeForConf(SizeToken<String>) noexcept {
  return false;
}

/*!
 * \brief Checks if the variant type has static size.
 * \details SOME/IP says valueless_by_exception is allowed. This ensures that even a variant with a single allowed type
 *          is not static size.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam Ts Type of the member.
 * \return Always return false for variant type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Conf, typename... Ts>
constexpr bool IsStaticSizeForConf(SizeToken<ara::core::Variant<Ts...>>) noexcept {
  return false;
}

}  // namespace detail

/*!
 * \brief Implementation of forward declared top-level symbol.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Configuration of the members.
 * \tparam T Type of the member.
 * \return True when the member has static size, and false otherwise.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... Confs, typename T>
constexpr bool IsStaticSize(SizeToken<T>) noexcept {
  return detail::IsStaticSizeForConf<TpPack, Confs...>(SizeToken<T>{});
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_IS_STATIC_SIZE_H_
