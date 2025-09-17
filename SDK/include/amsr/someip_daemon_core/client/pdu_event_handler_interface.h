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
/**        \file  pdu_event_handler_interface.h
 *        \brief  Pdu Event Handler Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_PDU_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_PDU_EVENT_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Represents a PDU message.
 */
using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;

/*!
 * \brief Interface for PDU event handler.
 */
class PduEventHandlerInterface {
 public:
  /*!
   * \brief default constructor.
   * \steady FALSE
   */
  PduEventHandlerInterface() noexcept = default;

  /*!
   * \brief   default destructor.
   * \steady  FALSE
   */
  virtual ~PduEventHandlerInterface() noexcept = default;

  PduEventHandlerInterface(PduEventHandlerInterface const &) = delete;
  PduEventHandlerInterface(PduEventHandlerInterface &&) = delete;
  PduEventHandlerInterface &operator=(PduEventHandlerInterface const &) & = delete;
  PduEventHandlerInterface &operator=(PduEventHandlerInterface &&) & = delete;

  /*!
   * \brief handle PDU Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return    -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void OnPduEvent(someip_protocol::internal::InstanceId const instance_id,
                          client::PduPacket packet) noexcept = 0;
  /*!
   * \brief Inform the application about an event subscription state change.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id The subscribed event identifier with the event subscription change.
   * \param[in] new_state Current event subscription state.
   *
   * \return    -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OnPduSubscriptionStateChange(configuration::types::SomeIpServiceInstanceId const &service_instance_id,
                                            someip_protocol::internal::EventId const event_id,
                                            client::eventgroup::EventSubscriptionState const new_state) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_PDU_EVENT_HANDLER_INTERFACE_H_
