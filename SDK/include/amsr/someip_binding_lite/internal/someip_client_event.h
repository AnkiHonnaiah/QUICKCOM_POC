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
 *         \brief  SOME/IP client event handler.
 *         \unit   SomeIpBinding::SomeIpBindingLite::Communication::Events::LiteSomeIpClientEvent
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_lite/internal/data_types.h"
#include "amsr/someip_binding_lite/someip_event_handler_interface.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief SOME/IP client event handler.
 * \vprivate Vector product internal API.
 */
class SomeIpClientEvent final : public someip_binding_core::internal::SomeipBindingEventHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in] event_handler A pointer to an event handler.
   * \steady    FALSE
   */
  explicit SomeIpClientEvent(SomeIpEventHandlerInterface* event_handler) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SomeIpClientEvent() noexcept final = default;

  SomeIpClientEvent(SomeIpClientEvent const&) = delete;
  SomeIpClientEvent(SomeIpClientEvent&&) = delete;
  SomeIpClientEvent& operator=(SomeIpClientEvent const&) = delete;
  SomeIpClientEvent& operator=(SomeIpClientEvent&&) = delete;

  /*!
   * \brief Callback function for event reception.
   * \param[in] event_message SOME/IP event message including the header.
   * \pre Unsupported count of buffer fragments shall not be used.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void OnEvent(someip_binding_core::internal::events::EventMessage const& event_message) noexcept final;

  /*!
   * \brief Callback function for signal based event reception.
   * \details This callback should not be invoked.
   * \param[in] pdu_message Signal based message including the header.
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void OnPduEvent(someip_binding_core::internal::events::PduMessage const& pdu_message) noexcept final;

  /*!
   * \brief Callback function for subscription state change.
   * \param[in] state The updated event subscription state of the payload.
   * \pre Invalid enumeration constant value shall not be used.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  void OnSubscriptionStateChange(SubscriptionStateType const state) noexcept final;

 private:
  /*!
   * \brief Pointer to an event handler.
   */
  SomeIpEventHandlerInterface* event_handler_;

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_;
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_CLIENT_EVENT_H_
