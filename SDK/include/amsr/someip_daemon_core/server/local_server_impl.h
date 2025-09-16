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
/**        \file  local_server_impl.h
 *        \brief  Manages the event communication between local client and local service.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_IMPL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <set>
#include <utility>
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/server/local_server_interface.h"
#include "amsr/someip_daemon_core/server/method_request_handler_interface.h"
#include "amsr/someip_daemon_core/server/remote_client_interface.h"
#include "amsr/someip_daemon_core/server/server_field_cache.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
using vac::container::literals::operator""_sv;

/*!
 * \brief LocalServerImpl.
 * \details Implementation class for LocalServer.
 */
class LocalServerImpl final : public LocalServerInterface, public MethodRequestHandler {
 private:
  /*!
   * \brief Type alias for a local packet sink type
   */
  using LocalPacketSinkType = packet_sink::LocalPacketSink;

  /*!
   * \brief Type alias for the shared pointer to LocalPacketSink.
   */
  using LocalPacketSinkSharedPtr = std::shared_ptr<LocalPacketSinkType>;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief Service instance ID.
   */
  using SomeIpServiceInstanceId = configuration::types::SomeIpServiceInstanceId;

  /*!
   * \brief Type alias for the unique pointer to RemoteClientInterface.
   */
  using RemoteClientInterfaceUniquePtr = amsr::UniquePtr<RemoteClientInterface>;

  /*!
   * \brief Container of unique pointers to remote client communication.
   */
  using RemoteClientContainer = ara::core::Vector<RemoteClientInterfaceUniquePtr>;

  /*!
   * \brief Container of events configured for a provided service instance that are configured as fields
   */
  using ProvidedFieldEvents =
      std::set<amsr::someip_protocol::internal::EventId, std::less<amsr::someip_protocol::internal::EventId>,
               ara::core::PolymorphicAllocator<amsr::someip_protocol::internal::EventId>>;

 public:
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Define destructor.
   *
   * \context Shutdown
   * \steady FALSE
   */
  ~LocalServerImpl() noexcept override;

  LocalServerImpl() = default;
  LocalServerImpl(LocalServerImpl const&) = delete;
  LocalServerImpl(LocalServerImpl&&) = delete;
  LocalServerImpl& operator=(LocalServerImpl const&) & = delete;
  LocalServerImpl& operator=(LocalServerImpl&&) & = delete;

  /*!
   * \brief Constructor of LocalServerImpl.
   *
   * \param[in] service_instance_id       The service instance id.
   * \param[in] communication_type        The communication type.
   * \param[in] packet_router             The packet router.
   * \param[in] server_field_events       A reference to the container of field events.
   *
   * \context Init
   * \steady FALSE
   */
  LocalServerImpl(SomeIpServiceInstanceId const& service_instance_id,
                  ServiceInstanceCommunicationType communication_type,
                  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
                  ProvidedFieldEvents const& server_field_events) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Offer the requested service for given service instance.
   *
   * \param[in] application The local application that offers the service.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \trace SPEC-8053469
   * \trace SPEC-4980348
   */
  void OfferService(LocalPacketSinkSharedPtr const& application) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Stop offer the service for given service instance.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  void StopOfferService() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local SOME/IP response message.
   *
   * \param[in] packet           A SOME/IP request message.
   * \param[in] recipient        A remote source identifier.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendMethodResponse(SomeIpPacket const& packet,
                          packet_router::RemoteSourceIdentifier const& recipient) const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local SOME/IP event.
   *
   * \param[in] packet    A SOME/IP request message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendEvent(SomeIpPacket const& packet) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local PDU event.
   *
   * \param[in] packet    A PDU request message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendPduEvent(PduPacket const& packet) const noexcept override;

  /*!
   * \brief Register the remote client(s).
   *
   * \param[in] remote_client    A unique pointer to remote client communication interface.
   *
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void RegisterRemoteClient(RemoteClientInterfaceUniquePtr&& remote_client) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Handle a method request and forward it to the application
   *
   * \param[in] instance_id The instance id of the sender.
   * \param[in] sender      The endpoint of the sender.
   * \param[in] packet      The received packet.
   * \return      True if the forwarding is successful, false otherwise.
   * \pre         The service must be offered.
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool HandleMethodRequest(
      someip_protocol::internal::InstanceId const instance_id, packet_router::RemoteSourceIdentifier const& sender,
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Handle a method request no return and forward it to the application
   *
   * \param[in] instance_id The instance id of the sender.
   * \param[in] packet      The received packet.
   * \pre         The service must be offered.
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool HandleMethodRequestNoReturn(
      someip_protocol::internal::InstanceId const instance_id,
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const noexcept override;

 private:
  /*!
   * \brief The service instance id
   */
  SomeIpServiceInstanceId const service_instance_id_;

  /*!
   * \brief The communication type instance.
   */
  ServiceInstanceCommunicationType const communication_type_;

  /*!
   * \brief A reference to the packet router.
   */
  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router_;

  /*!
   * \brief A container of remote clients.
   */
  RemoteClientContainer remote_clients_{};

  /*!
   * \brief The local application pointer that offers the service to the remote client.
   */
  std::shared_ptr<packet_sink::LocalPacketSink> application_{nullptr};

  /*!
   * \brief The server field cache object.
   */
  ServerFieldCache server_field_cache_;

  /*!
   * \brief   A flag indicating whether this local service is offered.
   * \details As an application may not call StopOfferService before calling ReleaseLocalServer, or may call it multiple
   *          times, we need to ensure that cleanup actions are triggered only once.
   */
  bool is_offered_{false};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("LocalServerImpl"_sv, service_instance_id_.deployment_id,
                                                      service_instance_id_.instance_id)};
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_IMPL_H_
