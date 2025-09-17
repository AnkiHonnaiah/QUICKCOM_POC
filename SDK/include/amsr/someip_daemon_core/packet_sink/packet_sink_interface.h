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
/**        \file  packet_sink_interface.h
 *        \brief  Packet Sink Interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_SINK_PACKET_SINK_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_SINK_PACKET_SINK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {

namespace packet_router {

struct RemoteSourceIdentifier;

}  // namespace packet_router

namespace packet_sink {

/*!
 * \brief Represents a SOME/IP message.
 */
using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

/*!
 * \brief Represents a PDU message.
 */
using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;

/*!
 * \brief LocalPacketSink.
 */
class LocalPacketSink {
 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  LocalPacketSink() = default;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  virtual ~LocalPacketSink() = default;

  LocalPacketSink(LocalPacketSink const &) = delete;
  LocalPacketSink(LocalPacketSink &&) = delete;
  LocalPacketSink &operator=(LocalPacketSink const &) & = delete;
  LocalPacketSink &operator=(LocalPacketSink &&) & = delete;

  /*!
   * \brief Forwards a SOME/IP request message to its destination.
   *
   * \param[in] instance_id    A SOME/IP service instance ID.
   * \param[in] sender         A remote source
   * \param[in] packet         A SOME/IP message.
   *
   * \steady  TRUE
   * \return "true" if the message is sent successfully, "false" otherwise.
   */
  virtual auto HandleMethodRequest(someip_protocol::internal::InstanceId const instance_id,
                                   packet_router::RemoteSourceIdentifier const &sender, Packet packet) noexcept
      -> bool = 0;

  /*!
   * \brief Forwards a SOME/IP request no return message to its destination.
   *
   * \param[in] instance_id    A SOME/IP service instance ID.
   * \param[in] packet         A SOME/IP message.
   *
   * \steady  TRUE
   * \return "true" if the message is sent successfully, "false" otherwise.
   */
  virtual auto HandleMethodRequestNoReturn(someip_protocol::internal::InstanceId const instance_id,
                                           Packet packet) noexcept -> bool = 0;

  /*!
   * \brief Forwards a PDU message to its destination.
   *
   * \param[in] instance_id    A SOME/IP service instance ID.
   * \param[in] packet         A PDU message.
   *
   * \steady  FALSE
   * \return "true" if the message is sent successfully, "false" otherwise.
   */
  virtual auto ForwardPdu(someip_protocol::internal::InstanceId const instance_id, PduPacket packet) noexcept
      -> bool = 0;
};

/*!
 * \brief RemotePacketSink.
 */
class RemotePacketSink {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  RemotePacketSink() = default;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  virtual ~RemotePacketSink() = default;

  RemotePacketSink(RemotePacketSink const &) = delete;
  RemotePacketSink(RemotePacketSink &&) = delete;
  RemotePacketSink &operator=(RemotePacketSink const &) & = delete;
  RemotePacketSink &operator=(RemotePacketSink &&) & = delete;

  /*!
   * \brief Forwards a SOME/IP message to its destination.
   *
   * \param[in] packet       A SOME/IP message.
   *
   * \steady TRUE
   * \return "true" if the message is sent successfully, "false" otherwise.
   */
  virtual auto ForwardFromLocal(Packet packet) noexcept -> bool = 0;

  /*!
   * \brief Forwards a PDU message to its destination.
   *
   * \param[in] packet       A PDU message.
   *
   * \steady TRUE
   * \return "true" if the message is sent successfully, "false" otherwise.
   */
  virtual auto ForwardPduFromLocal(PduPacket packet) noexcept -> bool = 0;
};

}  // namespace packet_sink
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_SINK_PACKET_SINK_INTERFACE_H_
