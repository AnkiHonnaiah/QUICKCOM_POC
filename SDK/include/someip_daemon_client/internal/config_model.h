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
/*!        \file  someip_daemon_client/internal/config_model.h
 *        \brief  Relevant configuration parameters for SOME/IP binding-related layers of the middleware.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONFIG_MODEL_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONFIG_MODEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include "amsr/ipc/unicast_address.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Type-alias for the IPC maximum queue size configuration.
 */
using IpcMaxQueueSize = std::size_t;

/*!
 * \brief Default value for max allowed Ipc queue size.
 */
static constexpr std::size_t const kDefaultSomeipdIpcMaxQueueSize{0};

/*!
 * \brief Specific configuration for the usage of SOME/IP as one transport binding.
 */
struct SomeIpDaemonClientConfigModel final {
  /*!
   * \brief IPC domain for communication with SOME/IP daemon
   */
  amsr::ipc::Domain someipd_ipc_domain{};

  /*!
   * \brief IPC port for communication with SOME/IP daemon
   */
  amsr::ipc::Port someipd_ipc_port{};

  /*!
   * \brief Configurable max allowed queue size of the Ipc to prevent crashes.
   */
  IpcMaxQueueSize ipc_max_queue_size{kDefaultSomeipdIpcMaxQueueSize};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_CONFIG_MODEL_H_
