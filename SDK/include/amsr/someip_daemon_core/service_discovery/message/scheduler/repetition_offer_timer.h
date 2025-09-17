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
/**        \file  repetition_offer_timer.h
 *        \brief  Repetition timer for Service Discovery messages.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_REPETITION_OFFER_TIMER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_REPETITION_OFFER_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>

#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender_interface.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief Structure that keeps track of the repetition counts.
 */
struct Repetitions {
  /*!
   * \brief Number of cycles since previous sent offer.
   */
  std::size_t cycle_count{0U};
  /*!
   * \brief Next cycle count at which the offer will be sent.
   */
  std::size_t next_send_cycle_count{0U};
  /*!
   * \brief The number of cycles between the final send offer and the previous one.
   * Example:
   * - Maximum repetitions set for this offer = 4
   * - Offer is sent in cycles:   1 . 3 ... 7 ....... 15
   * - Difference between cycles:   2c   4c      8c
   * - For a maximum repetition count = 4, the max_repetitions_cycle_count is 2^(4-1) = 8
   */
  std::size_t max_repetitions_cycle_count{0U};
};

/*!
 * \brief Handle periodic Offer service messages.
 */
class RepetitionOfferTimer final {
 public:
  /*!
   * \brief The signature for an action to be executed when a OfferService entry has been sent
   */
  using TimerSendActionType = vac::language::UniqueFunction<void()>;

  /*!
   * \brief A container for TimerPostSendAction
   */
  using TimerPostSendActionContainer = ara::core::Vector<TimerSendActionType>;

  /*!
   * \brief A map of last OfferEntry repetition post-send actions
   */
  using TimerPostSendActionMap = ara::core::Map<entries::ServiceEntryId, TimerSendActionType>;

  /*!
   * \brief Map of repetitions
   */
  using RepetitionsMap = ara::core::Map<entries::ServiceEntryId, Repetitions>;

  /*!
   * \brief Constructor
   *
   * \param[in] timer_manager A reference to the timer manager for scheduling of delays.
   * \param[in] repetitions_base_delay_ns Periodic delay of the timer in nanoseconds.
   * \param[in] sender Endpoint to send the periodic message.
   * \param[in] sd_message_builder SD Message builder.
   *
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  RepetitionOfferTimer(amsr::steady_timer::TimerManagerInterface *timer_manager,
                       std::chrono::nanoseconds const &repetitions_base_delay_ns,
                       ServiceDiscoveryMessageSenderInterface &sender,
                       ServiceDiscoveryMessageBuilder &sd_message_builder) noexcept;
  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RepetitionOfferTimer() noexcept = default;

  RepetitionOfferTimer() = delete;
  RepetitionOfferTimer(RepetitionOfferTimer const &) = delete;
  RepetitionOfferTimer(RepetitionOfferTimer &&) = delete;
  RepetitionOfferTimer &operator=(RepetitionOfferTimer const &) & = delete;
  RepetitionOfferTimer &operator=(RepetitionOfferTimer &&) & = delete;

  /*!
   * \brief ScheduleOfferServiceEntry
   *
   * \param[in] offer_entry Offer entry to be scheduled in next timer expiry.
   * \param[in] max_repetitions Maximum number of times the offer must be sent.
   * \param[in] post_action  An action that will be executed once the entry has been sent.
   *
   * \context   Timer
   * \reentrant FALSE
   * \trace     SPEC-10144695
   * \steady    FALSE
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const &offer_entry, std::size_t max_repetitions,
                                 TimerSendActionType &&post_action);

  /*!
   * \brief RemoveOfferServiceEntry
   *
   * \param[in] offer_entry_id Entry Id of the offer to be removed from the cyclic message.
   *
   * \pre -
   * \context   App, Timer, Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  void RemoveOfferServiceEntry(entries::ServiceEntryId const &offer_entry_id);

 private:
  /*!
   * \brief Sends periodic messages and prepares the next one.
   *
   * \pre -
   * \context   Timer
   * \reentrant FALSE
   * \trace     SPEC-10144657
   * \steady    FALSE
   */
  void HandleTimer() noexcept;

  /*!
   * \brief Check if a scheduled offer shall be sent in the next cycle (timer timeout)
   *
   * \context   Timer
   * \reentrant FALSE
   * \steady    FALSE
   */
  bool CheckSendCurrentCycle() noexcept;

  /*!
   * \brief A message builder.
   */
  ServiceDiscoveryMessageBuilder &sd_message_builder_;

  /*!
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSenderInterface &message_sender_;

  /*!
   * \brief The period of the timer.
   */
  std::chrono::nanoseconds const repetitions_base_delay_ns_;

  /*!
   * \brief Half of the period of the timer.
   */
  std::chrono::nanoseconds const half_period_ns_{repetitions_base_delay_ns_ / 2U};

  /*!
   * \brief Timer manager, used to get the current timestamp upon offer schedule.
   */
  amsr::steady_timer::TimerManagerInterface *timer_manager_;

  /*!
   * \brief The timer
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> timer_;

  /*!
   * \brief Container of offer service entries to be scheduled in next timer timeout.
   */
  entries::OfferServiceEntryContainer imminent_message_{};

  /*!
   * \brief Container of offer service entries to be scheduled in later timeouts.
   */
  entries::OfferServiceEntryContainer message_container_{};

  /*!
   * \brief A map of last OfferEntry repetition post-send actions
   */
  TimerPostSendActionMap offer_post_send_action_map_{};

  /*!
   * \brief A container for last OfferEntry repetition post-send actions
   */
  TimerPostSendActionContainer offer_post_send_action_container_{};

  /*!
   * \brief A container for Repetition counters
   */
  RepetitionsMap offer_repetitions_map_{};
};

/*!
 * \brief Represents a unique pointer to a RepetitionOfferTimer.
 */
using RepetitionOfferTimerUniquePtr = amsr::UniquePtr<RepetitionOfferTimer>;

/*!
 * \brief Map type of unique pointers to RepetitionOfferTimers and their cyclic delay.
 */
using RepetitionOfferTimerMap = ara::core::Map<std::chrono::nanoseconds, RepetitionOfferTimerUniquePtr>;

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_REPETITION_OFFER_TIMER_H_
