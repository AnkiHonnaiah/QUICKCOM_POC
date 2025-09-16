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
/*!        \file  mac_interface.h
 *        \brief  SOME/IP daemon specific interface for providing message authentication (MAC).
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the MAC component to allow optionally
 *                enabling / disabling the component this interface is defined. In order to provide actual IamInterface
 *                services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_MAC_INTERFACE_H_
#define LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_MAC_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/variant.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace message_authentication {

/*!
 * \brief Interface class for message authentication and validation.
 */
class MacInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  MacInterface() = default;

  /*!
   * \brief Define default destructor.
   * \steady FALSE
   */
  virtual ~MacInterface() = default;

  MacInterface(MacInterface const&) = delete;
  MacInterface(MacInterface&&) = delete;
  MacInterface& operator=(MacInterface const&) & = delete;
  MacInterface& operator=(MacInterface&&) & = delete;

  /*!
   * \brief Callback used to forward PDU messages.
   * \param[in] message The message to be forwarded.
   */
  using ForwardPduMessageCallback =
      vac::language::UniqueFunction<void(std::shared_ptr<someip_protocol::internal::PduMessage>&& message)>;
  /*!
   * \brief Callback used to forward SomeIp messages.
   * \param[in] message The message to be forwarded.
   */
  using ForwardSomeIpMessageCallback =
      vac::language::UniqueFunction<void(std::shared_ptr<someip_protocol::internal::SomeIpMessage>&& message)>;

  /*!
   * \vprivate Component private.
   *
   * \brief Verify given PDU.
   *
   * \param[in] service_id Service ID corresponding to the message.
   * \param[in] instance_id Instance ID corresponding to the message.
   * \param[in] event_id Event ID or method ID depending on the context.
   * \param[in] message Message to be verified.
   * \param[in] forward_callback Callback used to forward verified messages.
   *
   * \return result with void as value.
   * \context  ANY
   * \steady   TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual auto Verify(someip_protocol::internal::ServiceId const service_id,
                      someip_protocol::internal::InstanceId const instance_id,
                      someip_protocol::internal::EventId const event_id,
                      std::shared_ptr<someip_protocol::internal::PduMessage> message,
                      ForwardPduMessageCallback&& forward_callback) noexcept -> ara::core::Result<void> = 0;

  /*!
   * \vprivate Component private.
   *
   * \brief Verify given SomeIp message.
   *
   * \param[in] message Message to be verified.
   * \param[in] forward_callback Callback used to forward verified messages.
   *
   * \return result with void as value.
   * \context  ANY
   * \steady   TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual auto Verify(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                      ForwardSomeIpMessageCallback&& forward_callback) noexcept -> ara::core::Result<void> = 0;

  /*!
   * \vprivate Component private.
   *
   * \brief Generate a PDU message with MAC or digital signature and forwards it to the provided callback.
   *
   * \param[in] service_id Service ID corresponding to the message.
   * \param[in] instance_id Instance ID corresponding to the message.
   * \param[in] event_id Event ID or method ID depending on the context.
   * \param[in] message Packet for which MAC shall be generated.
   * \param[in] send_callback Callback to provide generated messages for transmission.
   *
   * \return result with void as value.
   * \context  ANY
   * \steady   TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual auto Generate(someip_protocol::internal::ServiceId const service_id,
                        someip_protocol::internal::InstanceId const instance_id,
                        someip_protocol::internal::EventId const event_id,
                        std::shared_ptr<someip_protocol::internal::PduMessage> message,
                        ForwardPduMessageCallback&& send_callback) noexcept -> ara::core::Result<void> = 0;

  /*!
   * \vprivate Component private.
   *
   * \brief Generate a SomeIp message with MAC or digital signature and forwards it to the provided callback.
   *
   * \param[in] message Packet for which MAC shall be generated.
   * \param[in] send_callback Callback to provide generated messages for transmission.
   *
   * \return result with void as value.
   * \context  ANY
   * \steady   TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual auto Generate(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                        ForwardSomeIpMessageCallback&& send_callback) noexcept -> ara::core::Result<void> = 0;
};

}  // namespace message_authentication
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_MAC_INTERFACE_H_
