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
/**        \file  someip_message_creator_interface.h
 *        \brief  SomeIp Message Creator Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {
namespace someip_message_creator {

/*!
 * \brief SomeIpMessage Creator Interface.
 */
class SomeIpMessageCreatorInterface {
 public:
  /*!
   * \brief Type alias for SomeIpMessage.
   */
  using SomeIpMessage = amsr::someip_protocol::internal::SomeIpMessage;

  /*!
   * \brief     Constructs the someip message creator interface.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  SomeIpMessageCreatorInterface() = default;
  /*!
   * \brief     Destroys the someip message creator interface.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~SomeIpMessageCreatorInterface() = default;

  SomeIpMessageCreatorInterface(SomeIpMessageCreatorInterface const &) = delete;
  SomeIpMessageCreatorInterface(SomeIpMessageCreatorInterface &&) = delete;
  SomeIpMessageCreatorInterface &operator=(SomeIpMessageCreatorInterface const &) & = delete;
  SomeIpMessageCreatorInterface &operator=(SomeIpMessageCreatorInterface &&) & = delete;

  /*!
   * \brief Named constructor of SomeIpMessage.
   *
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \return SomeIpMessage object containing the complete SOME/IP message,
   *        or an error if the buffer size is too small for the SOME/IP header.
   */
  virtual ara::core::Result<SomeIpMessage> CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer) const = 0;
};

}  // namespace someip_message_creator
}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_INTERFACE_H_
