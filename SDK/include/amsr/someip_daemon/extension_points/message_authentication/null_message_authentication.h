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
/*!        \file  null_message_authentication.h
 *        \brief  Default MAC implementation for the SOME/IP daemon that does not perform any actual MAC processing.
 *
 *********************************************************************************************************************/

#ifndef LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_NULL_MESSAGE_AUTHENTICATION_H_
#define LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_NULL_MESSAGE_AUTHENTICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "mac_interface.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace message_authentication {

/*!
 * \brief Null MAC adapter.
 */
class NullMessageAuthentication final : public MacInterface {
 public:
  /*!
   * \brief Factory method creating a new instance of the MAC interface implementation.
   * \details This function needs to be implemented by an actual MAC extension library.
   * \return A unique pointer to the newly created object.
   * \vprivate Product private.
   * \steady   FALSE
   */
  static inline auto Create() noexcept -> ara::core::Result<std::unique_ptr<MacInterface>> {
    using R = ara::core::Result<std::unique_ptr<MacInterface>>;
    return R::FromValue(std::make_unique<NullMessageAuthentication>());
  }

  /*!
   * \copydoc amsr::someip_daemon::extension_points::message_authentication::MacInterface::Verify
   * \steady  TRUE
   */
  inline auto Verify(someip_protocol::internal::ServiceId const service_id,
                     someip_protocol::internal::InstanceId const instance_id,
                     someip_protocol::internal::EventId const event_id,
                     std::shared_ptr<someip_protocol::internal::PduMessage> message,
                     ForwardPduMessageCallback &&forward_callback) noexcept -> ara::core::Result<void> final {
    static_cast<void>(service_id);
    static_cast<void>(instance_id);
    static_cast<void>(event_id);
    forward_callback(std::move(message));
    return {};
  }

  /*!
   * \copydoc amsr::someip_daemon::extension_points::message_authentication::MacInterface::Verify
   * \steady  TRUE
   */
  inline auto Verify(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                     ForwardSomeIpMessageCallback &&forward_callback) noexcept -> ara::core::Result<void> final {
    forward_callback(std::move(message));
    return {};
  }

  /*!
   * \copydoc amsr::someip_daemon::extension_points::message_authentication::MacInterface::Generate
   * \steady  TRUE
   */
  inline auto Generate(someip_protocol::internal::ServiceId const service_id,
                       someip_protocol::internal::InstanceId const instance_id,
                       someip_protocol::internal::EventId const event_id,
                       std::shared_ptr<someip_protocol::internal::PduMessage> message,
                       ForwardPduMessageCallback &&send_callback) noexcept -> ara::core::Result<void> final {
    static_cast<void>(service_id);
    static_cast<void>(instance_id);
    static_cast<void>(event_id);
    send_callback(std::move(message));
    return {};
  }

  /*!
   * \copydoc amsr::someip_daemon::extension_points::message_authentication::MacInterface::Generate
   * \steady  TRUE
   */
  inline auto Generate(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                       ForwardSomeIpMessageCallback &&send_callback) noexcept -> ara::core::Result<void> final {
    send_callback(std::move(message));
    return {};
  }
};

}  // namespace message_authentication
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_NULL_MESSAGE_AUTHENTICATION_H_
