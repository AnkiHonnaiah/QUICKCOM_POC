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
/**        \file  proxy_event_sync_interface.h
 *        \brief  Interface towards EventNotificationFunctor and SubscriptionStateUpdateFunctor.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_SYNC_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_SYNC_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface towards the SubscriptionStateUpdateFunctor and EventNotificationFunctor to handle subscription state
 *        updates and event notifications.
 */
class ProxyEventSyncInterface {
 public:
  /*!
   * \brief Constructs the class ProxyEventSyncInterface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProxyEventSyncInterface() noexcept = default;
  /*!
   * \brief Destroys the ProxyEventSyncInterface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProxyEventSyncInterface() noexcept = default;

  ProxyEventSyncInterface(ProxyEventSyncInterface const&) = delete;
  ProxyEventSyncInterface(ProxyEventSyncInterface&&) = delete;
  ProxyEventSyncInterface& operator=(ProxyEventSyncInterface const&) & = delete;
  ProxyEventSyncInterface& operator=(ProxyEventSyncInterface&&) & = delete;

  /*!
   * \brief   Notify event receive handler on reception of a new event sample. Internal use only.
   * \details Called in the context of the Default Thread Pool.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   *
   * \trace SPEC-4980082, SPEC-4980117, SPEC-4980378
   */
  virtual void HandleEventNotificationSync() noexcept = 0;

  /*!
   * \brief   Called upon event subscription state update (internal use only).
   * \details Called from the the Default Thread Pool Worker Thread.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual void HandleEventSubscriptionStateUpdateSync() noexcept = 0;
};
}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_PROXY_EVENT_SYNC_INTERFACE_H_
