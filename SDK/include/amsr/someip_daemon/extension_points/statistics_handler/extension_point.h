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
/*!        \file  extension_point.h
 *        \brief  Declaring the extension point for the statistics handler.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_EXTENSION_POINT_H_
#define LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_EXTENSION_POINT_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "statistics_handler_interface.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace statistics_handler {

/*!
 * \brief Factory method creating a new instance of the statistics handler interface implementation.
 * \details This function needs to be implemented by an actual statistics handler library.
 * \return A unique pointer to the newly created object.
 * \vprivate Product private.
 * \steady   FALSE
 */
auto Create() noexcept -> ara::core::Result<std::unique_ptr<StatisticsHandlerInterface>>;

}  // namespace statistics_handler
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_EXTENSION_POINT_H_
