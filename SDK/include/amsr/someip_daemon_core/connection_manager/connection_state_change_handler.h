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
 *        \brief  Class declaration of ConnectionStateChangeHandler
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpSender;
class UdpSender;

/*!
 * \brief           Class declaration of ConnectionStateChangeHandler
 */
class ConnectionStateChangeHandler {
 public:
  /*!
   * \brief           Constructs a default instance of Connection State Change Handler
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ConnectionStateChangeHandler() noexcept = default;

  /*!
   * \brief           Destructs instance
   * \pre             -
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual ~ConnectionStateChangeHandler() noexcept = default;

  ConnectionStateChangeHandler(ConnectionStateChangeHandler const &) = delete;
  ConnectionStateChangeHandler(ConnectionStateChangeHandler &&) = delete;
  ConnectionStateChangeHandler &operator=(ConnectionStateChangeHandler const &) = delete;
  ConnectionStateChangeHandler &operator=(ConnectionStateChangeHandler &&) = delete;

  /*!
   * \brief           TcpSender pointer type
   */
  using TcpSenderPtr = typename std::add_pointer<packet_sink::RemotePacketSink>::type;

  /*!
   * \brief           UdpSender pointer type
   */
  using UdpSenderPtr = typename std::add_pointer<UdpSender>::type;

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  /*!
   * \brief           Called when a TCP connection state is changed
   * \param[in]       sender
   *                  The TCP sender associated to the connection that changed its state.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  virtual void OnConnectionStateChange(TcpSenderPtr sender) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  /*!
   * \brief           Called when a UDP connection state is changed
   * \param[in]       sender
   *                  The UDP sender associated to the connection that changed its state.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual void OnConnectionStateChange(UdpSenderPtr sender) noexcept = 0;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_H_
