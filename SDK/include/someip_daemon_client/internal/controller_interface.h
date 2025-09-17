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
/*!        \file  someip_daemon_client/internal/controller_interface.h
 *        \brief  Interface for Controller classes.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONTROLLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONTROLLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip_daemon_client/internal/common_types.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Interface class for controllers.
 */
class ControllerInterface {
 public:
  /*!
   * \brief Constructor.
   * \steady FALSE
   */
  ControllerInterface() noexcept = default;
  ControllerInterface(ControllerInterface const&) = delete;
  ControllerInterface(ControllerInterface&&) = delete;
  auto operator=(ControllerInterface const&) -> ControllerInterface& = delete;
  auto operator=(ControllerInterface&&) -> ControllerInterface& = delete;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  virtual ~ControllerInterface() noexcept = default;

  /*!
   * \brief     Set the value for controller promise.
   * \param[in] reception_buffer The message's reception buffer.
   * \param[in] control_header The control message header.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual void SetPromiseValue(ReceptionBuffer const& reception_buffer,
                               amsr::someipd_app_protocol::internal::ControlMessageHeader const& control_header) = 0;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONTROLLER_INTERFACE_H_
