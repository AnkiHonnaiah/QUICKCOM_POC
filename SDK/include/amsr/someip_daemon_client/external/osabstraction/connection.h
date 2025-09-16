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
/*!        \file  connection.h
 *        \brief  Indirection to access Connection.
 *      \details  Indirections facilitate mocking of external libraries for testing purposes.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_OSABSTRACTION_CONNECTION_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_OSABSTRACTION_CONNECTION_H_

#include "amsr/ipc/connection.h"

namespace amsr {
namespace someip_daemon_client {
namespace external {
namespace osabstraction {
/*!
 * \brief Indirection to access Connection.
 */
using Connection = ::amsr::ipc::Connection;
}  // namespace osabstraction
}  // namespace external
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_OSABSTRACTION_CONNECTION_H_
