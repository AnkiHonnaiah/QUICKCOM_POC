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
 *        \brief  Definition of struct ServerCallbacks.
 *         \unit  ZeroCopyCommon::MemCon::SideChannel
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_CALLBACKS_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_CALLBACKS_H_

#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace side_channel {

/*!
 * \brief Struct containing server callbacks.
 * \vprivate Component Private
 */
struct ServerCallbacks {
  OnAckConnectionCallback on_ack_connection_cbk;
  OnStartListeningCallback on_start_listening_cbk;
  OnStopListeningCallback on_stop_listening_cbk;
  OnShutdownCallback on_shutdown_cbk;
};

}  // namespace side_channel
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SIDE_CHANNEL_SERVER_CALLBACKS_H_
