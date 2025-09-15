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
/**        \file deser_map.h
 *        \brief Contains static functions to deserialize maps.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_MAP_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/core/map.h"
#include "deser_forward.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {

/*!
 * \brief Primary struct template to deserialize a Map.
 * \details Required as multiple parameter packs are only permitted for specialized class templates.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConfs Determines the key length field size and endianness.
 * \tparam ValueConfs Determines the value length field size and endianness.
 */
template <typename TpPack, typename KeyConf, typename ValueConf>
class DeserializeMapImpl;

/*!
 * \brief Specialized template struct to deserialize maps.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConfs Parameter pack for LengthSize configs for map-keys.
 * \tparam ValueConfs Parameter pack for LengthSize configs for map-values.
 */
template <typename TpPack, typename... KeyConfs, typename... ValueConfs>
class DeserializeMapImpl<TpPack, ConfPack<KeyConfs...>, ConfPack<ValueConfs...>> {
 public:
  /*!
   * \brief Deserialize one entry of the provided map.
   * \tparam K Type of key.
   * \tparam V Type of value.
   * \tparam C Comparison functor type.
   * \tparam Alloc Allocator type.
   * \param[out] v Map the serialized data shall deserialized into.
   * \param[in,out] r Reader holding the serialized data.
   * \return true if deserialization was successful, false otherwise.
   * \pre It has been verified, that the buffer contains at least the number of bytes provided by
   *      GetKeyValueStaticSize().
   * \context Reactor|App
   * \reentrant TRUE for different reader objects.
   *
   * \internal
   * - Deserialize key and value.
   * \endinternal
   */
  template <typename K, typename V, typename C, typename Alloc>
  static Result Deserialize(ara::core::Map<K, V, C, Alloc>& v, Reader& r) noexcept {
    K key{};
    // Deserialize map key
    Result const deser_key_result{SomeIpProtocolDeserialize<TpPack, KeyConfs...>(r, key)};
    Result deser_value_result{false};
    // Deserialize map value
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (deser_key_result == true) {
      // clang-format off
      deser_value_result = SomeIpProtocolDeserialize<TpPack, ValueConfs...>(r, v[std::move(key)]);  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      // clang-format on
    }
    return deser_value_result;
  }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

  /*!
   * \brief Get static size of a single key / value pair.
   * \tparam K Type of key.
   * \tparam V Type of value.
   * \tparam C Comparison functor type.
   * \tparam Alloc Allocator type.
   * \return Calculated size.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return sum of static size of key and static size of value.
   * \endinternal
   */
  template <typename K, typename V, typename C, typename Alloc>
  static constexpr std::size_t GetKeyValueStaticSize(SizeToken<ara::core::Map<K, V, C, Alloc>>) noexcept {
    return SomeIpProtocolGetStaticSize<TpPack, KeyConfs...>(SizeToken<K>{}) +
           SomeIpProtocolGetStaticSize<TpPack, ValueConfs...>(SizeToken<V>{});
  }
};

}  // namespace detail

/*!
 * \brief Entry point to deserialize a map.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam KeyConf Determines the key length field size and endianness.
 * \tparam ValueConf Determines the value length field size and endianness.
 * \tparam K Type of key.
 * \tparam V Type of value.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \param[out] v Map the serialized data shall deserialized into.
 * \param[in,out] r Reader holding the serialized data.
 * \return true if deserialization was successful, false otherwise.
 * \pre Corresponding length field value has been verified.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - For each key/value pair
 *   - If static size of key/value pair can be verified
 *     - Deserialize key/value pair.
 *   - Otherwise deserialization fails.
 * - If bytes are still present, deserialization fails.
 * \endinternal
 */
template <typename TpPack, typename KeyConf, typename ValueConf, typename K, typename V, typename C, typename Alloc>
static Result DeserializeMap(ara::core::Map<K, V, C, Alloc>& v, Reader& r) noexcept {
  Result result{true};
  // Calculate the static size of a single map key / value pair
  constexpr SizeType static_size{detail::DeserializeMapImpl<TpPack, KeyConf, ValueConf>::GetKeyValueStaticSize(
      SizeToken<ara::core::Map<K, V, C, Alloc>>{})};

  // Deserialize key/value pairs until no more pair is remaining in the byte stream.
  while (r.VerifySize(static_size)) {
    result = detail::DeserializeMapImpl<TpPack, KeyConf, ValueConf>::Deserialize(v, r);
    if (!result) {
      break;
    }
  }

  // Check that all the elements are deserialized, if not then we must return false.
  result = result && (r.Size() == 0U);

  return result;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_MAP_H_
