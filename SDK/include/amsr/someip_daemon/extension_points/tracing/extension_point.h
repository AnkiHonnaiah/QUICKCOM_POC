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
/*!        \file  tracing/extension_point.h
 *        \brief  Create API that returns an implementation of the tracing interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_EXTENSION_POINT_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_EXTENSION_POINT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon/extension_points/tracing/tracing_interface.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace tracing {

/*!
 * \brief   Create API of the tracing implementation.
 *
 * \return  \p a unique pointer to the implementation of the tracing interface.
 * \steady  FALSE
 */
auto Create() noexcept -> std::unique_ptr<TracingInterface>;

}  // namespace tracing
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_EXTENSION_POINT_H_
