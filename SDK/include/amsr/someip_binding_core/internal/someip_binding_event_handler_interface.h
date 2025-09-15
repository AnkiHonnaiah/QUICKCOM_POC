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
/*!        \file  someip_binding_event_handler_interface.h
 *        \brief  SOME/IP Binding Event Handler Interface used by the Client Manager
 *      \details  Interface for handling event notification and subscription state change
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_EVENT_HANDLER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/events/event_message.h"
#include "amsr/someip_binding_core/internal/events/pdu_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for handling event notification and subscription state change.
 */
class SomeipBindingEventHandlerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SomeipBindingEventHandlerInterface() = default;

  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SomeipBindingEventHandlerInterface() = default;

  SomeipBindingEventHandlerInterface(SomeipBindingEventHandlerInterface const&) = delete;
  SomeipBindingEventHandlerInterface(SomeipBindingEventHandlerInterface&&) = delete;
  SomeipBindingEventHandlerInterface& operator=(SomeipBindingEventHandlerInterface const&) & = delete;
  SomeipBindingEventHandlerInterface& operator=(SomeipBindingEventHandlerInterface&&) & = delete;

  /*!
   * \brief     Callback function for Event reception.
   * \details   Called from ClientManager.
   * \param[in] event_message Someip event message including the header.
   * \pre          -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnEvent(::amsr::someip_binding_core::internal::events::EventMessage const& event_message) = 0;

  /*!
   * \brief     Callback function for Event reception.
   * \details   Called from ClientManager.
   * \param[in] pdu_message Pdu message including the header.
   * \pre          -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnPduEvent(::amsr::someip_binding_core::internal::events::PduMessage const& pdu_message) = 0;

  /*!
   * \brief     Callback function for subscription state change.
   * \details   Called from ClientManager.
   * \param[in] state The updated event subscription state the payload.
   * \pre          -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void OnSubscriptionStateChange(::amsr::someip_protocol::internal::SubscriptionState const state) = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_EVENT_HANDLER_INTERFACE_H_
