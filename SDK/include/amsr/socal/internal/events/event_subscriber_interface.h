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
/**        \file  event_subscriber_interface.h
 *        \brief  Interface for receiving event updates from the bindings.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_SUBSCRIBER_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_SUBSCRIBER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface for receiving event updates from the bindings.
 */
class EventSubscriberInterface {
 public:
  /*!
   * \brief Constructs the class EventSubscriberInterface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  EventSubscriberInterface() noexcept = default;
  /*!
   * \brief   Destroys the EventSubscriberInterface.
   * \details The destructor will not return until all scheduled tasks are removed or done.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~EventSubscriberInterface() noexcept = default;

  EventSubscriberInterface(EventSubscriberInterface const&) = delete;

  EventSubscriberInterface(EventSubscriberInterface&&) = delete;

  EventSubscriberInterface& operator=(EventSubscriberInterface const&) & = delete;

  EventSubscriberInterface& operator=(EventSubscriberInterface&&) & = delete;

  /*!
   * \brief   Handles a received event notification.
   * \details Called upon every new event reception from skeleton. Schedules a task in R-Port Default Thread Pool
   *          to invoke the event receive handler.
   * \pre         Must not be invoked prior subscription and after unsubscription.
   * \context     Backend
   * \threadsafe  FALSE for same EventSubscriberInterface instance, TRUE for different instances.
   * \reentrant   FALSE for same EventSubscriberInterface instance, FALSE for different instances.
   * \synchronous FALSE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void HandleEventNotification() noexcept = 0;

  /*!
   * \brief   Handles an update of the subscription state of the event.
   * \details Called upon subscription state update of this event. Schedules a task in R-Port Default Thread Pool
   *          to invoke the subscription state handler.
   * \param[in] state  The current subscription state of this event. This state is not evaluated anymore by Socal.
   * \pre         Must not be invoked prior subscription and after unsubscription.
   *              The caller must therefore synchronize this API against Unsubscribe().
   * \context     Backend | App during Unsubscription.
   * \threadsafe  FALSE for same EventSubscriberInterface instance, TRUE for different instances.
   * \reentrant   FALSE for same EventSubscriberInterface instance, FALSE for different instances.
   * \synchronous FALSE
   * \steady TRUE
   */
  virtual void HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState const state) noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_EVENT_SUBSCRIBER_INTERFACE_H_
