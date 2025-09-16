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
/**        \file  client_event_dispatcher.h
 *         \brief The client event dispatcher unit implements the Client event dispatcher interface to provide the
 *                functionalities related to someip event and pdu event routing.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/client_event_dispatcher_interface.h"
#include "amsr/someip_daemon_core/client/datatypes/client_pdu_event.h"
#include "amsr/someip_daemon_core/client/datatypes/client_someip_event.h"
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"

#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief ClientEventDispatcherInterface.
 */
class ClientEventDispatcher final : public ClientEventDispatcherInterface {
 private:
  /*!
   * \brief Map of SOME/IP event.
   */
  using SomeIpEventMap = ara::core::Map<someip_protocol::internal::EventId, datatypes::ClientSomeIpEvent>;
  /*!
   * \brief Map of PDU event.
   */
  using PduEventMap = ara::core::Map<someip_protocol::internal::EventId, datatypes::ClientPduEvent>;

 public:
  /*!
   * \brief     Constructs the Client Event Dispatcher.
   * \param[in] event_deployment The SOME/IP Event Deployment.
   * \param[in] someip_service_instance_id SOME/IP Service Instance id.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ClientEventDispatcher(ara::core::Vector<configuration::types::SomeIpEventDeployment> const& event_deployment,
                        configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id) noexcept;

  /*!
   * \brief     Destroys the Client Event Dispatcher.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ClientEventDispatcher() override = default;

  ClientEventDispatcher(ClientEventDispatcher const&) = delete;
  ClientEventDispatcher(ClientEventDispatcher&&) = delete;
  ClientEventDispatcher& operator=(ClientEventDispatcher const&) & = delete;
  ClientEventDispatcher& operator=(ClientEventDispatcher&&) & = delete;

  /*!
   * \brief Subscribe to SOME/IP event.
   *
   * \param[in] event_id         A SOME/IP event identifier.
   * \param[in] instance_id      A SOME/IP Instance identifier.
   * \param[in] event_handler    A pointer to the subscribed application.
   * \param[in] client_id        A SOME/IP client identifier.
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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void SubscribeSomeIpEvent(::amsr::someip_protocol::internal::EventId const event_id,
                            amsr::someip_protocol::internal::InstanceId const instance_id,
                            std::shared_ptr<client::SomeIpEventHandlerInterface> event_handler,
                            someip_protocol::internal::ClientId const client_id) noexcept override;

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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void SubscribePduEvent(::amsr::someip_protocol::internal::EventId const event_id,
                         std::shared_ptr<client::PduEventHandlerInterface> event_handler) noexcept override;

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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void UnsubscribeSomeIpEvent(::amsr::someip_protocol::internal::EventId const event_id,
                              std::shared_ptr<client::SomeIpEventHandlerInterface> event_handler) noexcept override;
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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void UnsubscribePduEvent(::amsr::someip_protocol::internal::EventId const event_id,
                           std::shared_ptr<client::PduEventHandlerInterface> event_handler) noexcept override;

  /*!
   * \brief Handle a SOME/IP Event notification sent to the connected application.
   *
   * \param[in] instance_id SOME/IP Instance id.
   * \param[in] packet      SOME/IP notification message.
   *
   * \pre       The event must be configured in the remote server. If not, an error is logged.
   *
   * \steady    TRUE
   * \context   Network
   * \reentrant FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnSomeIpEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                     client::Packet const& packet) noexcept override;

  /*!
   * \brief Handle a PDU Event notification sent to the connected application.
   *
   * \param[in] instance_id PDU Instance id.
   * \param[in] packet      PDU notification message.
   *
   * \pre       The event must be configured in the remote server. If not, an error is logged.
   *
   * \steady    TRUE
   * \context   Network
   * \reentrant FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnPduEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                  client::PduPacket const& packet) noexcept override;

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
   * \pre           -
   * \internal
   * \steady FALSE
   * \endinternal
   * \context App
   * \reentrant FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnSomeIpSubscriptionStateChange(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      ::amsr::someip_protocol::internal::EventId const event_id,
      someip_daemon_core::client::eventgroup::EventSubscriptionState new_state) noexcept override;

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
   * \context   App
   * \reentrant FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnPduSubscriptionStateChange(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      ::amsr::someip_protocol::internal::EventId const event_id,
      someip_daemon_core::client::eventgroup::EventSubscriptionState new_state) noexcept override;

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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void InvalidateFieldCacheEntry(::amsr::someip_protocol::internal::EventId const event_id) noexcept override;

 private:
  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] someip_service_instance_id     A SOME/IP service instance identifier.
   *
   * \return A string representing the logger context identifier for given SOME/IP service parameters.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  static ara::core::String GetLoggerContextId(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id);

  /*!
   * \brief Ara com logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief SomeIp event container.
   */
  SomeIpEventMap someip_event_map_{};

  /*!
   * \brief PDU event container.
   */
  PduEventMap pdu_event_map_{};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_EVENT_DISPATCHER_H_
