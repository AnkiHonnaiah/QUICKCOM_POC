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
/**        \file  eventgroup_state_machine.h
 *        \brief  A client eventgroup state machine
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_resubscription_listener.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_resubscription_timer.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_machine_context.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_owner.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_pool.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief A Service Discovery Client eventgroup state machine.
 */
class EventgroupStateMachine : public EventgroupStateContext, public EventgroupResubscriptionListener {
 public:
  /*!
   * \brief Constructor of EventgroupStateMachine.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \param[in,out] context A state machine context.
   * \context App
   * \pre     -
   * \steady  FALSE
   */
  EventgroupStateMachine(someip_protocol::internal::EventgroupId const eventgroup_id,
                         EventgroupStateMachineContext *context) noexcept;

  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  ~EventgroupStateMachine() noexcept override = default;

  EventgroupStateMachine() = delete;
  EventgroupStateMachine(EventgroupStateMachine const &) = delete;
  EventgroupStateMachine(EventgroupStateMachine &&) = delete;
  EventgroupStateMachine &operator=(EventgroupStateMachine const &) & = delete;
  EventgroupStateMachine &operator=(EventgroupStateMachine &&) & = delete;

  /*!
   * \brief Returns the SOME/IP eventgroup identifier.
   *
   * \return A SOME/IP eventgroup identifier.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  someip_protocol::internal::EventgroupId GetEventgroupId() const;

  /*!
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \return Current eventgroup subscription state.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  EventSubscriptionState GetSubscriptionState() const;

  /*!
   * \brief Gets the current count of the eventgroup subscribers.
   *
   * \return Current eventgroup subscribers count.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  std::size_t GetSubscribersCount() const;

  /*!
   * \brief Returns the multicast IP address assigned to the SOME/IP eventgroup.
   *
   * \return A multicast IP address.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  ara::core::Optional<someip_daemon_core::IpAddress> const &GetMulticastAddress() const;

  /*!
   * \brief Returns the multicast UDP port number assigned to the SOME/IP eventgroup.
   *
   * \return A multicast UDP port.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  someip_protocol::internal::Port GetMulticastPort() const;

  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in] is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void OnOfferService(bool is_multicast);

  /*!
   * \brief Called when a matching StopOfferService entry has been received.
   * \context App
   * \steady FALSE
   * \pre     -
   * \trace SPEC-4981666
   */
  void OnStopOfferService() noexcept;

  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   * \trace SPEC-5951201
   */
  void OnSubscribe();

  /*!
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.

   * \context App
   * \steady  FALSE
   * \pre     At least one subscriber shall be subsribed.
   * \trace SPEC-5951529
   */
  void OnUnsubscribe();

  /*!
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param[in] multicast_address A multicast IP address assigned to the SOME/IP eventgroup by server.
   * \param[in] multicast_port A multicast UDP port assigned to the SOME/IP eventgroup by server.
   * \context   App
   * \steady    FALSE
   * \trace SPEC-10144866
   */
  void OnSubscribeEventgroupAck(ara::core::Optional<someip_daemon_core::IpAddress> const &multicast_address,
                                someip_protocol::internal::Port const multicast_port);

  /*!
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void OnSubscribeEventgroupNack();

  /*!
   * \brief Called when a connection to the server has been established.
   * \context Network
   * \steady  FALSE
   */
  void OnConnectionEstablished();

  /*!
   * \brief Called when a connection to the server has been closed.
   * \context Network
   * \steady  FALSE
   */
  void OnConnectionClosed();

  /*!
   * \brief Indicates whether the reception of Acks is being monitored
   * \return Status of monitor Ack flag. Returns true for multicast reception and false for unicast reception.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  bool IsAckMonitoringActive() const override;

  /*!
   * \brief Activates monitoring of the reception of Acks
   * \steady FALSE
   */
  void ActivateAckMonitoring() override;

  /*!
   * \brief Enables eventgroup re-subscription mechanism
   * \param[in] subscription_retrial_config Configuration containing the max amount of repetitions and delay between
   *                                        them for subscription retrial.
   * \param[in] timer_manager Timer manager needed for the creation of a resubscription timer instance.
   * \context   Init
   * \steady    FALSE
   * \pre       This API must not be called before. resubscription_timer_ is empty.
   * \trace SPEC-9646202
   */
  void EnableEventgroupReSubscription(
      configuration::types::SomeIpSdClientEventGroupSubscriptionRetrialConfig const &subscription_retrial_config,
      amsr::steady_timer::TimerManagerInterface *timer_manager) noexcept;

 protected:
  /*!
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  bool IsServiceAvailable() const override;

  /*!
   * \brief Indicates whether the client is connected to the server.
   *
   * \return true if the connection to the server is available, false otherwise.
   * \pre     -
   * \context ANY
   * \steady  FALSE
   * \synchronous true
   */
  bool IsConnected() const override;

  /*!
   * \brief Indicates whether the SOME/IP eventgroup is requested or not.
   *
   * \return true if the SOME/IP eventgroup is requested and false otherwise.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  bool IsEventgroupRequested() const override;

  /*!
   * \brief Initiates the transmission of a SubscribeEventgroup entry.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void SendSubscribeEventgroup() override;

  /*!
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  void SendStopSubscribeEventgroup() override;

  /*!
   * \brief Starts listening for multicast UDP events of a given eventgroup.
   * \context Network
   * \steady  FALSE
   */
  void StartListenForMulticastEventgroup() override;

  /*!
   * \brief Stops listening for multicast UDP events of a given eventgroup.
   * \context Network
   * \steady  FALSE
   */
  void StopListenForMulticastEventgroup() override;

  /*!
   * \brief Initiates a state change.
   *
   * \param[in] handle The handle of a new state.
   * \pre     A valid new state shall be passed.
   * \context App
   * \steady  FALSE
   */
  void ChangeState(EventgroupStateHandle const handle) override;

  /*!
   * \brief Called by a state when the SOME/IP eventgroup has been subscribed.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnSubscribed() override;

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \return A string representing the logger context identifier for given SOME/IP service parameters.
   * \pre     -
   * \context ANY
   * \steady  FALSE
   */
  static ara::core::String GetLoggerContextId(someip_protocol::internal::EventgroupId const eventgroup_id);

  /*!
   * \brief     Return current state.
   * \return    Current state.
   * \pre       -
   * \reentrant False
   * \context   Testing
   * \steady    FALSE
   */
  EventgroupStateHandle GetCurrentState();

 private:
  /*!
   * \brief Implements behaviour for subscription retrial.
   * \pre     -
   * \details Whenever this API is triggered, we know no ACK has been received within the configured time, so a new
   * SubscribeEventgroup shall be sent to re-attempt subscription.
   * \context Timer
   * \steady  FALSE
   */
  void OnTimerExpired() noexcept override;

  /*!
   * \brief A SOME/IP eventgroup identifier.
   */
  someip_protocol::internal::EventgroupId const eventgroup_id_;

  /*!
   * \brief A multicast IP address assigned to the SOME/IP eventgroup by server.
   */
  ara::core::Optional<someip_daemon_core::IpAddress> subscription_multicast_address_{};

  /*!
   * \brief A multicast UDP port assigned to the SOME/IP eventgroup by server.
   */
  someip_protocol::internal::Port subscription_multicast_port_{};

  /*!
   * \brief A state machine context.
   */
  EventgroupStateMachineContext *context_;

  /*!
   * \brief A state pool.
   */
  EventgroupStatePool state_pool_{eventgroup_id_};

  /*!
   * \brief Status of listening to multicast eventgroup.
   */
  bool is_listening_to_multicast_{false};

  /*!
   * \brief Flag to indicate if the states should monitor Ack reception
   * \details Set to true when sending a SubscribeEventgroup message due to a multicast OfferService
   *          Set to false when sending a SubscribeEventgroup message due to a unicast OfferService
   *          Set to true upon reception of a multicast OfferService (even if no SubscribeEventgroup is received)
   *
   *          This flag is used to indicate whether the state machine should react to the reception of OfferService
   *          messages when a SubscribeEventgroup has been sent but no SubscribeEventgroupAck is received.
   *          If the flag is false, reception of OfferService messages in this situation don't trigger a re-send
   *          of Subscribe/StopSubscribe messages.
   */
  bool monitor_ack_{false};

  /*!
   * \brief A state owner.
   */
  EventgroupStateOwner state_owner_{state_pool_, *this};

  /*!
   * \brief The number of times the SOME/IP eventgroup has been subscribed.
   */
  std::size_t subscription_counter_{};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription, GetLoggerContextId(eventgroup_id_)};

  /*!
   * \brief An optional resubscription timer. Used if resubscription retrial mechanism is configured.
   */
  ara::core::Optional<EventgroupResubscriptionTimer> resubscription_timer_{ara::core::nullopt};
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_H_
