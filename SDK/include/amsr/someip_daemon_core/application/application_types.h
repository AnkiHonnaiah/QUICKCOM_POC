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
/**        \file  application_types.h
 *        \brief  Application Types
 *
 *      \details  Common types used for "Application".
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_TYPES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/ipc/credentials.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/someip_daemon_core/client/local_client_interface.h"
#include "amsr/someip_daemon_core/configuration/types/local_client_id.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/server/local_server.h"
#include "amsr/unique_ptr.h"
#include "ara/core/error_code.h"
#include "ara/core/map.h"
#include "ara/core/string.h"
#include "someipd_app_protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief The connection state.
 */
enum class ConnectionState : std::uint8_t {
  /*!
   * \brief Connection not established.
   */
  kDisconnected,

  /*!
   * \brief Connection established.
   */
  kConnected
};
/*!
 * \brief A type alias for the IPC credentials.
 */
using Credentials = amsr::ipc::Credentials;

/*!
 * \brief A type alias for the IPC status code.
 */
using ErrorCode = ara::core::ErrorCode;

/*!
 * \brief A type alias for the IPC unicast address.
 */
using UnicastAddress = amsr::ipc::UnicastAddress;

/*!
 * \brief Type definition for "SpecificHeaderView" to be used for communication between "ApplicationConnection" and
 *        routing command controllers.
 */
using SpecificHeaderView = amsr::someipd_app_protocol::internal::SpecificHeaderView;

/*!
 * \brief The type of an operation's result.
 */
enum class OperationResult : std::uint32_t {
  /*!
   * \brief The operation executed successfully.
   */
  kOk = 0U,

  /*!
   * \brief Execution of the operation failed.
   */
  kFailure,

  /*!
   * \brief Execution of the operation failed due to a configuration issue.
   */
  kConfigurationFailure,

  /*!
   * \brief The application does not have the right to execute the operation.
   */
  kAccessDenied
};

/*!
 * \brief Type to use for unequivocal identification of a service instance.
 */
using LocalServerId = amsr::someip_daemon_core::configuration::types::SomeIpServiceInstanceId;

/*!
 * \brief Type alias for a map to store local server unique pointers.
 */
using LocalServerMap = ara::core::Map<LocalServerId, amsr::UniquePtr<server::LocalServerInterface>>;

/*!
 * \brief Type alias for a map to store local client unique pointers.
 */
using LocalClientMap =
    ara::core::Map<configuration::types::LocalClientId, amsr::UniquePtr<client::LocalClientInterface>>;

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_TYPES_H_
