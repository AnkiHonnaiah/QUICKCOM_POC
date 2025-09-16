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
 *  \brief Indirection to access Reactor1Interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_REACTOR1_INTERFACE_H_
#define LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_REACTOR1_INTERFACE_H_

#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace ipc_service_discovery {
namespace external {
namespace internal {
namespace osabstraction {
/*!
 * \brief Indirection to access Reactor1Interface.
 */
using Reactor1Interface = ::osabstraction::io::reactor1::Reactor1Interface;

}  // namespace osabstraction
}  // namespace internal
}  // namespace external
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_EXTERNAL_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_EXTERNAL_INTERNAL_LIBOSABSTRACTION_REACTOR1_INTERFACE_H_
