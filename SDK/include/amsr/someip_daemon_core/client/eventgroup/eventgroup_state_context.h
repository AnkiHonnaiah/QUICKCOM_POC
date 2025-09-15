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
/**        \file  eventgroup_state_context.h
 *        \brief  An interface for client eventgroup state context
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

enum class EventgroupStateHandle : std::uint8_t;

/*!
 * \brief A Service Discovery Client eventgroup state context.
 */
class EventgroupStateContext {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  EventgroupStateContext() noexcept = default;

  /*!
   * \brief Define default destructor.
   * \steady FALSE
   */
  virtual ~EventgroupStateContext() noexcept = default;

  EventgroupStateContext(EventgroupStateContext const &) = delete;
  EventgroupStateContext(EventgroupStateContext &&) = delete;
  EventgroupStateContext &operator=(EventgroupStateContext const &) & = delete;
  EventgroupStateContext &operator=(EventgroupStateContext &&) & = delete;

  /*!
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual bool IsServiceAvailable() const = 0;

  /*!
   * \brief Indicates whether the client is connected to the server or not.
   *
   * \return true if the connection to the server is available, false otherwise.
   * \pre     -
   * \context     ANY
   * \synchronous true
   * \steady      FALSE
   */
  virtual bool IsConnected() const = 0;

  /*!
   * \brief Indicates whether the SOME/IP eventgroup is requested.
   *
   * \return true if the SOME/IP eventgroup is requested and false otherwise.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual bool IsEventgroupRequested() const = 0;

  /*!
   * \brief Initiates the transmission of a SubscribeEventgroup entry.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void SendSubscribeEventgroup() = 0;

  /*!
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void SendStopSubscribeEventgroup() = 0;

  /*!
   * \brief Starts listening for multicast UDP events.
   * \context Network
   * \steady  FALSE
   * \pre     -
   */
  virtual void StartListenForMulticastEventgroup() = 0;

  /*!
   * \brief Stops listening for multicast UDP events.
   * \context Network
   * \steady  FALSE
   * \pre     -
   */
  virtual void StopListenForMulticastEventgroup() = 0;

  /*!
   * \brief Initiates a state change.
   *
   * \param[in] handle The handle of a new state.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void ChangeState(EventgroupStateHandle const handle) = 0;

  /*!
   * \brief Called by a state when the SOME/IP eventgroup has been subscribed.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void OnSubscribed() = 0;

  /*!
   * \brief Indicates whether the reception of Acks is being monitored
   * \return  Status of Monitoring Status flag. Returns true for multicast reception and false for unicast reception.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual bool IsAckMonitoringActive() const = 0;

  /*!
   * \brief Activates monitoring of the reception of Acks
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  virtual void ActivateAckMonitoring() = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_CONTEXT_H_
