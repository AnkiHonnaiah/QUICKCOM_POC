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
/**        \file  eventgroup_resubscription_timer.h
 *        \brief  A class that implements a timer to manage eventgroup subscription retrial.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_TIMER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_resubscription_listener.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A timer to manage eventgroup subscription retrial. Timer expires each subscribe_retry_delay period until
 *        subscribe_retry_max attemps are reached.
 */
class EventgroupResubscriptionTimer final {
 public:
  /*!
   * \brief Constructor of EventgroupResubscriptionTimer.
   *
   * \param[in] timer_manager           Timer manager that manage a set of timers where this timer is added.
   * \param[in] listener                Listener to notify when timer expires and resubscription must be triggered.
   * \param[in] subscribe_retry_delay   Timer period between subscription retrials.
   * \param[in] subscribe_retry_max     Maximum number of subscription attemps.
   *
   * \steady FALSE
   */
  EventgroupResubscriptionTimer(
      amsr::steady_timer::TimerManagerInterface *timer_manager, EventgroupResubscriptionListener &listener,
      someip_protocol::internal::SubscribeEventgroupRetryDelay const subscribe_retry_delay,
      someip_protocol::internal::SubscribeEventgroupRetryMax const subscribe_retry_max) noexcept;

  /*!
   * \brief   Destructor of EventgroupResubscriptionTimer.
   * \steady  FALSE
   */
  ~EventgroupResubscriptionTimer() noexcept = default;

  EventgroupResubscriptionTimer(EventgroupResubscriptionTimer const &) = delete;
  EventgroupResubscriptionTimer(EventgroupResubscriptionTimer &&) = delete;
  EventgroupResubscriptionTimer &operator=(EventgroupResubscriptionTimer const &) & = delete;
  EventgroupResubscriptionTimer &operator=(EventgroupResubscriptionTimer &&) & = delete;

  /*!
   * \brief   Starts the timer.
   * \details This API call abstracts the call to "TimerInterface::Start()" with the correct timing parameter, stored as
   *          a member variable.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void StartTimer() noexcept;

  /*!
   * \brief   Stops the timer and reset subscription retrial count.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void StopTimer() noexcept;

 private:
  /*!
   * \brief     Increment subscribe_retry_count when the timer expires and notify the listener. Stops notifying if
   *            subscribe_retry_max is reached and stops the timer.
   * \context   Timer
   * \reentrant False
   * \steady    FALSE
   * \pre       -
   */
  void HandleTimer() noexcept;

  /*!
   * \brief Listener to be notified every time the timer expires.
   */
  EventgroupResubscriptionListener &listener_;

  /*!
   * \brief Delay between subscription retrials. Timer period.
   */
  someip_protocol::internal::SubscribeEventgroupRetryDelay const subscribe_retry_delay_;

  /*!
   * \brief Maximum subscription retrials to attempt. It is the maximum number of times the timer is
   *        restarted.
   */
  someip_protocol::internal::SubscribeEventgroupRetryMax const subscribe_retry_max_;

  /*!
   * \brief Subscription retrial count. It is incremented every time the timer expires.
   */
  someip_protocol::internal::SubscribeEventgroupRetryMax subscribe_retry_count{0U};

  /*!
   * \brief Timer to trigger periodic resubscription
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> resubscription_timer_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_TIMER_H_
