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
/**        \file  method_request_handler_interface.h
 *        \brief  Definition of RemoteClient errors
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_METHOD_REQUEST_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_METHOD_REQUEST_HANDLER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief Represents a SOME/IP message.
 */
using SomeIpPacketSharedPtr = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

class MethodRequestHandler {
 public:
  MethodRequestHandler() = default;
  virtual ~MethodRequestHandler() noexcept = default;

  MethodRequestHandler(MethodRequestHandler const&) = delete;
  MethodRequestHandler(MethodRequestHandler&&) = delete;
  MethodRequestHandler& operator=(MethodRequestHandler const&) & = delete;
  MethodRequestHandler& operator=(MethodRequestHandler&&) & = delete;

  /*!
   * \brief Handle a method request and forward it to the application
   *
   * \param[in] instance_id The instance id of the sender.
   * \param[in] sender      The endpoint of the sender.
   * \param[in] packet      The received packet.
   * \return      True if the forwarding is successful, false otherwise.
   * \pre         The service must be offered.
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual bool HandleMethodRequest(
      someip_protocol::internal::InstanceId const instance_id, packet_router::RemoteSourceIdentifier const& sender,
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const noexcept = 0;

  /*!
   * \brief Handler a method request no return and forward it to the respective application
   *
   * \param[in] instance_id The instance id of the sender.
   * \param[in] packet      The received packet.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual bool HandleMethodRequestNoReturn(
      someip_protocol::internal::InstanceId const instance_id,
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_METHOD_REQUEST_HANDLER_INTERFACE_H_
