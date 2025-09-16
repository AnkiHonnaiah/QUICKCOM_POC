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
/**        \file  local_server.h
 *        \brief  Wraps and handles memory ownership of LocalServerImpl for offered services.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/server/local_server_interface.h"
#include "amsr/someip_daemon_core/server/local_server_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief LocalServerImpl.
 * \details Implementation class for LocalServer.
 */
class LocalServer final : public LocalServerInterface {
 private:
  /*!
   * \brief Alias for SomeIpServiceInstanceId type
   */
  using SomeIpServiceInstanceId = configuration::types::SomeIpServiceInstanceId;

 public:
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Define destructor.
   * \steady FALSE
   */
  ~LocalServer() noexcept override;

  LocalServer() = delete;
  LocalServer(LocalServer const&) = delete;
  LocalServer(LocalServer&&) = delete;
  LocalServer& operator=(LocalServer const&) & = delete;
  LocalServer& operator=(LocalServer&&) & = delete;

  /*!
   * \brief Constructor of LocalServer.
   *
   * \param[in] service_instance_id    The service instance id.
   * \param[in] local_server_impl	   Unique pointer to this local server implementation.
   * \param[in] local_server_manager   Reference to the local server manager.
   *
   * \context Init
   * \steady FALSE
   */
  LocalServer(SomeIpServiceInstanceId const service_instance_id,
              amsr::UniquePtr<LocalServerInterface> local_server_impl,
              LocalServerManagerInterface& local_server_manager) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Offer the requested service for given service instance.
   *
   * \param[in] application The local application that offers the service.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \trace SPEC-8053469
   * \trace SPEC-4980348
   */
  void OfferService(LocalPacketSinkSharedPtr const& application) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Stop offer the service for given service instance.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  void StopOfferService() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local SOME/IP response message.
   *
   * \param[in] packet           A SOME/IP method response message.
   * \param[in] recipient        A remote source.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendMethodResponse(SomeIpPacket const& packet,
                          packet_router::RemoteSourceIdentifier const& recipient) const noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local SOME/IP event.
   *
   * \param[in] packet    A SOME/IP event message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendEvent(SomeIpPacket const& packet) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a local PDU event.
   *
   * \param[in] packet    A PDU event message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  void SendPduEvent(PduPacket const& packet) const noexcept override;

 private:
  /*!
   * \brief The service instance id
   */
  SomeIpServiceInstanceId const service_instance_id_;

  /*!
   * \brief A unique pointer to the implementation of this local server.
   */
  amsr::UniquePtr<LocalServerInterface> local_server_impl_;

  /*!
   * \brief A reference to the local server manager
   */
  LocalServerManagerInterface& local_server_manager_;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_H_
