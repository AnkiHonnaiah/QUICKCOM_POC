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
 *        \brief  Definition of enum ReceiverState.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_STATE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_STATE_H_

#include <cstdint>

#include "amsr/core/abort.h"

#include "ara/log/logstream.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief States of a registered receiver.
 * \vprivate Product Private
 */
enum class ReceiverState : std::uint8_t { kConnecting = 0, kConnected = 1, kDisconnected = 2, kCorrupted = 3 };

/*!
 * \brief       Stream a receiver state to a log stream.
 * \param       stream         Log stream to stream to.
 * \param       receiver_state Receiver state to stream to the log stream.
 * \return      The same log stream as \p stream.
 * \context     ANY
 * \pre         -
 *
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
inline auto operator<<(::ara::log::LogStream& stream, ReceiverState const& receiver_state) noexcept
    -> ::ara::log::LogStream& {
  char const* receiver_state_str{};
  switch (receiver_state) {
    case ReceiverState::kConnecting:
      receiver_state_str = "kConnecting";
      break;
    case ReceiverState::kConnected:
      receiver_state_str = "kConnected";
      break;
    case ReceiverState::kDisconnected:
      receiver_state_str = "kDisconnected";
      break;
    case ReceiverState::kCorrupted:
      receiver_state_str = "kCorrupted";
      break;
    default:  // COV_ZEROCOPYCOMMON_INV_STATE
      ::amsr::core::Abort("Invalid receiver state.");
      break;
  }
  return stream << receiver_state_str;
}

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_ZEROCOPYCOMMON_INV_STATE
//  \ACCEPT XX
//  \REASON [COV_MSR_INV_STATE]
//
// COV_JUSTIFICATION_END

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_STATE_H_
