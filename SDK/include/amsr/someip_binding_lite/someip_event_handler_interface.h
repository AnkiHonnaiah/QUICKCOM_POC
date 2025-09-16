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
/*!        \file   someip_binding_lite/someip_event_handler_interface.h
 *         \brief  SOME/IP event handler interface.
 *         \unit   SomeIpBinding::SomeIpBindingLite::Communication::Events::LiteSomeIpClientEvent
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/raw_data.h"
#include "amsr/someip_binding_lite/subscription_state.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief SOME/IP event handler interface.
 * \vpublic
 */
class SomeIpEventHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   * \steady FALSE
   */
  SomeIpEventHandlerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  virtual ~SomeIpEventHandlerInterface() noexcept = default;

  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface const&) = delete;
  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface&&) = delete;
  SomeIpEventHandlerInterface& operator=(SomeIpEventHandlerInterface const&) = delete;
  SomeIpEventHandlerInterface& operator=(SomeIpEventHandlerInterface&&) = delete;

  /*!
   * \brief Callback function for SOME/IP event reception.
   * \vpublic
   * \details \p raw_data SOME/IP event message is only valid within the callback.
   *          It shall not be accessed later on.
   * \param[in] raw_data SOME/IP event message in raw format.
   * \param[in] service_id Service ID.
   * \param[in] instance_id Instance ID.
   * \param[in] event_id Event ID.
   * \param[in] major_interface_version Major interface version.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const
  virtual void OnSomeIpEvent(RawData const& raw_data, ServiceId const service_id, InstanceId const instance_id,
                             EventId const event_id, MajorInterfaceVersion const major_interface_version) noexcept = 0;

  /*!
   * \brief Callback function for subscription state change.
   * \vpublic
   * \param[in] state The updated event subscription state of the payload.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const
  virtual void OnSubscriptionStateChange(SubscriptionState const state) noexcept = 0;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
