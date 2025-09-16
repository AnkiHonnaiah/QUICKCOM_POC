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
/*!        \file ser_forward.h
 *        \brief Forward declaration of the top level Serialize template.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_FORWARD_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_FORWARD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Forward declaration of the top level IsStaticSize template.
 * \tparam TpPack Transformation properties.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \return True if the passed data type has static size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename... Confs, typename T>
bool constexpr IsStaticSize(SizeToken<T>) noexcept;

/*!
 * \brief Forward declaration of the top level Serialize template.
 * \details Specializations of this template are made for all supported data types.
 * \tparam TpPack Transformation properties
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members
 * \tparam Ts Type of the member.
 * \param[in,out] w Writer holding the bufferView to which the data is wrote.
 * \param[in] ts Source of the data to be serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename... Ts>
void SomeIpProtocolSerialize(Writer& w, Ts const&... ts) noexcept;

namespace detail {

/*!
 * \brief Forward declaration of the top level GetRequiredBufferSize template for static size types.
 * \tparam IterCfg Iteration config.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for serialization.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Head, typename... Confs, typename T>
auto constexpr GetBufferSizeRoot(T const& t) noexcept
    -> std::enable_if_t<IsStaticSize<TpPack, Head, Confs...>(SizeToken<T>{}), serialization::InfSizeT>;

/*!
 * \brief Forward declaration of the top level GetRequiredBufferSize template for non-static size types.
 * \tparam IterCfg Iteration config.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Head, typename... Confs, typename T>
auto constexpr GetBufferSizeRoot(T const& t) noexcept
    -> std::enable_if_t<!IsStaticSize<TpPack, Head, Confs...>(SizeToken<T>{}), serialization::InfSizeT>;

/*!
 * \brief Returns the required buffer size to serialize a type with provided config as a Tlv field.
 * \tparam CallCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for TLV serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename CallCfg, typename TpPack, typename Head, typename... Confs, typename T>
InfSizeT constexpr GetTlvFieldBufferSizeRoot(T const& t) noexcept;

}  // namespace detail

/*!
 * \brief Returns the required buffer size to serialize a type with provided config
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Calculated buffer size for serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename Head, typename... Confs, typename T>
std::size_t constexpr GetRequiredBufferSize(T const& t) noexcept;

/*!
 * \brief Returns the required buffer size to serialize a type with provided config as a Tlv field.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for TLV serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename Head, typename... Confs, typename T>
std::size_t constexpr GetTlvFieldRequiredBufferSize(T const& t) noexcept;

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config
 * \tparam T Type of the member.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Calculated buffer size for serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename T, typename TpPack, typename Head, typename... Confs>
InfSizeT constexpr GetMaximumBufferSize() noexcept;

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config (for argument-dependent lookup).
 * \tparam T Type of the member.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Calculated buffer size for serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
InfSizeT constexpr GetMaximumBufferSize(T const&) noexcept;

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config as a Tlv field.
 * \tparam T Type of the member.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Required buffer size for TLV serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename T, typename TpPack, typename Head, typename... Confs>
InfSizeT constexpr GetTlvFieldMaximumBufferSize() noexcept;

/*!
 * \brief       Returns the maximum buffer size to serialize a type with provided
                config as a Tlv field. (for argument-dependent lookup).
 * \tparam      T Type of the member.
 * \tparam      TpPack Transformation properties.
 * \tparam      Head Either length size config or endianness.
 * \tparam      Confs Configuration of the members.
 * \return      Required buffer size for TLV serialization if the model has maximum sizes for all elements.
 * \pre         -
 * \context     Reactor|App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
InfSizeT constexpr GetTlvFieldMaximumBufferSize(T const&) noexcept;

/*!
 * \brief Forward declaration of the top level SerializeTlvField template.
 * \tparam TpPack Configuration parameters required to serialize the item.
 * \tparam dataId Data id to be serialized into the field.
 * \tparam wireType Wire type to be serialized into the field.
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
template <typename TpPack, DataId dataId, WireType wireType, typename... ItemConfs, typename T>
void SerializeTlvField(Writer& w, T const& t) noexcept;
}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_FORWARD_H_
