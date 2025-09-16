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
 *         \brief  SOME/IP client for subscription and unsubscription of events (implementation).
 *         \unit   SomeIpBinding::SomeIpBindingLite::Client::SomeIpBindingLiteBindingClient
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>

#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/internal/signal_based_client_event.h"
#include "amsr/someip_binding_lite/internal/someip_client_event.h"
#include "amsr/someip_binding_lite/signal_based_event_handler_interface.h"
#include "amsr/someip_binding_lite/someip_event_handler_interface.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief   Client for subscription and unsubscription of events.
 * \details This class wraps the required functionality of SOME/IP binding core without exposing the internal details.
 */
class SomeIpClientImpl final {
 public:
  /*!
   * \brief       Constructor.
   *
   * \param[in]   core_someip_client_wrapper    The SOME/IP binding core client wrapper.
   *
   * \pre         -
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit SomeIpClientImpl(::amsr::someip_binding_core::internal::LocalClient&& core_someip_client_wrapper) noexcept;

  /*!
   * \brief Destructor.
   *
   * \steady      FALSE
   */
  ~SomeIpClientImpl() noexcept;

  SomeIpClientImpl(SomeIpClientImpl&&) = delete;
  SomeIpClientImpl(SomeIpClientImpl const&) = delete;
  SomeIpClientImpl& operator=(SomeIpClientImpl const&) = delete;
  SomeIpClientImpl& operator=(SomeIpClientImpl&&) = delete;

  /*!
   * \brief Subscribe to a SOME/IP event.
   * \details \p event_handler shall point to a valid handler until unsubscribed and
   *          it shall not call any public API function as that will lead to a deadlock.
   *
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Handler for events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionTransmissionFailed If data transmission of a connection fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p event_id shall not be in use already (neither for a SOME/IP nor for a signal based event).
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE for same class instance.
   *              FALSE against other APIs for same class instance.
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> SubscribeSomeIpEvent(EventId const event_id,
                                               SomeIpEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief Subscribe to a signal based event.
   * \details \p event_handler shall point to a valid handler until unsubscribed and
   *          it shall not call any public function as that will lead to a deadlock.
   *
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Handler for events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionTransmissionFailed If data transmission of a connection fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p event_id shall not be in use already (neither for a SOME/IP nor for a signal based event).
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> SubscribeSignalBasedEvent(EventId const event_id,
                                                    SignalBasedEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief       Unsubscribe from a SOME/IP event.
   *
   * \param[in]   event_id ID of the event.
   *
   * \return      \p ara::core::Result with no value or an error.
   * \error       SomeIpBindingLiteErrc::kSystemConfigurationNotAvailable If system configuration is missing.
   * \error       SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre         -
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE for same class instance
   *              FALSE against other APIs for same class instance.
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> UnsubscribeSomeIpEvent(EventId const event_id) noexcept;

  /*!
   * \brief Unsubscribe from a signal based event.
   *
   * \param[in] event_id ID of the event.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kSystemConfigurationNotAvailable If system configuration is missing.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre -
   *
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> UnsubscribeSignalBasedEvent(EventId const event_id) noexcept;

  /*!
   * \brief      Get client's ID.
   *
   * \return     Client's ID.
   *
   * \pre -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  ClientId GetClientId() const noexcept;

 private:
  /*!
   * \brief The SOME/IP binding core client wrapper.
   */
  ::amsr::someip_binding_core::internal::LocalClient core_someip_client_wrapper_;

  /*!
   * \brief Map between SOME/IP event IDs and their related event handlers.
   */
  std::map<EventId, internal::SomeIpClientEvent> some_ip_client_events_map_{};

  /*!
   * \brief Map between signal based event IDs and their related event handlers.
   */
  std::map<EventId, internal::SignalBasedClientEvent> signal_based_client_events_map_{};

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_;

  /*!
   * \brief Check if the event ID is already in use or not.
   *
   * \param[in] event_id ID of the event.
   *
   * \pre \p event_id shall not be in use already (neither for a SOME/IP nor for a signal based event).
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void ValidateSubscription(EventId const event_id) const noexcept;
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_IMPL_H_
