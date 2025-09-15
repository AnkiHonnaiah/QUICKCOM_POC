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
/*!        \file  someip_event_handler_interface.h
 *        \brief  Someip Event Handler Interface.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_SOMEIP_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_SOMEIP_EVENT_HANDLER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Represents a SOME/IP message.
 */
using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

/*!
 * \brief Interface for SOME/IP event handler.
 */
class SomeIpEventHandlerInterface {
 public:
  /*!
   * \brief   default construct.
   * \steady  FALSE
   */
  SomeIpEventHandlerInterface() noexcept = default;

  /*!
   * \brief   default destructor.
   * \steady  FALSE
   */
  virtual ~SomeIpEventHandlerInterface() noexcept = default;

  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface const &) = delete;
  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface &&) = delete;
  SomeIpEventHandlerInterface &operator=(SomeIpEventHandlerInterface const &) & = delete;
  SomeIpEventHandlerInterface &operator=(SomeIpEventHandlerInterface &&) & = delete;

  /*!
   * \brief handle SOME/IP Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return    -
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OnSomeIpEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                             client::Packet const packet) noexcept = 0;

  /*!
   * \brief handle SOME/IP initial field notification sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   * \param[in] client_id     SOME/IP client ID.
   *
   * \return    -
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OnSomeIpInitialFieldNotification(amsr::someip_protocol::internal::InstanceId const instance_id,
                                                client::Packet const packet,
                                                amsr::someip_protocol::internal::ClientId const client_id) noexcept = 0;

  /*!
   * \brief Handle event subscription state change.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id The subscribed event identifier with the event subscription change.
   * \param[in] new_state Current event subscription state.
   *
   * \return        -
   * \pre           -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OnSomeIpSubscriptionStateChange(configuration::types::SomeIpServiceInstanceId const &service_instance_id,
                                               someip_protocol::internal::EventId const event_id,
                                               client::eventgroup::EventSubscriptionState const new_state) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_SOMEIP_EVENT_HANDLER_INTERFACE_H_
