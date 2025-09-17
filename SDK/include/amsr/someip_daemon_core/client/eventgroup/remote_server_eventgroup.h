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
/**        \file  remote_server_eventgroup.h
 *        \brief  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_REMOTE_SERVER_EVENTGROUP_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_REMOTE_SERVER_EVENTGROUP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief   RemoteServerEventgroup.
 * \details Structure containing the subscription state and a count of subscribers
 */
class RemoteServerEventgroup final {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] eventgroup_id The eventgroup id
   *
   * \pre         -
   * \context     Any
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit RemoteServerEventgroup(someip_protocol::internal::EventgroupId const eventgroup_id) noexcept;

  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  ~RemoteServerEventgroup() noexcept = default;

  RemoteServerEventgroup() = delete;
  RemoteServerEventgroup(RemoteServerEventgroup const&) = delete;
  RemoteServerEventgroup(RemoteServerEventgroup&&) = delete;
  RemoteServerEventgroup& operator=(RemoteServerEventgroup const&) & = delete;
  RemoteServerEventgroup& operator=(RemoteServerEventgroup&&) & = delete;

  /*!
   * \brief Trigger subscription to this eventgroup.
   *
   * \param[in] event_id The event id to subscribe to
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Subscribe(someip_protocol::internal::EventId const event_id) noexcept;

  /*!
   * \brief Trigger unsubscription to this eventgroup.
   *
   * \param[in] event_id The event id to unsubscribe to
   *
   * \pre         The eventgroup has been subscribed (count of Subscribe calls >= count of Unsubscribe calls)
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void Unsubscribe(someip_protocol::internal::EventId const event_id) noexcept;

  /*!
   * \brief Sets the subscription state to "Subscribed".
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnSubscriptionAccepted() noexcept;

  /*!
   * \brief Sets the subscription state to "Pending".
   *
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnSubscriptionCancelled() noexcept;

  /*!
   * \brief Gets subscription state.
   *
   * \return The current subscription state
   *
   * \details States:
   *          - NotSubscribed: if subscription_counter_ == 0
   *          - SubscriptionPending: if subscription_counter_ > 0 and subscription has not been accepted.
   *          - Subscribed: if subscription_counter_ > 0 and subscription has been accepted.
   *
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventSubscriptionState GetSubscriptionState() const noexcept;

 private:
  /*!
   * \brief The number of times the SOME/IP eventgroup has been subscribed.
   */
  bool subscription_accepted_{false};

  /*!
   * \brief The number of times the SOME/IP eventgroup has been subscribed.
   */
  std::size_t subscription_counter_{};
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_REMOTE_SERVER_EVENTGROUP_H_
