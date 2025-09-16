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
/**        \file  marshalling.h
 *        \brief  Type alias for internal usage
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_MARSHALLING_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_MARSHALLING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/byteorder.h"

namespace amsr {
namespace someip_daemon_core {
namespace marshalling {

/*!
 * \brief Type alias for Network-Byte-Order.
 */
using NetworkByteOrder = amsr::someip_protocol::internal::BigEndian;

/*!
 * \brief Indicating TCP protocol is applied.
 * \trace SPEC-10144563
 */
static constexpr std::uint8_t kProtoTcp{6U};

/*!
 * \brief Indicating UDP protocol is applied.
 * \trace SPEC-10144564
 * \trace SPEC-10144578
 */
static constexpr std::uint8_t kProtoUdp{17U};

}  // namespace marshalling
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_MARSHALLING_H_
