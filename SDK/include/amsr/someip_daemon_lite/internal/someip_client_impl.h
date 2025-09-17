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
/*!        \file   internal/someip_client_impl.h
 *         \brief  SOME/IP client for subscription and unsubscription of events (implementation).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpClient
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/local_client_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/internal/someip_client_event_handler.h"
#include "amsr/someip_daemon_lite/provided_instance_id.h"
#include "amsr/someip_daemon_lite/someip_event_handler_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief SOME/IP client for subscription and unsubscription of events.
 * \vprivate Component Private
 */
class SomeIpClientImpl final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] client_manager           Client manager.
   * \param[in] client_id                Client ID.
   * \param[in] service_id               Service ID.
   * \param[in] instance_id              Instance ID.
   * \param[in] major_interface_version  Major interface version.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  SomeIpClientImpl(ServiceId const service_id, ProvidedInstanceId const instance_id,
                   MajorInterfaceVersion const major_interface_version,
                   amsr::UniquePtr<someip_daemon_core::client::LocalClientInterface>&& uptr_local_client) noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   * \steady   FALSE
   */
  ~SomeIpClientImpl() noexcept = default;

  SomeIpClientImpl(SomeIpClientImpl&&) = delete;
  SomeIpClientImpl(SomeIpClientImpl const&) = delete;
  SomeIpClientImpl& operator=(SomeIpClientImpl const&) = delete;
  SomeIpClientImpl& operator=(SomeIpClientImpl&&) = delete;

  /*!
   * \brief Subscribe to a SOME/IP event.
   * \details \p event_handler shall point to a valid event handler until unsubscribed.
   *          and it shall not call any public API function as that will lead to a deadlock.
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpDaemonLiteErrc::* Translates and forwards error from LocalClient::SubscribeSomeIpEvent().
   *
   * \pre \p event_id shall not be in use already.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  ara::core::Result<void> SubscribeSomeIpEvent(EventId const event_id,
                                               SomeIpEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief Unsubscribe from a SOME/IP event.
   * \param[in] event_id ID of the event.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Wrong or already unsubscribed event.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  ara::core::Result<void> UnsubscribeSomeIpEvent(EventId const event_id) noexcept;

 private:
  /*!
   * \brief Owning pointer to a local client for SOME/IP communication.
   */
  amsr::UniquePtr<someip_daemon_core::client::LocalClientInterface> const uptr_local_client_;

  /*!
   * \brief Map between SOME/IP event IDs and their related event handlers.
   */
  ara::core::Map<EventId, std::shared_ptr<internal::SomeIpClientEventHandler>> some_ip_client_events_map_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                          someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                          ara::core::StringView{"SomeIpClientImpl"}};
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
