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
/**        \file  remote_client.h
 *        \brief  Remote Client(s).
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/generic_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/provided_someip_eventgroup.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/configuration/util/configuration_types_builder.h"
#include "amsr/someip_daemon_core/server/eventgroup/event_manager.h"
#include "amsr/someip_daemon_core/server/remote_client_communication.h"
#include "amsr/someip_daemon_core/server/remote_client_interface.h"
#include "amsr/someip_daemon_core/server/server_field_cache_interface.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_endpoint.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_processor.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/result.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief RemoteClient.
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
class RemoteClient : public RemoteClientInterface {
 public:
  /*!
   * \brief  Default constructor.
   * \steady FALSE
   */
  RemoteClient() noexcept = default;

  /*!
   * \brief  Default destructor.
   * \steady FALSE
   */
  ~RemoteClient() noexcept override = default;

  RemoteClient(RemoteClient const&) = delete;
  RemoteClient(RemoteClient&& other) = delete;
  RemoteClient& operator=(RemoteClient const&) & = delete;
  RemoteClient& operator=(RemoteClient&& other) & = delete;

  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;
  /*!
   * \brief Alias for ServiceDiscoveryServerInterface type.
   */
  using ServiceDiscoveryServerInterface = service_discovery::state_machine::server::ServiceDiscoveryServerInterface;
  /*!
   * \brief Alias for ServiceDiscoveryServer type.
   */
  using ServiceDiscoveryServer = service_discovery::state_machine::server::ServiceDiscoveryServer;

  /*!
   * \brief Register service discovery server.
   *
   * \param[in] service_discovery_server  Service discovery server.
   *
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Assert that service discovery server pointer is not nullptr.
   * - Register the service discovery server.
   * \endinternal
   */
  void RegisterServiceDiscoveryServer(
      std::shared_ptr<ServiceDiscoveryServerInterface> service_discovery_server) noexcept {
    assert(service_discovery_server != nullptr);
    service_discovery_server_ = service_discovery_server;
  }

  /*!
   * \brief Register remote client communication.
   *
   * \param[in] remote_client_communication  Remote Client Communication.
   *
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Assert that remote client Communication pointer is not nullptr.
   * - Register the remote client Communication.
   * \endinternal
   */

  void RegisterRemoteClientCommunication(std::shared_ptr<RemoteClientInterface> remote_client_communication) noexcept {
    assert(remote_client_communication != nullptr);
    remote_client_communication_ = remote_client_communication;
  }

  /*!
   * \brief Offer a provided service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Start assuming a successful result, as it's the default behavior when communication is disabled.
   * - If remote client Communication pointer is not nullptr, forward the call to RemoteClientCommunication.
   * - If service discovery server pointer is not nullptr, notify it about the service being up.
   * \endinternal
   */
  void OfferService() noexcept override {
    if (remote_client_communication_ != nullptr) {
      remote_client_communication_->OfferService();
    }

    if (service_discovery_server_ != nullptr) {
      service_discovery_server_->OnServiceUp();
    }
  }

  /*!
   * \brief Stop a provided service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If service discovery server pointer is not nullptr, notify it about the service being down.
   * - If remote client Communication pointer is not nullptr, forward the call to RemoteClientCommunication.
   * \endinternal
   */
  void StopOfferService() noexcept override {
    if (service_discovery_server_ != nullptr) {
      service_discovery_server_->OnServiceDown();
    }

    if (remote_client_communication_ != nullptr) {
      remote_client_communication_->StopOfferService();
    }
  }

  /*!
   * \brief Send a SOME/IP event message to a remote peer.
   *
   * \param[in] packet A SOME/IP message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If communication is disabled.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Set the return value to indicate failure.
   * - If remote client Communication pointer is not nullptr, forward the call to RemoteClientCommunication.
   * - Return the return value.
   * \endinternal
   */
  ara::core::Result<void> SendSomeIpEvent(SomeIpPacketSharedPtr packet) noexcept override {
    ara::core::Result<void> result{ara::core::ErrorCode{amsr::generic::GenErrc::kSystemConfiguration}};

    if (remote_client_communication_ != nullptr) {
      result = remote_client_communication_->SendSomeIpEvent(packet);
    }
    return result;
  }

  /*!
   * \brief Send a PDU event message to a remote peer.
   *
   * \param[in] packet A PDU message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If communication is disabled.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Set the return value to indicate failure.
   * - If remote client Communication pointer is not nullptr, forward the call to RemoteClientCommunication.
   * - Return the return value.
   * \endinternal
   */
  ara::core::Result<void> SendPduEvent(PduPacketSharedPtr packet) noexcept override {
    ara::core::Result<void> result{ara::core::ErrorCode{amsr::generic::GenErrc::kSystemConfiguration}};

    if (remote_client_communication_ != nullptr) {
      result = remote_client_communication_->SendPduEvent(packet);
    }
    return result;
  }

  /*!
   * \brief Register the field cache handler.
   *
   * \param[in] server_field_cache    The server field cache.
   *
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If remote client Communication pointer is not nullptr, forward the call to RemoteClientCommunication.
   * \endinternal
   */
  void RegisterFieldCacheHandler(ServerCacheInterface* server_field_cache) noexcept override {
    if (remote_client_communication_ != nullptr) {
      remote_client_communication_->RegisterFieldCacheHandler(server_field_cache);
    }
  }

 private:
  /*!
   * \brief A shared pointer to the remote client communication object.
   * \details As the event manager needs remote client communication to remove all eventgroup subscribers, a shared
   *          pointer is used here.
   */
  std::shared_ptr<RemoteClientInterface> remote_client_communication_{nullptr};

  /*!
   * \brief A shared pointer to a service discovery server.
   * \details As the SD endpoint needs an access to ServiceDiscoveryServer to register the server in server observers
   *          map, a shared pointer is used here.
   */
  std::shared_ptr<ServiceDiscoveryServerInterface> service_discovery_server_{nullptr};
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_H_
