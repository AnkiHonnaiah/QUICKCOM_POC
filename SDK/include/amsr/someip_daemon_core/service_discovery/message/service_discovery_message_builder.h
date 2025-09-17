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
/**        \file  service_discovery_message_builder.h
 *        \brief  SD Message Builder
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_

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
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_header_builder.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_payload_builder.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message builder.
 * \trace SPEC-4981634
 * \trace SPEC-4663610
 */
class ServiceDiscoveryMessageBuilder final {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryMessageBuilder() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryMessageBuilder() noexcept = default;

  ServiceDiscoveryMessageBuilder(ServiceDiscoveryMessageBuilder const&) = delete;
  ServiceDiscoveryMessageBuilder(ServiceDiscoveryMessageBuilder&&) = delete;
  ServiceDiscoveryMessageBuilder& operator=(ServiceDiscoveryMessageBuilder const&) = delete;
  ServiceDiscoveryMessageBuilder& operator=(ServiceDiscoveryMessageBuilder&&) = delete;

  /*!
   * \brief Builds SD Messages for one shot unicast SD messages.
   *
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] subscribe_eventgroup_entries Container of SubscribeEventgroup entries.
   * \param[in] stop_subscribe_eventgroup_entries Container of StopSubscribeEventgroup entries.
   * \param[in] subscribe_eventgroup_ack_entries Container of SubscribeEventgroupAck entries.
   * \param[in] subscribe_eventgroup_nack_entries Container of SubscribeEventgroupNAck entries.entries.
   * \param[in] receiver_address Address of the unicast receiver.
   *
   * \return A container of SD messages.
   * \pre -
   * \context Timer
   * \steady TRUE
   */
  ServiceDiscoveryMessageContainer BuildMessage(
      entries::OfferServiceEntryContainer const& offer_service_entries,
      entries::SubscribeEventgroupEntryContainer const& subscribe_eventgroup_entries,
      entries::StopSubscribeEventgroupEntryContainer const& stop_subscribe_eventgroup_entries,
      entries::SubscribeEventgroupAckEntryContainer const& subscribe_eventgroup_ack_entries,
      entries::SubscribeEventgroupNAckEntryContainer const& subscribe_eventgroup_nack_entries,
      amsr::someip_protocol::internal::AddressPair const& receiver_address);

  /*!
   * \brief Builds SD Messages for one shot multicast SD messages.
   *
   * \param[in] find_service_entries Container of FindService entries.
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] stop_offer_service_entries Container of StopOfferService entries.
   *
   * \return A container of SD messages.
   * \pre -
   * \context Timer
   * \steady FALSE
   */
  ServiceDiscoveryMessageContainer BuildMessage(
      entries::FindServiceEntryContainer const& find_service_entries,
      entries::OfferServiceEntryContainer const& offer_service_entries,
      entries::StopOfferServiceEntryContainer const& stop_offer_service_entries);

  /*!
   * \brief Builds SD Messages for cyclic multicast SD messages.
   *
   * \param[in] offer_service_entries Container of OfferService entries.
   *
   * \return A container of SD message payloads.
   * \pre -
   * \context Timer
   * \steady FALSE
   */
  ServiceDiscoveryMessageContainer BuildMessage(entries::OfferServiceEntryContainer const& offer_service_entries);

 private:
  /*!
   * \brief Creates a service discovery message from the given entries, options.
   *
   * \param[in] sd_message_payload Payload containing Entries and options.
   *
   * \return A ServiceDiscoveryMessage.
   *
   * \pre -
   * \context Timer
   * \steady FALSE
   * \trace SPEC-4981486
   * \trace SPEC-4981487
   * \trace SPEC-4981488
   * \trace SPEC-4981489
   * \trace SPEC-4981490
   * \trace SPEC-4981497
   * \trace SPEC-4981498
   * \trace SPEC-4981499
   * \trace SPEC-4981500
   * \trace SPEC-4981513
   * \trace SPEC-10144518
   * \trace SPEC-10144520
   * \trace SPEC-10144528
   * \trace SPEC-10144529
   * \trace SPEC-10144530
   * \trace SPEC-10144532
   * \trace SPEC-10144538
   * \trace SPEC-10144539
   */
  static ServiceDiscoveryMessage CreateSdMessageFromPayload(ServiceDiscoveryMessagePayload const& sd_message_payload);

  /*!
   * \brief Fill in the header of the SD Message for unicast sending.
   *
   * \param[in,out] sd_message The SD message to modify.
   * \param[in] receiver_address Address of the unicast receiver.
   *
   * \pre -
   * \context Timer
   * \steady TRUE
   * \trace SPEC-10144536
   */
  void FillUnicastHeader(ServiceDiscoveryMessage& sd_message,
                         amsr::someip_protocol::internal::AddressPair const& receiver_address);

  /*!
   * \brief Fill in the header of the SD Message for multicast sending.
   *
   * \param[in,out] sd_message The SD message to modify.
   *
   * \pre -
   * \context Timer
   * \steady FALSE
   * \trace SPEC-10144536
   */
  void FillMulticastHeader(ServiceDiscoveryMessage& sd_message);

  /*!
   * \brief SOME/IP SD Message Header builder.
   *
   * \trace SPEC-4981506
   */
  ServiceDiscoveryMessageHeaderBuilder sd_message_header_builder_{};
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_
