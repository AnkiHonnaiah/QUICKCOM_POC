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
 *         \brief Definition of types commonly used inside MemCon.
 *         \unit ZeroCopyCommon::MemCon
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_TYPES_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_TYPES_H_

#include <cstdint>
#include <memory>
#include <tuple>

#include "amsr/core/span.h"

#include "vac/language/unique_function.h"

#include "amsr/memory/memory_exchange_handle_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_interface.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_interface.h"
#include "amsr/memory/readable_memory_view.h"

#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_class.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"

#include "amsr/zero_copy_common/memcon/internal/client/client_state.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_technology.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_state.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {

/*!
 * \brief Type to describe a readable memory resource.
 */
struct ReadableMemoryResourceDescriptor {
  /*!
   * \brief The memory handle of the readable memory resource.
   */
  ::amsr::memory::MemoryHandle memory_handle{};
  /*!
   * \brief A view to the readable memory resource.
   */
  ::amsr::memory::ReadableMemoryView readable_memory_view{};
};

/*!
 * \brief Type to configure number of slots used for Zero-Copy.
 */
struct NumSlots {
  using Type = ::amsr::zero_copy_common::logic::internal::SlotHandle::SlotIndex;

  Type value;
};

/*!
 * \brief Type to configure the slot content size.
 */
struct SlotContentSize {
  using Type = std::size_t;

  Type value;
};

/*!
 * \brief Type to configure the slot content alignment.
 */
struct SlotContentAlignment {
  using Type = std::size_t;

  Type value;
};

/*!
 * \brief Type to configure the maximum number of receivers.
 */
struct MaxNumberReceivers {
  using Type = ::amsr::zero_copy_common::logic::internal::ReceiverHandle::ReceiverIndex;

  Type value;
};

/*!
 * \brief Type of a ClassHandle.
 */
using ClassHandle = ::amsr::zero_copy_common::logic::internal::ClassHandle;

/*!
 * \brief Type to specify the number of receiver classes.
 */
struct ReceiverClassNumber {
  using Type = ClassHandle::ReceiverClassIndex;

  Type value;
};

/*!
 * \brief Type to specify the limit of a receiver class.
 */
struct ReceiverClassLimit {
  using Type = ::amsr::zero_copy_common::logic::internal::ReceiverClass::ClassLimit;

  Type value;
};

/*!
 * \brief Type used for logic servers.
 */
using LogicServerPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ServerInterface>;

/*!
 * \brief Type to specify the memory technology used for Zero-Copy communication.
 */
using MemoryTechnology = ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryTechnology;

/*!
 * \brief Type used for the state of the client.
 */
using ClientState = ::amsr::zero_copy_common::memcon::internal::client::ClientState;

/*!
 * \brief Type used for the state of the server.
 */
using ServerState = ::amsr::zero_copy_common::memcon::internal::server::ServerState;

/*!
 * \brief Type used for the state of the receiver.
 */
using ReceiverState = ::amsr::zero_copy_common::memcon::internal::server::ReceiverState;

/*!
 * \brief Type to specify the memory offset in an allocated/mapped memory.
 */
struct MemoryOffset {
  using Type = std::size_t;

  Type value;
};

/*!
 * \brief Type to specify the memory size of an allocated/mapped memory.
 */
struct MemorySize {
  using Type = std::size_t;

  Type value;
};

/*!
 * \brief Type to locate a memory area by offset and size.
 */
struct MemoryAreaConfig {
  MemoryOffset offset;
  MemorySize size;
};

/*!
 * \brief       Compare two memory area configurations for equality.
 * \param       lhs Left memory area config instance.
 * \param       rhs Right memory area config instance.
 * \return      True, if lhs is element-wise equal to rhs. False, otherwise.
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 */
constexpr auto operator==(MemoryAreaConfig const& lhs, MemoryAreaConfig const& rhs) -> bool {
  return std::tie(lhs.offset.value, lhs.size.value) == std::tie(rhs.offset.value, rhs.size.value);
}

/*!
 * \brief Type of a complete queue memory configuration for exchange between server and client.
 */
struct QueueMemoryConfig {
  MemoryAreaConfig head_config;
  MemoryAreaConfig tail_config;
  MemoryAreaConfig buffer_config;
};

/*!
 * \brief       Compare two queue memory configurations for equality.
 * \param       lhs Left queue memory config instance.
 * \param       rhs Right queue memory config instance.
 * \return      True, if lhs is element-wise equal to rhs. False, otherwise.
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 */
constexpr auto operator==(QueueMemoryConfig const& lhs, QueueMemoryConfig const& rhs) -> bool {
  return std::tie(lhs.head_config, lhs.tail_config, lhs.buffer_config) ==
         std::tie(rhs.head_config, rhs.tail_config, rhs.buffer_config);
}

/*!
 * \brief Type of a slot memory configuration for exchange between server and client.
 */
struct SlotMemoryConfig {
  NumSlots number_slots;
  SlotContentSize slot_content_size;
  SlotContentAlignment slot_content_alignment;
};

/*!
 * \brief       Compare two slot memory configurations for equality.
 * \param       lhs Left slot memory config instance.
 * \param       rhs Right slot memory config instance.
 * \return      True, if lhs is element-wise equal to rhs. False, otherwise.
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 */
constexpr auto operator==(SlotMemoryConfig const& lhs, SlotMemoryConfig const& rhs) -> bool {
  return std::tie(lhs.number_slots.value, lhs.slot_content_size.value, lhs.slot_content_alignment.value) ==
         std::tie(rhs.number_slots.value, rhs.slot_content_size.value, rhs.slot_content_alignment.value);
}

/*!
 * \brief Type to configure the partitioning of one slot.
 */
struct OneSlotConfig {
  MemoryAreaConfig gvf_config;
  MemoryAreaConfig content_config;
};

/*!
 * \brief Type used for a memory exchange handle pointer.
 */
using MemoryExchangeHandlePtr = std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface>;

/*!
 * \brief Type used for read-writable memory.
 */
using ReadWritableMemoryPtr = std::unique_ptr<::amsr::memory::ReadWritableMemoryInterface>;

/*!
 * \brief Type used for readable memory.
 */
using ReadableMemoryPtr = std::unique_ptr<::amsr::memory::ReadableMemoryInterface>;

/*!
 * \brief Type used for a read-writable memory view.
 */
using ReadWritableMemoryView = ::amsr::memory::ReadWritableMemoryView;

/*!
 * \brief Type used for a readable memory view.
 */
using ReadableMemoryView = ::amsr::memory::ReadableMemoryView;

/*!
 * \brief Type used for the callback that is called each time an error on the side channel is encountered.
 * \param error_code Error code indicating the cause of the error.
 *                   Must be one of the following error codes:
 *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the peer
 *                       closed the connection without adhering to the protocol.
 *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the peer crashed.
 *                     - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If any protocol error is
 *                       noticed during the communication with the peer.
 *                     This is not checked.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnErrorCallback = ::vac::language::UniqueFunction<void(::amsr::core::ErrorCode error_code)>;

/*!
 * \brief Type used for the callback that is called each time a connection request is received.
 * \param slot_memory_config Configuration of the slot memory.
 * \param slot_memory_exchange_handle Memory exchange handle of the slot memory.
 *                                    Must not be a nullptr. This is not checked.
 * \param server_queue_memory_config Configuration of the server queue memory.
 * \param server_queue_memory_exchange_handle Memory exchange handle of the server queue memory.
 *                                            Must not be a nullptr. This is not checked.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnConnectionRequestCallback = ::vac::language::UniqueFunction<void(
    SlotMemoryConfig slot_memory_config, MemoryExchangeHandlePtr slot_memory_exchange_handle,
    QueueMemoryConfig queue_memory_config, MemoryExchangeHandlePtr queue_memory_exchange_handle)>;

/*!
 * \brief Type used for the callback that is called each time a connection ack is received.
 * \param queue_memory_config Configuration of the queue memory.
 * \param queue_memory_exchange_handle Memory exchange handle of the queue memory.
 *                                     Must not be a nullptr. This is not checked.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnAckConnectionCallback = ::vac::language::UniqueFunction<void(
    QueueMemoryConfig queue_memory_config, MemoryExchangeHandlePtr queue_memory_exchange_handle)>;

/*!
 * \brief Type used for the callback that is called each time a queue initialization ack is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnAckQueueInitializationCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for the callback that is called each time a notification is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnNotificationCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for the callback that is called each time a start listening is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnStartListeningCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for the callback that is called each time a stop listening is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnStopListeningCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for the callback that is called each time a shutdown is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnShutdownCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for the callback that is called each time a termination is received.
 * \context REACTOR
 * \pre -
 *
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
using OnTerminationCallback = ::vac::language::UniqueFunction<void()>;

/*!
 * \brief Type used for a side channel message containing a message view and a memory exchange handle.
 */
struct Message {
  ::amsr::core::Span<std::uint8_t const> message_view;
  MemoryExchangeHandlePtr memory_exchange_handle;
};

}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_TYPES_H_
