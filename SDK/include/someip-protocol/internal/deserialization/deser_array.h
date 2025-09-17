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
/**        \file deser_array.h
 *        \brief Contains static functions to deserialize arrays.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_ARRAY_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "ara/core/array.h"
#include "deser_forward.h"
#include "deser_sizing.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief Deserializes the array according to the given transformation properties and item configuration.
 * \details Deserialize arrays with different endianness or non-arithmetic arrays.
 * \tparam TpPack Transformation property. Not required in this case.
 * \tparam ItemConf Endianness.
 * \tparam T Type of objects stored in the given array.
 * \tparam N Size of the array in bytes.
 * \param[out] array Array the deserialized value shall be stored into.
 * \param[in,out] reader Reader holding the bufferView on the serialized data.
 * \return true if deserialization was successful, false otherwise.
 * \pre It has been verified that least N bytes are present.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Deserialize each element of the array.
 * - Return false if any error occurs.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T, std::size_t N>
static auto DeserializeArray(ara::core::Array<T, N>& array, Reader& reader) noexcept
    -> std::enable_if_t<!TypeTraitIsOptimizableContainer<T, ItemConf...>(), Result> {
  Result result{true};
  for (T& item : array) {
    // clang-format off
    result = SomeIpProtocolDeserialize<TpPack, ItemConf...>(reader, item); // VCA_SOMEIPPROTOCOL_DESER_ARRAY_READER_REFERENCE
    // clang-format on
    if (!result) {
      break;
    }
  }

  return result;
}

/*!
 * \brief Specialized deserialization function to deserialize arithmetic arrays with same endianness.
 * \tparam TpPack Transformation property. Not required in this case.
 * \tparam ItemConf Endianness.
 * \tparam T Type of objects stored in the given array.
 * \tparam N Size of the array in bytes.
 * \param[out] array Array the deserialized value shall be stored into.
 * \param[in,out] reader Reader holding the bufferView on the serialized data.
 * \return Always return true.
 * \pre It has been verified that least N bytes are present.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Store serialized value into destination array.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T, std::size_t N>
static auto DeserializeArray(ara::core::Array<T, N>& array, Reader& reader) noexcept
    -> std::enable_if_t<TypeTraitIsOptimizableContainer<T, ItemConf...>(), Result> {
  reader.ReadArray(array);  // VCA_SOMEIPPROTOCOL_DESER_ARRAY_REFERENCE

  return true;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_ARRAY_H_
