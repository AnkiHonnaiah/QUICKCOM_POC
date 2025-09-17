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
/**        \file  service_discovery_server_message_builder_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * Interface for Service Discovery Server message builder.
 */
class ServiceDiscoveryServerMessageBuilderInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \steady Init
   * \steady FALSE
   */
  ServiceDiscoveryServerMessageBuilderInterface() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \steady Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryServerMessageBuilderInterface() noexcept = default;

  ServiceDiscoveryServerMessageBuilderInterface(ServiceDiscoveryServerMessageBuilderInterface const &) = delete;
  ServiceDiscoveryServerMessageBuilderInterface(ServiceDiscoveryServerMessageBuilderInterface &&) = delete;
  ServiceDiscoveryServerMessageBuilderInterface &operator=(ServiceDiscoveryServerMessageBuilderInterface const &) & =
      delete;
  ServiceDiscoveryServerMessageBuilderInterface &operator=(ServiceDiscoveryServerMessageBuilderInterface &&) & = delete;
  /*!
   * \brief Creates SD OfferService entry
   *
   * \return An OfferServiceEntry
   *
   * \pre          -
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual message::entries::OfferServiceEntry MakeOfferServiceEntry() const = 0;

  /*!
   * \brief Returns the service entry Id of this service.
   *
   * \return A service entry id.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual message::entries::ServiceEntryId GetServiceEntryId() const = 0;

  /*!
   * \brief Creates SD a SD StopOfferService entry.
   *
   * \return A StopOfferService entry
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual message::entries::StopOfferServiceEntry MakeStopOfferServiceEntry() const = 0;

  /*!
   * \brief Creates SD entry for a positive SD entry in response to a SubscribeEventgroup request.
   *
   * \param[in] eventgroup_id  A SOME/IP eventgroup identifier.
   * \param[in] ttl            A TTL value.
   * \param[in] counter        A counter value.
   *
   * \return A entries::SubscribeEventgroupAckEntry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  virtual message::entries::SubscribeEventgroupAckEntry MakeSubscribeEventgroupAckEntry(
      amsr::someip_protocol::internal::EventgroupId const eventgroup_id, amsr::someip_protocol::internal::Ttl const ttl,
      amsr::someip_protocol::internal::SdEntryCounter const counter) const = 0;

  /*!
   * \brief Creates SD entry for a negative SD entry in response to a SubscribeEventgroup request.
   *
   * \param[in] eventgroup_id  A SOME/IP eventgroup identifier.
   * \param[in] counter        A counter value.
   *
   * \return A entries::SubscribeEventgroupNAckEntry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual message::entries::SubscribeEventgroupNAckEntry MakeSubscribeEventgroupNackEntry(
      amsr::someip_protocol::internal::EventgroupId const eventgroup_id,
      amsr::someip_protocol::internal::SdEntryCounter const counter) const = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_
