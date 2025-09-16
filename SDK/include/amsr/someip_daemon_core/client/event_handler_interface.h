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
/**        \file    event_handler_interface.h
 *        \brief    Event Handler Interface for SOME/IP and PDU.
 *        \details  This common interface would be a temporary workaround until ApplicationClientHandler gets different
 *                  calls from the application for (SomeIp | PDU). Then we would directly call the specific APIs.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENT_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Interface for SOME/IP and PDU event handler.
 */
class EventHandlerInterface : public PduEventHandlerInterface, public SomeIpEventHandlerInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \steady  FALSE
   */
  EventHandlerInterface() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  virtual ~EventHandlerInterface() noexcept = default;

  EventHandlerInterface(EventHandlerInterface const &) = delete;
  EventHandlerInterface(EventHandlerInterface &&) = delete;
  EventHandlerInterface &operator=(EventHandlerInterface const &) & = delete;
  EventHandlerInterface &operator=(EventHandlerInterface &&) & = delete;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENT_HANDLER_INTERFACE_H_
