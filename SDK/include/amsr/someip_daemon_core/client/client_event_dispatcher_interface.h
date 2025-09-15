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
/**        \file  client_event_dispatcher_interface.h
 *        \brief  This file contains the API's which is to be used for routing of someip events and Pdu events.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief ClientEventDispatcherInterface class contains the API's which is to be used for
 *        routing of someip events and PDU events.
 */
class ClientEventDispatcherInterface {
 public:
  /*!
   * \brief Constructor of ClientEventDispatcherInterface.
   * \steady FALSE
   */
  ClientEventDispatcherInterface() noexcept = default;

  /*!
   * \brief Destructor of ClientEventDispatcherInterface.
   * \steady FALSE
   */
  virtual ~ClientEventDispatcherInterface() noexcept = default;

  ClientEventDispatcherInterface(ClientEventDispatcherInterface const&) = delete;
  ClientEventDispatcherInterface(ClientEventDispatcherInterface&&) = delete;
  ClientEventDispatcherInterface& operator=(ClientEventDispatcherInterface const&) & = delete;
  ClientEventDispatcherInterface& operator=(ClientEventDispatcherInterface&&) & = delete;

  /*!
   * \brief Subscribe to SOME/IP event.
   *
   * \param[in] event_id        A SOME/IP event identifier.
   * \param[in] instance_id     A SOME/IP Instance identifier.
   * \param[in] event_handler   A pointer to the event observer.
   * \param[in] client_id       A SOME/IP client identifier.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in the
   *      RemoteServer, where the API is only called in case the event is found in the configuration.
   *      Abort is triggered in case the pre-condition is not met, as it's a contract violation and implementation
   *      issue.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   App
   * \reentrant FALSE
   */
  virtual void SubscribeSomeIpEvent(::amsr::someip_protocol::internal::EventId const event_id,
                                    amsr::someip_protocol::internal::InstanceId const instance_id,
                                    std::shared_ptr<client::SomeIpEventHandlerInterface> event_handler,
                                    someip_protocol::internal::ClientId const client_id) noexcept = 0;

  /*!
   * \brief Subscribe to PDU event.
   *
   * \param[in] event_id         A PDU event identifier.
   * \param[in] event_handler    A pointer to the subscribed application.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in
   *      the RemoteServer, where the API is only called in case the event is found in the configuration. Abort is
   *      triggered in case the pre-condition is not met, as it's a contract violation and implementation issue.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   App
   * \reentrant FALSE
   */
  virtual void SubscribePduEvent(::amsr::someip_protocol::internal::EventId const event_id,
                                 std::shared_ptr<client::PduEventHandlerInterface> event_handler) noexcept = 0;

  /*!
   * \brief Unsubscribe to SomeIp event.
   *
   * \param[in] event_id       A SomeIp event identifier.
   * \param[in] event_handler  A pointer to the subscribed application.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in the
   *      RemoteServer, where the API is only called in case the event is found in the configuration.
   *      Abort is triggered in case the pre-condition is not met, as it's a contract violation and implementation
   *      issue.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   App
   * \reentrant FALSE
   */
  virtual void UnsubscribeSomeIpEvent(::amsr::someip_protocol::internal::EventId const event_id,
                                      std::shared_ptr<client::SomeIpEventHandlerInterface> event_handler) noexcept = 0;

  /*!
   * \brief Unsubscribe to PDU event.
   *
   * \param[in] event_id         A PDU event identifier.
   * \param[in] event_handler    A pointer to the subscribed application.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in
   *      the RemoteServer, where the API is only called in case the event is found in the configuration. Abort is
   *      triggered in case the pre-condition is not met, as it's a contract violation and implementation issue.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   App
   * \reentrant FALSE
   */
  virtual void UnsubscribePduEvent(::amsr::someip_protocol::internal::EventId const event_id,
                                   std::shared_ptr<client::PduEventHandlerInterface> event_handler) noexcept = 0;

  /*!
   * \brief handle SOME/IP Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in the
   *      RemoteServer, where the API is only called in case the event is found in the configuration.
   *      Abort is triggered in case the pre-condition is not met, as it's a contract violation and implementation
   *      issue.
   *
   * \pre       -
   * \internal
   * \steady TRUE
   * \endinternal
   * \context   Network
   * \reentrant FALSE
   */
  virtual void OnSomeIpEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                             client::Packet const& packet) noexcept = 0;

  /*!
   * \brief handle PDU Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in
   *      the RemoteServer, where the API is only called in case the event is found in the configuration. Abort is
   *      triggered in case the pre-condition is not met, as it's a contract violation and implementation issue.
   *
   * \internal
   * \steady TRUE
   * \endinternal
   * \context   Network
   * \reentrant FALSE
   */
  virtual void OnPduEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                          client::PduPacket const& packet) noexcept = 0;

  /*!
   * \brief Notify observer about the event subscription state change.
   *
   * \param[in] someip_service_instance_id SOME/IP Service Instance id.
   * \param[in] event_id The subscribed event identifier with the event subscription change.
   * \param[in] new_state Current event subscription state.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in the
   *      RemoteServer, where the API is only called in case the event is found in the configuration.
   *      Abort is triggered in case the pre-condition is not met, as it's a contract violation and implementation
   *      issue.
   *
   * \pre           -
   * \internal
   * \steady FALSE
   * \endinternal
   * \context Network
   * \reentrant FALSE
   */
  virtual void OnSomeIpSubscriptionStateChange(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      ::amsr::someip_protocol::internal::EventId const event_id,
      someip_daemon_core::client::eventgroup::EventSubscriptionState new_state) noexcept = 0;

  /*!
   * \brief Notify observer about the event subscription state change.
   *
   * \param[in] someip_service_instance_id SOME/IP Service Instance id.
   * \param[in] event_id The subscribed event identifier with the event subscription change.
   * \param[in] new_state Current event subscription state.
   *
   * \pre The event must have been configured in the event deployment since this unit will only be integrated in
   *      the RemoteServer, where the API is only called in case the event is found in the configuration. Abort is
   *      triggered in case the pre-condition is not met, as it's a contract violation and implementation issue.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   Network
   * \reentrant FALSE
   */
  virtual void OnPduSubscriptionStateChange(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      ::amsr::someip_protocol::internal::EventId const event_id,
      someip_daemon_core::client::eventgroup::EventSubscriptionState new_state) noexcept = 0;

  /*!
   * \brief Invalidate the field cache entry.
   * \details If the field cache conatins value for the event id, then reset the content of field cache
   *          for this specific event.
   *
   * \param[in] event_id The event id.
   *
   * \internal
   * \steady FALSE
   * \endinternal
   * \context   App
   * \reentrant FALSE
   */
  virtual void InvalidateFieldCacheEntry(::amsr::someip_protocol::internal::EventId const event_id) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_INTERFACE_H_
