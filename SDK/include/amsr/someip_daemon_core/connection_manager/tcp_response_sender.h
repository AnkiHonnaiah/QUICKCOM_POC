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
 *        \brief  Class declaration for TcpResponseSender
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RESPONSE_SENDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RESPONSE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;
class TcpConnection;

/*!
 * \brief           Class for TcpResponseSender
 */
class TcpResponseSender final : public packet_sink::RemotePacketSink {
 public:
  /*!
   * \brief           A TCP connection pointer type
   */
  using TcpConnectionPtr = TcpConnection*;

  /*!
   * \brief           Constructor of TcpResponseSender
   * \param[in]       endpoint
   *                  A reference to the TCP endpoint this response sender belongs to.
   * \param[in]       connection
   *                  A TCP connection this response sender belongs to.
   * \context         Init
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpResponseSender(TcpEndpoint& endpoint, TcpConnectionPtr connection) noexcept;

  TcpResponseSender(TcpResponseSender const&) = delete;
  TcpResponseSender& operator=(TcpResponseSender const&) = delete;
  TcpResponseSender(TcpResponseSender&&) = delete;
  TcpResponseSender& operator=(TcpResponseSender&&) = delete;

  /*!
   * \brief           Destructor of TcpResponseSender
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpResponseSender() noexcept final;
  /*!
   * \brief           Sends a SOME/IP message to remote peer
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool ForwardFromLocal(packet_sink::Packet packet) noexcept final;
  /*!
   * \brief           Sends a PDU message to remote peer
   * \param[in]       packet
   *                  A PDU message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool ForwardPduFromLocal(packet_sink::PduPacket packet) noexcept final;

 private:
  /*!
   * \brief           A reference to the TCP endpoint this response sender belongs to
   */
  TcpEndpoint& endpoint_;

  /*!
   * \brief           A TCP connection this response sender belongs to
   */
  TcpConnectionPtr connection_;

  /*!
   * \brief           Friend test, to test the ForwardPduFromLocal() function on TcpResponseSender
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpResponseSender, ForwardPduFromLocal);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RESPONSE_SENDER_H_
