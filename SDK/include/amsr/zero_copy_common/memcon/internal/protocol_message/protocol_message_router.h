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
 *        \brief Parse buffer view and convert it to a typed protocol message.
 *        \unit ZeroCopyCommon::MemCon::ProtocolMessage
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_ROUTER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_ROUTER_H_

#include <cstddef>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/memory/memory_exchange_handle_interface.h"

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message_types.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_messages.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace protocol_message {

/*!
 * \brief Parse a byte stream containing the message and convert it to the typed message.
 * \details The parser converts a byte stream, received over a side channel connection, to a typed message.
 *          For every message an abstract method must be implemented to handle the received message.
 *          The received buffer view is analyzed, verified and finally copied to the typed message.
 *          The typed message is forwarded to the user with the help of the abstract methods.
 * \trace   DSGN-MemCon-SideChannelProtocol
 * \vprivate
 */
class ProtocolMessageRouter {
 public:
  /*!
   * \brief Type used for ZeroCopyCommon error code.
   */
  using Errc = ::amsr::zero_copy_common::common::ZeroCopyCommonErrc;

  /*!
   * \brief Byte position of the message id inside the buffer view.
   */
  static constexpr std::size_t kIdPosition{0};

 protected:
  /*!
   * \brief Parse the given buffer view and convert it to a typed protocol message.
   * \details The given buffer will be parsed and converted to a typed protocol message. The abstract method for
   *          this typed protocol message will be called and the protocol message is forwarded to the user.
   * \param[in] buffer_view            A const buffer view to the protocol message buffer to be parsed.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle possibly sent with the message
   *                                   provided to be parsed.
   * \return A Result object of void type.
   * \error ZeroCopyCommonErrc::kMessageCorrupted Received message is corrupted. Message id is out of bounce.
   *
   * \context ANY
   * \pre The buffer must contain a valid message.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Parse(ConstMessageConstView buffer_view,
             std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Create a default ProtocolMessageRouter.
   * \vprivate Component Private
   */
  ProtocolMessageRouter() noexcept = default;

  /*!
   * \brief Destroy a ProtocolMessageRouter.
   * \vprivate Component Private
   */
  ~ProtocolMessageRouter() noexcept = default;

  /*!
   * \brief Create a copy of a ProtocolMessageRouter.
   * \vprivate Component Private
   */
  ProtocolMessageRouter(ProtocolMessageRouter const&) = default;

  /*!
   * \brief Move a ProtocolMessageRouter into a new instance.
   * \vprivate Component Private
   */
  ProtocolMessageRouter(ProtocolMessageRouter&&) noexcept = default;

  /*!
   * \brief Copy a different ProtocolMessageRouter into this instance.
   * \vprivate Component Private
   */
  auto operator=(ProtocolMessageRouter const&) & noexcept -> ProtocolMessageRouter& = default;

  /*!
   * \brief Move a different ProtocolMessageRouter into this instance.
   * \vprivate Component Private
   */
  auto operator=(ProtocolMessageRouter&&) & noexcept -> ProtocolMessageRouter& = default;

 private:
  /*!
   * \brief Called if received protocol message is an AckConnection message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to memory exchange handle interface.
   *                                   Must point to a valid memory else registered error callback function is called.
   * \error Previously registered error callback with an error code is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an erroneous AckConnection message is received,
   *                                                      a server intended AckConnection is received by the
   *                                                      client or when the a memory exchange handle is missing.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnAckConnection(
      ::amsr::core::Result<AckConnection> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an AckQueueInitialization message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle must not be sent with the
   *                                   AckQueueInitialization message. Must be a nullptr, else it is considered to be a
   *                                   protocol error and a registered error callback is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an erroneous AckQueueInitialization message is
   *                                                      received, when client intended AckQueueInitialization
   *                                                      is received by the server, when AckQueueInitialization
   *                                                      is received without connection request messages or when
   *                                                      AckQueueInitialization received with a memory exchange handle.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_ZEROCOPYCOMMON_V8.3.1_defaultParameterInVirtualMethod
  virtual void OnAckQueueInitialization(
      ::amsr::core::Result<AckQueueInitialization> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an ConnectionRequestQueueMemory message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to memory exchange handle interface.
   *                                   Must point to a valid memory else registered error callback function is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When a ConnectionRequestQueueMemory message is received
   *                                                      without a previous ConnectionRequestSlotMemory message or
   *                                                      when an erroneous ConnectionRequestQueueMemory message is
   *                                                      received, i.e. with a corrupted size or without a memory
   *                                                      exchange handle, or when a client intended
   *                                                      ConnectionRequestQueueMemory is received by the server.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnConnectionRequestQueueMemory(
      ::amsr::core::Result<ConnectionRequestQueueMemory> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an ConnectionRequestSlotMemory message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to memory exchange handle interface.
   *                                   Must point to a valid memory else registered error callback function is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an erroneous ConnectionRequestSlotMemory message is
   *                                                      received, i.e. with a corrupted size or without a memory
   *                                                      exchange handle, or when client intended
   *                                                      OnConnectionRequestSlotMemory is received by the server.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnConnectionRequestSlotMemory(
      ::amsr::core::Result<ConnectionRequestSlotMemory> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an StartListening message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle must not be sent with the
   *                                   StartListening message. Must be a nullptr, else it is considered to be a
   *                                   protocol error and a registered error callback is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an Erroneous StartListening message is received, i.e.
   *                                                      with a corrupted size or with a memory exchange handle, or
   *                                                      when a server intended StartListening message is received by
   *                                                      the client.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_ZEROCOPYCOMMON_V8.3.1_defaultParameterInVirtualMethod
  virtual void OnStartListening(
      ::amsr::core::Result<StartListening> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an StopListening message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle must not be sent with the
   *                                   StopListening message. Must be a nullptr, else it is considered to be a
   *                                   protocol error and a registered error callback is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an Erroneous StopListening message is received, i.e.
   *                                                      with a corrupted size or with a memory exchange handle, or
   *                                                      when a server intended StopListening message is received by
   *                                                      the client.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_ZEROCOPYCOMMON_V8.3.1_defaultParameterInVirtualMethod
  virtual void OnStopListening(
      ::amsr::core::Result<StopListening> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an Shutdown message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle must not be sent with the
   *                                   Shutdown message. Must be a nullptr, else it is considered to be a
   *                                   protocol error and a registered error callback is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an Erroneous Shutdown message is received i.e. with a
   *                                                      corrupted size or with a memory exchange handle,.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_ZEROCOPYCOMMON_V8.3.1_defaultParameterInVirtualMethod
  virtual void OnShutdown(
      ::amsr::core::Result<Shutdown> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept = 0;

  /*!
   * \brief Called if received protocol message is an Termination message.
   * \param[in] result A reference to the result of the deserialization operation. On success, it contains the
   *                   deserialized message. The reference is only valid during the call of the function.
   * \param[in] memory_exchange_handle A unique pointer to a memory exchange handle must not be sent with the
   *                                   Termination message. Must be a nullptr, else it is considered to be a
   *                                   protocol error and a registered error callback is called.
   * \error Previously registered error callback with one of the following error codes is called.
   *        common::ZeroCopyCommonErrc::kMessageCorrupted When an Erroneous Termination message is received, i.e. with
   *                                                      a corrupted size or with a memory exchange handle, or when
   *                                                      client intended Termination notification is received by the
   *                                                      server.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_ZEROCOPYCOMMON_V8.3.1_defaultParameterInVirtualMethod
  virtual void OnTermination(
      ::amsr::core::Result<Termination> const& result,
      std::unique_ptr<::amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept = 0;
};

}  // namespace protocol_message
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_ROUTER_H_
