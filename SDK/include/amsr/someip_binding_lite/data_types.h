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
/*!        \file   someip_binding_lite/data_types.h
 *         \brief  Public data types.
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteDataTypes
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_DATA_TYPES_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_DATA_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Service ID of a SOME/IP message.
 */
using ServiceId = std::uint16_t;

/*!
 * \brief Instance ID of a SOME/IP message.
 */
using InstanceId = std::uint16_t;

/*!
 * \brief ID of an event.
 */
using EventId = std::uint16_t;

/*!
 * \brief Major interface version of a SOME/IP message.
 */
using MajorInterfaceVersion = std::uint8_t;

/*!
 * \brief Minor interface version of a SOME/IP message.
 */
using MinorInterfaceVersion = std::uint32_t;

/*!
 * \brief ID of a client,
 */
using ClientId = std::uint16_t;

/*!
 * \brief Type-alias for the maximum size of the IPC transmission queue.
 */
using IpcMaxQueueSize = std::uint32_t;

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_DATA_TYPES_H_
