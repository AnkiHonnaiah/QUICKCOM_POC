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
/**        \file  oneshot_unicast_timer.h
 *
 *        \brief  This timer accumulates SD messages that are to be sent as unicast message to a certain address/port -
 *                combination when the timer expires.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // unique_ptr
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_eventgroup_entry_id.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/oneshot_unicast_timer_observer_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender_interface.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief OneshotUnicastTimer
 */
class OneshotUnicastTimer final {
 public:
  /*!
   * \brief The signature for an action to be executed when a SubscribeEventgroupAck entry
   *        has been sent
   */
  using TimerSendActionType = vac::language::UniqueFunction<void()>;

  /*!
   * \brief constructor of the MessageSchedulerOneShotTimer.
   * \param[in] timer_manager A pointer to the TimerManager object that is holding this timer.
   * \param[in] to_address the IP to_address of the receiver to whom the message is send.
   * \param[in] port The port to which the message is send.
   * \param[in] observer An instance of ServiceDiscoveryMessageUnicastTimerObserver.
   * \param[in] message_sender An instance of ServiceDiscoveryMessageSender.
   * \param[in] sd_message_builder SD Message builder.
   *
   * \context App, Network
   * \steady TRUE
   */
  OneshotUnicastTimer(amsr::steady_timer::TimerManagerInterface* timer_manager,
                      amsr::someip_protocol::internal::IpAddress const& to_address,
                      amsr::someip_protocol::internal::Port port, OneshotUnicastTimerObserverInterface& observer,
                      ServiceDiscoveryMessageSenderInterface& message_sender,
                      ServiceDiscoveryMessageBuilder& sd_message_builder) noexcept;

  /*!
   * \brief Define default destructor.
   * \context Timer
   * \steady FALSE
   */
  ~OneshotUnicastTimer() noexcept = default;

  OneshotUnicastTimer(OneshotUnicastTimer const&) = delete;
  OneshotUnicastTimer& operator=(OneshotUnicastTimer const&) & = delete;
  OneshotUnicastTimer(OneshotUnicastTimer&&) = delete;
  OneshotUnicastTimer& operator=(OneshotUnicastTimer&&) & = delete;

  /*!
   * \brief Adds an 'OfferService' entry to the next SD message that is sent.
   *
   * \param[in] entry SOME/IP SD entry struct.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                 std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Adds a 'subscribe' entry to the next SD message that is sent.
   *
   * \param[in] entry SOME/IP SD entry struct.
   * \pre -
   * \context App
   * \steady TRUE
   * \reentrant FALSE
   */
  void ScheduleSubscribeEventgroupEntry(entries::SubscribeEventgroupEntry const& entry,
                                        std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Adds a 'stop subscribe' entry to the next SD message that is sent.
   *
   * \param[in] entry SOME/IP SD entry struct.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleStopSubscribeEventgroupEntry(entries::StopSubscribeEventgroupEntry const& entry,
                                            std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Adds a 'subscribe acknowledged' entry to the next SD message that is sent.
   *        Stores a post-send action to be executed after the transmission of the SD message
   *
   * \param[in] entry        SOME/IP SD entry struct.
   * \param[in] post_action  An action that will be executed once the entry has been sent.
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  void ScheduleSubscribeEventgroupAckEntry(entries::SubscribeEventgroupAckEntry const& entry,
                                           std::chrono::nanoseconds const& max_delay,
                                           TimerSendActionType&& post_action) noexcept;

  /*!
   * \brief Adds a 'subscribe acknowledged' entry to the next SD message that is sent.
   *
   * \param[in] entry        SOME/IP SD entry struct.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleSubscribeEventgroupAckEntry(entries::SubscribeEventgroupAckEntry const& entry,
                                           std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Adds a 'subscribe not acknowledged' entry to the next SD message that is sent.
   *
   * \param[in] entry SOME/IP SD entry struct.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleSubscribeEventgroupNAckEntry(entries::SubscribeEventgroupNAckEntry const& entry,
                                            std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Removes an 'OfferService' entry from the next SD message that is sent.
   *
   * \param[in] entry_id ID to identify the entry that has to be removed.
   *
   * \context App
   * \steady FALSE
   */
  void RemoveOfferServiceEntry(entries::ServiceEntryId const& entry_id);

  /*!
   * \brief Removes an 'SubscribeEventGroup' entry from the next SD message that is sent.
   *
   * \param[in] entry_id ID to identify the entry that has to be removed.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  void RemoveSubscribeEventgroupEntry(entries::EventgroupEntryId const& entry_id);

  /*!
   * \brief Converts all scheduled Subscribe ACKs into Subscribe NACK.
   *        For the given entry_id {service_id, major_version, instance_id}
   *        will be used to find the matching scheduled ACKs.
   *
   * \param[in] service_entry_id entries::ServiceEntryId to identify the entries that have to be removed.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  void RejectAllEventgroupEntriesForService(entries::ServiceEntryId const& service_entry_id);

 private:
  /*!
   * \brief Executed when timer expires. Handles message building, sending and notifying the observer that the timer is
   * expired and can be destroyed.
   *
   * \pre -
   * \context Timer
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-4981641
   * \trace SPEC-4981642
   * \trace SPEC-10144658
   */
  void HandleTimer() noexcept;

  /*!
   * \brief Updates the timer timeout
   * \details If the timer hasn't started, the timer starts.
   *          If the current timer would expire sooner than the max_delay, it will not be updated.
   * \param[in] max_delay The new delay to update the timer with.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  void UpdateTimer(std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief The address identifying the receiver
   */
  amsr::someip_protocol::internal::AddressPair const to_address_;

  /*!
   * \brief An observer to be called when the message is sent
   */
  OneshotUnicastTimerObserverInterface& observer_;

  /*!
   * \brief A message builder.
   */
  ServiceDiscoveryMessageBuilder& sd_message_builder_;

  /*!
   * \brief A SD message sender to send the accumulated SD message when the timer expires
   */
  ServiceDiscoveryMessageSenderInterface& message_sender_;

  /*!
   * \brief The timer manager, used to access the current clock to compute a next expiry (now + timeout)
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief The timer used to notify this unit when all accumulated SD entries shall be transmitted
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> one_shot_timer_;

  /*!
   * \brief entries::StopOfferServiceEntryContainer Vector of StopOfferService entries.
   */
  entries::OfferServiceEntryContainer offer_service_entry_container_{};

  /*!
   * \brief A container for SubscribeEntry
   */
  entries::SubscribeEventgroupEntryContainer subscribe_eventgroup_entry_container_{};

  /*!
   * \brief A container for StopSubscribeEntry
   */
  entries::StopSubscribeEventgroupEntryContainer stop_subscribe_eventgroup_entry_container_{};

  /*!
   * \brief A container for SubscribeAckEntry
   */
  entries::SubscribeEventgroupAckEntryContainer subscribe_eventgroup_ack_entry_container_{};

  /*!
   * \brief A container for SubscribeAckEntry post-send actions
   */
  ara::core::Map<entries::EventgroupEntryId, TimerSendActionType> ack_post_send_action_map_{};

  /*!
   * \brief A container for SubscribeNackEntry
   */
  entries::SubscribeEventgroupNAckEntryContainer subscribe_eventgroup_nack_entry_container_{};
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_H_
