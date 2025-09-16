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
/**        \file  amsr/someip_binding_core/internal/constants.h
 *        \brief  SOME/IP binding specific constants.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpBindingCore
 *
 *      \details  -
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONSTANTS_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <tuple>

#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief The maximum wait time to receive a response from daemon for a control command.
 */
using ResponseTimeout = std::chrono::steady_clock::duration;

/*!
 * \brief The default maximum wait time in milliseconds to receive a response from daemon.
 */
static constexpr ResponseTimeout kDefaultResponseTimeOut{std::chrono::milliseconds{10000U}};

/*!
 * \brief The default maximum wait time in milliseconds to establish a connection to the daemon.
 */
static constexpr ResponseTimeout kDefaultIpcConnectionTimeOut{std::chrono::milliseconds{10000U}};

/*!
 * \brief The default time period in nanoseconds to wait between different polls while receiving a response from
 * daemon.
 */
static constexpr ResponseTimeout kDefaultResponseTriggerPeriod{std::chrono::nanoseconds{100U}};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONSTANTS_H_
