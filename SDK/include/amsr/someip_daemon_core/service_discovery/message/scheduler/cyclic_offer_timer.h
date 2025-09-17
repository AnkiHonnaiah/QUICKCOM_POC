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
/**        \file  cyclic_offer_timer.h
 *        \brief  Periodic timer for Service Discovery messages.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_CYCLIC_OFFER_TIMER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_CYCLIC_OFFER_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender_interface.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief Handle periodic Offer service messages
 * \trace SPEC-4663369
 */
class CyclicOfferTimer final {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] timer_manager A reference to the timer manager for scheduling of delays.
   * \param[in] cyclic_offer_period_ns Periodic delay of the timer in nanoseconds.
   * \param[in] sender Endpoint to send the periodic message.
   * \param[in] sd_message_builder SD Message builder.
   *
   * \context Init
   * \steady FALSE
   */
  CyclicOfferTimer(amsr::steady_timer::TimerManagerInterface *timer_manager,
                   std::chrono::nanoseconds const &cyclic_offer_period_ns,
                   ServiceDiscoveryMessageSenderInterface &sender,
                   ServiceDiscoveryMessageBuilder &sd_message_builder) noexcept;

  /*!
   * \brief Define destructor
   * \context Shutdown
   * \steady FALSE
   */
  ~CyclicOfferTimer() noexcept = default;

  CyclicOfferTimer() = delete;
  CyclicOfferTimer(CyclicOfferTimer const &) = delete;
  CyclicOfferTimer(CyclicOfferTimer &&) = delete;
  CyclicOfferTimer &operator=(CyclicOfferTimer const &) & = delete;
  CyclicOfferTimer &operator=(CyclicOfferTimer &&) & = delete;

  /*!
   * \brief ScheduleOfferServiceEntry
   *
   * \param[in] offer_entry Offer entry to be scheduled in next timer expiry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-4981595
   */
  void ScheduleOfferServiceEntry(entries::OfferServiceEntry const &offer_entry);

  /*!
   * \brief RemoveOfferServiceEntry
   *
   * \param[in] offer_entry_id Entry Id of the offer to be removed from the cyclic message.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  void RemoveOfferServiceEntry(entries::ServiceEntryId const &offer_entry_id);

 private:
  /*!
   * \brief Sends periodic messages and prepares the next one.
   *
   * \return always true because this is a cyclic timer.
   * \pre -
   * \context Timer
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-10144657
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
   * \brief Period of the timer.
   */
  std::chrono::nanoseconds const cyclic_offer_period_ns_;

  /*!
   * \brief Half of the period of the timer.
   */
  std::chrono::nanoseconds const half_period_ns_{cyclic_offer_period_ns_ / 2U};

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
   * \brief Container of offer service entries to be scheduled after the next timeout.
   */
  entries::OfferServiceEntryContainer deferred_message_{};
};

/*!
 * \brief Represents a unique pointer to a CyclicOfferTimer.
 */
using CyclicOfferTimerUniquePtr = amsr::UniquePtr<CyclicOfferTimer>;
/*!
 * \brief Map type of unique pointers to CyclicOfferTimers and their cyclic delay.
 */
using CyclicOfferTimerMap = ara::core::Map<std::chrono::nanoseconds, CyclicOfferTimerUniquePtr>;

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_CYCLIC_OFFER_TIMER_H_
