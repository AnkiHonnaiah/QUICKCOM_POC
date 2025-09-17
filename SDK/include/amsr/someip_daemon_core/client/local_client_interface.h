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
/**        \file  local_client_interface.h
 *        \brief  Local Client Interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/client/event_handler_interface.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief A Local Client interface.
 */
class LocalClientInterface {
 public:
  /*!
   * \brief Event observer.
   */
  using EventObserver = std::shared_ptr<EventHandlerInterface>;
  /*!
   * \brief SOME/IP event observer.
   */
  using SomeIpEventObserver = std::shared_ptr<SomeIpEventHandlerInterface>;
  /*!
   * \brief PDU event observer.
   */
  using PduEventObserver = std::shared_ptr<PduEventHandlerInterface>;
  /*!
   * \brief   Constructor of LocalClientInterface.
   * \steady  FALSE
   */
  LocalClientInterface() noexcept = default;

  /*!
   * \brief   Destructor of LocalClientInterface.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~LocalClientInterface() noexcept = default;

  LocalClientInterface(LocalClientInterface const&) = delete;
  LocalClientInterface(LocalClientInterface&&) = delete;
  LocalClientInterface& operator=(LocalClientInterface const&) & = delete;
  LocalClientInterface& operator=(LocalClientInterface&&) & = delete;

  /*!
   * \brief Register response handler.
   *
   * \param[in] response_handler    A pointer to the response handler.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void RegisterResponseHandler(MethodResponseHandlerInterface* response_handler) noexcept = 0;

  /*!
   * \brief Subscribe to SOME/IP or PDU event.
   *
   * \param[in] event_id    A SOME/IP event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    The current event subscription state.
   *
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   *
   * \pre       This API has not been called before for this event ID.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<void> SubscribeEvent(someip_protocol::internal::EventId const event_id,
                                                 EventObserver const observer) noexcept = 0;

  /*!
   * \brief Unsubscribe to SOME/IP or PDU event.
   *
   * \param[in] event_id   A SOME/IP event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void UnsubscribeEvent(someip_protocol::internal::EventId const event_id,
                                EventObserver const observer) noexcept = 0;

  /*!
   * \brief       Forwards a local SOME/IP request message.
   * \param[in]   packet      A SOME/IP request message.
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered
   * Runtime check failed: service is not offered.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownMethod
   * Configuration check failed: method not configured.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable
   * Runtime check failed: connection is not established.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed
   * Runtime check failed: connection transmission failed.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual ara::core::Result<void> SendMethodRequest(Packet& packet) const noexcept = 0;

  /*!
   * \brief Subscribe to SOME/IP event.
   *
   * \param[in] event_id    A SOME/IP event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    Empty ara::core::Result in case of successful subscription or an error.
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   *
   * \pre       This API has not been called before for this event ID.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<void> SubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                                                       SomeIpEventObserver const observer) noexcept = 0;

  /*!
   * \brief Unsubscribe to SOME/IP event.
   *
   * \param[in] event_id   A SOME/IP event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void UnsubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                                      SomeIpEventObserver const observer) noexcept = 0;

  /*!
   * \brief Subscribe to PDU event.
   *
   * \param[in] event_id    A PDU event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    Empty ara::core::Result in case of successful subscription or an error.
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   *
   * \pre       This API has not been called before for this event ID.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<void> SubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                                    PduEventObserver const observer) noexcept = 0;

  /*!
   * \brief Unsubscribe to PDU event.
   *
   * \param[in] event_id   A PDU event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void UnsubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                   PduEventObserver const observer) noexcept = 0;

  /*!
   * \brief Get the clint Id.
   *
   * \return    client Id.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual someip_protocol::internal::ClientId GetClientId() const noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_INTERFACE_H_
