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
/**        \file  eventgroup_state.h
 *        \brief  Client eventgroup state interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_context.h"
#include "vac/statemachine/state.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief Service Discovery Client eventgroup states.
 */
enum class EventgroupStateHandle : std::uint8_t {
  kNotSubscribed = 0,       /*! 'NOT_SUBSCRIBED' state */
  kSubscriptionPending = 1, /*! 'SUBSCRIPTION_PENDING' state */
  kSubscribed = 2,          /*! 'SUBSCRIBED' state */
  kSubscriptionRenewal = 3, /*! 'SUBSCRIPTION_RENEWAL' state */
};

/*!
 * \brief A Service Discovery Client eventgroup state.
 * \trace SPEC-10144847
 */
class EventgroupState : public vac::statemachine::State<EventgroupStateHandle, EventgroupStateContext> {
 public:
  using vac::statemachine::State<EventgroupStateHandle, EventgroupStateContext>::State;

  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  ~EventgroupState() noexcept override = default;

  EventgroupState() = delete;
  EventgroupState(EventgroupState const&) = delete;
  EventgroupState(EventgroupState&&) = delete;
  EventgroupState& operator=(EventgroupState const&) & = delete;
  EventgroupState& operator=(EventgroupState&&) & = delete;
  /*!
   * \brief Called when the SOME/IP eventgroup has been requested.
   *
   * \param[in,out] context A state context.
   * \context App
   * \pre     -
   * \steady FALSE
   * \trace SPEC-10144850
   */
  virtual void OnRequested(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when the SOME/IP eventgroup has been released.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnReleased(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in,out] context A state context.
   * \param[in] is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   * \context App
   * \steady  FALSE
   * \pre     -
   * \trace SPEC-4981688
   */
  virtual void OnOfferService(EventgroupStateContext& context, bool is_multicast) = 0;

  /*!
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnStopOfferService(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnSubscribeEventgroupAck(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnSubscribeEventgroupNack(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when a connection to the server has been established.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnConnectionEstablished(EventgroupStateContext& context) = 0;

  /*!
   * \brief Called when a connection to the server has been destroyed.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnConnectionClosed(EventgroupStateContext& context) const noexcept = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_H_
