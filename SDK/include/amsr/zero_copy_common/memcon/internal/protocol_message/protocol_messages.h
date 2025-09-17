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
/*!       \file
 *        \brief Utilities for ProtocolMessage usage.
 *        \unit ZeroCopyCommon::MemCon::ProtocolMessage
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGES_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGES_H_

#include <array>
#include <cstdint>

#include "amsr/zero_copy_common/memcon/internal/protocol_message/ack_connection.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/ack_queue_initialization.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/connection_request_queue_memory.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/connection_request_slot_memory.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/shutdown.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/start_listening.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/stop_listening.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/termination.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace protocol_message {

/*!
 * \brief Provides utilities for usage of messages.
 * \vprivate Component internal.
 */
class ProtocolMessages {
 public:
  /*!
   * \brief Number of protocol messages.
   */
  static constexpr std::size_t kNumberOfMessages{8};

 private:
  /*!
   * \brief Type used for storing sizes of all protocol messages.
   */
  using MessageSizes = std::array<std::size_t, kNumberOfMessages>;

 public:
  /*!
   * \brief Get the maximum size of all protocol messages in bytes.
   * \return The maximum protocol message size.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   *
   * \internal
   * - Create an array of sizes of all protocol messages.
   * - Retrieve and return the maximum protocol message size.
   * \endinternal
   */
  static constexpr auto GetMaxSize() noexcept -> std::size_t {
    MessageSizes const message_sizes{ConnectionRequestSlotMemory::GetSize(),
                                     ConnectionRequestQueueMemory::GetSize(),
                                     AckConnection::GetSize(),
                                     AckQueueInitialization::GetSize(),
                                     StartListening::GetSize(),
                                     StopListening::GetSize(),
                                     Shutdown::GetSize(),
                                     Termination::GetSize()};

    return GetMaxSize(message_sizes);
  }

 private:
  /*!
   * \brief Get the maximum size of given protocol message sizes.
   * \param message_sizes An array (with maximum size of kNumberOfMessages) of protocol message sizes from which the
   *                      maximum value is calculated.
   * \return The maximum protocol message size.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   *
   * \internal
   * - Iterate over all message sizes and return the maximum size.
   * \endinternal
   */
  static constexpr auto GetMaxSize(MessageSizes const& message_sizes) noexcept -> std::size_t {
    std::size_t max_message_size{0};

    // Range based for loop can not be used, because iterator of std::array is not constexpr.
    // VECTOR NC AutosarC++17_10-A6.5.1: MD_ZEROCOPYCOMMON_A6.5.1_false_positive
    for (std::size_t i{0}; i < message_sizes.size(); i++) {
      std::size_t const current_message_size{message_sizes[i]};
      if (current_message_size > max_message_size) {
        max_message_size = current_message_size;
      }
    }
    return max_message_size;
  }
};

}  // namespace protocol_message
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGES_H_
