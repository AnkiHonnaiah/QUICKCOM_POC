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
/*!        \file  application_client_handler_interface.h
 *        \brief  Application Client Handler Interface
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/application/application_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Handles the server interface of a local application.
 */
class ApplicationClientHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   *
   * \pre -
   * \context App
   * \steady  FALSE
   */
  ApplicationClientHandlerInterface() = default;

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
  virtual ~ApplicationClientHandlerInterface() = default;

  ApplicationClientHandlerInterface(ApplicationClientHandlerInterface const&) = delete;
  ApplicationClientHandlerInterface(ApplicationClientHandlerInterface const&&) = delete;
  auto operator=(ApplicationClientHandlerInterface const&) -> ApplicationClientHandlerInterface& = delete;
  auto operator=(ApplicationClientHandlerInterface const&&) -> ApplicationClientHandlerInterface& = delete;

  /*!
   * \brief Called when the connected application requests the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return the newly allocated SOME/IP client identifier if the operation is successful and error otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<someip_protocol::internal::ClientId> RequestService(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept = 0;
  /*!
   * \brief Called when the connected application request a start of service discovery for the given service instance.
   *
   * \param[in] service_id          The service instance id.

   *
   * \return "true" if the operation is successful and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   */
  virtual bool StartServiceDiscovery(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept = 0;

  /*!
   * \brief Called when the connected application request to stop service discovery for the given service instance.
   *
   * \param[in] service_instance_id The service instance id.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual bool StopServiceDiscovery(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept = 0;
  /*!
   * \brief Request an event subscription as a connected application.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id            A SOME/IP event identifier.
   * \param[in] client_id           The subscriber unique client identifier.
   *
   * \return "OperationResult::kOk" if the service is successfully offered, an error code otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual OperationResult SubscribeEvent(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                         someip_protocol::internal::EventId const event_id,
                                         someip_protocol::internal::ClientId const client_id) noexcept = 0;

  /*!
   * \brief Cancel an event subscription as a connected application.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id            A SOME/IP event identifier.
   * \param[in] client_id           The subscriber unique client identifier.
   *
   * \return "OperationResult::kOk" if the event is successfully unsubscribe, an error code otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual OperationResult UnsubscribeEvent(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                           someip_protocol::internal::EventId const event_id,
                                           someip_protocol::internal::ClientId const client_id) noexcept = 0;

  /*!
   * \brief Releases a previously allocated SOME/IP client identifier.
   *
   * \param[in] service_instance_id A SOME/IP service identifier.
   * \param[in] client_id           A SOME/IP client identifier.
   *
   * \return "true" if the operation is successful and "false" otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual bool ReleaseService(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                              someip_protocol::internal::ClientId const client_id) noexcept = 0;
};
}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_CLIENT_HANDLER_INTERFACE_H_
