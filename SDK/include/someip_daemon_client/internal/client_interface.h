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
/*!        \file  someip_daemon_client/internal/client_interface.h
 *        \brief  Declaration of SOME/IP events handlers of ClientInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CLIENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd_app_protocol/internal/pdu_message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Interface class for callbacks from SomeIpDaemonClient to SomeipPosixClientManager. This must be realized by
 * SomeipPosixClientManager.
 */
class ClientInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \vprivate  Product Private
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ClientInterface() = default;
  /*!
   * \brief Define destructor.
   * \vprivate  Product Private
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientInterface() = default;
  /*!
   * \brief Delete copy constructor.
   */
  ClientInterface(ClientInterface const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  ClientInterface(ClientInterface&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(ClientInterface const&) & -> ClientInterface& = delete;
  /*!
   * \brief Delete move assignment.
   */
  auto operator=(ClientInterface&&) & -> ClientInterface& = delete;

  /*!
   * \brief       Receive handler for SOME/IP events.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet The SOME/IP message.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate  Product Private
   * \synchronous FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTNEXTLINE (readability-avoid-const-params-in-decls)
  virtual void HandleReceive(amsr::someip_protocol::internal::InstanceId const instance_id,
                             amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) const noexcept = 0;

  /*!
   * \brief       Receive handler for signal based PDU events.
   * \param[in]   instance_id The PDU instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet The PDU message.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate  Product Private
   * \synchronous FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTNEXTLINE (readability-avoid-const-params-in-decls)
  virtual void HandleReceivePdu(amsr::someip_protocol::internal::InstanceId const instance_id,
                                amsr::someipd_app_protocol::internal::PduMessage&& packet) const noexcept = 0;

  /*!
   * \brief       Receive handler for SOME/IP initial field notification.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet      The SOME/IP message.
   * \param[in]   client_id   The subscriber client id.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate    Product Private
   * \synchronous FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTBEGIN (readability-avoid-const-params-in-decls)
  virtual void HandleReceiveInitialFieldNotification(
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someipd_app_protocol::internal::SomeIpMessage&& packet,
      amsr::someip_protocol::internal::ClientId const client_id) const noexcept = 0;
  // NOLINTEND (readability-avoid-const-params-in-decls)

  /*!
   * \brief Handler for notifying about newly offered services.
   * \param[in] service_instance Newly offered service instance.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate  Product Private
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnServiceInstanceUp(
      amsr::someip_protocol::internal::ServiceInstance const& service_instance) const noexcept = 0;

  /*!
   * \brief Handler for notifying about stopped services.
   * \param[in] service_instance Stopped service instance.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate  Product Private
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnServiceInstanceDown(
      amsr::someip_protocol::internal::ServiceInstance const& service_instance) const noexcept = 0;

  /*!
   * \brief Handler for notifying about event subscription state change.
   * \param[in] event_subscription_state_entry The entry containing the event details with the current subscription
   * state.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate  Product Private
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleEventSubscriptionStateUpdate(
      amsr::someip_protocol::internal::EventSubscriptionState const& event_subscription_state_entry) const noexcept = 0;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CLIENT_INTERFACE_H_
