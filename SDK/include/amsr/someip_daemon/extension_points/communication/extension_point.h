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
/*!        \file  communication/extension_point.h
 *        \brief  Create API that shall not return any extension point (null implementation).
 *         \unit  SomeIpDaemon::CommunicationExtensionPoints::CustomAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_NULL_COMMUNICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_EXTENSION_POINT_H_
#define LIB_NULL_COMMUNICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_EXTENSION_POINT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon/extension_points/communication/communication_extension_point_interface.h"
#include "amsr/someip_daemon_lite/someip_daemon_lite.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace communication {

/*!
 * \brief Create API of communication extension points.
 * \vpublic
 * \details The functional implementation shall be done by a user defined library.
 *
 * \param[in] lite_api Reference to a class instance exposing lite API to access the daemon's core.
 *
 * \return \p ara::core::Result with an empty container of communication extension points or an error.
 *
 * \error * No error is returned from the default implementation.
 *
 * \trace SomeIpDaemon::CommunicationExtensionPoints::CustomAPI
 * \steady FALSE
 */
ara::core::Result<ara::core::Vector<std::unique_ptr<CommunicationExtensionPointInterface>>> Create(
    amsr::someip_daemon_lite::SomeIpDaemonLite const &lite_api) noexcept;

}  // namespace communication
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_NULL_COMMUNICATION_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_EXTENSION_POINT_H_
