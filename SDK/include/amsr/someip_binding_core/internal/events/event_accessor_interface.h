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
/**        \file  event_accessor_interface.h
 *        \brief  Event Accessor Interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_ACCESSOR_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_ACCESSOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_binding_core/internal/events/client_event_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_event_handler_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief Interface class that combines ClientEventInterface and SomeIPBindingEventHandlerInterface.
 */
// VECTOR NC AutosarC++17_10-M10.2.1: MD_SOMEIPBINDING_AutosarC++17_10-M10.2.1_entity_names_in_multiple_inheritance
class EventAccessorInterface : public ClientEventInterface, public SomeipBindingEventHandlerInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  EventAccessorInterface() = default;

  /*!
   * \brief Define destructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~EventAccessorInterface() noexcept override = default;

  EventAccessorInterface(EventAccessorInterface const&) = delete;
  EventAccessorInterface(EventAccessorInterface&&) = delete;
  EventAccessorInterface& operator=(EventAccessorInterface const&) & = delete;
  EventAccessorInterface& operator=(EventAccessorInterface&&) & = delete;
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_EVENT_ACCESSOR_INTERFACE_H_
