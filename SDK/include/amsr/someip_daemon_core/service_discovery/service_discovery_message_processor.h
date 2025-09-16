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
/**        \file  service_discovery_message_processor.h
 *        \brief  SD message Processor.
 *
 *      \details  This class is responsible for processing all incoming SD Messages.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_PROCESSOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/entry_interpretation_error.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/find_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/stop_offer_service_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/stop_subscribe_eventgroup_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_ack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/subscribe_eventgroup_nack_entry_interpreter.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/reboot_management/reboot_detection.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_endpoint_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_endpoint_observer.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief Forward declaration ServiceDiscoveryEndpoint to avoid cyclic dependency.
 */
class ServiceDiscoveryEndpoint;

/*!
 * \brief ServiceDiscoveryMessageProcessor.
 *        This class is responsible for processing all incoming SD Messages.
 *        The incoming message will invoked by calling OnSdMessageReceived, then this class will perform the following
 *        checks:
 *          - Message Validation
 *          - Reboot Detection
 *          - IAM Access Check
 *        and in case of invalid entry/option/message, the corresponding error handling will be done (e.g. by dropping
 *        the message or sending a NACK to message_scheduler). In case of a valid entry, it will be forwarded to the
 *        an endpoint oberver.
 * \trace SPEC-4663612
 */
class ServiceDiscoveryMessageProcessor final {
 public:
  /*!
   * \brief Type of key of endpoint observers maps.
   */
  class ServiceDiscoveryEndpointObserverMapKey {
   public:
    /*!
     * \brief The service ID.
     */
    amsr::someip_protocol::internal::ServiceId const service_id;
    /*!
     * \brief The instance ID.
     */
    amsr::someip_protocol::internal::InstanceId const instance_id;
    /*!
     * \brief The major version.
     */
    amsr::someip_protocol::internal::MajorVersion const major_version;
    /*!
     * \brief The minor version.
     */
    amsr::someip_protocol::internal::MinorVersion const minor_version;

    /*!
     * \brief Compare two MapKeys.
     *
     * \param[in] other The MapKey to compare against.
     * \return true for valid comparison, otherwise false.
     *
     * \context Init, Shutdown
     * \steady FALSE
     */
    bool operator<(ServiceDiscoveryEndpointObserverMapKey const& other) const noexcept {
      bool result{};

      if (this->service_id < other.service_id) {
        result = true;
      } else if (this->service_id > other.service_id) {
        result = false;
      } else if (this->major_version < other.major_version) {
        result = true;
      } else if (this->major_version > other.major_version) {
        result = false;
      } else if (this->instance_id < other.instance_id) {
        result = true;
      } else {
        result = false;
      }

      return result;
    }
  };

  /*!
   * \brief Type of value of server endpoint observers maps.
   */
  using ServiceDiscoveryServerEndpointObserverSharedPtr =
      std::shared_ptr<state_machine::server::ServiceDiscoveryServerEndpointObserver>;

  /*!
   * \brief Type of server endpoint observers map.
   */
  using ServerEndpointObserverMap =
      ara::core::Map<ServiceDiscoveryEndpointObserverMapKey, ServiceDiscoveryServerEndpointObserverSharedPtr>;

  /*!
   * \brief Type of value of client endpoint observers maps.
   */
  using ServiceDiscoveryClientEndpointObserverSharedPtr =
      std::shared_ptr<state_machine::client::ServiceDiscoveryClientEndpointObserver>;

  /*!
   * \brief Type of client endpoint observers map.
   */
  using ClientEndpointObserverMap =
      ara::core::Map<ServiceDiscoveryEndpointObserverMapKey, ServiceDiscoveryClientEndpointObserverSharedPtr>;

  /*!
   * \brief Constructor.
   *
   * \param[in] unicast_sd_address      The unicast address of our SD endpoint, used to avoid detecting our own reboot.
   * \param[in] identity_access_manager The Identity Access Manager to check access rights.
   * \param[in] message_scheduler       The target to which the negative responses might be sent to.
   * \param[in] statistics_handler      The statistics handler
   *
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryMessageProcessor(
      someip_daemon_core::IpAddress const& unicast_sd_address, iam::IamInterface& identity_access_manager,
      message::scheduler::SchedulerInterface& message_scheduler,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept;

  /*!
   * \brief Default destructor
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryMessageProcessor() noexcept = default;

  ServiceDiscoveryMessageProcessor() = delete;
  ServiceDiscoveryMessageProcessor(ServiceDiscoveryMessageProcessor const&) = delete;
  ServiceDiscoveryMessageProcessor(ServiceDiscoveryMessageProcessor&&) = delete;
  ServiceDiscoveryMessageProcessor& operator=(ServiceDiscoveryMessageProcessor const&) & = delete;
  ServiceDiscoveryMessageProcessor& operator=(ServiceDiscoveryMessageProcessor&&) & = delete;

  /*!
   * \brief Register a server endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \param[in] value The server endpoint observer to register.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterServer(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key,
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryServerEndpointObserverSharedPtr value) noexcept;

  /*!
   * \brief Register a client endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \param[in] value The client endpoint observer to register.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterClient(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key,
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryClientEndpointObserverSharedPtr value) noexcept;

  /*!
   * \brief Unregister the server endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \pre -
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterServer(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key) noexcept;

  /*!
   * \brief Unregister the client endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \pre -
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterClient(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key) noexcept;

  /** Start {SD Message Reception} ************************************************************************************/

  /*!
   * \brief Function to handle reception of an SOME/IP SD message.
   *
   * \details API OnRebootDetected() is called in case reboot detection entries and options are extracted
   *          from the received message, and forwarded to the endpoint observer through API
   *          OnMulticastSdMessage() or OnUnicastSdMessage(), depending on the value of is_multicast.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] is_multicast A flag that determines whether the received message has been send via multicast or not.
   * \param[in] sd_message The received SOME/IP SD message.
   *
   * \context Network
   * \steady TRUE
   * \trace SPEC-4981512
   * \trace SPEC-4981518
   */
  void OnSdMessageReceived(someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
                           bool const is_multicast, message::ServiceDiscoveryMessage const& sd_message) noexcept;

  /** End {SD Message Reception} **************************************************************************************/

 private:
  /*!
   * \brief Handle service discovery entries received via multicast.
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entries      A container of entries received in a SOME/IP SD message.
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-10144599
   * \trace SPEC-10144840
   * \trace SPEC-4663614
   */
  void OnSdMulticastMessage(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                            message::entries::ServiceDiscoveryEntryContainer const& entries,
                            message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief Handle service discovery entries received via unicast.
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entries      A container of entries received in a SOME/IP SD message.
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-10144599
   * \trace SPEC-10144840
   * \trace SPEC-4663614
   */
  void OnSdUnicastMessage(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                          message::entries::ServiceDiscoveryEntryContainer const& entries,
                          message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief FindService entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The FindService entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-10144714
   * \trace SPEC-22451155
   */
  void OnFindServiceEntry(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                          message::entries::ServiceDiscoveryEntry const& entry,
                          message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief SubscribeEventgroup entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The SubscribeEventgroup entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   *
   * \trace SPEC-4981632
   * \trace SPEC-10144598
   * \trace SPEC-10144824
   */
  void OnSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                                  message::entries::ServiceDiscoveryEntry const& entry,
                                  message::options::ServiceDiscoveryOptionContainer const& options) noexcept;
  /*!
   * \brief StopSubscribeEventgroup entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The StopSubscribeEventgroup entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   *
   * \trace SPEC-22451187
   */
  void OnStopSubscribeEventgroupEntry(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                                      message::entries::ServiceDiscoveryEntry const& entry,
                                      message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief OfferService entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The FindService entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   * \param[in] is_multicast A flag that determines whether the received message has been send via multicast or not.
   *
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   *
   * \trace SPEC-22451161
   * \trace SPEC-22451162
   */
  void OnOfferServiceEntry(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                           message::entries::ServiceDiscoveryEntry const& entry,
                           message::options::ServiceDiscoveryOptionContainer const& options,
                           bool const is_multicast) noexcept;

  /*!
   * \brief StopOfferService entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The FindService entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  void OnStopOfferServiceEntry(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                               message::entries::ServiceDiscoveryEntry const& entry,
                               message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief SubscribeEventgroupAck entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The FindService entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  void OnSubscribeEventgroupAck(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                                message::entries::ServiceDiscoveryEntry const& entry,
                                message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief SubscribeEventgroupNack entry reception callback
   *
   * \param[in] ip_address   The unicast IP address of the sender of the service discovery message.
   * \param[in] port         The UDP port of the sender of the service discovery message.
   * \param[in] entry        The FindService entry
   * \param[in] options      A container of options received in a SOME/IP SD message.
   *
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  void OnSubscribeEventgroupNack(someip_daemon_core::IpAddress const& ip_address, amsr::net::ip::Port const port,
                                 message::entries::ServiceDiscoveryEntry const& entry,
                                 message::options::ServiceDiscoveryOptionContainer const& options) noexcept;

  /*!
   * \brief Function to handle reboot detection for unicast and multicast SOME/IP SD message.
   *
   * \param[in] address An IP address identifying sender's IP address.
   * \param[in] port    A port identifying sender's port.
   *
   * \context Network
   * \steady TRUE
   */
  void OnRebootDetected(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port const port) noexcept;

  /*!
   * \brief The unicast address of our SD endpoint
   * \details As a SD endpoint sending multicast messages will receive its own SD messages, this address is used to
   *          avoid detection of our own ECU reboot in case of receiving our own SD messages in the wrong order.
   */
  someip_daemon_core::IpAddress const unicast_sd_address_;

  /*!
   * \brief Identity and access management.
   */
  iam::IamInterface& identity_access_manager_;

  /*!
   * \brief Reboot detection.
   */
  reboot_management::RebootDetection reboot_detection_{};

  /*!
   * \brief SD Message sender.
   */
  message::scheduler::SchedulerInterface& message_scheduler_;

  /*!
   * \brief SD Statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoveryMessageProcessor"}};

  /*!
   * \brief Map of server endpoint observers.
   */
  typename ServiceDiscoveryMessageProcessor::ServerEndpointObserverMap server_observers_map_{};

  /*!
   * \brief Map of client endpoint observers.
   */
  typename ServiceDiscoveryMessageProcessor::ClientEndpointObserverMap client_observers_map_{};
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_PROCESSOR_H_
