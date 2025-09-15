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
/**        \file  local_server_manager.h
 *        \brief  Local Server Manager used to manage creation of all LocalServers, erasing, and looking up.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/server/local_server_error.h"
#include "amsr/someip_daemon_core/server/local_server_interface.h"
#include "amsr/someip_daemon_core/server/local_server_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief This class responsible for managing local server implementation's lifecycle
 */
class LocalServerManager final : public LocalServerManagerInterface {
 private:
  /*!
   * \brief Alias for SomeIpServiceInstanceId type
   */
  using SomeIpServiceInstanceId = configuration::types::SomeIpServiceInstanceId;

 public:
  /*!
   * \brief Unique pointer to a LocalServerInterface.
   */
  using LocalServerUniquePtr = amsr::UniquePtr<server::LocalServerInterface>;

  /*!
   * \brief       Constructor.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  LocalServerManager() = default;

  /*!
   * \brief  Destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~LocalServerManager() noexcept override;

  LocalServerManager(LocalServerManager const&) = delete;
  LocalServerManager(LocalServerManager&&) = delete;
  LocalServerManager& operator=(LocalServerManager const&) & = delete;
  LocalServerManager& operator=(LocalServerManager&&) & = delete;

  /*!
   * \brief       Get a local server by service ID, instance ID, major version, minor version.
   *
   * \param[in]   service_instance_id   The service instance id.
   *
   * \return      The local server or nullptr if not found.
   * \error       amsr::someip_daemon_core::server::LocalServerOperationError::kServiceNotConfigured if the
   *              service instance id is not configured
   * \error       amsr::someip_daemon_core::server::LocalServerOperationError::kServiceAlreadyRequested if the
   *              service instance id is already requested
   * \pre         -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<LocalServerUniquePtr> RequestLocalServer(
      SomeIpServiceInstanceId const service_instance_id) noexcept;

  /*!
   * \brief       Emplace the local server into the LocalServerMap.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   local_server          The local Server.
   *
   * \return      True if emplace succeeded, false if emplace failed
   * due to already-existing local server with same service instance id.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool EmplaceLocalServer(SomeIpServiceInstanceId const service_instance_id,
                          LocalServerUniquePtr local_server) noexcept;

  /*!
   * \brief Returns ownership of a local server impl to the local server manager.
   *
   * \param[in] service_instance_id    The service instance id.
   * \param[in] local_server_impl      Implementation object to return memory ownership to the manager.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void Release(SomeIpServiceInstanceId const service_instance_id,
               LocalServerUniquePtr local_server_impl) noexcept override;

 private:
  /*!
   * \brief Map of local servers.
   */
  using LocalServerMap = ara::core::Map<SomeIpServiceInstanceId, LocalServerUniquePtr>;

  /*!
   * \brief The local servers.
   */
  LocalServerMap local_servers_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    ara::core::StringView{"LocalServerManager"}};
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_H_
