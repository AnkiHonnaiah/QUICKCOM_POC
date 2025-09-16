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
/*!        \file  application_server_handler_interface.h
 *        \brief Application Server Handler Interface
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Handles the server interface of a local application.
 */
class ApplicationServerHandlerInterface {
 public:
  /*!
   * \brief The return type of 'RequestLocalServer' operation.
   */
  struct RequestLocalServerOperationResult {
    /*!
     * \brief Result of the operation.
     */
    OperationResult result_{};
  };

  /*!
   * \brief Constructor.
   *
   * \pre -
   * \context App
   * \steady  FALSE
   */
  ApplicationServerHandlerInterface() = default;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Destructor.
   *
   * \pre -
   * \context Shutdown, Timer
   * \steady  FALSE
   */
  virtual ~ApplicationServerHandlerInterface() = default;

  ApplicationServerHandlerInterface(ApplicationServerHandlerInterface const&) = delete;
  ApplicationServerHandlerInterface(ApplicationServerHandlerInterface const&&) = delete;
  auto operator=(ApplicationServerHandlerInterface const&) -> ApplicationServerHandlerInterface& = delete;
  auto operator=(ApplicationServerHandlerInterface const&&) -> ApplicationServerHandlerInterface& = delete;

  /*!
   * \brief     Called when the connected application requests a local server.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return    The operation's result.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual RequestLocalServerOperationResult RequestLocalServer(
      configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept = 0;

  /*!
   * \brief     Called when the connected application releases a local server.
   *
   * \param[in] service_instance_id The service instance id.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void ReleaseLocalServer(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept = 0;

  /*!
   * \brief     Called when the connected application offers the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OfferService(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept = 0;

  /*!
   * \brief     Called when the connected application stops offering the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void StopOfferService(configuration::types::SomeIpServiceInstanceId const service_instance_id) noexcept = 0;
};
}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_SERVER_HANDLER_INTERFACE_H_
