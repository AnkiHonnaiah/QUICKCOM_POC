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
/*!        \file   someip_binding_lite/someip_client.h
 *         \brief  SOME/IP client for subscription and unsubscription of events (delegator).
 *         \unit   SomeIpBinding::SomeIpBindingLite::Client::SomeIpBindingLiteBindingClient
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_CLIENT_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_CLIENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/someip_binding_lite/internal/someip_client_impl.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief SOME/IP client for subscription and unsubscription of events.
 * \vpublic
 */
class SomeIpClient final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] implementation The SomeIpClient implementation.
   * \vprivate Vector product internal API.
   *
   * \steady      FALSE
   */
  explicit SomeIpClient(std::unique_ptr<internal::SomeIpClientImpl>&& implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~SomeIpClient() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details This will invalidate the owned resource. Any call to an instance of a
   *          moved-from object of this type will trigger an abort.
   * \steady   FALSE
   * \vprivate Vector product internal API.
   */
  SomeIpClient(SomeIpClient&&) noexcept = default;

  SomeIpClient(SomeIpClient const&) = delete;
  SomeIpClient& operator=(SomeIpClient const&) = delete;
  SomeIpClient& operator=(SomeIpClient&&) = delete;

  /*!
   * \brief Subscribe to a SOME/IP event.
   * \details \p event_handler shall point to a valid handler until unsubscribed.
   *          and it shall not call any public function as that will lead to a deadlock.
   * \vpublic
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Handler for events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionTransmissionFailed If data transmission of a connection fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p event_id shall not be in use already.
   * \pre This function shall not be invoked on a moved-from instance.
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
                                               SomeIpEventHandlerInterface* event_handler) const noexcept;

  /*!
   * \brief Subscribe to a signal based event.
   * \details \p event_handler shall point to a valid handler until unsubscribed.
   *          and it shall not call any public function as that will lead to a deadlock.
   * \vpublic
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Handler for events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionTransmissionFailed If data transmission of a connection fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p event_id shall not be in use already.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   */
  ara::core::Result<void> SubscribeSignalBasedEvent(EventId const event_id,
                                                    SignalBasedEventHandlerInterface* event_handler) const noexcept;

  /*!
   * \brief Unsubscribe from a SOME/IP event.
   * \vpublic
   * \param[in] event_id ID of the event.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kSystemConfigurationNotAvailable If system configuration is missing.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre This function shall not be invoked on a moved-from instance.
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE for same class instance
   *              FALSE against other APIs for same class instance.
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> UnsubscribeSomeIpEvent(EventId const event_id) const noexcept;

  /*!
   * \brief Unsubscribe from a signal based event.
   * \vpublic
   * \param[in] event_id ID of the event.
   * \return \p ara::core::Result with no value or an error.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   */
  ara::core::Result<void> UnsubscribeSignalBasedEvent(EventId const event_id) const noexcept;

  /*!
   * \brief Get client's ID.
   * \vpublic
   * \return Client's ID.
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  ClientId GetClientId() const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  std::unique_ptr<internal::SomeIpClientImpl> implementation_;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_CLIENT_H_
