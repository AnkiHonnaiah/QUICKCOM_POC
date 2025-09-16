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
/*!        \file someip-protocol/internal/marshalling.h
 *        \brief Aliases used in AraCom components.
 *         \unit SomeIpProtocol::Serdes::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MARSHALLING_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MARSHALLING_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/**********************************************************************************************************************
 *  Aliases
 *********************************************************************************************************************/
/*!
 * \brief   Type definition for PacketBuffer.
 * \details The packet buffer could be of any container type, but for reasons of compatibility with the current
 *          implementation and for a rapid prototype this shall be a vector.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using PacketBuffer = ara::core::Vector<std::uint8_t>;

/*!
 * \brief Type definition for PacketBufferView to be used in Deserializer.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using PacketBufferView = amsr::core::Span<std::uint8_t const> const;
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MARSHALLING_H_
