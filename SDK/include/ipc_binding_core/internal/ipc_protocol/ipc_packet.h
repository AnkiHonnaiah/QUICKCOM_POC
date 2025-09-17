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
/**        \file
 *        \brief  Defines the IPC packet.
 *        \unit   IpcBinding::IpcBindingCore::IpcPacket
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PACKET_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PACKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

#include "amsr/shared_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/span.h"
#include "ara/core/vector.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {

/*!
 * \brief   Represents an IPC message.
 * \details Attention: Packet heap memory is NOT pre-initialized with default values.
 */
// using PacketBuffer = ara::core::Vector<std::uint8_t, ara::core::PolymorphicAllocator<std::uint8_t>>;
using PacketBuffer =
    ara::core::Vector<std::uint8_t,
                      vac::memory::NonZeroingAllocator<std::uint8_t, ara::core::PolymorphicAllocator<std::uint8_t>>>;

/*!
 * \brief Represents a const span to an IPC message.
 */
using PacketSpan = ara::core::Span<PacketBuffer::value_type const>;

/*!
 * \brief Represents a shared pointer to a IPC Packet.
 */
using IpcPacketShared = amsr::SharedPtr<PacketBuffer>;

/*!
 * \brief   Construct IpcPacketShared.
 * \details Creates a PacketBuffer, common arguments are none, a size, or another PacketBuffer. Due to internal usage of
 *          the amsr::SharedPtr, the usage of magic numbers will be cause a narrowing error. Define a const of type
 *          PacketBuffer::size_type for the size.
 *
 * \tparam Args  Argument type list for constructing the managed object.
 *
 * \param[in] args  Arguments to be passed to the managed object's constructor.
 *
 * \return A IpcPacketShared.
 *
 * \pre        -
 * \context    ANY
 * \threadsafe TRUE
 * \reentrant  TRUE
 * \steady     TRUE
 *
 * \spec requires true; \endspec
 */
template <typename... Args>
auto IpcPacketSharedConstruct(Args&&... args) noexcept -> IpcPacketShared {
  PacketBuffer::allocator_type alloc{};
  return amsr::MakeShared<PacketBuffer>(std::forward<Args>(args)..., alloc).Value();
}

}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PACKET_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_FunctionCallCoverage_unused_utility_function
//   \ACCEPT  XX
//   \REASON  The API is existing to provide a complete basic utility class.
//            A coverage gap is reported for the API which is never directly used by other units.
//            The API is verified with unit tests. Therefore no further component or multi unit tests are necessary.
// COV_JUSTIFICATION_END
