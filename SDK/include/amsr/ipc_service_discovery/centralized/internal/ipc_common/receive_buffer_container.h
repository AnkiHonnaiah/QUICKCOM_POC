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
/*! \file
 *  \brief A container holding buffers for receiving SafeIpc messages.
 *  \unit IpcServiceDiscovery::Centralized::IpcCommon
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_RECEIVE_BUFFER_CONTAINER_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_RECEIVE_BUFFER_CONTAINER_H_

#include "amsr/ipc_service_discovery/centralized/internal/types/buffer_types.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/message_buffer_type.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/abort_wrapper/abort.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief A container holding a message buffer and a mutable IO buffer for receiving SafeIpc messages.
 * \details This container holds a message buffer and an IO buffer which references the message buffer.
 *          The SafeIpc connection requires an IO buffer to receive messages, this buffer must exist
 *          until the reception process has been completed. Furthermore, the container provides
 *          getters with different views to the internal members.
 * \vprivate Component internal.
 */
class ReceiveBufferContainer {
 public:
  /*!
   * \brief Get a mutable IO buffer view to the internal message buffer.
   * \return A mutable IO buffer view to the internal message buffer.
   *         This view is only valid as long as this instance exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto GetMutableIOBufferView() noexcept -> MutableIOBufferView {
    return MutableIOBufferView{io_buffer_container_.data(), 1u};
  }

  /*!
   * \brief Get a const message view with a given size to the internal message buffer.
   * \param[in] message_size The size of the returned view. The given size must not exceed
   *                         the internal buffer size Messages::GetMaxSize().
   * \return A mutable IO buffer view to the internal message buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto GetConstBufferView(std::size_t message_size) noexcept -> ConstBufferView {
    if (message_size > message_buffer_.size()) {
      Abort("Message size exceeds buffer size.");  // COV_IpcSD_utility
    }
    return ConstBufferView{message_buffer_.data(), message_size};
  }

 private:
  /*! Memory buffer for incoming SafeIpc messages. */
  MessageBuffer message_buffer_{};
  /*! Container of mutable IO buffer with a pointer to a buffer and the size of the buffer. */
  MutableIOBufferContainer io_buffer_container_{{{message_buffer_.data(), message_buffer_.size()}}};
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_RECEIVE_BUFFER_CONTAINER_H_
