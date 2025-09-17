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
 *        \brief  SOME/IP UDP response sender class
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RESPONSE_SENDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RESPONSE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class UdpEndpoint;
class UdpConnection;

/*!
 * \brief           Declaration SOME/IP UDP response sender class, UdpResponseSender
 */
class UdpResponseSender final : public packet_sink::RemotePacketSink {
 public:
  /*!
   * \brief           A UDP endpoint pointer type
   */
  using UdpEndpointPtr = std::shared_ptr<UdpEndpoint>;

  /*!
   * \brief           A UDP connection pointer type
   */
  using UdpConnectionPtr = UdpConnection*;

  /*!
   * \brief           Constructor of UdpResponseSender
   * \param[in]       endpoint
   *                  A UDP endpoint this response sender belongs to.
   * \param[in]       connection
   *                  A UDP connection this response sender belongs to.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpResponseSender(UdpEndpointPtr endpoint, UdpConnectionPtr connection) noexcept;

  UdpResponseSender(UdpResponseSender const&) = delete;
  UdpResponseSender& operator=(UdpResponseSender const&) = delete;
  UdpResponseSender(UdpResponseSender&&) = delete;
  UdpResponseSender& operator=(UdpResponseSender&&) = delete;

  /*!
   * \brief           Destructs instance of UdpResponseSender
   * \context         Shutdown
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ~UdpResponseSender() noexcept final;

  /*!
   * \brief           Sends a SOME/IP message to remote peer
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  bool ForwardFromLocal(packet_sink::Packet packet) noexcept final;

  /*!
   * \brief           Sends a PDU message to remote peer
   * \param[in]       packet
   *                  A PDU message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  bool ForwardPduFromLocal(packet_sink::PduPacket packet) noexcept final;

 private:
  /*!
   * \brief           A UDP endpoint this response sender belongs to
   */
  UdpEndpointPtr endpoint_;

  /*!
   * \brief           A UDP connection this response sender belongs to
   */
  UdpConnectionPtr connection_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_RESPONSE_SENDER_H_
