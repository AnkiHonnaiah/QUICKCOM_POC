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
/*!        \file  event_notification_functor.h
 *        \brief  Specialized Threadpool functor to handle EventNotification asynchronously.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_NOTIFICATION_FUNCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_NOTIFICATION_FUNCTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include "amsr/socal/internal/events/proxy_event_sync_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Functor for an event receive notification to the application.
 *
 * \unit Socal::Proxy::ProxyEvent::EventNotificationFunctor
 */
class EventNotificationFunctor final {
 public:
  /*!
   * \brief Initialize the functor to call on event notification.
   * \param[in] event  The event to notify.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit EventNotificationFunctor(ProxyEventSyncInterface& event) noexcept;

  /*!
   * \brief Delete copy constructor.
   */
  EventNotificationFunctor(EventNotificationFunctor const&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(EventNotificationFunctor const&) noexcept -> EventNotificationFunctor& = delete;

  /*!
   * \brief Move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  EventNotificationFunctor(EventNotificationFunctor&&) noexcept = default;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(EventNotificationFunctor&&) & noexcept -> EventNotificationFunctor& = delete;

  /*!
   * \brief Destroys the EventNotificationFunctor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  ~EventNotificationFunctor() noexcept = default;

  /*!
   * \brief   Execute the event notification handler.
   * \details Called in the context of the Default Thread Pool.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void operator()() const noexcept;

 private:
  /*!
   * \brief The event to notify.
   */
  std::reference_wrapper<ProxyEventSyncInterface> event_;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_NOTIFICATION_FUNCTOR_H_
