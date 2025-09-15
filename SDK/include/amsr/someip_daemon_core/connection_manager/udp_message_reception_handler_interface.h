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
 *        \brief  This interface is used for reception of protocol messages received from UDP datagrams.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           UdpMessageReceptionHandlerInterface used for protocol message processing
 */
class UdpMessageReceptionHandlerInterface {
 public:
  /*!
   * \brief           Default constructor
   * \steady          TRUE
   */
  UdpMessageReceptionHandlerInterface() noexcept = default;
  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  virtual ~UdpMessageReceptionHandlerInterface() noexcept = default;

  UdpMessageReceptionHandlerInterface(UdpMessageReceptionHandlerInterface const&) = delete;
  UdpMessageReceptionHandlerInterface(UdpMessageReceptionHandlerInterface&&) = delete;
  UdpMessageReceptionHandlerInterface& operator=(UdpMessageReceptionHandlerInterface const&) = delete;
  UdpMessageReceptionHandlerInterface& operator=(UdpMessageReceptionHandlerInterface&&) = delete;

  /*!
   * \brief           Processes the given protocol message
   * \details         Upon success the PDU or SOME/IP message is forwarded to the packet router.
   * \param[in]       message
   *                  The protocol message to proccess.
   * \return          Result<void> on success.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual ara::core::Result<void> ProcessMessage(message_reader::ProtocolMessage const& message) const noexcept = 0;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_INTERFACE_H_
