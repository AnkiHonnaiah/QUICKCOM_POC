/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Declaration of the side channel parts of the Zero-Copy Common interface.
 *         \unit  ZeroCopyCommon::MemCon
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SIDE_CHANNEL_H_
#define LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SIDE_CHANNEL_H_

// TODO: unit

#include "amsr/zero_copy_common/memcon/internal/safe_ipc_side_channel/side_channel.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace internal {

/*!
 * \brief Interface of a side channel.
 */
using SideChannelInterface = memcon::internal::side_channel::SideChannelInterface;

/*!
 * \brief Implementation for the SideChannelInterface using SafeIPC.
 */
using SafeIPCSideChannel = memcon::internal::safe_ipc_side_channel::SideChannel;

}  // namespace internal
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SIDE_CHANNEL_H_
