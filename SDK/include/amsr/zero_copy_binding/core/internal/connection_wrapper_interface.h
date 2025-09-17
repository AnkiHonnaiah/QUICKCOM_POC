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
 *         \brief  The interface of the connection wrapper used in the event backends.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/optional.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/credentials.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"
#include "ara/core/result.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief     The interface of the connection wrapper.
 * \details   The interface is used in the factory and hence should not be templated.
 *
 * \unit      ZeroCopyBinding::ZeroCopyBindingCore::ConnectionWrapper
 * \vprivate  Component private
 */
class ConnectionWrapperInterface {
 public:
  /*!
   * \brief       Informs the user that message reception has been completed.
   * \param[in]   message_view  View of the received message which was copied to the buffer provided by MemoryBufferPtr.
   *                            The view content is only valid during the execution of this callback function.
   *                            May contain the following error codes:
   *                      osabstraction::OsabErrc::kTruncation    The provided buffer was too small for the IPC message.
   *                      osabstraction::OsabErrc::kDisconnected  The connection has been terminated by the peer.
   *                      osabstraction::OsabErrc::kProtocolError The connection is broken and should be closed.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  using OnMessageReceivedCallback = ::vac::language::UniqueFunction<void(
      ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> const message_view) const>;

  /*!
   * \brief       Callback that notifies user about a completed asynchronous connect operation.
   * \param[in]   result  The result of the connect operation.
   *                      For the error codes, see CDD of the ConnectCompletionCallback of the wrapped connection class.
   * \context     Reactor
   * \pre         -
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \vprivate    Product private
   */
  using ConnectCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void> result) const>;

  /*!
   * \brief       Default constructor.
   */
  ConnectionWrapperInterface() noexcept = default;

  /*!
   * \brief       Default destructor.
   */
  virtual ~ConnectionWrapperInterface() noexcept = 0;

  ConnectionWrapperInterface(ConnectionWrapperInterface const&) = delete;
  auto operator=(ConnectionWrapperInterface const&) -> ConnectionWrapperInterface& = delete;
  ConnectionWrapperInterface(ConnectionWrapperInterface&&) = delete;
  auto operator=(ConnectionWrapperInterface&&) -> ConnectionWrapperInterface& = delete;

  /*!
   * \brief       Calls ConnectAsync() on the stored connection object.
   * \param[in]   remote_address        Unicast address of the remote server to connect to.
   * \param[in]   callback              Callback to notify about the completion of the connect operation.
   * \param[in]   c2s_buffer_size_hint  Optional parameter that indicates the shared memory buffer size (in bytes).
   * \error       See CDD of ConnectAsync() of the wrapped connection class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  virtual auto ConnectAsync(::amsr::ipc::UnicastAddress remote_address, ConnectCompletionCallback callback,
                            ::amsr::core::Optional<std::size_t> c2s_buffer_size_hint) noexcept
      -> ::ara::core::Result<void> = 0;

  /*!
   * \brief       Tries to send a binary message synchronously.
   * \param[in]   message_view  View to binary data sent. The binary buffer must be valid until the SendSync() call
   *                            returns synchronously.
   * \error       See CDD of SendSync() of the wrapped connection class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto SendSync(::amsr::core::Span<std::uint8_t const> message_view) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Triggers the asynchronous reception of the next incoming binary message.
   * \details     Message reception over the side channel is done asynchronously by copying the data into the provided
   *              memory buffer and calling the OnMessageReceivedCallback, where the data can be used.
   * \param[in]   on_message_received_callback  Callback function which is invoked when the message reception is
   *                                            completed. Must be a valid UniqueFunction.
   * \param[in]   memory_buffer_ptr             Unique binary memory buffer for the message reception. If the memory
   *                                            buffer is too small, an error result is passed to the
   *                                            on_message_received_callback. Must not be nullptr.
   * \error       See CDD of ReceiveAsync() of the wrapped connection class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  virtual auto ReceiveAsync(OnMessageReceivedCallback on_message_received_callback,
                            ::vac::memory::allocator::MemoryBufferPtr memory_buffer_ptr) noexcept
      -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Calls Close() on the stored connection object.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual void Close() noexcept = 0;

  /*!
   * \brief       Calls CheckIsOpen() on the stored connection object.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto CheckIsOpen() const noexcept -> bool = 0;

  /*!
   * \brief       Calls IsInUse() on the stored connection object.
   * \return      true if the wrapped connection is open or a callback is currently executing, false otherwise.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

  /*!
   * \brief       Determines the peer's integrity level.
   * \return      A result containing the peer's integrity level.
   * \error       osabstraction::OsabErrc::kUninitialized   If the connection has never before been connected to a peer.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE (only if called after the connection is established)
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto GetPeerIntegrityLevel() const noexcept -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> = 0;

  /*!
   * \brief       Determines the own integrity level.
   * \return      A result containing the own integrity level.
   * \error       osabstraction::OsabErrc::kUnexpected  Unexpected error.
   * \error       osabstraction::OsabErrc::kResource    Insufficient resources.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto GetOwnIntegrityLevel() const noexcept -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> = 0;

  /*!
   * \brief       Returns a reference to the stored connection object.
   * \details     The returned reference is valid during the object's lifetime.
   * \return      A reference to the stored connection.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto GetConnection() noexcept -> ::amsr::ipc::Connection& = 0;

  /*!
   * \brief       Query the identity of the communication peer.
   * \return      Credentials of communication peer.
   * \error       See CDD of GetPeerIdentity() of the wrapped connection class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto GetPeerIdentity() const noexcept -> ::amsr::core::Result<::amsr::ipc::Credentials> = 0;
};

inline ConnectionWrapperInterface::~ConnectionWrapperInterface() noexcept = default;

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_INTERFACE_H_
