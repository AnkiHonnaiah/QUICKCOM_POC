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
 *        \brief  Definition of enum ClientState.
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_STATE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_STATE_H_

#include <cstdint>

#include "ara/log/logstream.h"

#include "amsr/core/abort.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief States of a client.
 * \vprivate Product Private
 */
enum class ClientState : std::uint8_t {
  kConnecting = 0,
  kConnected = 1,
  kDisconnectedRemote = 2,
  kCorrupted = 3,
  kDisconnected = 4
};

/*!
 * \brief Stream a client state to a log stream.
 * \param stream Log stream to stream to.
 * \param client_state Client state to stream to the log stream.
 * \return The same log stream as \p stream.
 * \context ANY
 * \pre -
 *
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
inline auto operator<<(::ara::log::LogStream& stream, ClientState client_state) noexcept -> ::ara::log::LogStream& {
  char const* client_state_str{};
  switch (client_state) {
    case ClientState::kConnecting:
      client_state_str = "kConnecting";
      break;
    case ClientState::kConnected:
      client_state_str = "kConnected";
      break;
    case ClientState::kDisconnectedRemote:
      client_state_str = "kDisconnectedRemote";
      break;
    case ClientState::kCorrupted:
      client_state_str = "kCorrupted";
      break;
    case ClientState::kDisconnected:
      client_state_str = "kDisconnected";
      break;
    default:  // COV_ZEROCOPYCOMMON_INV_STATE
      ::amsr::core::Abort("Invalid client state.");
      break;
  }
  return stream << client_state_str;
}

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_ZEROCOPYCOMMON_INV_STATE
//  \ACCEPT XX
//  \REASON [COV_MSR_INV_STATE]
//
// COV_JUSTIFICATION_END

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_STATE_H_
