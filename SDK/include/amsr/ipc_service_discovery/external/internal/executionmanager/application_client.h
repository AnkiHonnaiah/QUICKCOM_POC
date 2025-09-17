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
 *  \brief Indirection to access ApplicationClient.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_EXECUTIONMANAGER_APPLICATION_CLIENT_H_
#define LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_EXECUTIONMANAGER_APPLICATION_CLIENT_H_

#include "ara/exec/application_client.h"

namespace amsr {
namespace ipc_service_discovery {
namespace external {
namespace internal {
namespace executionmanager {

/*!
 * \brief Indirection to access ApplicationClient.
 */
using ApplicationClient = ara::exec::ApplicationClient;

}  // namespace executionmanager
}  // namespace internal
}  // namespace external
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_EXECUTIONMANAGER_APPLICATION_CLIENT_H_
