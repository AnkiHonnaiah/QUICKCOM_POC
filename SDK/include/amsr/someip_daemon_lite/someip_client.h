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
/*!        \file   someip_daemon_lite/someip_client.h
 *         \brief  SOME/IP client for subscription and unsubscription of events (delegator).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpClient
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_CLIENT_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_CLIENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/core/map.h"

#include "amsr/someip_daemon_lite/internal/someip_client_impl.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_lite {

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
   * \steady   FALSE
   */
  explicit SomeIpClient(amsr::UniquePtr<internal::SomeIpClientImpl>&& implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  ~SomeIpClient() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details Needed for compatibility with \p ara::core::Result.
   *          Member \p implementation_ is set to "nullptr" after the invocation of this function.
   * \vprivate Vector product internal API.
   * \steady   FALSE
   */
  SomeIpClient(SomeIpClient&&) noexcept = default;

  SomeIpClient(SomeIpClient const&) = delete;
  SomeIpClient& operator=(SomeIpClient const&) = delete;
  SomeIpClient& operator=(SomeIpClient&&) = delete;

  /*!
   * \brief Subscribe to a SOME/IP event.
   * \details \p event_handler shall point to a valid event handler until unsubscribed.
   *          and it shall not call any public function as that will lead to a deadlock.
   * \vpublic
   * \param[in] event_id ID of the event.
   * \param[in] event_handler Events' callbacks.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Event not found, does not map to a required
   *                                                               eventgroup, or already subscribed to.
   *
   * \pre \p event_id shall not be in use already.
   * \pre This call shall not be invoked on a moved instance.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> SubscribeSomeIpEvent(EventId const event_id,
                                               SomeIpEventHandlerInterface* event_handler) const noexcept;

  /*!
   * \brief Unsubscribe from a SOME/IP event.
   * \vpublic
   * \param[in] event_id ID of the event.
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Wrong or already unsubscribed event.
   *
   * \pre This call shall not be invoked on a moved instance.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> UnsubscribeSomeIpEvent(EventId const event_id) const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  amsr::UniquePtr<internal::SomeIpClientImpl> implementation_;
};

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_CLIENT_H_
