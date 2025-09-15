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
/*!        \file  someip_daemon_client/internal/aracom_ipc_connection.h
 *        \brief  AraCom Configuration for Basic IPC Connection.
 *
 *      \details  This class encapsulates the required classes for basic IPC Connection.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_ARACOM_IPC_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_ARACOM_IPC_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_client/external/osabstraction/connection.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief ara::com IPC Connection class.
 *
 * \details This class encapsulates the required classes for IPC Connection.
 * It is mainly required for testing to mock the IPC behavior.
 *
 * \tparam TemplateConfiguration Contains the following required type definition:
 *         - typename TemplateConfiguration::IpcConnectionType
 */
template <typename TemplateConfiguration>
class AraComIpcConnection {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  AraComIpcConnection() = default;
  /*!
   * \brief Define destructor.
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~AraComIpcConnection() noexcept = default;
  /*!
   * \brief Delete copy constructor.
   */
  AraComIpcConnection(AraComIpcConnection const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  AraComIpcConnection(AraComIpcConnection&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(AraComIpcConnection const&) & -> AraComIpcConnection& = delete;
  /*!
   * \brief Delete move assignment.
   */
  auto operator=(AraComIpcConnection&&) & -> AraComIpcConnection& = delete;

  /*!
   * \brief The specific type of a class responsible for handling the IPC connection.
   */
  using IpcConnectionType = amsr::someip_daemon_client::external::osabstraction::Connection;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_ARACOM_IPC_CONNECTION_H_
