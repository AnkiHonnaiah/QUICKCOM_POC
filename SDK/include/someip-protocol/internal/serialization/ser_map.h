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
/*!        \file ser_map.h
 *        \brief Defines the serialization function for the value of a map.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_MAP_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/core/map.h"
#include "ser_forward.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

namespace detail {

/*!
 * \brief Primary struct template to serialize a Map.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConf Determines the key length field size and endianness.
 * \tparam ValueConf Determines the value length field size and endianness.
 */
template <typename TpPack, typename KeyConf, typename ValueConf>
class SerializeMap;

/*!
 * \brief Serialize Map.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConfs Parameter pack for LengthSize configs for map-keys.
 * \tparam ValueConfs Parameter pack for LengthSize configs for map-values.
 */
template <typename TpPack, typename... KeyConfs, typename... ValueConfs>
class SerializeMap<TpPack, ConfPack<KeyConfs...>, ConfPack<ValueConfs...>> {
 public:
  /*!
   * \brief Serialize Map.
   * \tparam K Type of key.
   * \tparam V Type of value.
   * \param[in] v The pair to be serialized.
   * \param[in, out] w The writer holding the buffer to which the pair is serialized.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Serialize key.
   * - Serialize value.
   * \endinternal
   */
  template <typename K, typename V>
  static void serialize(std::pair<K, V> const& v, Writer& w) noexcept {
    SomeIpProtocolSerialize<TpPack, KeyConfs...>(w, v.first);
    SomeIpProtocolSerialize<TpPack, ValueConfs...>(w, v.second);
  }
};

}  // namespace detail

/*!
 * \brief Entry point to serialize a map.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConf Determines the key length field size and endianness.
 * \tparam ValueConf Determines the value length field size and endianness.
 * \tparam K Type of key.
 * \tparam V Type of value.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \param[in] v Map that should be serialized.
 * \param[in, out] w Writer holding the buffer to which the data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Loop over the map and serialize each key/value pair.
 * \endinternal
 */
template <typename TpPack, typename KeyConf, typename ValueConf, typename K, typename V, typename C, typename Alloc>
static void serializeMap(ara::core::Map<K, V, C, Alloc> const& v, Writer& w) noexcept {
  for (std::pair<K, V> const& keyValue : v) {  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
    // clang-format off
    detail::SerializeMap<TpPack, KeyConf, ValueConf>::serialize(keyValue, w); // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
    // clang-format on
  }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_MAP_H_
