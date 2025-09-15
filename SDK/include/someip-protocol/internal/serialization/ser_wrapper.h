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
/*!       \file ser_wrapper.h
 *        \brief Defines the serialization function for all supported primitives and
 *               containers. Types with conditional length fields requires an instantiation of
 *               the LengthSize tag type. Containers use a parameter pack named ItemConf
 *               which must contain all configuration required to serialize an item.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *        \complexity High metrics are expected for complex data structures due to the nature of the recursive design.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_WRAPPER_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>
#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "ara/core/array.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "ser_array.h"
#include "ser_lengthfield.h"
#include "ser_map.h"
#include "ser_sizing.h"
#include "ser_string.h"
#include "ser_variant.h"
#include "ser_vector.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

namespace detail {

/*!
 * \brief Calls the serialization function for a struct with a length field.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam UserStruct Type of struct.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] s Structure to serialize.
 * \return void.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Call serializeWithLengthField with a lambda function calling SomeIpProtocolSerialize for the user struct.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename UserStruct>
auto SomeIpProtocolSerialize(Writer& w, UserStruct const& s) noexcept
    -> std::enable_if_t<(!IsPrimitiveType<UserStruct>::value) && (!IsEnumType<UserStruct>::value) &&
                            (!traits::IsBasicString<UserStruct>::value),
                        void> {
  // clang-format off
  serializeWithLengthField<LengthConf>(w, [&s, &w]() { SomeIpProtocolSerialize<TpPack>(w, s); }); // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  // clang-format on
}

/*!
 * \brief Calls the serialization function for a primitive.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Endianness.
 * \tparam Primitive Type of primitive.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] t Primitive to serialize.
 * \return void.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Conf, typename Primitive>
auto SomeIpProtocolSerialize(Writer& w, Primitive const& t) noexcept
    -> std::enable_if_t<IsPrimitiveType<Primitive>::value, void> {
  w.writePrimitive<std::decay_t<Primitive>, Conf>(t);  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
}

/*!
 * \brief Calls the serialization function for a enum.
 * \tparam TpPack Transformation Property.
 * \tparam Conf Endianness.
 * \tparam EnumType Type of enum.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] t Enum, the data to serialize.
 * \return void.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Cast enum the its underlying type and write it to the buffer.
 * \endinternal
 */
template <typename TpPack, typename Conf, typename EnumType>
auto SomeIpProtocolSerialize(Writer& w, EnumType const& t) noexcept
    -> std::enable_if_t<IsEnumType<EnumType>::value, void> {
  using UnderlyingType = typename std::underlying_type<EnumType>::type;
  UnderlyingType const enumValue{static_cast<UnderlyingType>(t)};
  w.writePrimitive<UnderlyingType, Conf>(enumValue);
}

/*!
 * \brief Calls the serialization function for an array.
 * \details No support for array length fields available. See AMSR-24446.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the array elements holding (length field size and endianness info).
 * \tparam T Type of the array.
 * \tparam N Size of the array.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] v Array to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
// VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
template <typename TpPack, typename LengthConf, typename... ItemConf, typename T, std::size_t N>
void SomeIpProtocolSerialize(Writer& w, ara::core::Array<T, N> const& v) noexcept {
  serializeWithLengthField<LengthConf>(
      w, [&w, &v]() { return serializeArray<TpPack, ItemConf...>(v, w); });  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
}

/*!
 * \brief Calls the serialization function for a string.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam String String type.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] v String to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Call serializeWithLengthField with a lambda function that serializes the string.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename String,
          std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
void SomeIpProtocolSerialize(Writer& w, String const& v) noexcept {
  // clang-format off
  serializeWithLengthField<LengthConf>(w, [&w, &v]() { serializeString<TpPack>(v, w); });  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  // clang-format on
}

/*!
 * \brief Calls the serialization function for a SOME/IP union.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Configuration of the sub-elements of the union.
 * \tparam Ts Type of the underlying variants.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] v Variant to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Call serializeWithLengthField with a lambda function that serializes the variant.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename LengthConf, typename... ItemConf, typename... Ts>
void SomeIpProtocolSerialize(Writer& w, ara::core::Variant<Ts...> const& v) noexcept {
  // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  constexpr std::size_t VarTypeSize{Tp<TpPack>::kSizeOfUnionTypeSelectorField};
  // clang-format off
  serializeWithLengthField<LengthConf, VarTypeSize>(w, [&w, &v]() { serializeVariant<TpPack, ItemConf...>(v, w); });  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  // clang-format on
}

// VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.8_VoidFunctionHasSideEffectAbort
// VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M7.1.2_ParameterDeclaredAsConst
/*!
 * \brief Serialize non-TLV Optional.
 * \details This is NOT how TLV treat Optionals. Instead this implementation is here to allow compilation, and
 * NOT usage, of modeled data types with optional members. These are used for signal-based update bits. The
 * Signal2ServiceSerDes generator will create serializers/deserializers for this use-case. However, the SomeIpProtocol
 * generator will also generate serializers/deserializers. These will never be used, but still have to compile.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Configuration of the contained item.
 * \tparam T Contained type.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] o Optional to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - If the optional has a value
 *    - use that for serialization.
 * - Else
 *    - use a default value for serialization.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T>
void SomeIpProtocolSerialize(Writer& w, ara::core::Optional<T> const& o) noexcept {
  static_cast<void>(o);
  static_cast<void>(w);
  amsr::core::Abort(
      "Non-TLV optional is not supported for SOME/IP. This code should compile, to allow for signal-based update "
      "bits.");
}

/*!
 * \brief Calls the serialization function for a vector.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] v Vector to serialize.
 * \pre           -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Call serializeWithLengthField with a lambda function that serializes the vector.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename T, typename Alloc>
void SomeIpProtocolSerialize(Writer& w, ara::core::Vector<T, Alloc> const& v) noexcept {
  // clang-format off
  serializeWithLengthField<LengthConf>(w, [&w, &v]() { return serializeVector<TpPack, ItemConf...>(v, w); }); // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  // clang-format on
}

/*!
 * \brief Calls the serialization function for a Map.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Configurations holding endianness and length information for keys and values.
 * \tparam K Type of used keys.
 * \tparam V Type of values.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] v Map to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Call serializeWithLengthField with a lambda function that serializes the map.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename K, typename V, typename C,
          typename Alloc>
void SomeIpProtocolSerialize(Writer& w, ara::core::Map<K, V, C, Alloc> const& v) noexcept {
  // clang-format off
  serializeWithLengthField<LengthConf>(w, [&w, &v]() { return serializeMap<TpPack, ItemConf...>(v, w); }); // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
  // clang-format on
}

}  // namespace detail

/*!
 * \brief Implementation of forward declared top-level symbol.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration holding length field info and/or endianness info of the sub-elements.
 * \tparam Ts Serialization types.
 * \param[in,out] w Writer containing the buffer to serialize to.
 * \param[in] ts Types to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename... Ts>
void SomeIpProtocolSerialize(Writer& w, Ts const&... ts) noexcept {
  // clang-format off
  detail::SomeIpProtocolSerialize<TpPack, Head, Confs...>(w, ts...);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  // clang-format on
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_WRAPPER_H_
