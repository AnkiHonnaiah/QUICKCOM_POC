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
/*!        \file  client_event.h
 *        \brief  Client Event
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientEvent
 *
 *      \details  This class shall be used for event communication with the binding core.
 *                The life cycle is controller by its user.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/events/client_event_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief This class shall be used for event communication with the binding core.
 */
class ClientEvent final {
 public:
  /*!
   * \brief       Constructor
   *
   * \param[in]   implementation  A shared pointer to the binding client event implementation.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires implementation!= nullptr;
   * \endspec
   */
  explicit ClientEvent(std::shared_ptr<ClientEventInterface> implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~ClientEvent() noexcept = default;

  /*!
   * \brief       Default move constructor
   *
   * \context     ANY
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ClientEvent(ClientEvent&&) noexcept = default;

  ClientEvent& operator=(ClientEvent&&) noexcept = delete;
  ClientEvent(ClientEvent const&) = delete;
  ClientEvent& operator=(ClientEvent const&) & = delete;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::SubscribeEvent
   */
  amsr::core::Result<void> SubscribeEvent(std::size_t const cache_size,
                                          std::weak_ptr<EventNotificationInterface> handler) noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::UnsubscribeEvent
   */
  void UnsubscribeEvent() noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::ReadSamples
   */
  ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::SampleCacheContainer& ReadSamples(
      std::size_t const max_samples) noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::GetEventId
   */
  ::amsr::someip_protocol::internal::EventId GetEventId() const noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::GetSubscriptionCallState
   */
  bool GetSubscriptionCallState() const noexcept;

 private:
  /*!
   * \brief A shared pointer to the binding client event implementation.
   */
  std::shared_ptr<ClientEventInterface> implementation_;
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_H_
