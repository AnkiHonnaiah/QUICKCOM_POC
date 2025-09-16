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
/*!        \file   internal/someip_client_event_handler.h
 *         \brief  SOME/IP client event handler.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpClient
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_HANDLER_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/someip_event_handler_interface.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief SOME/IP client event handler.
 * \vprivate Vector product internal API.
 */
class SomeIpClientEventHandler final : public someip_daemon_core::client::SomeIpEventHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in] event_handler A pointer to an event handler.
   * \vprivate Product Private
   * \steady   FALSE
   */
  explicit SomeIpClientEventHandler(someip_daemon_lite::SomeIpEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Product Private
   * \steady   FALSE
   */
  ~SomeIpClientEventHandler() noexcept final = default;

  SomeIpClientEventHandler() = delete;
  SomeIpClientEventHandler(SomeIpClientEventHandler const&) = delete;
  SomeIpClientEventHandler(SomeIpClientEventHandler&&) = delete;
  SomeIpClientEventHandler& operator=(SomeIpClientEventHandler const&) = delete;
  SomeIpClientEventHandler& operator=(SomeIpClientEventHandler&&) = delete;

  /*!
   * \brief Callback function for event reception.
   *
   * \param[in] instance_id SOME/IP instance ID.
   * \param[in] packet SOME/IP event message including the header.
   *
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Product Private
   * \steady      TRUE
   */
  void OnSomeIpEvent(InstanceIdType const instance_id, PacketType const packet) noexcept final;

  /*!
   * \brief Callback function for initial field notification reception.
   *
   * \param[in] instance_id SOME/IP instance ID.
   * \param[in] packet SOME/IP event message including the header.
   * \param[in] client_id SOME/IP client ID.
   *
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Product Private
   * \steady      FALSE
   */
  void OnSomeIpInitialFieldNotification(InstanceIdType const instance_id, PacketType const packet,
                                        ClientId const client_id) noexcept final;

  /*!
   * \brief Callback function for subscription state change.
   *
   * \param[in] service_instance_id The service instance ID.
   * \param[in] event_id The subscribed event identifier with the event subscription change.
   * \param[in] new_state Current state of the event subscription.
   *
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Product Private
   * \steady      FALSE
   */
  void OnSomeIpSubscriptionStateChange(SomeIpServiceInstanceIdType const& service_instance_id, EventId const event_id,
                                       SubscriptionStateType const new_state) noexcept final;

 private:
  /*!
   * \brief Pointer to an event handler.
   */
  someip_daemon_lite::SomeIpEventHandlerInterface* event_handler_;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                          someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                          ara::core::StringView{"SomeIpClientEventHandler"}};
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_HANDLER_H_
