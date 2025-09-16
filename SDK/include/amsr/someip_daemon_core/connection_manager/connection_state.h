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
/**        \file  connection_state.h
 *        \brief  Declaration of Connection State type.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief Connection State
 */
enum class ConnectionState : std::uint8_t {
  kDisconnected,  //<! No connection is available.
  kConnecting,    //<! Connection is requested but not yet established.
  kConnected      //<! Connection is established.
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_STATE_H_
