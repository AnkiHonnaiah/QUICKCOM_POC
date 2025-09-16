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
/**        \file  event_notification_interface.h
 *        \brief  Event Notification Interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_NOTIFICATION_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_NOTIFICATION_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief Interface class for SOME/IP Event notification handlers.
 */
class EventNotificationInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  EventNotificationInterface() = default;

  EventNotificationInterface(EventNotificationInterface const&) = delete;
  EventNotificationInterface(EventNotificationInterface&&) = delete;
  EventNotificationInterface& operator=(EventNotificationInterface const&) & = delete;
  EventNotificationInterface& operator=(EventNotificationInterface&&) & = delete;

  /*!
   * \brief       Callback function to be triggered upon an event reception.
   *
   * \details
   *
   * \pre         -
   *
   * \context     Reactor
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  virtual void OnEventReceived() noexcept = 0;

  /*!
   * \brief       Callback function to be triggered upon event subscription state changes.
   *
   * \details
   *
   * \param[in]   state The updated event subscription state.
   *
   * \pre         -
   *
   * \context     Reactor
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  virtual void OnEventSubscriptionStateUpdate(
      ::amsr::someip_protocol::internal::SubscriptionState const state) noexcept = 0;

 protected:
  /*!
   * \brief Define destructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~EventNotificationInterface() noexcept = default;
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_NOTIFICATION_INTERFACE_H_
