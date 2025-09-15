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
/**        \file  someip_binding_core/internal/proxy_handler_interface.h
 *        \brief  Manager to handle common logic for everything on proxy side within SOME/IP
 *                binding.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_PROXY_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_PROXY_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>

#include "amsr/someip_binding_core/internal/events/event_message.h"
#include "amsr/someip_binding_core/internal/events/pdu_message.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"
#include "someipd_app_protocol/internal/pdu_message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface defining API for the Proxy side.
 */
class ProxyHandlerInterface : public methods::ProxyMethodResponseHandlerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  ProxyHandlerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \internal steady FALSE \endinternal
   */
  ~ProxyHandlerInterface() noexcept override = default;

  ProxyHandlerInterface(ProxyHandlerInterface const&) = delete;
  ProxyHandlerInterface(ProxyHandlerInterface&&) = delete;
  ProxyHandlerInterface& operator=(ProxyHandlerInterface const&) & = delete;
  ProxyHandlerInterface& operator=(ProxyHandlerInterface&&) & = delete;

  /*!
   * \brief       Handle reception of a PDU event message.
   * \param[in]   event The PDU event message.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleSignalBasedEvent(events::PduMessage const& event) noexcept = 0;

  /*!
   * \brief       Handle reception of an event message.
   * \param[in]   event The event message.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleSomeIpEvent(internal::events::EventMessage const& event) noexcept = 0;

  /*!
   * \brief       Handle reception of an event subscription state update.
   * \param[in]   event_subscription_state_entry The event subscription state.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void HandleEventSubscriptionStateUpdate(
      someip_protocol::internal::EventSubscriptionState const& event_subscription_state_entry) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_PROXY_HANDLER_INTERFACE_H_
