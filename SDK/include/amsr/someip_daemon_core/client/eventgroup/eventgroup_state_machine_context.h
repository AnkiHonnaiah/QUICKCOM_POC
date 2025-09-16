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
/**        \file  eventgroup_state_machine_context.h
 *        \brief  A client eventgroup state machine context.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A Service Discovery Client eventgroup state machine context.
 */
class EventgroupStateMachineContext {
 public:
  /*!
   * \brief   Define default constructor.
   * \steady  FALSE
   */
  EventgroupStateMachineContext() noexcept = default;

  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  virtual ~EventgroupStateMachineContext() noexcept = default;

  EventgroupStateMachineContext(EventgroupStateMachineContext const &) = delete;
  EventgroupStateMachineContext(EventgroupStateMachineContext &&) = delete;
  EventgroupStateMachineContext &operator=(EventgroupStateMachineContext const &) & = delete;
  EventgroupStateMachineContext &operator=(EventgroupStateMachineContext &&) & = delete;

  /*!
   * \brief Checks the availability of the service.
   *
   * \return true if the service is available, and false otherwise.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual bool IsServiceAvailable() const = 0;

  /*!
   * \brief Indicates whether the client is connected to the server.
   *
   * \return true if the connection to the server is available, false otherwise.
   * \pre     -
   * \context     ANY
   * \synchronous true
   * \steady      FALSE
   */
  virtual bool IsConnected() const = 0;

  /*!
   * \brief Initiates the transmission of a SubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual void SendSubscribeEventgroup(someip_protocol::internal::EventgroupId const eventgroup_id) = 0;

  /*!
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual void SendStopSubscribeEventgroup(someip_protocol::internal::EventgroupId const eventgroup_id) = 0;

  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual void OnEventgroupSubscribed(someip_protocol::internal::EventgroupId const eventgroup_id) = 0;

  /*!
   * \brief Starts listening for multicast UDP events of a given eventgroup.
   *
   * \param[in] multicast_address  A multicast IP address to which events of the given eventgroup will be sent.
   * \param[in] multicast_port     A UDP port number.
   *
   * \return true if we started listening to the given multicast address and false otherwise.
   * \pre     -
   * \context Network
   * \steady  FALSE
   */
  virtual bool StartListenForMulticastEventgroup(someip_daemon_core::IpAddress const &multicast_address,
                                                 someip_protocol::internal::Port const multicast_port) = 0;

  /*!
   * \brief Stops listening for multicast UDP events of a given eventgroup.
   * \pre     -
   * \context Network
   * \steady  FALSE
   */
  virtual void StopListenForMulticastEventgroup() = 0;

  /*!
   * \brief Checks the type of the current active offer
   * \return true if the current active offer is multicast
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual bool IsActiveOfferMulticast() const = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_MACHINE_CONTEXT_H_
