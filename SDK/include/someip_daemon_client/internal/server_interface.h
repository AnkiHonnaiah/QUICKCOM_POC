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
/*!        \file  someip_daemon_client/internal/server_interface.h
 *        \brief  Declaration of handlers of ServerInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Interface class for callbacks from SomeIpDaemonClient to SomeipPosixServerManager. This must be realized by
 * SomeipPosixServerManager.
 */
class ServerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \vprivate  Product Private
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ServerInterface() = default;
  /*!
   * \brief Define destructor.
   * \vprivate  Product Private
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServerInterface() = default;
  /*!
   * \brief Delete copy constructor.
   */
  ServerInterface(ServerInterface const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  ServerInterface(ServerInterface&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(ServerInterface const&) & -> ServerInterface& = delete;
  /*!
   * \brief Delete move assignment.
   */
  auto operator=(ServerInterface&&) & -> ServerInterface& = delete;

  /*!
   * \brief                  Receive handler for SOME/IP requests.
   * \details                This API is called upon reception of a new method request.
   * \param[in] instance_id  The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in] packet       Method Request SOME/IP header + Payload
   * \pre                    -
   * \context                Reactor
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \vprivate               Product Private
   * \synchronous            TRUE
   * \steady                 TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTNEXTLINE (readability-avoid-const-params-in-decls)
  virtual void HandleReceive(amsr::someip_protocol::internal::InstanceId const instance_id,
                             amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SERVER_INTERFACE_H_
