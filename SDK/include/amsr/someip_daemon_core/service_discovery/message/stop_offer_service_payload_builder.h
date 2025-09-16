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
/**        \file  stop_offer_service_payload_builder.h
 *        \brief  Payload builder for stop offer service message.
 *
 *      \details  Builds a SD message payload for stop offer service message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_OFFER_SERVICE_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_OFFER_SERVICE_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message payload builder for StopOfferService entry type.
 */
class StopOfferServicePayloadBuilder final {
 public:
  StopOfferServicePayloadBuilder() = delete;
  ~StopOfferServicePayloadBuilder() = delete;
  StopOfferServicePayloadBuilder(StopOfferServicePayloadBuilder const&) = delete;
  StopOfferServicePayloadBuilder(StopOfferServicePayloadBuilder&&) = delete;
  StopOfferServicePayloadBuilder& operator=(StopOfferServicePayloadBuilder const&) = delete;
  StopOfferServicePayloadBuilder& operator=(StopOfferServicePayloadBuilder&&) = delete;

  /*!
   * \brief Creates a SD Message Payload containing an entry and its options from a given StopOfferService Entry.
   * \param[in] entry A StopOfferService entry.
   * \return A SD Message Payload containing the given entry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-10144550
   * \trace SPEC-10144552
   * \trace SPEC-10144553
   * \trace SPEC-10144554
   * \trace SPEC-10144558
   * \trace SPEC-10144561
   * \trace SPEC-10144562
   * \trace SPEC-10144563
   * \trace SPEC-10144564
   * \trace SPEC-10144566
   * \trace SPEC-10144652
   * \trace SPEC-10144653
   * \trace SPEC-10144654
   * \trace SPEC-10144656
   */
  static ServiceDiscoveryMessagePayload CreatePayload(entries::StopOfferServiceEntry const& entry);

 private:
  /*!
   * \brief Getting the Unicast SD message type.
   *
   * \param[in] address Container of address details.
   *
   * \return An enum class value corresponding to unicast SD type.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static options::SomeIpSdEndpointOptionType GetUnicastSdMessageType(someip_daemon_core::IpAddress const& address);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_STOP_OFFER_SERVICE_PAYLOAD_BUILDER_H_
