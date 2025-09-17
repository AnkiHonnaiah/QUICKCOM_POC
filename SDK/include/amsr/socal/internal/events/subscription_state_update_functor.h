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
/*!        \file  subscription_state_update_functor.h
 *        \brief  Specialized Threadpool functor to handle subscription state changes asynchronously.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SUBSCRIPTION_STATE_UPDATE_FUNCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SUBSCRIPTION_STATE_UPDATE_FUNCTOR_H_

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
 * \brief Functor for an event subscription state update notification to the application.
 *
 * \unit Socal::Proxy::ProxyEvent::SubscriptionStateUpdateFunctor
 */
class SubscriptionStateUpdateFunctor final {
 public:
  /*!
   * \brief Initialize the functor to call on subscription state changes.
   * \param[in] event  The event to notify.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit SubscriptionStateUpdateFunctor(ProxyEventSyncInterface& event) noexcept;

  /*!
   * \brief Delete copy constructor.
   */
  SubscriptionStateUpdateFunctor(SubscriptionStateUpdateFunctor const&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(SubscriptionStateUpdateFunctor const&) noexcept -> SubscriptionStateUpdateFunctor& = delete;

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
  SubscriptionStateUpdateFunctor(SubscriptionStateUpdateFunctor&&) noexcept = default;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(SubscriptionStateUpdateFunctor&&) & noexcept -> SubscriptionStateUpdateFunctor& = delete;

  /*!
   * \brief Destroys the SubscriptionStateUpdateFunctor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  ~SubscriptionStateUpdateFunctor() noexcept = default;

  /*!
   * \brief   Execute the subscription state notification handler.
   * \details Called from the the Default Thread Pool Worker Thread.
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

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SUBSCRIPTION_STATE_UPDATE_FUNCTOR_H_
