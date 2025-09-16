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
/**        \file  local_client_id.h
 *        \brief  Unique identifier for the local client (ServiceId, MajorVersion, InstanceId, ClientId).
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_LOCAL_CLIENT_ID_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_LOCAL_CLIENT_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {
/*!
 * \brief This is a unique identifier for the local client.
 */
struct LocalClientId {
  /*!
   * \brief The SOME/IP service instance ID.
   */
  configuration::types::SomeIpServiceInstanceId service_instance_id{};
  /*!
   * \brief The SOME/IP client id.
   */
  ::amsr::someip_protocol::internal::ClientId const client_id{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Defines a strict weak ordering relationship for LocalClientId keys, used for comparing client Ids inside
   * local clients map for find().
   * \param[in]   other A key to compare against.
   * \return      True if this key is less than the other one, false otherwise.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool operator<(LocalClientId const& other) const noexcept {
    return std::make_tuple(service_instance_id, client_id) <
           std::make_tuple(other.service_instance_id, other.client_id);
  }
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_LOCAL_CLIENT_ID_H_
