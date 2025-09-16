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
/**        \file  service_discovery_payload_builder.h
 *        \brief  SD Message Payload Builder
 *
 *      \details  Builds a SD message payload from different container of SD Entries
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message builder.
 */
class ServiceDiscoveryPayloadBuilder final {
 public:
  /*!
   * \brief Alias for a pair of an entry container and an option container.
   */
  using Message = message::ServiceDiscoveryMessagePayload;

  ServiceDiscoveryPayloadBuilder() = delete;
  ~ServiceDiscoveryPayloadBuilder() = delete;
  ServiceDiscoveryPayloadBuilder(ServiceDiscoveryPayloadBuilder const&) = delete;
  ServiceDiscoveryPayloadBuilder(ServiceDiscoveryPayloadBuilder&&) = delete;
  ServiceDiscoveryPayloadBuilder& operator=(ServiceDiscoveryPayloadBuilder const&) = delete;
  ServiceDiscoveryPayloadBuilder& operator=(ServiceDiscoveryPayloadBuilder&&) = delete;

  /*!
   * \brief Builds payload for one shot unicast SD messages
   *
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] subscribe_eventgroup_entries Container of SubscribeEventgroup entries.
   * \param[in] stop_subscribe_eventgroup_entries Container of StopSubscribeEventgroup entries.
   * \param[in] subscribe_eventgroup_ack_entries Container of SubscribeEventgroupAck entries.
   * \param[in] subscribe_eventgroup_nack_entries Container of SubscribeEventgroupNAck entries.
   *
   * \return A container of SD message payloads.
   * \pre -
   * \context Timer
   * \steady TRUE
   * \reentrant FALSE
   */
  static ServiceDiscoveryMessagePayloadContainer BuildPayload(
      entries::OfferServiceEntryContainer const& offer_service_entries,
      entries::SubscribeEventgroupEntryContainer const& subscribe_eventgroup_entries,
      entries::StopSubscribeEventgroupEntryContainer const& stop_subscribe_eventgroup_entries,
      entries::SubscribeEventgroupAckEntryContainer const& subscribe_eventgroup_ack_entries,
      entries::SubscribeEventgroupNAckEntryContainer const& subscribe_eventgroup_nack_entries);

  /*!
   * \brief Builds payload for cyclic unicast SD messages
   *
   * \param[in] offer_service_entries Container of OfferService entries.
   *
   * \return A container of SD message payloads.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static ServiceDiscoveryMessagePayloadContainer BuildPayload(
      entries::OfferServiceEntryContainer const& offer_service_entries);

  /*!
   * \brief Builds payload for one shot multicast SD messages
   *
   * \param[in] find_service_entries Container of FindService entries.
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] stop_offer_service_entries Container of StopOfferService entries.
   *
   * \return A container of SD message payloads.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static ServiceDiscoveryMessagePayloadContainer BuildPayload(
      entries::FindServiceEntryContainer const& find_service_entries,
      entries::OfferServiceEntryContainer const& offer_service_entries,
      entries::StopOfferServiceEntryContainer const& stop_offer_service_entries);

 private:
  /*!
   * \brief Builds payload for any SD message
   *
   * \param[in] find_service_entries Container of FindService entries.
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] stop_offer_service_entries Container of StopOfferService entries.
   * \param[in] subscribe_eventgroup_entries Container of SubscribeEventgroup entries.
   * \param[in] stop_subscribe_eventgroup_entries Container of StopSubscribeEventgroup entries.
   * \param[in] subscribe_eventgroup_ack_entries Container of SubscribeEventgroupAck entries.
   * \param[in] subscribe_eventgroup_nack_entries Container of SubscribeEventgroupNAck entries.
   *
   * \return A container of SD message payloads.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static ServiceDiscoveryMessagePayloadContainer BuildPayload(
      entries::FindServiceEntryContainer const& find_service_entries,
      entries::OfferServiceEntryContainer const& offer_service_entries,
      entries::StopOfferServiceEntryContainer const& stop_offer_service_entries,
      entries::SubscribeEventgroupEntryContainer const& subscribe_eventgroup_entries,
      entries::StopSubscribeEventgroupEntryContainer const& stop_subscribe_eventgroup_entries,
      entries::SubscribeEventgroupAckEntryContainer const& subscribe_eventgroup_ack_entries,
      entries::SubscribeEventgroupNAckEntryContainer const& subscribe_eventgroup_nack_entries);

  /*!
   * \brief Builds payload for any SD message, service part
   *
   * \param[in] payload_container Payload container to store a potential full working payload
   * \param[in] working_payload Target message payload
   * \param[in] find_service_entries Container of FindService entries.
   * \param[in] offer_service_entries Container of OfferService entries.
   * \param[in] stop_offer_service_entries Container of StopOfferService entries.
   *
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static void BuildPayloadServices(ServiceDiscoveryMessagePayloadContainer& payload_container,
                                   ServiceDiscoveryMessagePayload& working_payload,
                                   entries::FindServiceEntryContainer const& find_service_entries,
                                   entries::OfferServiceEntryContainer const& offer_service_entries,
                                   entries::StopOfferServiceEntryContainer const& stop_offer_service_entries);

  /*!
   * \brief Builds payload for any SD message, eventgroup part
   *
   * \param[in] payload_container Payload container to store a potential full working payload
   * \param[in] working_payload Target message payload
   * \param[in] subscribe_eventgroup_entries Container of SubscribeEventgroup entries.
   * \param[in] stop_subscribe_eventgroup_entries Container of StopSubscribeEventgroup entries.
   * \param[in] subscribe_eventgroup_ack_entries Container of SubscribeEventgroupAck entries.
   * \param[in] subscribe_eventgroup_nack_entries Container of SubscribeEventgroupNAck entries.
   *
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static void BuildPayloadEventGroups(
      ServiceDiscoveryMessagePayloadContainer& payload_container, ServiceDiscoveryMessagePayload& working_payload,
      entries::SubscribeEventgroupEntryContainer const& subscribe_eventgroup_entries,
      entries::StopSubscribeEventgroupEntryContainer const& stop_subscribe_eventgroup_entries,
      entries::SubscribeEventgroupAckEntryContainer const& subscribe_eventgroup_ack_entries,
      entries::SubscribeEventgroupNAckEntryContainer const& subscribe_eventgroup_nack_entries);

  /*!
   * \brief Checks if given entries and options would fit into a SD Message Payload
   * transported in a single UDP message
   *
   * \param[in] entries_and_options Entries and options to evaluate
   * \param[in] payload Target message payload
   *
   * \return True if the given entries and options would fit into the payload, false otherwise
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static bool EvaluatePayload(ServiceDiscoveryMessagePayload const& entries_and_options,
                              ServiceDiscoveryMessagePayload const& payload);

  /*!
   * \brief Inserts an entry and its options into a working payload. If the working
   * payload is full, it will be inserted into the payload container
   *
   * \param[in] entries_and_options Entries and options to insert
   * \param[in] payload_container Payload container to store a potential full working payload
   * \param[in] working_payload Target message payload
   *
   * \return True if the given entries and options would fit into the payload, false otherwise
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static void InsertEntry(ServiceDiscoveryMessagePayload&& entries_and_options,
                          ServiceDiscoveryMessagePayloadContainer& payload_container,
                          ServiceDiscoveryMessagePayload& working_payload);
  /*!
   * \brief Computes the size (in Bytes) of a Service Discovery Message Payload
   *
   * \param[in] payload Payload
   *
   * \return Size in bytes of the payload
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static std::size_t CalculateSizeOf(ServiceDiscoveryMessagePayload const& payload);

  /*!
   * \brief Computes the size (in Bytes) of a Service Discovery Option
   *
   * \param[in] option Option
   *
   * \return Size in bytes of the option
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static std::size_t CalculateSizeOf(options::ServiceDiscoveryOption const& option);

  /*!
   * \brief Finds an option within an option container
   *
   * \param[in] option_container Option container
   * \param[in] option Option to insert
   *
   * \return Iterator pointing to the option within the container if found, otherwise pointing to the end of the
   * container
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static options::ServiceDiscoveryOptionContainer::const_iterator FindOption(
      options::ServiceDiscoveryOptionContainer const& option_container, options::ServiceDiscoveryOption const& option);

  /*!
   * \brief Adds new options into the options container of a payload if the input option is
   * not yet present in the options container.
   *
   * \param[in] option Option to be inserted
   * \param[in,out] payload SD message payload to insert the option
   *
   * \return Index of the option within the container
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  static uint8_t AddOption(options::ServiceDiscoveryOption&& option, ServiceDiscoveryMessagePayload& payload);

  /*!
   * \brief Adds an entry and its options into a payload. The entry and its
   * options are encapsulated in a ServiceDiscoveryMessagePayload object
   *
   * \param[in] entries_and_options SD Message payload containing the input entry and its options
   * \param[in,out] payload SD message payload to insert the entry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-10144544
   * \trace SPEC-10144776
   * \trace SPEC-10144780
   * \trace SPEC-10144803
   * \trace SPEC-10144806
   * \trace SPEC-10144807
   * \trace SPEC-10144808
   * \trace SPEC-10144809
   * \trace SPEC-10144810
   * \trace SPEC-10144642
   * \trace SPEC-10144643
   */
  static void AddToPayload(ServiceDiscoveryMessagePayload&& entries_and_options,
                           ServiceDiscoveryMessagePayload& payload);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_PAYLOAD_BUILDER_H_
