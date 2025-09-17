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
/**        \file  scheduler.h
 *        \brief  SD Message Scheduler
 *
 *      \details  This class manages transmission of Service Discovery messages with a given delay through
 *                ServiceDiscoveryMessageSenderInterface. It contains a queue to store messages together with a timer
 *                for each message. It also avoids duplicate sending of the same message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_H_

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
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/cyclic_offer_timer.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/oneshot_multicast_timer.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/oneshot_unicast_timer.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/oneshot_unicast_timer_observer_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/repetition_offer_timer.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "vac/container/static_list.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief A queue managing sending service discovery messages that need to be sent at some time in the future.
 * \details Attention: The queue does not support handling of different service discovery message / entry types.
 * Therefore a dedicated queue instance must be used for every message type.
 */
class Scheduler : public OneshotUnicastTimerObserverInterface,
                  public OneshotMulticastTimerObserverInterface,
                  public SchedulerInterface {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] message_sender A message sender.
   * \param[in] timer_manager A timer manager.
   * \param[in] sd_message_builder SD Message builder.
   *
   * \context Init
   * \steady  FALSE
   */
  Scheduler(ServiceDiscoveryMessageSenderInterface& message_sender,
            amsr::steady_timer::TimerManagerInterface* timer_manager,
            ServiceDiscoveryMessageBuilder& sd_message_builder) noexcept;
  /*!
   * \brief Define destructor.
   * \context Shutdown
   * \steady  FALSE
   */
  ~Scheduler() noexcept override = default;

  Scheduler() = delete;
  Scheduler(Scheduler const&) = delete;
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(Scheduler const&) & = delete;
  Scheduler& operator=(Scheduler&&) & = delete;

  /*!
   * \brief Create a new cyclic timer.
   *
   * \param[in] cyclic_offer_delay Cyclic delay of the timer.
   * \pre -
   * \context Init
   * \steady FALSE
   * \reentrant FALSE
   */
  void CreateCyclicTimer(std::chrono::nanoseconds const& cyclic_offer_delay);

  /*!
   * \brief      Create a new repetition offer timer.
   * \param[in]  initial_repetitions_base_delay initial repetitions base delay of the timer.
   * \param[in]  sender The service discovery message sender.
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   * \vprivate
   */
  void CreateRepetitionOfferTimer(std::chrono::nanoseconds const& initial_repetitions_base_delay,
                                  ServiceDiscoveryMessageSenderInterface& sender);

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
  void ScheduleFindServiceEntry(entries::FindServiceEntry const& entry, std::chrono::nanoseconds const& min_delay,
                                std::chrono::nanoseconds const& max_delay) override;

  /*!
   * \brief Schedule a Stop OfferService Entry to be sent via multicast.
   *
   * \param[in] entry StopOffer Entry to be scheduled.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   * \trace SPEC-4663613
   */
  void ScheduleStopOfferServiceEntry(entries::StopOfferServiceEntry const& entry) override;

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
   * \trace SPEC-10144819
   */
  void ScheduleSubscribeEventgroupEntry(entries::SubscribeEventgroupEntry const& entry,
                                        std::chrono::nanoseconds const& min_delay,
                                        std::chrono::nanoseconds const& max_delay,
                                        amsr::someip_protocol::internal::AddressPair const& receiver_address) override;

  /*!
   * \brief Schedule a StopSubscribeEventgroup Entry to be sent to a specific receiver.
   *
   * \param[in] entry StopSubscribeEventgroup Entry to be scheduled.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-10144819
   */
  void ScheduleStopSubscribeEventgroupEntry(
      entries::StopSubscribeEventgroupEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) override;

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
  void ScheduleSubscribeEventgroupAckEntry(entries::SubscribeEventgroupAckEntry const& entry,
                                           std::chrono::nanoseconds const& min_delay,
                                           std::chrono::nanoseconds const& max_delay,
                                           amsr::someip_protocol::internal::AddressPair const& receiver_address,
                                           TimerSendActionType&& post_action) override;

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
  void ScheduleSubscribeEventgroupAckEntry(
      entries::SubscribeEventgroupAckEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) override;

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
  void ScheduleSubscribeEventgroupNAckEntry(
      entries::SubscribeEventgroupNAckEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) override;

  /*!
   * \brief Schedule a unicast OfferService SOME/IP SD message after the specified delay.
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
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry, std::chrono::nanoseconds const& min_delay,
                                 std::chrono::nanoseconds const& max_delay,
                                 amsr::someip_protocol::internal::AddressPair const& receiver_address) override;

  /*!
   * \brief  Schedule an OfferService Entry to be sent via multicast. Called from initial wait phase.
   *
   * \param[in] entry       OfferService Entry to be scheduled.
   * \param[in] min_delay   Minimum delay to wait before sending this entry.
   * \param[in] max_delay   Maximum delay to wait before sending this entry.
   * \param[in] post_action An action that will be executed once the entry has been sent.
   *
   * \pre -
   * \context App
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry, std::chrono::nanoseconds const& min_delay,
                                 std::chrono::nanoseconds const& max_delay,
                                 TimerSendActionType&& post_action) noexcept override;

  /*!
   * \brief Schedule a cyclic OfferService Entry.
   * \details Schedules sending a periodic OfferService SOME/IP SD message, this message is triggered by a timer as we
   *          first go through the initial wait phase, which is controlled by another scheduler.
   *
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                 std::chrono::nanoseconds const& cyclic_offer_delay) override;

  /*!
   * \brief Remove an OfferService Entry from the cyclic timer.
   *
   * \param[in] entry_id of the OfferService Entry to be unscheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  void UnscheduleOfferServiceEntry(entries::ServiceEntryId const& entry_id,
                                   std::chrono::nanoseconds const& cyclic_offer_delay) override;

  /*!
   * \brief Remove an OfferService Entry from the multicast one shot timer.
   *
   * \param[in] entry_id Entry id of the OfferService Entry to be unscheduled.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void UnscheduleOfferServiceEntry(entries::ServiceEntryId const& entry_id) override;

  /*!
   * \brief Callback to indicate that the multicast timer has been expired.
   *        The timer will be destroyed.
   * \pre -
   * \context Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  void OnMulticastTimerExpired() override;

  /*!
   * \brief Callback to indicate that a unicast timer has been expired.
   *        The timer will be removed from the container.
   *
   * \param[in] address_pair Pair of IP-Address and port of the unicast message receiver
   *
   * \context Timer
   * \steady FALSE
   */
  void OnUnicastTimerExpired(amsr::someip_protocol::internal::AddressPair const& address_pair) override;

  /*!
   * \brief      Schedule repetition phase Offer Service messages.
   * \param[in]  initial_repetitions_base_delay The base delay.
   * \param[in]  initial_repetitions_max The number of repetitions.
   * \param[in]  post_action The post action to perform.
   * \param[in]  offer_service_entry The Offer Service entry.
   * \pre        -
   * \context Timer
   * \steady FALSE
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   */
  void ScheduleOfferService(std::chrono::nanoseconds const&,
                            configuration::ConfigurationTypesAndDefs::InitialRepetitionsMax,
                            RepetitionOfferTimer::TimerSendActionType&&,
                            message::entries::OfferServiceEntry&& offer_service_entry) override;

  /*!
   * \brief      Unschedule repetition phase Offer Service messages.
   * \param[in]  initial_repetitions_base_delay The base delay.
   * \param[in]  service_entry_id The service entry ID.
   * \pre        -
   * \context    Timer, App, Network
   * \steady     FALSE
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   */
  void UnscheduleOfferService(std::chrono::nanoseconds const&, entries::ServiceEntryId const&) override;

 private:
  /*!
   * \brief Find an existing CyclicTimer based on its cyclic delay.
   *
   * \param[in] cyclic_offer_delay The cyclic delay of the timer.
   *
   * \return CyclicOfferTimerMap iterator indicating the timer with the provided period.
   * \pre -
   * \context App, Network
   * \steady FALSE
   * \reentrant FALSE
   */
  CyclicOfferTimerMap::iterator FindCyclicTimer(std::chrono::nanoseconds const& cyclic_offer_delay);

  /*!
   * \brief Pointer type for unicast message sender timer.
   */
  using UnicastOneshotTimerUniquePtr = amsr::UniquePtr<OneshotUnicastTimer>;
  /*!
   * \brief Map type for unicast message sender timers.
   */
  using UnicastMessageSenderTimerMap =
      ara::core::Map<amsr::someip_protocol::internal::AddressPair, UnicastOneshotTimerUniquePtr>;

  /*!
   * \brief Find an existing unicast message sender or create a new one for the target address/port.
   *
   * \param[in] receiver_address The unicast address to search for.
   *
   * \return Pair of UnicastMessageSenderTimer iterator and a bool indicating whether the timer is found or created
   *         true = timer is found
   *         false = new timer is created
   * \pre -
   * \context App, Network
   * \steady TRUE
   * \reentrant FALSE
   */
  std::pair<UnicastMessageSenderTimerMap::iterator, bool> FindOrCreateUnicastMessageSenderTimer(
      amsr::someip_protocol::internal::AddressPair const& receiver_address);

  /*!
   * \brief Set the given delay to the given unicast timer and start it.
   *        The timer is always updated with the max_delay.  min_delay is always ignored.
   *
   * \param[in,out] timer_iterator The timer to update.
   * \param[in] min_delay Minimum delay to wait before sending this entry [ignored].
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \pre -
   * \context App, Network
   * \steady FALSE
   * \reentrant FALSE
   */
  static void StartUnicastTimer(UnicastMessageSenderTimerMap::iterator const& timer_iterator,
                                std::chrono::nanoseconds const& min_delay, std::chrono::nanoseconds const& max_delay);

  /*!
   * \brief Update the given unicast timer according to the given delay.
   *        If max_delay is before the next timer expiry, the timer is updated with the max_delay.
   *        Otherwise, expiry time is not touched. min_delay is always ignored.
   *
   * \param[in,out] timer_iterator The timer to update.
   * \param[in] min_delay Minimum delay to wait before sending this entry [ignored].
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \pre -
   * \context App, Network
   * \steady FALSE
   * \reentrant FALSE
   */
  static void UpdateUnicastTimer(UnicastMessageSenderTimerMap::iterator const& timer_iterator,
                                 std::chrono::nanoseconds const& min_delay, std::chrono::nanoseconds const& max_delay);

  /*!
   * \brief A message builder.
   */
  ServiceDiscoveryMessageBuilder& sd_message_builder_;

  /*!
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSenderInterface& message_sender_;

  /*!
   * \brief A timer manager for message sending.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief Map of CyclicOfferTimers {key = Cyclic delay}.
   */
  CyclicOfferTimerMap cyclic_offer_timers_map_{};

  /*!
   * \brief Map of unicast message sender timers {key = Receiver Address}.
   */
  UnicastMessageSenderTimerMap unicast_message_timers_map_{};

  /*!
   * \brief Multicast message timer.
   */
  ara::core::Optional<OneshotMulticastTimer> multicast_timer_{};

  /*!
   * \brief Map to accommodate the repetition offer timers.
   */
  RepetitionOfferTimerMap repetition_offer_timers_map_{};

  /*!
   * \brief In order to test ScheduleOfferServiceEntry(OfferServiceEntry&, TimerSendActionType&&), which is never called
   * by the Scheduler, the access to private member multicast_timer_ is required.
   */
  FRIEND_TEST(UT__Scheduler, OneShotMulticastTimer_ScheduleOfferServiceEntry);
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_SCHEDULER_H_
