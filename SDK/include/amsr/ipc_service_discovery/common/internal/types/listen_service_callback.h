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
/*! \file
 *  \brief Public common ListenServiceCallback type.
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_LISTEN_SERVICE_CALLBACK_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_LISTEN_SERVICE_CALLBACK_H_

#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/types/provided_state.h"
#include "amsr/ipc_service_discovery/common/internal/types/service_instance_endpoint.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Interface of the callback function that indicates that a listened service instance changed its state.
 * \details It is not allowed to call any API of the IpcServiceDiscovery within this callback.
 * \param[in] required_service_instance_identifier The required service instance.
 * \param[in] service_instance_endpoint Endpoint of the provided service instance.
 * \param[in] provided_state The state update of the provided service instance.
 * \context REACTOR
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product Private
 */
using ListenServiceCallback = vac::language::UniqueFunction<void(
    RequiredServiceInstanceIdentifier required_service_instance_identifier,
    ServiceInstanceEndpoint service_instance_endpoint, ProvidedState provided_state)>;

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_LISTEN_SERVICE_CALLBACK_H_
