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
 *        \brief  Extension point defining factory function for actual MAC implementation.
 *
 *********************************************************************************************************************/
#ifndef LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_EXTENSION_POINT_H_
#define LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_EXTENSION_POINT_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"

#include "mac_interface.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace message_authentication {

/*!
 * \brief Factory method creating a new instance of the MAC interface implementation.
 * \details This function needs to be implemented by an actual MAC extension library.
 * \return A unique pointer to the newly created object.
 * \vprivate Product private.
 * \steady   FALSE
 */
auto Create() noexcept -> ara::core::Result<std::unique_ptr<MacInterface>>;

}  // namespace message_authentication
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_NULL_MESSAGE_AUTHENTICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_MESSAGE_AUTHENTICATION_EXTENSION_POINT_H_
