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
/**        \file  someip_message_creator.h
 *        \brief  SomeIp Message Creator.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/packet_router/someip_message_creator/someip_message_creator_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {
namespace someip_message_creator {

/*!
 * \brief SomeIpMessage Creator.
 */
class SomeIpMessageCreator : public SomeIpMessageCreatorInterface {
 public:
  /*!
   * \brief     Default constructor.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  SomeIpMessageCreator() = default;

  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~SomeIpMessageCreator() override = default;

  SomeIpMessageCreator(SomeIpMessageCreator const&) = delete;
  SomeIpMessageCreator(SomeIpMessageCreator&&) = delete;
  SomeIpMessageCreator& operator=(SomeIpMessageCreator const&) & = delete;
  SomeIpMessageCreator& operator=(SomeIpMessageCreator&&) & = delete;

  /*!
   * \brief Named constructor of SomeIpMessage.
   *
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \return SomeIpMessage object containing the complete SOME/IP message,
   *        or an error if the buffer size is too small for the SOME/IP header.
   */
  ara::core::Result<SomeIpMessage> CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer) const override;
};

}  // namespace someip_message_creator
}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SOMEIP_MESSAGE_CREATOR_SOMEIP_MESSAGE_CREATOR_H_
