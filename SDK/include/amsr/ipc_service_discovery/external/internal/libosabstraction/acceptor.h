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
 *  \brief Indirection to access Acceptor.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_ACCEPTOR_H_
#define LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_ACCEPTOR_H_

#include "amsr/ipc/acceptor.h"

namespace amsr {
namespace ipc_service_discovery {
namespace external {
namespace internal {
namespace osabstraction {
/*!
 * \brief Indirection to access Acceptor.
 */
using Acceptor = ::amsr::ipc::Acceptor;

}  // namespace osabstraction
}  // namespace internal
}  // namespace external
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_ACCEPTOR_H_
