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
/**        \file  scheduler_interface.h
 *        \brief  Interface for SD Message Scheduler
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_service_entry_id.h"

#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/repetition_offer_timer.h"
#include "someip-protocol/internal/message.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief Interface for scheduling SD messages
 */
class SchedulerInterface {
 public:
  /*!
   * \brief Function signature of action to be executed after
   *        transmission of an ACK
   */
  using TimerSendActionType = vac::language::UniqueFunction<void()>;
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady  FALSE
   */
  SchedulerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~SchedulerInterface() noexcept = default;

  SchedulerInterface(SchedulerInterface const&) = delete;
  SchedulerInterface(SchedulerInterface&&) = delete;
  SchedulerInterface& operator=(SchedulerInterface const&) & = delete;
  SchedulerInterface& operator=(SchedulerInterface&&) & = delete;

  /*!
   * \brief Schedule a FindService Entry to be sent via multicast.
   *
   * \param[in] entry FindService Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  virtual void ScheduleFindServiceEntry(entries::FindServiceEntry const& entry,
                                        std::chrono::nanoseconds const& min_delay,
                                        std::chrono::nanoseconds const& max_delay) = 0;

  /*!
   * \brief Schedule a Stop OfferService Entry to be sent via multicast.
   *
   * \param[in] entry StopOffer Entry to be scheduled.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void ScheduleStopOfferServiceEntry(entries::StopOfferServiceEntry const& entry) = 0;

  /*!
   * \brief Schedule a SubscribeEventgroup Entry to be sent to a specific receiver.
   *
   * \param[in] entry SubscribeEventgroup Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual void ScheduleSubscribeEventgroupEntry(
      entries::SubscribeEventgroupEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) = 0;

  /*!
   * \brief Schedule a StopSubscribeEventgroup Entry to be sent to a specific receiver.
   *
   * \param[in] entry StopSubscribeEventgroup Entry to be scheduled.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  virtual void ScheduleStopSubscribeEventgroupEntry(
      entries::StopSubscribeEventgroupEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) = 0;

  /*!
   * \brief Schedule a SubscribeEventgroupAck Entry to be sent to a specific receiver.
   *        A post-send action will be executed after the Ack has been sent
   *
   * \param[in] entry            SubscribeEventgroupAck Entry to be scheduled.
   * \param[in] min_delay        Minimum delay to wait before sending this entry.
   * \param[in] max_delay        Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \param[in] post_action      An action that will be executed once the entry has been sent.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  virtual void ScheduleSubscribeEventgroupAckEntry(entries::SubscribeEventgroupAckEntry const& entry,
                                                   std::chrono::nanoseconds const& min_delay,
                                                   std::chrono::nanoseconds const& max_delay,
                                                   amsr::someip_protocol::internal::AddressPair const& receiver_address,
                                                   TimerSendActionType&& post_action) = 0;

  /*!
   * \brief Schedule a SubscribeEventgroupAck Entry to be sent to a specific receiver.
   *
   * \param[in] entry            SubscribeEventgroupAck Entry to be scheduled.
   * \param[in] min_delay        Minimum delay to wait before sending this entry.
   * \param[in] max_delay        Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  virtual void ScheduleSubscribeEventgroupAckEntry(
      entries::SubscribeEventgroupAckEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) = 0;

  /*!
   * \brief Schedule a SubscribeEventgroupNack Entry to be sent to a specific receiver.
   *
   * \param[in] entry SubscribeEventgroupNack Entry to be scheduled.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  virtual void ScheduleSubscribeEventgroupNAckEntry(
      entries::SubscribeEventgroupNAckEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) = 0;

  /*!
   * \brief Schedule a unicast OfferService Entry.
   *
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                         std::chrono::nanoseconds const& min_delay,
                                         std::chrono::nanoseconds const& max_delay,
                                         amsr::someip_protocol::internal::AddressPair const& receiver_address) = 0;

  /*!
   * \brief      Schedule repetition phase Offer Service messages.
   * \param[in]  initial_repetitions_base_delay The base delay.
   * \param[in]  initial_repetitions_max The number of repetitions.
   * \param[in]  post_action The post action to perform.
   * \param[in]  offer_service_entry The Offer Service Entry.
   * \pre        -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \vprivate
   */
  virtual void ScheduleOfferService(
      std::chrono::nanoseconds const& initial_repetitions_base_delay,
      configuration::ConfigurationTypesAndDefs::InitialRepetitionsMax initial_repetitions_max,
      RepetitionOfferTimer::TimerSendActionType&& post_action,
      message::entries::OfferServiceEntry&& offer_service_entry) = 0;

  /*!
   * \brief      Unschedule repetition phase Offer Service messages.
   * \param[in]  initial_repetitions_base_delay The base delay.
   * \param[in]  service_entry_id The service entry ID.
   * \pre        -
   * \context Timer, App, Network
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \vprivate
   */
  virtual void UnscheduleOfferService(std::chrono::nanoseconds const& initial_repetitions_base_delay,
                                      entries::ServiceEntryId const& service_entry_id) = 0;

  /*!
   * \brief Schedule a multicast OfferService Entry.
   *
   * \param[in] entry       OfferService Entry to be scheduled.
   * \param[in] min_delay   Minimum delay to wait before sending this entry.
   * \param[in] max_delay   Maximum delay to wait before sending this entry.
   * \param[in] post_action An action that will be executed once the entry has been sent.
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady FALSE
   */
  virtual void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                         std::chrono::nanoseconds const& min_delay,
                                         std::chrono::nanoseconds const& max_delay,
                                         TimerSendActionType&& post_action) noexcept = 0;

  /*!
   * \brief Schedule a cyclic OfferService Entry.
   *
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                         std::chrono::nanoseconds const& cyclic_offer_delay) = 0;

  /*!
   * \brief Remove an OfferService Entry from the cyclic timer.
   *
   * \param[in] entry_id Entry id of the OfferService Entry to be unscheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  virtual void UnscheduleOfferServiceEntry(entries::ServiceEntryId const& entry_id,
                                           std::chrono::nanoseconds const& cyclic_offer_delay) = 0;

  /*!
   * \brief Remove an OfferService Entry from the multicast timer.
   *
   * \param[in] entry_id Entry id of the OfferService Entry to be unscheduled.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void UnscheduleOfferServiceEntry(entries::ServiceEntryId const& entry_id) = 0;
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_INTERFACE_H_
