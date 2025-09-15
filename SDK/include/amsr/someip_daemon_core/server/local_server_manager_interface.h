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
/**        \file  local_server_manager_interface.h
 *        \brief  Local Server Manager Interface.
 *
 *      \details  Provides Release API to return memory ownership of a local server implementation
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/server/local_server_interface.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief Interface for a local server manager.
 */
class LocalServerManagerInterface {
 public:
  /*!
   * \brief Delete default constructor.
   * \steady FALSE
   */
  LocalServerManagerInterface() = default;

  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  virtual ~LocalServerManagerInterface() noexcept = default;

  LocalServerManagerInterface(LocalServerManagerInterface const&) = delete;
  LocalServerManagerInterface(LocalServerManagerInterface&&) = delete;
  LocalServerManagerInterface& operator=(LocalServerManagerInterface const&) & = delete;
  LocalServerManagerInterface& operator=(LocalServerManagerInterface&&) & = delete;

  /*!
   * \brief Alias for SomeIpServiceInstanceId type
   */
  using SomeIpServiceInstanceId = configuration::types::SomeIpServiceInstanceId;

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
  virtual void Release(SomeIpServiceInstanceId const service_instance_id,
                       amsr::UniquePtr<LocalServerInterface> local_server_impl) noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_MANAGER_INTERFACE_H_
