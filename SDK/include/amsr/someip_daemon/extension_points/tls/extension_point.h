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
/*!        \file  tls/extension_point.h
 *        \brief  Create API that return extensions handler.
 *         \unit  SomeIpDaemon::TLSExtensionsHandler::CustomAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_NULL_TLS_EXTENSIONS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_EXTENSION_POINT_H_
#define LIB_NULL_TLS_EXTENSIONS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_EXTENSION_POINT_H_

#include "amsr/someip_daemon/extension_points/tls/tls_extensions_handler_interface.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace tls {

/*!
 * \brief Create API of TLS extensions handler.
 * \details The functional implementation shall be done by a user defined library.
 * \return \p ara::core::Result with an empty container of TLS extensions handler or an error.
 * \trace SomeIpDaemon::TLSExtensionsHandler::CustomAPI
 * \steady FALSE
 */
ara::core::Result<ara::core::Vector<std::unique_ptr<TlsExtensionsHandlerInterface>>> Create() noexcept;

}  // namespace tls
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr
#endif  // LIB_NULL_TLS_EXTENSIONS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_EXTENSION_POINT_H_
