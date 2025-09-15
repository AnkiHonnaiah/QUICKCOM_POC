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
/**        \file  amsr/someip_binding_core/internal/client_handler_interface.h
 *        \brief  ara::com SOME/IP Binding Client Handler Interface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/pdu_message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for SOME/IP ClientManager implementations.
 */
class ClientHandlerInterface {
 public:
  /*!
   * \brief Default default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ClientHandlerInterface() noexcept = default;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady FALSE \endinternal
   */
  virtual ~ClientHandlerInterface() noexcept = default;

  ClientHandlerInterface(ClientHandlerInterface const&) = delete;
  ClientHandlerInterface(ClientHandlerInterface&&) = delete;
  ClientHandlerInterface& operator=(ClientHandlerInterface const&) & = delete;
  ClientHandlerInterface& operator=(ClientHandlerInterface&&) & = delete;

  /*!
   * \brief       Handles EventNotifications and MethodResponses.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port).
   * \param[in]   packet      The SOME/IP message.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   * \trace       SPEC-4980078, SPEC-4980113, SPEC-4981472, SPEC-4981474, SPEC-10144318
   */
  virtual void HandleSomeIpEvent(::amsr::someip_protocol::internal::InstanceId const instance_id,
                                 ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept = 0;

  /*!
   * \brief       Receive handler for SOME/IP initial field notifications.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port).
   * \param[in]   client_id   The SOME/IP client id.
   * \param[in]   packet      The SOME/IP message.
   * \pre -       The incoming SOME/IP message must be of type kNotification.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   * \trace       SPEC-4980078, SPEC-4980113, SPEC-4981472, SPEC-4981474, SPEC-10144318, SPEC-4981462
   */
  virtual void HandleSomeIpInitialFieldNotification(
      ::amsr::someip_protocol::internal::InstanceId const instance_id,
      ::amsr::someip_protocol::internal::ClientId const client_id,
      ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept = 0;

  /*!
   * \brief       Handles signal based EventNotifications.
   * \param[in]   instance_id The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in]   packet      The PDU message.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-4980079, SPEC-4980114
   */
  virtual void HandleSignalBasedEvent(::amsr::someip_protocol::internal::InstanceId const instance_id,
                                      ::amsr::someipd_app_protocol::internal::PduMessage&& packet) noexcept = 0;

  /*!
   * \brief       Receive handler for SOME/IP method response.
   * \details     This API is called upon respond to a message.
   * \param[in]   instance_id The SOME/IP instance id of the requested service.
   * \param[in]   packet      Serialized SOME/IP Method Response [SOME/IP message]
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-10144318
   */
  virtual void HandleMethodResponse(::amsr::someip_protocol::internal::InstanceId const instance_id,
                                    ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept = 0;

  /*!
   * \brief       Called on asynchronous notification about event subscription state update.
   * \details     HandleEventSubscriptionStateUpdate is called every time an event subscription state has been
   *              updated.
   * \param[in]   event_subscription_state_entry The entry containing the event details with the current
   *                                             subscription state.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleEventSubscriptionStateUpdate(
      ::amsr::someip_protocol::internal::EventSubscriptionState const& event_subscription_state_entry) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_HANDLER_INTERFACE_H_
