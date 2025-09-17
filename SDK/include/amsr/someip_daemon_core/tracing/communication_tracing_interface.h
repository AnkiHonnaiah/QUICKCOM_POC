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
/*!        \file  someip_daemon_core/tracing/communication_tracing_interface.h
 *        \brief  SOME/IP daemon specific interface for providing tracing services for communication messages.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_COMMUNICATION_TRACING_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_COMMUNICATION_TRACING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_daemon_core/tracing/application_tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/network_tracing_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace tracing {

/*!
 * \brief Communication tracing interface.
 */
class CommunicationTracingInterface : public ApplicationTracingInterface, public NetworkTracingInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady  FALSE
   */
  CommunicationTracingInterface() noexcept = default;

  /*!
   * \brief Define default destructor.
   *
   * \steady  FALSE
   */
  ~CommunicationTracingInterface() noexcept override = default;

  CommunicationTracingInterface(CommunicationTracingInterface const &) = delete;
  CommunicationTracingInterface(CommunicationTracingInterface &&) = delete;
  CommunicationTracingInterface &operator=(CommunicationTracingInterface const &) & = delete;
  CommunicationTracingInterface &operator=(CommunicationTracingInterface &&) & = delete;
};

}  // namespace tracing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_COMMUNICATION_TRACING_INTERFACE_H_
