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
/*!        \file   amsr/someip_binding_lite/internal/constants.h
 *         \brief  Internal (private) constants.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_CONSTANTS_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_CONSTANTS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_lite/data_types.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief Default maximum size of the IPC transmission queue.
 */
static constexpr IpcMaxQueueSize const kDefaultSomeIpDaemonIpcMaxQueueSize{1024u};

/*!
 * \brief Instance ID that represents all IDs.
 */
static constexpr std::uint16_t const kInstanceIdAll{0xffffu};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_CONSTANTS_H_
