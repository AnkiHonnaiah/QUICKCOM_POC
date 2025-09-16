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
/**        \file  client_someip_event.h
 *        \brief  This file contains the datatypes which is to be used for routing of someip events.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_DATATYPES_CLIENT_SOMEIP_EVENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_DATATYPES_CLIENT_SOMEIP_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace datatypes {

/*!
 * \brief SomeIp event routing table entry is a structure that contains an event observer
 *        which is interested in receiving the event notification status and
 *        a subscription counter to count how many times the same observer has subscribed to this event.
 */
struct SomeIpEventRoutingTableEntry {
  /*!
   * \brief Observer interested in event and subscription status update notifications.
   */
  std::shared_ptr<client::SomeIpEventHandlerInterface> event_observer{};

  /*!
   * \brief Counts how many times the same observer (application) has subscribed to this event.
   */
  std::size_t subscription_count{};
};

/*!
 * \brief Client SomeIp Event is a structure which contains an optional field cache and a container of
 *        event routing table entries.
 */
struct ClientSomeIpEvent {
  /*!
   * \brief Field Cache is a optional value. It is used only when a field event is configured.
   */
  ara::core::Optional<client::Packet> field_cache{};
  /*!
   * \brief SomeIp Event routing table container that holds the event observer which is interested in receiving the
   *        event notification status and a subscription counter to count how many times the same observer has
   *        subscribed to this event.
   */
  ara::core::Vector<SomeIpEventRoutingTableEntry> event_routing_table{};
};

}  // namespace datatypes
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_DATATYPES_CLIENT_SOMEIP_EVENT_H_
