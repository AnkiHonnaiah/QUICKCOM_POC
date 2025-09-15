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
/*!        \file   someip_daemon_lite/someip_event_handler_interface.h
 *         \brief  SOME/IP event handler interface.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::InterfaceClasses
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/raw_data.h"
#include "amsr/someip_daemon_lite/subscription_state.h"

namespace amsr {
namespace someip_daemon_lite {

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

  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface&&) = delete;
  SomeIpEventHandlerInterface(SomeIpEventHandlerInterface const&) = delete;
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
  virtual void OnSubscriptionStateChange(SubscriptionState const state) noexcept = 0;
};

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_EVENT_HANDLER_INTERFACE_H_
