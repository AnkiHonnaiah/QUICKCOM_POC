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
 *        \brief  Definition of interface ClientInterface.
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_INTERFACE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_INTERFACE_H_

#include <cstdint>

#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "vac/language/unique_function.h"

#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief Interface of a client.
 * \vprivate Product Private
 */
class ClientInterface {
 public:
  /*!
   * \brief Type used for the state of a client.
   */
  using ClientState = ::amsr::zero_copy_common::memcon::internal::ClientState;

  /*!
   * \brief Type used for a slot token.
   */
  using SlotToken = ::amsr::zero_copy_common::logic::internal::SlotToken;

  /*!
   * \brief Type used for the callback that can be registered to be called each time when a notification is received.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  using OnNotificationCallback = ::amsr::zero_copy_common::memcon::internal::OnNotificationCallback;

  /*!
   * \brief Type used for the callback that is called when a state transition happened in the client.
   * \details The callback is called each time a state transition happened asynchronously in the client.
   *          I.e., the state transition is triggered by the peer, not by the user of the client.
   *          Therefore, this callback is only called in the reactor context.
   * \param client_state The new client state.
   * \param opt_error_code Optional containing an error code that identifies the cause of the error.
   *                       Only contains a value when client_state is set to ClientState::kCorrupted.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the server
   *                       closed the connection without adhering to the protocol.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server
   *                       crashed.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices
   *                       a protocol error during the communication with the server.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  using OnStateTransitionCallback = ::vac::language::UniqueFunction<void(
      ClientState client_state, ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code)>;

  /*!
   * \brief Type used to describe a readable memory resource.
   */
  using ReadableMemoryResourceDescriptor = ::amsr::zero_copy_common::memcon::internal::ReadableMemoryResourceDescriptor;

  /*!
   * \brief Destruct the ClientInterface.
   * \details The preconditions ensure that this client is only destructed when the end state is reached and when it's
   *          ensured that no asynchronous reception of messages or notifications is currently ongoing.
   *          As for any other object, the user of this class is responsible for general lifetime concerns (including
   *          captures of this client in other asynchronous tasks).
   * \context ANY
   * \pre The client must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual ~ClientInterface() noexcept = 0;

  /*!
   * \brief Connect the client.
   * \details After a successful call to this method, the client is ready to participate in the connection establishment
   *          handshake.
   *          The connection establishment handshake is initiated by the server.
   *          Once a state transition to a new state has happened, the OnStateTransitionCallback is called with the
   *          new state.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnecting or if the client is in state ClientState::kConnecting and this method has already
   *        been called before.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-ConnectionEstablishmentClient
   * \vprivate Product Private
   */
  virtual auto Connect() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Disconnect the client.
   * \details After a call to this method, the client is in state ClientState::kDisconnected.
   *          Stops future asynchronous reception of messages and notifications.
   *          Thus, this stops future calls to the OnStateTransitionCallback and the OnNotificationCallback.
   *          However, asynchronous reception of messages and notifications may currently be ongoing.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is in state
   *        ClientState::kDisconnected.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kDisconnected.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kDisconnected.
   * \context ANY
   * \pre All received slot tokens must be released.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-ConnectionClientShutdown
   * \vprivate Product Private
   */
  virtual auto Disconnect() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Query whether the OnNotificationCallback is in use.
   * \details The client OnNotificationCallback is in use when there was a call to StartListening() that was not
   *          followed by a call to StopListening() or Disconnect(). Additionally, asynchronous reception of
   *          notifications may currently be ongoing even after a call to StopListening() or Disconnect().
   *          This function checks both conditions.
   *          Note that the usage status may change at any time. This means that between a call to this function and a
   *          call to another function, the usage status may have already changed again.
   * \return True, if the OnNotificationCallback is in use. False, otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto IsOnNotificationCallbackInUse() const noexcept -> bool = 0;

  /*!
   * \brief Query whether the client is in use to receive messages or notifications.
   * \details The client is in use when there was not yet a call to Disconnect(). Additionally, asynchronous reception
   *          of messages and notifications may currently be ongoing even after a call to Disconnect(). This function
   *          checks both conditions.
   *          When this function has returned false once, it will always return false for all future calls.
   * \return True, if the client is in use to receive messages or notifications. False, otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

  /*!
   * \brief Get the state of the client.
   * \details Note that the state may change at any time. This means that between a call to this function and a call to
   *          another function, the state may have already changed again.
   * \return State of the client.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto GetState() const noexcept -> ClientState = 0;

  /*!
   * \brief Start listening to notifications.
   * \details After a successful call to this method, the client is still in ClientState::kConnected but in notified
   *          mode.
   *          Requests the server to start sending a notification each time a slot is sent to this client.
   *          The OnNotificationCallback is called each time when a notification is received.
   * \param on_notification_callback Callback that is called each time when a notification is received.
   *                                 Must not contain a nullptr.
   *                                 Must stay valid until it is not used anymore. This cannot be checked.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected and polling mode or if a previously configured OnNotificationCallback is still in
   *        use, i.e. IsOnNotificationCallbackInUse() returns true.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kCorrupted.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-MessageNotification
   * \vprivate Product Private
   */
  virtual auto StartListening(OnNotificationCallback on_notification_callback) noexcept
      -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Stop listening to notifications.
   * \details After a successful call to this method, the client is still in ClientState::kConnected but in polling
   *          mode.
   *          Requests the server to stop sending a notification each time a slot is sent to this client.
   *          Stops future asynchronous reception of notifications.
   *          Thus, this stops future calls to the OnNotificationCallback.
   *          However, asynchronous reception of notifications may currently be ongoing.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected and notified mode.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server crashed.
   *        The client is now in state ClientState::kCorrupted.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-MessageNotification
   * \vprivate Product Private
   */
  virtual auto StopListening() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Receive a slot that was sent from the server.
   * \details A received slot may be used with the AccessSlotContent() and ReleaseSlot() methods.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          This receive operation must synchronize with ServerInterface::SendSlot() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, after the server has sent a slot by calling ServerInterface::SendSlot(), all clients belonging
   *          to the same Zero-Copy instance must be able to receive the sent slot.
   *          Note that either all clients that are not rate-limited are able to receive a slot or none is.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \return Optional containing a SlotToken representing the received slot if available, empty Optional otherwise.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected or ClientState::kDisconnectedRemote.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server. Duplicate reception of a slot is treated as a protocol error.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-MessageTransfer
   * \vprivate Product Private
   */
  virtual auto ReceiveSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> = 0;

  /*!
   * \brief Access the content of a received slot.
   * \details Access is read-only.
   * \param token SlotToken representing the slot to access the content for.
   * \return Read-only view of the content of the slot represented by the \p token.
   *         The readable view is valid until the corresponding \p token is moved back to the client using
   *         ReleaseSlot().
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is in state
   *        ClientState::kCorrupted.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-MessageTransfer
   * \vprivate Product Private
   */
  virtual auto AccessSlotContent(SlotToken const& token) const noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> = 0;

  /*!
   * \brief Release a previously received slot to the server.
   * \details A released slot may be received again.
   *          This operation requires to give-up ownership of the \p token.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          This release operation must synchronize with ServerInterface::ReclaimSlots() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, if a client releases a slot using this method, the server belonging to the same Zero-Copy
   *          instance is able to reclaim the slot from the client.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \param token SlotToken representing the slot to release.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   *        The client is now in state ClientState::kCorrupted.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-MemCon-MessageTransfer
   * \vprivate Product Private
   */
  virtual auto ReleaseSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Get the readable resource descriptor for the slot memory.
   * \return Readable resource descriptor for the slot memory.
   *         If the contained memory handle has the value of ::amsr::memory::kInvalidMemoryHandle, this indicates that
   *         an integer memory object handle is not available.
   *         The readable resource descriptor is valid until but excluding ClientState::kDisconnected is reached.
   *         The readable resource descriptor may not be used to change the state of the underlying operating system
   *         memory object.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the client is not in state
   *        ClientState::kConnected, ClientState::kDisconnectedRemote or ClientState::kCorrupted.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-ZeroCopyCommon-AccessSlotMemoryResource
   * \vprivate Product Private
   */
  virtual auto GetSlotMemoryResourceDescriptor() const noexcept
      -> ::amsr::core::Result<ReadableMemoryResourceDescriptor> = 0;

 protected:
  /*!
   * \brief Construct a ClientInterface.
   * \vprivate Product Private
   */
  ClientInterface() noexcept = default;

  /*!
   * \brief Construct a ClientInterface by copying.
   * \vprivate Product Private
   */
  ClientInterface(ClientInterface const&) noexcept = default;

  /*!
   * \brief Construct a ClientInterface by moving.
   * \vprivate Product Private
   */
  ClientInterface(ClientInterface&&) noexcept = default;

  /*!
   * \brief Copy another ClientInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientInterface const&) & noexcept -> ClientInterface& = default;

  /*!
   * \brief Move another ClientInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientInterface&&) & noexcept -> ClientInterface& = default;
};

inline ClientInterface::~ClientInterface() noexcept = default;

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_INTERFACE_H_
