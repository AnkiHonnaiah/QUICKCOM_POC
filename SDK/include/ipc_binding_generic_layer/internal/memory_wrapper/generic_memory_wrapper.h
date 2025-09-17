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
/*!        \file
 *        \brief  Contains an implementation for a generic memory wrapper.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_H_

#include <array>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/generic_memory_cache_interface.h"
#include "ara/core/span.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {

/*!
 * \brief Interface for GenericMemoryWrapper.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericMemoryWrapperInterface : public ::amsr::socal::internal::GenericMemoryWrapperInterface {
 public:
  /*!
   * \brief Type alias for IpcPacketShared.
   */
  using IpcPacketShared = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared;
  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  GenericMemoryWrapperInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  ~GenericMemoryWrapperInterface() noexcept override = default;

  GenericMemoryWrapperInterface(GenericMemoryWrapperInterface const&) = delete;
  GenericMemoryWrapperInterface(GenericMemoryWrapperInterface&&) = delete;
  auto operator=(GenericMemoryWrapperInterface const&) -> GenericMemoryWrapperInterface& = delete;
  auto operator=(GenericMemoryWrapperInterface&&) -> GenericMemoryWrapperInterface& = delete;

  /*!
   * \brief Stores the received packet.
   *
   * \param[in] packet          A packet to be stored in the memory wrapper.
   * \param[in] payload_offset  Index in the packet where the payload starts.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual auto StorePacket(IpcPacketShared&& packet, std::size_t payload_offset) noexcept -> void = 0;
};

/*!
 * \brief Wrapper object holding memory for a generic sample.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericMemoryWrapper final : public GenericMemoryWrapperInterface {
 public:
  /*!
   * \brief Type-alias for immutable buffer view.
   */
  using ImmutableBufferView = ::amsr::socal::GenericDataFragment;

  /*!
   * \brief Type-alias for container of immutable buffer view.
   */
  using ImmutableBufferViews = ::amsr::socal::GenericDataType;

  /*!
   * \brief Type-alias for mutable buffer view.
   */
  using MutableBufferView = ::ara::core::Span<uint8_t>;

  /*!
   * \brief Type-alias for an array containing one ImmutableBufferView.
   */
  using ImmutableBufferViewContainer = ::amsr::core::Array<ImmutableBufferView, 1>;

  /*!
   * \brief Type-alias for an array containing one MutableBufferView.
   */
  using MutableBufferViewContainer = ::amsr::core::Array<MutableBufferView, 1>;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */

  GenericMemoryWrapper() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~GenericMemoryWrapper() noexcept final = default;

  GenericMemoryWrapper(GenericMemoryWrapper&&) noexcept = delete;
  auto operator=(GenericMemoryWrapper&&) noexcept -> GenericMemoryWrapper& = delete;
  GenericMemoryWrapper(GenericMemoryWrapper const&) noexcept = delete;
  auto operator=(GenericMemoryWrapper const&) noexcept -> GenericMemoryWrapper& = delete;

  /*!
   * \brief Get the read-only view over the underlying data buffers.
   *
   * \return Read-only view over the underlying data buffers. The returned view must be valid until the lifetime of
   *         *this object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Return the immutable buffer view list containing single packet view.
   * \endinternal
   */
  auto GetData() const noexcept -> ImmutableBufferViews final { return ImmutableBufferViews{single_packet_view_}; }

  /*!
   * \copydoc amsr::ipc_binding_generic_layer::internal::GenericMemoryWrapperInterface::StorePacket
   *
   * \internal
   * - Validate the packet. If failed abort.
   * - Move the give packet into the generic memory wrapper.
   * - Create and store a immutable single packet buffer view over the payload.
   * \endinternal
   */
  void StorePacket(IpcPacketShared&& packet, std::size_t payload_offset) noexcept final {
    if (packet->size() < payload_offset) {
      ::amsr::ipc_binding_core::internal::logging::AraComLogger const logger{
          ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
          ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
          ::amsr::core::StringView{"GenericMemoryWrapper"}};

      logger.TerminateOnViolation("The packet length is shorter than expected",
                                  [&packet, &payload_offset](::ara::log::LogStream& s) {
                                    s << ", expected message length: " << payload_offset;
                                    std::size_t const packet_size{packet->size()};
                                    s << ", actual message length: " << packet_size;
                                  },
                                  {static_cast<char const*>(__func__), __LINE__});
    }

    packet_ = std::move(packet);

    ::amsr::ipc_binding_core::internal::ipc_protocol::PacketBuffer& packet_buffer{*packet_};
    ImmutableBufferView const immutable_buffer_view{&(packet_buffer[payload_offset]), packet_->size() - payload_offset};

    single_packet_view_[0] = immutable_buffer_view;
  }

 private:
  /*!
   * \brief Event notification packet.
   */
  IpcPacketShared packet_{};

  /*!
   * \brief Single buffer view on the packet_.
   */
  ImmutableBufferViewContainer single_packet_view_{};
};

}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_H_
