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
/*!        \file ser_array.h
 *        \brief Defines the serialization function for the value of an array.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_ARRAY_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/array.h"
#include "ser_forward.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serializes an array. This handles only the value part of the array, and not any possible length field.
 * \tparam TpPack Transformation properties parameter pack.
 * \tparam ItemConf Configuration parameters required to serialize an element.
 * \tparam T Array element type.
 * \tparam N Number of elements in the array.
 * \param[in] array Array containing the data to be serialized.
 * \param[out] writer Writer holding the bufferView to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Loop over all array elements and serialize each.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T, std::size_t N>
static auto serializeArray(ara::core::Array<T, N> const& array, Writer& writer) noexcept
    -> std::enable_if_t<!TypeTraitIsOptimizableContainer<T, ItemConf...>(), void> {
  for (T const& item : array) {
    // clang-format off
    SomeIpProtocolSerialize<TpPack, ItemConf...>(writer, item); // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
    // clang-format on
  }
}

/*!
 * \brief Specialization for an array consisting of an arithmetic type with same endianness.
 * \tparam TpPack Transformation property. Not required in this case.
 * \tparam ItemConf Length size config pack. Not required in this case.
 * \tparam N Size of the array in bytes.
 * \param[in] array Array containing the data to be serialized.
 * \param[out] writer Writer holding the bufferView to to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Serialize array.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T, std::size_t N>
static auto serializeArray(ara::core::Array<T, N> const& array, Writer& writer) noexcept
    -> std::enable_if_t<TypeTraitIsOptimizableContainer<T, ItemConf...>(), void> {
  writer.writeArray(array);  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_ARRAY_H_
