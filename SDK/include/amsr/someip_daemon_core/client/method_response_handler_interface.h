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
/**        \file  method_response_handler_interface.h
 *        \brief  Method Response handler Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_METHOD_RESPONSE_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_METHOD_RESPONSE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Represents a SOME/IP message.
 */
using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

/*!
 * \brief Interface for method response handler.
 */
class MethodResponseHandlerInterface {
 public:
  /*!
   * \brief   default constructor.
   * \steady  FALSE
   */
  MethodResponseHandlerInterface() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  virtual ~MethodResponseHandlerInterface() noexcept = default;

  MethodResponseHandlerInterface(MethodResponseHandlerInterface const &) = delete;
  MethodResponseHandlerInterface(MethodResponseHandlerInterface &&) = delete;
  MethodResponseHandlerInterface &operator=(MethodResponseHandlerInterface const &) & = delete;
  MethodResponseHandlerInterface &operator=(MethodResponseHandlerInterface &&) & = delete;

  /*!
   * \brief handle method responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void OnMethodResponse(someip_protocol::internal::InstanceId const instance_id,
                                client::Packet packet) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_METHOD_RESPONSE_HANDLER_INTERFACE_H_
