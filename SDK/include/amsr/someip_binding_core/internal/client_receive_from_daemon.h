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
/**        \file  amsr/someip_binding_core/internal/client_receive_from_daemon.h
 *        \brief  ara::com SOME/IP Binding Client Receive From Daemon.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientReceiveFromDaemon
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_RECEIVE_FROM_DAEMON_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_RECEIVE_FROM_DAEMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_binding_core/internal/client_handler_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "someip_daemon_client/internal/client_interface.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_sv
/*!
 * \brief The StringView operator.
 */
using ::vac::container::literals::operator""_sv;

/*!
 * \brief Class to receive method requests from DaemonClient and forward them to ClientHandlerInterface.
 */
class ClientReceiveFromDaemon final : public ::amsr::someip_daemon_client::internal::ClientInterface {
 public:
  /*!
   * \brief Constructor.
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ClientReceiveFromDaemon();

  /*!
   * \brief Default destructor.
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ClientReceiveFromDaemon() noexcept final = default;

  ClientReceiveFromDaemon(ClientReceiveFromDaemon const&) = delete;
  ClientReceiveFromDaemon(ClientReceiveFromDaemon&&) = delete;
  ClientReceiveFromDaemon& operator=(ClientReceiveFromDaemon const&) & = delete;
  ClientReceiveFromDaemon& operator=(ClientReceiveFromDaemon&&) & = delete;

  /*!
   * \brief                  Register Client Handler
   * \details                Register ClientHandler to forward messages upon receiving it from DaemonClient.
   * \param[in] client_handler Pointer to clientHandlerInterface
   * \pre                    -
   * \context                Init
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void RegisterClientHandler(ClientHandlerInterface* client_handler) noexcept;

  /*!
   * \brief                  Unregister Client Handler
   *
   * \pre                    -
   * \context                Shutdown
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void UnregisterClientHandler() noexcept;

  /*!
   * \brief                  Register Service Discovery Handler
   * \details                Register ServiceDiscoveryHandler to forward notifications upon receiving it from
   * DaemonClient.
   * \param[in] service_discovery_handler Pointer to ServiceDiscoveryHandlerInterface.
   * \pre                    -
   * \context                Init
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void RegisterServiceDiscoveryHandler(service_discovery::ServiceListenerInterface* service_discovery_handler) noexcept;

  /*!
   * \brief                  Unregister Service Discovery Handler
   *
   * \pre                    -
   * \context                Shutdown
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void UnregisterServiceDiscoveryHandler() noexcept;

  /*!
   * \brief       Receive handler for SOME/IP events.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet The SOME/IP message.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleReceive(::amsr::someip_protocol::internal::InstanceId const instance_id,
                     ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) const noexcept final;

  /*!
   * \brief       Handler for SOME/IP initial field notifications.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port).
   * \param[in]   packet The SOME/IP message.
   * \param[in]   client_id SOME/IP client id.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleReceiveInitialFieldNotification(
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someipd_app_protocol::internal::SomeIpMessage&& packet,
      amsr::someip_protocol::internal::ClientId const client_id) const noexcept final;

  /*!
   * \brief       Receive handler for signal based PDU events.
   * \param[in]   instance_id The PDU instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet The PDU message.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleReceivePdu(amsr::someip_protocol::internal::InstanceId const instance_id,
                        amsr::someipd_app_protocol::internal::PduMessage&& packet) const noexcept final;

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
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleEventSubscriptionStateUpdate(amsr::someip_protocol::internal::EventSubscriptionState const&
                                              event_subscription_state_entry) const noexcept final;

  /*!
   * \brief       Called on asynchronous notification about newly offered services.
   * \param[in]   service_instance Found service instance.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceUp(
      amsr::someip_protocol::internal::ServiceInstance const& service_instance) const noexcept final;

  /*!
   * \brief       Called upon a stopped service.
   * \param[in]   service_instance The stopped service instance.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceDown(
      amsr::someip_protocol::internal::ServiceInstance const& service_instance) const noexcept final;

 private:
  /*!
   * \brief Pointer to service_discovery::ServiceListenerInterface
   */
  service_discovery::ServiceListenerInterface* service_discovery_handler_{nullptr};

  /*!
   * \brief Pointer to ClientHandlerInterface
   */
  ClientHandlerInterface* client_handler_{nullptr};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_RECEIVE_FROM_DAEMON_H_
