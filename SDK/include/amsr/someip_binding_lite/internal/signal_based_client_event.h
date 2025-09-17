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
 *         \brief  Signal based client event handler.
 *         \unit   SomeIpBinding::SomeIpBindingLite::Communication::Events::LiteSignalBasedClientEvent
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SIGNAL_BASED_CLIENT_EVENT_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SIGNAL_BASED_CLIENT_EVENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_lite/internal/data_types.h"
#include "amsr/someip_binding_lite/signal_based_event_handler_interface.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief Signal based client event handler.
 * \vprivate Vector product internal API.
 */
class SignalBasedClientEvent final : public someip_binding_core::internal::SomeipBindingEventHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in] event_handler A pointer to an event handler.
   * \steady      FALSE
   */
  explicit SignalBasedClientEvent(SignalBasedEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief Destructor.
   * \steady      FALSE
   */
  ~SignalBasedClientEvent() noexcept final = default;

  SignalBasedClientEvent(SignalBasedClientEvent const&) = delete;
  SignalBasedClientEvent(SignalBasedClientEvent&&) = delete;
  SignalBasedClientEvent& operator=(SignalBasedClientEvent const&) = delete;
  SignalBasedClientEvent& operator=(SignalBasedClientEvent&&) = delete;

  /*!
   * \brief Callback function for event reception.
   * \details This callback should not be invoked.
   * \param[in] event_message SOME/IP event message including the header.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void OnEvent(someip_binding_core::internal::events::EventMessage const& event_message) noexcept final;

  /*!
   * \brief Callback function for event reception.
   * \param[in] pdu_message Signal based message including the header.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void OnPduEvent(someip_binding_core::internal::events::PduMessage const& pdu_message) noexcept final;

  /*!
   * \brief Callback function for subscription state change.
   * \param[in] state The updated event subscription state of the payload.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  void OnSubscriptionStateChange(SubscriptionStateType const state) noexcept final;

 private:
  /*!
   * \brief Pointer to an event handler.
   */
  SignalBasedEventHandlerInterface* event_handler_;

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_{
      someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "SignalBasedClientEvent"_sv};
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SIGNAL_BASED_CLIENT_EVENT_H_
