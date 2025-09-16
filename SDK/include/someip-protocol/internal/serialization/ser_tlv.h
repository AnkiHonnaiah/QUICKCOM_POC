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
/*!        \file ser_tlv.h
 *        \brief Defines functions for serializing a TLV structure.
 *        \unit SomeIpProtocol::Serdes::TLV
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_TLV_H_
#define LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_TLV_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <type_traits>
#include <utility>
#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/serialization/ser_lengthfield.h"
#include "someip-protocol/internal/serialization/ser_wrapper.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {
namespace detail {

/*!
 * \brief Alias for the size of the length field.
 */
using LengthFieldSize = std::size_t;

/*!
 * \brief   Maps primitive size to WireType.
 * \tparam  Size The size of the primitive wire type.
 */
template <std::size_t Size>
struct PrimitiveWireType;

/*! \brief Maps primitive size to WireType. */
template <>
struct PrimitiveWireType<1> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::D8)> {};

/*! \brief Maps primitive size to WireType. */
template <>
struct PrimitiveWireType<2> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::D16)> {};

/*! \brief Maps primitive size to WireType. */
template <>
struct PrimitiveWireType<4> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::D32)> {};

/*! \brief Maps primitive size to WireType. */
template <>
struct PrimitiveWireType<8> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::D64)> {};

/*! \brief Maps length field size to WireType. */
template <std::size_t Size>
struct LengthFieldWireType;

/*! \brief Maps length field size to WireType. */
template <>
struct LengthFieldWireType<1> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::L1)> {};

/*! \brief Maps length field size to WireType. */
template <>
struct LengthFieldWireType<2> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::L2)> {};

/*! \brief Maps length field size to WireType. */
template <>
struct LengthFieldWireType<4> : std::integral_constant<WireType, static_cast<WireType>(WireTypeClass::L4)> {};

/*! \brief Array type for holding the tag bytes. */
using TagArray = ara::core::Array<std::uint8_t, 2>;

/*!
 * \brief Creates a TLV tag from DataId and WireType.
 * \param[in] dataId Data id to use in the tag.
 * \param[in] wireType Wire type to use in the tag.
 * \return An array with two bytes representing the tag.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \trace SPEC-4981328
 * \trace SPEC-4981329
 */
constexpr TagArray createTag(DataId dataId, WireType wireType) {
  return {static_cast<std::uint8_t>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(wireType & 0x07U) << 4U) |
                                    (static_cast<std::uint8_t>(dataId >> 8U) & 0x0FU)),
          static_cast<std::uint8_t>(dataId & 0xFFU)};
}

}  // namespace detail

/*!
 * \brief Base template for TLV serialization.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam T Type of data to be serialized.
 * \tparam IsPrimitive True in case of a primitive.
 * \tparam ItemConfs Configuration parameters required to serialize the item.
 */
template <typename TpPack, DataId dataId, typename T, bool IsPrimitive, typename... ItemConfs>
class SerializeTlvFieldImpl;

/*!
 * \brief Serializes a primitive type using TLV.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam T Type of data to be serialized.
 * \tparam ItemConfs Configuration parameters required to serialize the item.
 */
template <typename TpPack, DataId dataId, typename T, typename... ItemConfs>
class SerializeTlvFieldImpl<TpPack, dataId, T, true, ItemConfs...> {
 public:
  /*!
   * \brief Serializes a primitive type using TLV.
   * \param[in,out] w Writer to serialize to.
   * \param[in] t Data to serialize.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Create tag from configured dataId and wireType.
   * - Serialize tag.
   * - Serialize data.
   * \endinternal
   */
  static void serialize(Writer& w, T t) {
    detail::TagArray const tag{detail::createTag(dataId, detail::PrimitiveWireType<sizeof(std::decay_t<T>)>::value)};
    w.writeArray(tag);
    SomeIpProtocolSerialize<TpPack, ItemConfs...>(w, t);
  }
};

/*!
 * \brief Serializes a non-primitive type using TLV.
 * \details SOME/IP requires length fields before all types except primitives when in TLV context.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam T Type of data to be serialized.
 * \tparam LengthConf Length field configuration holding endianness and length field size information.
 * \tparam ItemConfs Configuration parameters required to serialize the item.
 */
template <typename TpPack, DataId dataId, typename T, typename LengthConf, typename... ItemConfs>
class SerializeTlvFieldImpl<TpPack, dataId, T, false, LengthConf, ItemConfs...> {
 public:
  /*!
   * \brief Serializes a non-primitive type using TLV when dynamic length field size is NOT used.
   * \details If the length field size is zero in the configuration, it will be set to 4 bytes by default. This is
   *          because static length field size is applied when wire type is equal to 4 (so the length field size cannot
   *          be interpreted from the wiretype) and also length field size cannot be zero. In many other types such as
   *          Union and String, a default 4 bytes length field size is used, so we set the length field size to 4 bytes
   *          when we see zero in the configuration.
   * \tparam dynamicLengthFieldSize true if dynamic length field size is used.
   *         Defaults to Tp<TpPack>::DynamicLengthFieldIsSizeActive::value.
   * \param[in,out] w Writer to serialize to.
   * \param[in] t Data to serialize.
   * \return void.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Create tag from configured dataId and wireType.
   * - Serialize tag.
   * - If the length field configuration is 0
   *   - Call serializeWithLengthField with a lengthSize of 4. Supply a lambda function that calls
   *     SomeIpProtocolSerialize with a lengthSize of 0.
   * - Otherwise call serializeWithLengthField with the configured lengthSize. Supply a lambda function that calls
   *   SomeIpProtocolSerialize with a lengthSize of 0.
   * \endinternal
   */
  template <bool dynamicLengthFieldSize = Tp<TpPack>::DynamicLengthFieldIsSizeActive::value>
  static auto serialize(Writer& w, T const& t) noexcept -> std::enable_if_t<!dynamicLengthFieldSize, void> {
    detail::TagArray const tag{detail::createTag(dataId, static_cast<WireType>(WireTypeClass::LCONF))};

    w.writeArray(tag);
    if (LengthConf::value == 0U) {
      serializeWithLengthField<LengthSize<4U, typename Tp<TpPack>::ByteOrder>>(
          w, [&w, &t]() { SomeIpProtocolSerialize<TpPack, LengthSize<0>, ItemConfs...>(w, t); });
    } else {
      serializeWithLengthField<LengthConf>(
          w, [&w, &t]() { SomeIpProtocolSerialize<TpPack, LengthSize<0>, ItemConfs...>(w, t); });
    }
  }

  /*!
   * \brief Serializes a non-primitive type using TLV when dynamic length field size is used.
   * \tparam dynamicLengthFieldSize true if dynamic length field size is used.
   *         Defaults to Tp<TpPack>::DynamicLengthFieldIsSizeActive::value
   * \param[in,out] w Writer to serialize to.
   * \param[in] t Data to serialize.
   * \return void.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Create tag from configured dataId and wireType.
   * - Serialize tag.
   * - Call serializeWithLengthField with a lambda function that calls SomeIpProtocolSerialize with a lengthSize of 0.
   * \endinternal
   */
  template <bool dynamicLengthFieldSize = Tp<TpPack>::DynamicLengthFieldIsSizeActive::value>
  static auto serialize(Writer& w, T const& t) noexcept -> std::enable_if_t<dynamicLengthFieldSize, void> {
    detail::TagArray const tag{detail::createTag(dataId, detail::LengthFieldWireType<LengthConf::value>::value)};

    w.writeArray(tag);
    serializeWithLengthField<LengthConf>(
        w, [&w, &t]() { SomeIpProtocolSerialize<TpPack, LengthSize<0>, ItemConfs...>(w, t); });
  }
};

/*!
 * \brief Serializes a single TLV field.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam ItemConfs Configuration parameters required to serialize the item.
 * \tparam T Type of data to be serialized.
 * \param[in,out] w Writer to serialize to.
 * \param[in] t Data to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, DataId dataId, typename... ItemConfs, typename T>
void SerializeTlvField(Writer& w, T const& t) noexcept {
  // Primitives and Enums does not have a length field
  constexpr bool IsPrimitiveOrEnum{IsPrimitiveType<T>::value || IsEnumType<T>::value};
  SerializeTlvFieldImpl<TpPack, dataId, std::decay_t<T>, IsPrimitiveOrEnum, ItemConfs...>::serialize(w, t);
}

/*!
 * \brief Specialization for optional fields.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam T Type of data to be serialized.
 * \tparam LengthConf Length field configuration holding endianness and length field size information.
 * \tparam ItemConfs Configuration parameters required to serialize the item.
 */
template <typename TpPack, DataId dataId, typename T, typename LengthConf, typename... ItemConfs>
class SerializeTlvFieldImpl<TpPack, dataId, ara::core::Optional<T>, false, LengthConf, ItemConfs...> {
 public:
  /*!
   * \brief Serializes an optional field.
   * \param[in,out] w Writer to serialize to.
   * \param[in] t Data to serialize.
   * \return true if the serialization is successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the optional has a value.
   *   - Call SerializeTlvField with the data in the optional.
   * \endinternal
   */
  static void serialize(Writer& w, ara::core::Optional<T> const& t) {
    if (t) {
      SerializeTlvField<TpPack, dataId, LengthConf, ItemConfs...>(w, *t);
    }
  }
};
}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_TLV_H_
