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
 *        \brief  Definition of enum ServerState.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_STATE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_STATE_H_

#include <cstdint>

#include "amsr/core/abort.h"

#include "ara/log/logstream.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief States of a server.
 * \vprivate Product Private
 */
enum class ServerState : std::uint8_t { kConnected = 0, kDisconnected = 1 };

/*!
 * \brief       Stream a server state to a log stream.
 * \param       stream       Log stream to stream to.
 * \param       server_state Server state to stream to the log stream.
 * \return      The same log stream as \p stream.
 * \context     ANY
 * \pre         -
 *
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
inline auto operator<<(::ara::log::LogStream& stream, ServerState const& server_state) noexcept
    -> ::ara::log::LogStream& {
  char const* server_state_str{};
  switch (server_state) {
    case ServerState::kConnected:
      server_state_str = "kConnected";
      break;
    case ServerState::kDisconnected:
      server_state_str = "kDisconnected";
      break;
    default:  // COV_ZEROCOPYCOMMON_INV_STATE
      ::amsr::core::Abort("Invalid server state.");
      break;
  }
  return stream << server_state_str;
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

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_STATE_H_
