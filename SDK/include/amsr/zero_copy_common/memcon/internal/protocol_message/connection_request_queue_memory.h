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
 *        \brief ConnectionRequestQueueMemory message.
 *        \unit ZeroCopyCommon::MemCon::ProtocolMessage
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_CONNECTION_REQUEST_QUEUE_MEMORY_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_CONNECTION_REQUEST_QUEUE_MEMORY_H_

#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace protocol_message {

/*!
 * \brief ConnectionRequestQueueMemory message.
 * \vprivate Component internal.
 */
class ConnectionRequestQueueMemory : public ProtocolMessage<ConnectionRequestQueueMemory> {
 public:
  /*!
   * \brief Queue memory configuration.
   */
  // VECTOR NL VectorC++-V11.0.2: MD_ZEROCOPYCOMMON_V11.0.2_InAClassAllMutableDataMembersShallBePrivate
  QueueMemoryConfig queue_memory_config;

  /*!
   * \brief Create with default values.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr ConnectionRequestQueueMemory() noexcept : ConnectionRequestQueueMemory{QueueMemoryConfig{}} {};

  /*!
   * \brief Create with explicit values.
   * \param[in] queue_memory_config_p Queue memory configuration.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  explicit constexpr ConnectionRequestQueueMemory(QueueMemoryConfig queue_memory_config_p) noexcept
      : ProtocolMessage{ProtocolMessage::Id::kConnectionRequestQueueMemory},
        queue_memory_config{queue_memory_config_p} {}

  /*!
   * \brief Compare messages against another.
   * \param[in] other ProtocolMessage to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(ConnectionRequestQueueMemory const& other) const noexcept -> bool {
    return (queue_memory_config == other.queue_memory_config);
  }
};

}  // namespace protocol_message
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_CONNECTION_REQUEST_QUEUE_MEMORY_H_
