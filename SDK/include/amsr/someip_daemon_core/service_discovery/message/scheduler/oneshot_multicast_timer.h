/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH. Vector Informatik GmbH grants to you only those rights as
 * set out in the license conditions. All other rights remain
 * with Vector Informatik GmbH. \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  oneshot_multicast_timer.h
 *        \brief  Message Scheduler One-shot Multicast Timer
 *
 *      \details  This class accumulates Service Discovery entries to be sent in a one-shot manner as a multicast
 *                SOME/IP message. After the duration of the timer expires, the entries are sent as individual SOME/IP
 *                messages. It optimizes building the payload size internally, to reduce network traffic.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // unique_ptr
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_service_entry_id.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/oneshot_multicast_timer_observer_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender_interface.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief Unit responsible for accumulating multicast SD message to be sent together once the timer expires.
 */
class OneshotMulticastTimer final {
 public:
  /*!
   * \brief The signature for an action to be executed when an OfferService entry has been sent.
   */
  using TimerSendActionType = vac::language::UniqueFunction<void()>;

  /*!
   * \brief Constructor
   *
   * \param[in] timer_manager A timer manager
   * \param[in] observer An observer to be notified after a timeout
   * \param[in] message_sender A message sender
   * \param[in] sd_message_builder SD Message builder.
   *
   * \context Init
   * \steady  FALSE
   */
  OneshotMulticastTimer(amsr::steady_timer::TimerManagerInterface* timer_manager,
                        OneshotMulticastTimerObserverInterface& observer,
                        ServiceDiscoveryMessageSenderInterface& message_sender,
                        ServiceDiscoveryMessageBuilder& sd_message_builder) noexcept;

  /*!
   * \brief Define destructor
   * \context Timer
   * \steady  FALSE
   */
  ~OneshotMulticastTimer() noexcept = default;

  OneshotMulticastTimer(OneshotMulticastTimer const&) = delete;
  OneshotMulticastTimer(OneshotMulticastTimer&&) = delete;
  OneshotMulticastTimer& operator=(OneshotMulticastTimer const&) & = delete;
  OneshotMulticastTimer& operator=(OneshotMulticastTimer&&) & = delete;

  /*!
   * \brief   Adds a find service entry to the stored service discovery message
   * \details If the max delay is smaller than the current expiration time of the entry, the timer will be updated
   *          with this new expiration time.
   * \param[in] entry Entry to be added
   * \param[in] max_delay The maximum delay required by this entry for transmission.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleFindServiceEntry(entries::FindServiceEntry const& entry,
                                std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief   Adds an offer service entry to the stored service discovery message
   * \details If the max delay is smaller than the expiration time of the entry, the timer will be updated
   *          with this new expiration time.
   * \param[in] entry Entry to be added
   * \param[in] max_delay The maximum delay required by this entry for transmission.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry,
                                 std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief   Adds an offer service entry to the stored service discovery message
   * \details If the max delay is smaller than the current expiration time of the entry, the timer will be updated
   *          with this new expiration time.
   * \param[in] entry Entry to be added
   * \param[in] max_delay The maximum delay required by this entry for transmission.
   * \param[in] post_action  An action that will be executed once the entry has been sent.
   *
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const& entry, std::chrono::nanoseconds const& max_delay,
                                 TimerSendActionType&& post_action) noexcept;

  /*!
   * \brief   Adds a stop offer service entry to the stored service discovery message
   * \details If the max delay is smaller than the current expiration time of the entry, the timer will be updated
   *          with this new expiration time.
   * \param[in] entry Entry to be added
   * \param[in] max_delay The maximum delay required by this entry for transmission.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \reentrant FALSE
   * \steady    FALSE
   */
  void ScheduleStopOfferServiceEntry(entries::StopOfferServiceEntry const& entry,
                                     std::chrono::nanoseconds const& max_delay) noexcept;

  /*!
   * \brief Removes an offer service entry if it exists, otherwise do nothing
   * \param[in] entry_id ID for entry to be removed
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  void RemoveOfferServiceEntry(entries::ServiceEntryId const& entry_id) noexcept;

 private:
  /*!
   * \brief Executed when timer expires. Handles message building, sending and notifying the observer that the timer is
   * expired and can be destroyed.
   *
   * \return always false. It is an one shot timer, so we don't want the timer to start again.
   * \pre -
   * \context Timer
   * \reentrant FALSE
   * \steady FALSE
   * \trace SPEC-10144657
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
   * \brief An observer to be notified when the timer has expired
   */
  OneshotMulticastTimerObserverInterface& observer_;

  /*!
   * \brief A message builder.
   */
  ServiceDiscoveryMessageBuilder& sd_message_builder_;

  /*!
   * \brief A message sender
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
   * \brief A container for service entries
   */
  entries::FindServiceEntryContainer find_service_entry_container_{};

  /*!
   * \brief A container for offer service entries
   */
  entries::OfferServiceEntryContainer offer_service_entry_container_{};

  /*!
   * \brief A container for OfferService post-send actions
   */
  ara::core::Map<entries::ServiceEntryId, TimerSendActionType> offer_post_send_action_map_{};

  /*!
   * \brief A container for stop offer service entries
   */
  entries::StopOfferServiceEntryContainer stop_offer_service_entry_container_{};
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_H_
