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
/*!        \file   internal/data_types.h
 *         \brief  Internal (private) data types.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpDaemonLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_DATA_TYPES_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_DATA_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief Subscription state.
 * \vprivate Component Private
 */
using SubscriptionStateType = someip_daemon_core::client::eventgroup::EventSubscriptionState;

/*!
 * \brief SOME/IP service instance ID.
 * \vprivate Component Private
 */
using SomeIpServiceInstanceIdType = someip_daemon_core::configuration::types::SomeIpServiceInstanceId;

/*!
 * \brief SOME/IP message.
 * \vprivate Component Private
 */
using PacketType = someip_daemon_core::client::Packet;

/*!
 * \brief Instance ID.
 * \vprivate Component Private
 */
using InstanceIdType = std::uint16_t;

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_DATA_TYPES_H_
