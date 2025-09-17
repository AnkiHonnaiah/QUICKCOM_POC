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
 *         \brief EventNotificationHandler represents a proxy event xf notifier.
 *         \unit SomeIpBinding::SomeIpBindingXf::Communication::Events::ProxyEventXf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_NOTIFICATION_HANDLER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_NOTIFICATION_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief NotificationHandler class for proxy event xf notifications.
 */
class EventNotificationHandler final
    : public ::amsr::someip_binding_core::internal::events::EventNotificationInterface {
 public:
  /*!
   * \brief Type alias for EventNotificationInterface.
   */
  using EventNotificationInterface = ::amsr::someip_binding_core::internal::events::EventNotificationInterface;

  /*!
   * \brief       Constructor
   *
   * \param[in]   proxy_event_xf   Reference to the proxy event xf handler to delegate the calls.
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit EventNotificationHandler(EventNotificationInterface& proxy_event_xf)
      : EventNotificationInterface{}, proxy_event_xf_{proxy_event_xf} {}

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~EventNotificationHandler() noexcept = default;

  EventNotificationHandler(EventNotificationHandler const&) = delete;
  EventNotificationHandler(EventNotificationHandler&&) = delete;
  EventNotificationHandler& operator=(EventNotificationHandler const&) & = delete;
  EventNotificationHandler& operator=(EventNotificationHandler&&) & = delete;

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventReceived
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventReceived() noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return proxy_event_xf_.OnEventReceived();
  }

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventSubscriptionStateUpdate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventSubscriptionStateUpdate(::amsr::someip_protocol::internal::SubscriptionState const state) noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return proxy_event_xf_.OnEventSubscriptionStateUpdate(state);
  }

 private:
  /*!
   * \brief Reference to the proxy event handler to delegate the calls.
   */
  EventNotificationInterface& proxy_event_xf_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_NOTIFICATION_HANDLER_H_
