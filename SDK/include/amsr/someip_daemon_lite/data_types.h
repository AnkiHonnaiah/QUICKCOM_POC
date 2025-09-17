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
/*!        \file   someip_daemon_lite/data_types.h
 *         \brief  Public data types.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpDaemonLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_DATA_TYPES_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_DATA_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace someip_daemon_lite {

/*!
 * \brief Service ID of a SOME/IP message.
 * \vpublic
 */
using ServiceId = std::uint16_t;

/*!
 * \brief Instance ID of a SOME/IP message.
 * \vpublic
 */
using InstanceId = std::uint16_t;

/*!
 * \brief ID of an event.
 * \vpublic
 */
using EventId = std::uint16_t;

/*!
 * \brief Major interface version of a SOME/IP message.
 * \vpublic
 */
using MajorInterfaceVersion = std::uint8_t;

/*!
 * \brief Minor interface version of a SOME/IP message.
 * \vpublic
 */
using MinorInterfaceVersion = std::uint32_t;

/*!
 * \brief ID of a client.
 * \vpublic
 */
using ClientId = std::uint16_t;

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_DATA_TYPES_H_
