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
/*!        \file
 *         \brief  Collection of the types related to the Ipc service discovery used by the ZeroCopyBinding.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_TYPES_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_centralized.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"

namespace amsr {
namespace zero_copy_binding {
namespace common {
namespace internal {

namespace service_discovery {

/*!
 * \brief Type of the Ipc service discovery.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_ZEROCOPYBINDING_M3.4.1_typeNotDeclaredInsideFunction
using ServiceDiscovery = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized;

/*!
 * \brief Type of the interface of the Ipc service discovery.
 */
using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

/*!
 * \brief Type of the error code of the service discovery.
 */
using IpcServiceDiscoveryErrc = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryErrc;

/*!
 * \brief Identifier used to identify provided service instances uniquely within a binding type.
 */
using ProvidedServiceInstanceIdentifier = ::amsr::ipc_service_discovery::internal::ProvidedServiceInstanceIdentifier;

/*!
 * \brief Identifier used to identify required service instances uniquely within a binding type.
 */
using RequiredServiceInstanceIdentifier = ::amsr::ipc_service_discovery::internal::RequiredServiceInstanceIdentifier;

/*!
 * \brief Universal identifier that has to be specialized to a provided or required identifier before usage.
 */
using ServiceInstanceIdentifier = ::amsr::ipc_service_discovery::internal::ServiceInstanceIdentifier;

/*!
 * \brief State of a service instance for reporting if the instance is currently provided or not.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_ZEROCOPYBINDING_M3.4.1_typeNotDeclaredInsideFunction
using ProvidedState = ::amsr::ipc_service_discovery::internal::ProvidedState;

/*!
 * \brief Endpoint for a provided service instance.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_ZEROCOPYBINDING_M3.4.1_typeNotDeclaredInsideFunction
using ServiceInstanceEndpoint = ::amsr::ipc_service_discovery::internal::ServiceInstanceEndpoint;

/*!
 * \brief Unicast address used for connecting to an endpoint.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_ZEROCOPYBINDING_M3.4.1_typeNotDeclaredInsideFunction
using UnicastAddress = ::amsr::ipc_service_discovery::internal::UnicastAddress;

/*!
 * \brief Type of the binding using the service discovery.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_ZEROCOPYBINDING_M3.4.1_typeNotDeclaredInsideFunction
using BindingType = ::amsr::ipc_service_discovery::internal::BindingType;

/*!
 * \brief Callback function used to report ProvidedState updates for a listened service instance.
 */
using ListenServiceCallback = ::amsr::ipc_service_discovery::internal::ListenServiceCallback;

}  // namespace service_discovery

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_TYPES_H_
