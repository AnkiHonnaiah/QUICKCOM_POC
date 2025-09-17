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
/**        \file deser_forward.h
 *        \brief Forward declaration of the top level Deserialize template.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_FORWARD_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_FORWARD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief   Work-around to make argument-dependent lookup work.
 * \details This function definition is required to introduce SomeIpProtocolDeserialize to the compiler,
 *          so that argument-dependent lookup can run.
 * \tparam  K Dummy template argument.
 * \return  True if deserialization was successful, false otherwise.
 * \pre     -
 * \context Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
template <std::int32_t K>
// VECTOR NL AutosarC++17_10-A13.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A13.3.1_overload_w_forwarding_reference
Result SomeIpProtocolDeserialize() noexcept;

/*!
 * \brief Forward declaration of the top level Deserialize template.
 * \details Specializations of this template are made for all supported datatypes.
 * \tparam TpPack Transformation properties
 * \tparam Head Either length size config or endianness .
 * \tparam Confs Configuration of the members
 * \tparam Ts Type of the member.
 * \param[in] r Reader holding the bufferView on the serialized data.
 * \param[out] ts Destination the deserialized value shall be stored into.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename Head, typename... Confs, typename... Ts>
Result SomeIpProtocolDeserialize(Reader& r, Ts&&... ts) noexcept;

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_FORWARD_H_
