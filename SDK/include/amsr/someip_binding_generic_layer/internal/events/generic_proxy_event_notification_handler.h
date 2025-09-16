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
/*!        \file
 *         \brief GenericProxyEventNotificationHandler represents a generic proxy event notifier.
 *         \unit SomeIpBinding::SomeIpBindingGenericLayer::Events::GenericProxyEvent
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_NOTIFICATION_HANDLER_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_NOTIFICATION_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace events {

/*!
 * \brief NotificationHandler class for generic proxy event notifications.
 */
class GenericProxyEventNotificationHandler final
    : public ::amsr::someip_binding_core::internal::events::EventNotificationInterface {
 public:
  /*!
   * \brief Type alias for EventNotificationInterface.
   */
  using EventNotificationInterface = ::amsr::someip_binding_core::internal::events::EventNotificationInterface;

  /*!
   * \brief       Constructor
   *
   * \param[in]   generic_proxy_event   Reference to the generic proxy event handler to delegate the calls.
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  explicit GenericProxyEventNotificationHandler(EventNotificationInterface& generic_proxy_event)
      : EventNotificationInterface{}, generic_proxy_event_{generic_proxy_event} {}

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~GenericProxyEventNotificationHandler() noexcept = default;

  GenericProxyEventNotificationHandler(GenericProxyEventNotificationHandler const&) = delete;
  GenericProxyEventNotificationHandler(GenericProxyEventNotificationHandler&&) = delete;
  GenericProxyEventNotificationHandler& operator=(GenericProxyEventNotificationHandler const&) & = delete;
  GenericProxyEventNotificationHandler& operator=(GenericProxyEventNotificationHandler&&) & = delete;

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventReceived
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventReceived() noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return generic_proxy_event_.OnEventReceived();
  }

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventSubscriptionStateUpdate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventSubscriptionStateUpdate(::amsr::someip_protocol::internal::SubscriptionState const state) noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return generic_proxy_event_.OnEventSubscriptionStateUpdate(state);
  }

 private:
  /*!
   * \brief Reference to the generic proxy event handler to delegate the calls.
   */
  EventNotificationInterface& generic_proxy_event_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_NOTIFICATION_HANDLER_H_
