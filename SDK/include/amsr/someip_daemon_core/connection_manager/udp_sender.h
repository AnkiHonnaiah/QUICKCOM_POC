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
 *        \brief  Class declaration for UdpSender
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SENDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/connection_manager/connection_state_change_handler.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_transmit_remapper_interface.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class UdpEndpoint;
class UdpConnection;

/*!
 * \brief           UdpSender
 */
class UdpSender final : public packet_sink::RemotePacketSink {
 public:
  /*!
   * \brief           A UDP endpoint pointer type
   */
  using UdpEndpointPtr = std::shared_ptr<UdpEndpoint>;

  /*!
   * \brief           A UDP connection pointer type
   */
  using UdpConnectionPtr = typename std::add_pointer<UdpConnection>::type;

  /*!
   * \brief           A connection state change handler pointer type
   */
  using ConnectionStateChangeHandlerPtr = typename std::add_pointer<ConnectionStateChangeHandler>::type;

  /*!
   * \brief           Constructs instance of UdpSender
   * \param[in]       endpoint
   *                  A UDP endpoint this sender belongs to.
   * \param[in]       connection
   *                  A UDP connection this sender belongs to.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpSender(UdpEndpointPtr endpoint, UdpConnectionPtr connection) noexcept;

  /*!
   * \brief           Constructs instance of UdpSender including the remapper
   * \param[in]       endpoint
   *                  A UDP endpoint this sender belongs to.
   * \param[in]       connection
   *                  A UDP connection this sender belongs to.
   * \param[in]       remapper
   *                  The transmit remapper.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpSender(UdpEndpointPtr endpoint, UdpConnectionPtr connection,
            amsr::UniquePtr<pdu_id_mappers::PduIdTransmitRemapperInterface> remapper) noexcept;

  UdpSender(UdpSender const&) = delete;
  UdpSender& operator=(UdpSender const&) = delete;
  UdpSender(UdpSender&&) = delete;
  UdpSender& operator=(UdpSender&&) = delete;

  /*!
   * \brief           Destructs instance of UdpSender
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  ~UdpSender() noexcept final;

  /*!
   * \brief           Returns the remote IP address
   * \return          A remote IP address.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  someip_daemon_core::IpAddress const& GetRemoteAddress() const noexcept;

  /*!
   * \brief           Returns the remote port number
   * \return          A remote port number.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  amsr::net::ip::Port GetRemotePort() const noexcept;

  /*!
   * \brief           Sets a connection state change handler
   * \param[in]       handler
   *                  A connection state change handler.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  void SetConnectionStateChangeHandler(ConnectionStateChangeHandlerPtr handler) noexcept;

  /*!
   * \brief           Sends a SOME/IP message to remote peer
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  bool ForwardFromLocal(packet_sink::Packet packet) noexcept override;

  /*!
   * \brief           Sends a PDU message to remote peer
   * \param[in]       packet
   *                  A PDU message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  bool ForwardPduFromLocal(packet_sink::PduPacket packet) noexcept override;
  /*!
   * \brief           Notifies a UDP connection state change handler about a state change
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  void Notify() noexcept;

  /*!
   * \brief           Determines whether the underlying connection is already established
   * \return          true if the connection is established, otherwise false.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool IsConnected() const noexcept;

 private:
  /*!
   * \brief           A UDP endpoint this receiver belongs to
   */
  UdpEndpointPtr endpoint_;

  /*!
   * \brief           A UDP connection this sender belongs to
   */
  UdpConnectionPtr connection_;

  /*!
   * \brief           A connection state change handler
   */
  ConnectionStateChangeHandlerPtr handler_{nullptr};

  /*!
   * \brief           The remapper which maps the service ID and event ID to PDU ID
   */
  amsr::UniquePtr<pdu_id_mappers::PduIdTransmitRemapperInterface> remapper_{nullptr};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SENDER_H_
